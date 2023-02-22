#include "Boss.h"
#include<random>

void Boss::Initialize()
{
	fishParent.pos.Initialize();
	fishParent.radius = 20.0f;

	if (!fishes.empty()) {
		fishes.clear();
	}
}

void Boss::Update()
{
	//���`�Ԃ̋��Q�̍X�V
	fishParent.pos.TransferMatrix();

	for (int i = 0; i < fishes.size(); i++) {
		//���̃��W�A��(���̎���O��)�����Z
		fishes[i].radian += fishes[i].spd;
		if (fishes[i].radian > 360.0f) {
			fishes[i].radian -= 360.0f;
			fishes[i].spd = Random(0.0f, 1.25f);
		}

		//���W���v�Z
		Vector3 pos;
		
	//	pos.y = fishParent.radius - fishes[i].radius;
		pos.x = sin(PI / 180.0f * fishes[i].radian) * fishes[i].radius;
		pos.z = cos(PI / 180.0f * fishes[i].radian) * fishes[i].radius;
		pos.y = fishes[i].pos.translation_.y;
		fishes[i].pos.translation_ = pos;
		//fishes[i].pos.rotation_.y =PI / fishes[i].radian * 180.0f;
		fishes[i].pos.TransferMatrix();
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
	newFish.pos.Initialize();
	newFish.pos.scale_ = { 5.0f,5.0f,5.0f };
	newFish.pos.translation_.y = posY;
	//newFish.pos.rotation_.y = 0.78f;
	newFish.pos.TransferMatrix();

	newFish.spd = Random(0.0f, 1.25f);

	//�z��ɂ���
	fishes.push_back(newFish);
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
