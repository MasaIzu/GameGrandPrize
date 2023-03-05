#include "Boss.h"
#include<random>
#include"Quaternion.h"
#include"Affin.h"
#include"ImGuiManager.h"
#include"Easing.h"

Boss::~Boss()
{
	
}

void Boss::Initialize()
{
	fishParent.pos.Initialize();
	fishParent.radius = 20.0f;

	if (!fishes.empty()) {
		fishes.clear();
	}

	randSpdParam = 3.75f;

	//���̃��f��������
	swordModel.reset(Model::CreateFromOBJ("dammySword", true));
	phase1 = BossFirstPhase::Idle;
	nextPhaseInterval = attackCooltime;

	swordTransform.Initialize();
	swordTransform.TransferMatrix();
}

void Boss::Update(const Vector3& targetPos)
{
	//��1�`�Ԃ̋��Q�̍X�V
	ImGui::Begin("sword");


	switch (phase1) {
	case BossFirstPhase::Idle:
		IdleUpdate();
		break;
	case BossFirstPhase::Atk_Sword:
		AtkSwordUpdate(targetPos);
		break;
	case BossFirstPhase::Atk_Rush:
		AtkRushUpdate();
		break;
	case BossFirstPhase::BeginMotion:
		BeginMotionUpdate();
		break;
	default:
		break;

	}

	
	ImGui::End();

}

void Boss::CreateFish(float posY)
{
	//y���W���e�̔��a�𒴂��Ȃ��悤�ɂ���
	if (fabs(posY) > fishParent.radius) {
		if (posY > 0)posY = fishParent.radius;
		else posY = -fishParent.radius;
	}

	//�V���������쐬
	fish newFish;
	//�p�x�������_���Ɏw��
	newFish.radian = Random(0.0f, 360.0f);
	//�^����ꂽ�������甼�a��ݒ�
	newFish.radius = sqrt(fishParent.radius * fishParent.radius - posY * posY);
	//���[���h�s�񏉊���
	newFish.pos.Initialize();
	newFish.pos.scale_ = { 0.5f,0.5f,0.5f };
	//Y���W�͈�����
	newFish.pos.translation_.y = posY;
	//newFish.pos.rotation_.y = 0.78f;
	newFish.pos.TransferMatrix();

	//���x�������_���Ɍ���
	newFish.spd = Random(0.0f, randSpdParam);

	//�e�̃|�C���^�Ⴄ
	newFish.pos.parent_ = &fishParent.pos;

	//�����Ȃ���̑傫���͎����̔��a��1/10���烉���_����
	float displacementParam = newFish.radius / 10.0f * 1.5f;
	newFish.displacement = Vector3(Random(0.0f, displacementParam), Random(0.0f, displacementParam), Random(0.0f, displacementParam));

	Vector3 pos;

	//	pos.y = fishParent.radius - fishes[i].radius;
	pos.x = sin(PI / 180.0f * newFish.radian) * newFish.radius;
	pos.z = cos(PI / 180.0f * newFish.radian) * newFish.radius;
	pos.y = (sqrt(fishParent.radius * fishParent.radius - newFish.radius * newFish.radius) * (newFish.pos.translation_.y / fabs(newFish.pos.translation_.y)));

	pos += newFish.displacement;

	newFish.pos.translation_ = pos;
	newFish.pos.TransferMatrix();
	//�z��ɂ���
	fishes.push_back(newFish);
}

void Boss::Draw(ViewProjection viewProMat)
{
	if (phase1 == BossFirstPhase::Atk_Sword) {
		swordModel->Draw(swordTransform, viewProMat);
	}

	swordModel->Draw(swordTransform, viewProMat);
}

