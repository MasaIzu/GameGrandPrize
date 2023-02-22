#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>

struct fish {
	WorldTransform pos;
	float radian;
	float radius;
	float spd;
};

class Boss
{
public:
	//�����`�����鋅�̒��S�̃��[���h���W
	fish fishParent;
	//���Q
	std::vector<fish> fishes;


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
