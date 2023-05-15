#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>
#include"EasingData.h"
#include"ViewProjection.h"
#include <BaseCollider.h>
#include"Sprite.h"

struct fish {
	WorldTransform pos;	//���[���h���W
	float radian;	//����O���p�̊p�x
	float radius;	//����O���p�̔��a
	float spd;	//�ړ����x
	Vector3 displacement;	//�e���W����̔����Ȃ���
	Vector3 randomVec;

	bool isUpper = false;

	Vector3 beforePos;
	Vector3 controll1;
	Vector3 controll2;
	Vector3 afterPos;
	EasingData easeMove;
};

enum class BossFishPhase {
	Idle,//�ҋ@
	Atk_Sword,//���ό`�U��
	Atk_Rush,//�ːi�U��
	BeginMotion,//���[�V�����J�n����
	Death,//���S

};

enum class BossFishSwordPhase {
	Start,
	Create,
	Cooltime_Create,
	Move,
	Attack,
	Destroy,
	Cooltime_Destroy,
};

class BossFish
{
public:
	static const int fishMaxCount = 200;			//�����̍ő吔
	std::unique_ptr<Model> fishBodyModel = nullptr;	//���̑̃��f��
	std::unique_ptr<Model> fishEyeModel = nullptr;	//���̖ڋʃ��f��

public:
	~BossFish();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="targetPos">�W�I�̍��W</param>
	void Update(const Vector3& targetPos, const Vector3 stagePos, float stageRadius);

	/// <summary>
	/// �����̐���
	/// </summary>
	/// <param name="spawnPos">����������W</param>
	void CreateFish(const Vector3& spawnPos);

	/// <summary>
	/// �{�X����
	/// </summary>
	void Spawn(const Vector3& fishCreatePos);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProMat">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProMat);

	/// <summary>
	/// �̗͂̕`��
	/// </summary>
	void DrawHealth();

	/// <summary>
	/// �_���[�W���󂯂����̃R�[���o�b�N�֐�
	/// </summary>
	/// <param name="atk">�_���[�W��</param>
	void Damage(int atk);

	// �{�X�����S�����玀�S�t�F�[�Y�Ɉڂ�
	void Death();

	//���̐��̃Q�b�^�[
	int GetFishCount() { return fishes.size(); }

	//�����蔻��p���W�̃Q�b�^�[1
	Vector3 GetSwordCollisionCube1()const { return posSwordColCube1; }
	//�����蔻��p���W�̃Q�b�^�[2
	Vector3 GetSwordCollisionCube2()const { return posSwordColCube2; }
	//���̍��W�̃Q�b�^�[
	Matrix4 GetSwordWorldPos() { return swordTransform.matWorld_; }
	//�e���W�̃Q�b�^�\
	Vector3 GetParentPos()const { return fishParent.pos.translation_; }
	int GetHealth()const { return bossHealth; }

	// ���񂾎��̉��o���I����Ă��邩
	bool GetIsDeathEnd()const { return IsDeathEnd; }

	void Reset();

private:
	/// <summary>
	/// ��U����Ԃ̍X�V
	/// </summary>
	void UpdateIdle();

	/// <summary>
	/// ���U���̍X�V
	/// </summary>
	void UpdateAtkSword();

	/// <summary>
	/// �ːi�U���̍X�V
	/// </summary>
	void UpdateAtkRush();

	/// <summary>
	/// �U���O�\������̍X�V
	/// </summary>
	void UpdateBeginMotion();

	/// <summary>
	/// ���S�������̍X�V����
	/// </summary>
	void UpdateDeath();

	/// <summary>
	/// ���̓����蔻��̍X�V
	/// </summary>
	void SwordColCubeUpdate();

	/// <summary>
	/// ���̔z������������ɕ��ׂ�
	/// </summary>
	void SortFishMin();

	/// <summary>
	/// ���̌����̍X�V
	/// </summary>
	void FishDirectionUpdate();

	// �X�v���C�g�̏�����
	void SpriteInitialize();

	//���Ɋւ��铖���蔻��
	void SwordCollisionON();

	void SwordCollisionUpdate();

	void SwordCollisionOFF();

	private:

		fish fishParent;			//���̒��S
		std::vector<fish> fishes;	//���Q�z��
		
		std::unique_ptr<Model> swordModel = nullptr;	//���̃��f���f�[�^
		float randSpdParam = 0;							//�����_���ŕω����鑬�x�̊�{�l
		BossFishPhase phase1;							//�{�X���`�Ԃ̃t�F�[�Y
		const int attackCooltime = 60 * 3;				//���̍U���܂ł̃N�[���^�C��
		const int beginAttackDelay = 60 * 1;			//�U���̗\�����쎞��
		int nextPhaseInterval = 0;						//���̃t�F�[�Y�܂ł̎���
		const int rushMaxCount = 3;						//�ːi�U��(�Г�)�������
		int rushCount = 0;								//�ːi�U���̎c���
		WorldTransform swordTransform;					//���̃��[���h���W
		
