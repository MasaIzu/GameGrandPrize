#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>
#include"EasingData.h"
#include"ViewProjection.h"
#include <BaseCollider.h>

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

	//���̃��f���f�[�^
	std::unique_ptr<Model> swordModel = nullptr;

	//���̃��f���f�[�^
	std::unique_ptr<Model> fishBodyModel = nullptr;
	std::unique_ptr<Model> fishEyeModel = nullptr;

	//�����_���ŕω����鑬�x�̊�{�l
	float randSpdParam = 0;

	void Initialize();

	void Update(const Vector3& targetPos);

	void CreateFish(float posY = 0);

	void Draw(ViewProjection viewProMat);

	BossFirstPhase phase1;
	const int attackCooltime = 60 * 3;	//���̍U���܂ł̃N�[���^�C��
	const int beginAttackDelay = 60 * 1;	//�U���̗\�����쎞��


	//�U���̃��[�V��������^�C�}�[	�@�@�����@�ړ�  �U���@  ���U
	const int atkSwordMotionTime = 120 + 45 + 150 + 120;
	int nextPhaseInterval = 0;
	const int rushMaxCount = 3;	//�ːi�U��(�Г�)�������
	int rushCount = 0;	//�ːi�U���̎c���


	int GetFishCount() { return fishes.size(); }

	WorldTransform swordTransform;

	static const int fishMaxCount = 200;

	Vector3 GetSwordCollisionCube1()const { return posSwordColCube1; }
	Vector3 GetSwordCollisionCube2()const { return posSwordColCube2; }

private:
	//�t�F�[�Y���Ƃ̍X�V����
	void IdleUpdate();

	void AtkSwordUpdate(const Vector3& targetPos);

	void AtkRushUpdate(const Vector3& targetPos);

	void BeginMotionUpdate();

	void FishLookFront(Vector3 pos,Vector3 dirVec,int fishNum);

	void SwordColCubeUpdate();
	
	WorldTransform Transform;
	Vector3 swordPos = {0,0,0 };
	EasingData easeSwordPos;
	EasingData easeSwordScale;

	const int moveFishMax = 120;


	EasingData easePFishToSword[120];	//���̈ړ��p�C�[�W���O�^�C�}�[
	std::vector<int> choiceFishIndex;	//�z�񂩂牽�Ԗڂ̋����I�΂�Ă��邩(�d���΍�)
	Vector3 parentBeforePos, parentAfterPos;
	Vector3 fishesBeforePos[fishMaxCount], fishesControllP1[fishMaxCount], fishesControllP2[fishMaxCount],fishesAfterPos[fishMaxCount];
	Vector3 beforeScale, afterScale;

	// �R���C�_�[
	BaseCollider* collider = nullptr;
	float radius = 20.0f;//�����蔻�蔼�a

	//�����蔻��p�̌��̑傫���p�ϐ�
	WorldTransform testTrans;
	WorldTransform testTrans2;
	Vector3 posSwordColCube1;
	Vector3 posSwordColCube2;
	//���̃I�u�W�F�N�g�Ƃ��Ă̑傫��(�����蔻��p)
	const float swordSizeX1 = -0.3f;
	const float swordSizeX2 = 0.6f;
	const float swordSizeY1 = -1.0f;
	const float swordSizeY2 = 14.6f;
	const float swordSizeZ1 = -1.0f;
	const float swordSizeZ2 = 1.0f;

	EasingData easeParentPos;

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
/// 3���x�W�G�Ȑ����
/// </summary>
/// <param name="start">�n�_</param>
/// <param name="contRollP1">����_1</param>
/// <param name="contRollP2">����_2</param>
/// <param name="end">�I�_</param>
/// <param name="t">����</param>
/// <returns>���W</returns>
Vector3 LerpBezireCubic(const Vector3& start, const Vector3& contRollP1, const Vector3& contRollP2, const Vector3& end, float t);

Vector3 LerpBezireQuadratic(const Vector3& start, const Vector3& contRollP, const Vector3& end, float t);


/// <summary>
/// 0~1�ւ̐��`��Ԃ�InBack�̕�Ԃɕϊ�����
///</summary>
/// <param name="t">����</param>
/// <returns></returns>
float LerpConbertInback(float t);

float LerpConbertOut(float t);

