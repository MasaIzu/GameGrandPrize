#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>

struct fish {
	WorldTransform pos;	//���[���h���W
	float radian;	//����O���p�̊p�x
	float radius;	//����O���p�̔��a
	float spd;	//�ړ����x
	Vector3 displacement;	//�e���W����̔����Ȃ���
	Vector3 randomVec;
};

enum class BossFirstPhase {
	Idle,//�ҋ@
	Atk_Sword,//���ό`�U��
	Atk_Rush,//�ːi�U��
	BeginMotion,//���[�V�����J�n����

};

class Boss
{
public:
	//�����`�����鋅�̒��S�̃��[���h���W
	fish fishParent;
	//���Q
	std::vector<fish> fishes;

	//�����_���ŕω����鑬�x�̊�{�l
	float randSpdParam = 0;

	void Initialize();

	void Update();

	void CreateFish(float posY = 0);

	BossFirstPhase phase1;
	const int attackCooltime = 60 * 3;
	const int beginAttackDelay = 60 * 1;
	int nextPhaseInterval = 0;

	int GetFishCount() { return fishes.size(); }
private:
	//�t�F�[�Y���Ƃ̍X�V����
	void IdleUpdate();

	void AtkSwordUpdate();

	void AtkRushUpdate();

	void BeginMotionUpdate();
};

/// <summary>
/// �C�ӂ̐��l�͈̔͂��烉���_���Ȓl��Ԃ�
/// </summary>
/// <param name="num1">�l1</param>
/// <param name="num2">�l2</param>
/// <returns>�l1�ƒl2�̊Ԃ̃����_���Ȓl</returns>
float Random(float num1, float num2);