void Boss::IdleUpdate()
{
	//���Q�̒��S(�^��)�̍��W�X�V
	fishParent.pos.TransferMatrix();

	//��1�C���̍X�V
	for (int i = 0; i < fishes.size(); i++) {

		//���̃��W�A��(���̎���O��)�����Z
		fishes[i].radian += fishes[i].spd;
		if (fishes[i].radian > 360.0f) {
			fishes[i].radian -= 360.0f;
			fishes[i].spd = Random(0.0f, randSpdParam);
		}

		//���W���v�Z
		Vector3 pos;

		//	pos.y = fishParent.radius - fishes[i].radius;
		pos.x = sin(PI / 180.0f * fishes[i].radian) * fishes[i].radius;
		pos.z = cos(PI / 180.0f * fishes[i].radian) * fishes[i].radius;
		pos.y = (sqrt(fishParent.radius * fishParent.radius - fishes[i].radius * fishes[i].radius) * (fishes[i].pos.translation_.y / fabs(fishes[i].pos.translation_.y)));

		pos += fishes[i].displacement;

		fishes[i].pos.translation_ = pos;
		//fishes[i].pos.rotation_.y =PI / fishes[i].radian * 180.0f;
		fishes[i].pos.TransferMatrix();
	}

	//�U���̃N�[���^�C�������炷
	nextPhaseInterval--;
	if (nextPhaseInterval == 0) {
		//0�ɂȂ�����N�[���^�C�����U���J�n���[�V�����̎��Ԃɐݒ�
		nextPhaseInterval = beginAttackDelay;
		//�t�F�[�Y���ڍs
		phase1 = BossFirstPhase::BeginMotion;
		//�ق�Ƃ́��̃t�F�[�Y�͗\���s���Ɉڍs�����ǂ܂��������Ȃ������Ȃ̂ł�������U���J�n�ɑ��ڂ�
		phase1 = BossFirstPhase::Atk_Sword;
		nextPhaseInterval = atkSwordMotionTime;

		//���Q�̗���]�̂��߂̃����_���ȃx�N�g�����쐬
		for (int i = 0; i < fishes.size(); i++) {
			fishes[i].randomVec = Vector3(Random(0.0f, 1.0f), Random(0.0f, 1.0f), Random(0.0f, 1.0f));
			fishes[i].randomVec.normalize();
		}

	}
}