		int nextDamageInterval = 30;					//���Ƀ_���[�W���󂯂�܂ł̎���
		int damageTimer = 0;							//�{�X�̖��G����

		// �{�X��HP�֘A
		const float bossHpMax = 20;
		float bossHealth = bossHpMax;							//�{�X��HP

		std::unique_ptr<Sprite> healthSprite;    // HP�̃X�v���C�g
		std::unique_ptr<Sprite> healthAlfaSprite;// HP�̉��̕����̃X�v���C�g

		Vector2 hpSize;                       // Hp�̃X�v���C�g�̃T�C�Y
		Vector2 hpAlfaSize = { 553.0f,25.0f };// Hp�̉��̃X�v���C�g�̃T�C�Y
		bool IsHpAlfa = false;                // Hp�̉��̂��̂�����邩�ǂ���
		int hpAlfaTimer = 0;                  // Hp�̉��̂��̂������o���܂ł̃^�C�}�[

		std::unique_ptr<Sprite> HP_barSprite; // Hp�̃o�[�̃X�v���C�g

		// �{�X�̎��S�����[�V����
		bool IsDeathEnd = false;              // ���S��̉��o���I����Ă��邩
		bool ISDeadCalculation = false;       // ���S��̃x�N�g���̌v�Z��������ďI����Ă��邩�ǂ���
		float fishDeadSpeed = 0.65f;
		std::vector<Vector3>fishDeadVel;      // ���S�㏬�������ł����x�N�g��
		int deathTimer = 0;
		const int deathTimerMax = 100;

		Vector3 targetPos;				//�W�I(�v���C���[)�̍��W
		BossFishSwordPhase bossSwordPhase;	//���̍s���t�F�[�Y
		WorldTransform Transform;		//���̃��[���h���W
		Vector3 swordPos = { 0,0,0 };	//���̍��W
		EasingData easeSwordPos;		//���̍��W�C�[�W���O�p�f�[�^
		EasingData easeSwordScale;		//���̃X�P�[���C�[�W���O�p�f�[�^
		float swordRotAngle = 0;		//���̉�]�p
		const int moveFishMax = 120;	//���̍ő吔
		EasingData easePFishToSword[fishMaxCount];	//���̈ړ��p�C�[�W���O�^�C�}�[
		std::vector<int> choiceFishIndex;			//�z�񂩂牽�Ԗڂ̋����I�΂�Ă��邩(�d���΍�)
		Vector3 parentBeforePos;					//�e���W�̈ړ��O���W(�⊮�p)
		Vector3 parentAfterPos;						//�e���W�̈ړ�����W(�⊮�p)
		Vector3 fishesBeforePos[fishMaxCount];		//�����̈ړ��O���W(�⊮�p)
		Vector3 fishesControllP1[fishMaxCount];		//�����̈ړ�����p���W1(�⊮�p)
		Vector3 fishesControllP2[fishMaxCount];		//�����̈ړ�����p���W2(�⊮�p)
		Vector3 fishesAfterPos[fishMaxCount];		//�����̈ړ�����W(�⊮�p)
		Vector3 beforeScale;						//�ω��O�̃X�P�[��
		Vector3 afterScale;							//�ω���̃X�P�[��
		float lenTargetToFishes[fishMaxCount];		//�����ƕW�I�̋���(�X�J���[)
		uint32_t whiteTexture = 0;	//���F�̃e�N�X�`��
		// �R���C�_�[
		BaseCollider* collider = nullptr;
		float radius = 30.0f;//�����蔻�蔼�a

		//�����蔻��p�̌��̑傫���p�ϐ�
		WorldTransform testTrans;
		WorldTransform testTrans2;
		Vector3 posSwordColCube1;
		Vector3 posSwordColCube2;
		//���̃I�u�W�F�N�g�Ƃ��Ă̑傫��(�����蔻��p)
		const float swordSizeX1 = 1.0f;
		const float swordSizeX2 = 1.0f;
		const float swordSizeY1 = 0.0f;
		const float swordSizeY2 = 0.0f;
		const float swordSizeZ1 = -1.0f - 2.0f;
		const float swordSizeZ2 = 12.4f;

		EasingData easeParentPos;

		Vector3 angleVec{ 0,0,0 };

		int moveFlag = 0;

		uint32_t healthPicture = 0;
		//std::unique_ptr<Sprite> healthSprite;

		static const int SphereCount = 10;

		BaseCollider* AttackCollider[SphereCount];
		Vector3 colliderPos[SphereCount];
		Matrix4 worldSpherePos[SphereCount];
		WorldTransform playerAttackTransformaaaa_[SphereCount];
		std::unique_ptr<Model> startModel;
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

/// <summary>
/// �������̊m����true��Ԃ��֐�
/// </summary>
/// <param name="param"></param>
/// <returns>�����̊m����true</returns>
bool IsPercent(float param = 100.0f);

Matrix4 CreateMatRot(const Vector3& pos, const Vector3& target);