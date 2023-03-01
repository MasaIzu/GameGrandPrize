#include "Boss.h"
#include<random>
#include"Quaternion.h"
#include"Affin.h"
#include"ImGuiManager.h"

Boss::~Boss()
{
	delete swordModel;
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
	swordModel = Model::CreateFromOBJ("dammySword", true);
	phase1 = BossFirstPhase::Idle;
	nextPhaseInterval = attackCooltime;
	swordTransform.Initialize();
}

void Boss::Update()
{
	//��1�`�Ԃ̋��Q�̍X�V

	switch (phase1) {
	case BossFirstPhase::Idle:
		IdleUpdate();
		break;
	case BossFirstPhase::Atk_Sword:
		AtkSwordUpdate();
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

void Boss::AtkSwordUpdate()
{
	//���̐����J�n
	//�s���̐؂�ւ�(�J�n)�^�C�~���O�Ŋe�s���̃t���[�����ŃC�[�W���O�^�C�}�[�𓮂���

	if (nextPhaseInterval == atkSwordMotionTime) {
		swordTransform.scale_ = { 0,0,0 };
		easeSwordScale.Start(120);
	}

	if (nextPhaseInterval > atkSwordMotionTime - 120) {
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
		easePFishToSword[choiceFishIndex.size() - 1].Start(15);

	}

	for (int i = 0; i < choiceFishIndex.size(); i++) {
		easePFishToSword[i].Update();

		fishes[choiceFishIndex[i]].pos.translation_ = LerpBezire(fishesBeforePos[i], fishesControllP1[i], fishesControllP2[i], swordPos, easePFishToSword[i].GetTimeRate());
		fishes[choiceFishIndex[i]].pos.scale_ = Lerp({ 0.5f,0.5f,0.5f }, { 0,0,0 }, easePFishToSword[i].GetTimeRate());
		fishes[choiceFishIndex[i]].pos.TransferMatrix();
	}

	//�������X�ɂ�����������
	Vector3 swordScale;
	swordScale = Lerp({ 0,0,0 }, { 4,4,4 }, easeSwordScale.GetTimeRate());
	easeSwordScale.Update();

	ImGui::Begin("sword");
	ImGui::SliderFloat("rotaX", &swordTransform.rotation_.x, 0.0f, 360.0f);
	ImGui::SliderFloat("rotaY", &swordTransform.rotation_.y, 0.0f, 360.0f);
	ImGui::SliderFloat("rotaZ", &swordTransform.rotation_.z, 0.0f, 360.0f);

	swordTransform.rotation_.z = PI / 2;
	swordTransform.rotation_.y = PI / 6;

	swordTransform.scale_ = swordScale;
	swordTransform.translation_ = swordPos;
	swordTransform.TransferMatrix();
	ImGui::End();

	//�����v���C���[�̑O�܂ňړ�(45f���炢�H)

	//�U��(60f)

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

Vector3 LerpBezire(const Vector3& start, const Vector3& contRollP1, const Vector3& contRollP2, const Vector3& end, float t)
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
