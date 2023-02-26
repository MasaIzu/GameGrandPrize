#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>

struct fish {
	WorldTransform pos;	//���[���h���W
	float radian;	//����O���p�̊p�x
	float radius;	//����O���p�̔��a
	float spd;	//�ړ����x
	Vector3 displacement;	//�e���W����̔����Ȃ���
};

class Boss
{
public:

	~Boss();

	//�����`�����鋅�̒��S�̃��[���h���W
	fish fishParent;
	//���Q
	std::vector<fish> fishes;

	//���̃��f���f�[�^�@
	Model* swordModel = nullptr;

	//�����_���ŕω����鑬�x�̊�{�l
	float randSpdParam = 0;

	void Initialize();

	void Update();

	void CreateFish(float posY = 0);

};

/// <summary>
/// �C�ӂ̐��l�͈̔͂��烉���_���Ȓl��Ԃ�
/// </summary>
/// <param name="num1">�l1</param>
/// <param name="num2">�l2</param>
/// <returns>�l1�ƒl2�̊Ԃ̃����_���Ȓl</returns>
float Random(float num1, float num2);
