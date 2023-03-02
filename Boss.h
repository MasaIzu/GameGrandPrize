#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>
#include"EasingData.h"
#include"ViewProjection.h"

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

	~Boss();

	//�����`�����鋅�̒��S�̃��[���h���W
	fish fishParent;
	//���Q
	std::vector<fish> fishes;

	//���̃��f���f�[�^�@
	std::unique_ptr<Model> swordModel = nullptr;

	//�����_���ŕω����鑬�x�̊�{�l
	float randSpdParam = 0;

	void Initialize();

	void Update();

	void CreateFish(float posY = 0);

	void Draw(ViewProjection viewProMat);

	BossFirstPhase phase1;
	const int attackCooltime = 60 * 3;
	const int beginAttackDelay = 60 * 1;

	//�U���̃��[�V��������^�C�}�[	�@�@�����@�ړ�  �U���@���U   Ӱ��ݕ��
	const int atkSwordMotionTime = 120 + 45 + 60 + 60 + (10 * 3);
	int nextPhaseInterval = 0;

	int GetFishCount() { return fishes.size(); }
private:
	//�t�F�[�Y���Ƃ̍X�V����
	void IdleUpdate();

	void AtkSwordUpdate();

	void AtkRushUpdate();

	void BeginMotionUpdate();

	EasingData easeData;

	WorldTransform swordTransform;

	WorldTransform Transform;
	Vector3 swordPos = { 30,-30,20 };
	EasingData easeSwordScale;

	EasingData easePFishToSword[120];	//���̈ړ��p�C�[�W���O�^�C�}�[
	Vector3 spd[120];	//�ړ����鋛�̈ړ����x
	std::vector<int> choiceFishIndex;	//�z�񂩂牽�Ԗڂ̋����I�΂�Ă��邩(�d���΍�)
	Vector3 fishesBeforePos[120], fishesControllP1[120], fishesControllP2[120];
};

/// <summary>
/// �C�ӂ̐��l�͈̔͂��烉���_���Ȓl��Ԃ�
/// </summary>
/// <param name="num1">�l1</param>
/// <param name="num2">�l2</param>
/// <returns>�l1�ƒl2�̊Ԃ̃����_���Ȓl</returns>
float Random(float num1, float num2);

/// <summary>
/// ���`���
/// </summary>
/// <param name="start">�n�_</param>
/// <param name="end">�I�_</param>
/// <param name="t">����</param>
/// <returns>���W</returns>
Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

/// <summary>
/// �x�W�G�Ȑ����
/// </summary>
/// <param name="start">�n�_</param>
/// <param name="contRollP1">����_1</param>
/// <param name="contRollP2">����_2</param>
/// <param name="end">�I�_</param>
/// <param name="t">����</param>
/// <returns>���W</returns>
Vector3 LerpBezire(const Vector3& start, const Vector3& contRollP1, const Vector3& contRollP2, const Vector3& end, float t);