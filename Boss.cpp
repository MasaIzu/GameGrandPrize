#include "Boss.h"
#include<random>
#include"Quaternion.h"
#include"Affin.h"

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
	swordModel = Model::CreateFromOBJ("dammySword");
	phase1 = BossFirstPhase::Idle;
	nextPhaseInterval = attackCooltime;
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
	newFish.pos.scale_ = { 5.0f,5.0f,5.0f };
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
		nextPhaseInterval =  beginAttackDelay;
		//�t�F�[�Y���ڍs
		phase1 = BossFirstPhase::BeginMotion;
		//���Q�̗���]�̂��߂̃����_���ȃx�N�g�����쐬
		for (int i = 0; i < fishes.size(); i++) {
			fishes[i].randomVec = Vector3(Random(0.0f,1.0f), Random(0.0f, 1.0f), Random(0.0f, 1.0f));
			fishes[i].randomVec.normalize();
		}

	}
}

void Boss::AtkSwordUpdate()
{
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
		Quaternion randomRotate =(cross,fishes[i].radian);
		pos = randomRotate.RotateVector(fishes[i].pos.translation_);
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