void Boss::AtkSwordUpdate(const Vector3& targetPos)
{
	//���̐����J�n
	//�s���̐؂�ւ�(�J�n)�^�C�~���O�Ŋe�s���̃t���[�����ŃC�[�W���O�^�C�}�[�𓮂���

	const int swordCreateTime = 120;
	const int swordMoveTime = 45;
	const int swordAtkTime = 150;

	if (nextPhaseInterval == atkSwordMotionTime) {
		swordTransform.scale_ = { 0,0,0 };
		easeSwordScale.Start(120 + 15);
		//���̍��W
		swordPos.x = fishParent.pos.translation_.x + 30;
		swordPos.y = fishParent.pos.translation_.y - 30;
		swordPos.z = fishParent.pos.translation_.z;
		//swordPos.x = fishParent.pos.translation_.x;
	}
	else if (nextPhaseInterval == atkSwordMotionTime - swordCreateTime - swordMoveTime)
	{
		easeSwordMove.Start(swordAtkTime);
	}

	float distancePtoSword = 90.0f;

	if (nextPhaseInterval > atkSwordMotionTime - swordCreateTime) {
		//�G���S���猕�̈ʒu�̒��S�܂ňړ�����(120f)
		//���t���[�������_���ɋ��Q���狛��I�сA�I�΂ꂽ����10f�Ō��̒��S�܂ňړ�����

		//�ŏ��ɂǂ̋������܂ňړ������邩���߂�
		int goFishToSwordIndex = 0;
		goFishToSwordIndex = static_cast<int>(Random(0, fishes.size()));
		//�d���΍�Ŕz�����T��
		if (!choiceFishIndex.empty()) {
			for (int i = 0; i < choiceFishIndex.size(); i++) {
				//�d�����Ă����������x�����_���ɐU�蕪����
				if (choiceFishIndex[i] == goFishToSwordIndex) {
					goFishToSwordIndex = static_cast<int>(Random(0, fishes.size()));
					//for���ŏ������蒼��
					i = -1;
				}
			}
		}
		//�z��ɑ}��
		choiceFishIndex.push_back(goFishToSwordIndex);
		//���I�z�񖖔��̗v�f�̃C�[�W���O������10f�ŊJ�n������(���ԂɈړ����Ă��炤����)

		Vector3 pos;
		float randomParam = 10.0f;
		//�ړ����J�n���鋛�̌��̍��W������Ă����A����_���C�C�����Ɍ��߂�
		fishesBeforePos[choiceFishIndex.size() - 1] = fishes[choiceFishIndex[choiceFishIndex.size() - 1]].pos.translation_;
		//����_1�͎n�_����덷x(����5)�̃����_���Ȓn�_�B����_2�͏I�_����덷x�Ƀ����_��
		pos = fishesBeforePos[choiceFishIndex.size() - 1];
		fishesControllP1[choiceFishIndex.size() - 1] = Vector3(Random(pos.x - randomParam, pos.x + randomParam), Random(pos.y - randomParam, pos.y + randomParam), Random(pos.z - randomParam, pos.z + randomParam));
		pos = swordPos;
		fishesControllP2[choiceFishIndex.size() - 1] = Vector3(Random(pos.x - randomParam, pos.x + randomParam), Random(pos.y - randomParam, pos.y + randomParam), Random(pos.z - randomParam, pos.z + randomParam));
		easePFishToSword[choiceFishIndex.size() - 1].Start(60);

		//�������X�ɂ�����������
		Vector3 swordScale;
		swordScale = Lerp({ 0,0,0 }, { 4,4,4 }, easeSwordScale.GetTimeRate());
		easeSwordScale.Update();
		swordTransform.scale_ = swordScale;
		swordTransform.translation_ = swordPos;

		//���ړ��̃C�[�W���O���J�n����(���ۂ̍��W�ړ���X�V�͌���if���ł���Ă�̂ŉe���Ȃ�)
		easeSwordMove.Start(swordMoveTime);

	}//�A�j���[�V�������Ԃ��ړ�����(���̐�+�Ō�Ɉړ��������̈ړ�����)��菬�����Ȃ����Ȃ玟�̃��[�V����(�U���J�n���W�ւ̈ړ�)���J�n
	else if (nextPhaseInterval > atkSwordMotionTime - swordCreateTime - swordMoveTime) {

		Vector3 rotaVec;
		rotaVec.x = sin(PI / 3.0f);
		rotaVec.z = cos(PI / 3.0f);
		rotaVec.normalize();
		rotaVec *= distancePtoSword;

		//�W�I�̍��W�Ɗ|���Z
		Vector3 aftetVec;
		aftetVec = targetPos + rotaVec;

		ImGui::Text("afterVec:%f,%f,%f", aftetVec.x, aftetVec.y, aftetVec.z);

		easeSwordMove.Update();
		Vector3 pos;
		pos =  Lerp(swordPos, aftetVec, LerpConbertOut( easeSwordMove.GetTimeRate()));
		swordTransform.rotation_.z = easeSwordMove.GetTimeRate() * -(PI / 2.0f);
		swordTransform.rotation_.x = easeSwordMove.GetTimeRate() * -(PI / 2.0f);

		swordTransform.translation_ = pos;

	}
	else if (nextPhaseInterval > atkSwordMotionTime - swordCreateTime - swordMoveTime - swordAtkTime) {
		Vector3 rotaVec;
		rotaVec.x = sin(PI / 3.0f);
		rotaVec.z = cos(PI / 3.0f);
		rotaVec.normalize();
		rotaVec *= distancePtoSword;

		//�W�I�̍��W�Ɗ|���Z
		Vector3 beforePos,afterPos;
		beforePos = targetPos + rotaVec;
		afterPos = beforePos;
		afterPos.x = -afterPos.x;
		//afterPos.z = -afterPos.z;

		easeSwordMove.Update();
		Vector3 pos;
		pos = LerpBezireQuadratic(beforePos, targetPos, afterPos, LerpConbertInback(easeSwordMove.GetTimeRate()));

		swordTransform.rotation_.x = -(PI / 2.0f) - (LerpConbertInback(easeSwordMove.GetTimeRate()) * PI / 3.0f);
		swordTransform.translation_ = pos;
	}


	//�C�[�W���O�ɂ��X�P�[���ƍ��W�̐���
	for (int i = 0; i < choiceFishIndex.size(); i++) {
		easePFishToSword[i].Update();

		fishes[choiceFishIndex[i]].pos.translation_ = LerpBezireCubic(fishesBeforePos[i], fishesControllP1[i], fishesControllP2[i], swordPos, easePFishToSword[i].GetTimeRate());
		fishes[choiceFishIndex[i]].pos.scale_ = Lerp({ 0.5f,0.5f,0.5f }, { 0,0,0 }, easePFishToSword[i].GetTimeRate());
		fishes[choiceFishIndex[i]].pos.TransferMatrix();
	}

	ImGui::SliderFloat("rotaX", &swordTransform.rotation_.x, 0.0f, 360.0f);
	ImGui::SliderFloat("rotaY", &swordTransform.rotation_.y, 0.0f, 360.0f);
	ImGui::SliderFloat("rotaZ", &swordTransform.rotation_.z, 0.0f, 360.0f);

	

	//swordTransform.rotation_.z = -PI / 2;
	//swordTransform.rotation_.y = PI / 6;

	
	swordTransform.TransferMatrix();

	//�^�C�}�[����
	nextPhaseInterval--;

	//���[�V�����I��
}

void Boss::AtkRushUpdate()
{
}

void Boss::BeginMotionUpdate()
{
	//���Q�̒��S(�^��)�̍��W�X�V
	fishParent.pos.TransferMatrix();

	//��1�C���̍X�V
	for (int i = 0; i < fishes.size(); i++) {

		//���̃��W�A��(���̎���O��)�����Z
		fishes[i].radian += fishes[i].spd;
		if (fishes[i].radian > 360.0f) {
			fishes[i].radian -= 360.0f;
			fishes[i].spd = Random(0.0f, randSpdParam);
		}


		//�����_���Ɏ擾�����x�N�g���Ǝ����W���猴�_(���Q�̒��S)�̃x�N�g���̊O�ς��Ƃ�A����]�̎����쐬
		Vector3 vec = fishes[i].pos.translation_ - fishParent.pos.translation_;
		Vector3 cross;
		cross = fishes[i].randomVec.cross(vec);

		Vector3 pos;
		Quaternion randomRotate = (cross, fishes[i].radian);
		//	pos = randomRotate.RotateVector(fishes[i].pos.translation_);
		fishes[i].pos.translation_ = pos;
		fishes[i].pos.TransferMatrix();

	}

	//�U���̃N�[���^�C�������炷
	nextPhaseInterval--;
	if (nextPhaseInterval == 0) {
		//0�ɂȂ�����N�[���^�C�����U���J�n���[�V�����̎��Ԃɐݒ�
		nextPhaseInterval = beginAttackDelay;
		//�t�F�[�Y���ڍs
		phase1 = BossFirstPhase::BeginMotion;
	}
}

float Random(float num1, float num2)
{
	//�������珬�������Ƃ����������𕪂���
	float min, max;
	min = num2;
	max = num1;
	if (num1 < num2) {
		min = num1;
		max = num2;
	}
	//�����V�[�h������
	std::random_device seedGem;
	//�����Z���k�E�c�C�X�^�[�̗����G���W��
	std::mt19937_64 engine(seedGem());
	//�����͈͂̎w��
	std::uniform_real_distribution<float> dist(min, max);

	//�����_���Ȓl�𐶐����ĕԂ�
	return dist(engine);
}

Vector3 Lerp(const Vector3& start, const Vector3& end, float t)
{
	return start * (1.0f - t) + end * t;
}

Vector3 LerpBezireCubic(const Vector3& start, const Vector3& contRollP1, const Vector3& contRollP2, const Vector3& end, float t)
{
	Vector3 p1, p2, p3, p4, p5, result;
	p1 = Lerp(start, contRollP1, t);
	p2 = Lerp(contRollP1, contRollP2, t);
	p3 = Lerp(contRollP2, end, t);
	p4 = Lerp(p1, p2, t);
	p5 = Lerp(p2, p3, t);
	result = Lerp(p4, p5, t);

	return result;
}

Vector3 LerpBezireQuadratic(const Vector3& start, const Vector3& contRollP, const Vector3& end, float t)
{
	Vector3 p1, p2,p3;
	p1 = Lerp(start, contRollP, t);
	p2 = Lerp(contRollP,end, t);
	p3 = Lerp(p1, p2, t);

	return p3;
}

float LerpConbertInback(float t)
{
	const float c1 = 1.70158;
	const float c3 = c1 + 1;

	return c3 * t *t *t - c1 * t * t;
}

float LerpConbertOut(float t)
{
	return 1 - pow(1-t,5);
}



