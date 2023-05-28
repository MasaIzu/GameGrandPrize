#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>
#include"EasingData.h"
#include"ViewProjection.h"
#include <BaseCollider.h>
#include"Sprite.h"

#include "Player.h"
#include "Input.h"
#include"EasingData.h"


enum BossWarrierPart {
	//(骨)は描画する
	//(関節)は描画しなぁE
	//骨は動かさず関節の回転で動かぁE
	//enumの頁E��は子になるにつれて下に行くように決めた
	Root,		//大本(関節)
	Chest,		//胸(骨)
	Head,		//頭(骨)
	ShoulderL,	//左肩(関節)
	ArmL,		//左腁E骨)
	elbowL,		//左肁E関節)
	HandL,		//左扁E骨)
	ShoulderR,	//右肩(関節)
	ArmR,		//右腁E骨)
	elbowR,		//右肁E関節)
	HandR,		//右扁E骨)
	Crotch,		//股(関節)
	Waist,		//腰(骨)
	Boss2PartMax,	//ボスの部位数
};

struct BossWarrierModel {
	Model* model;
	WorldTransform Transform;
	bool isDraw;
};

enum class Attack
{
	StandBy,
	ArmSwing,
	Tornado,
	MultiLaunchSword,
	LaunchSword,
	SwordSwing,
	Spawm,
	Approach,
	Death,
};

enum class BossAttackPhase
{
	Before,
	Attack,
	After,
};

enum class BossDeathPhase {
	RiseBody,
	BreakBody,
	FallHead,
	MotionEnd,
	BossDeathPhaseMax,
	BossDeathPhaseIncrement = 1,
};

enum class MovePartIndex {
	Chest,
	Waist,
	ArmL,
	ArmR,
	HandL,
	HandR,
	MovePartMax,
};

class BossWarrier
{

public:
	~BossWarrier();

	void Initialize();

	void Spawn(const Vector3& boss1Pos);

	void Update(const Vector3& targetPos);

	void Draw(const ViewProjection& viewProMat);
	void DrawParticle(const ViewProjection& viewProMat);
	/// <summary>
	/// �̗͂̕`��
	/// </summary>
	void DrawHealth();

	void SetPlayer(Player* player_) { pl = player_; }

	void MultiLaunchSword();
	void StartMultiLaunchSword();
	void LaunchSword();
	void StartLaunchSword();
	void BossTornado();

	void LaunchSwordDraw(ViewProjection viewProMat);

	void Rota();

	EasingData GetEasingData()const { return easeRotArm; }

	WorldTransform GetRootTransform() const {return boss2Model[BossWarrierPart::Root].Transform; }

	bool GetAlive() const { return isAlive; }

	void Damage(int damage);

	void reset();
private:

	std::unique_ptr<Model> swordModel = nullptr;	//剣のモチE��チE�Eタ

	Input* input_ = nullptr;
	Player* pl = nullptr;
	BossWarrierModel boss2Model[BossWarrierPart::Boss2PartMax];	//ボス第二形態�EモチE��
	std::unique_ptr<Model> bossArmLModel_Gu;
	std::unique_ptr<Model> bossArmLModel_Pa;
	std::unique_ptr<Model> bossArmLModel;
	std::unique_ptr<Model> bossArmRModel_Gu;
	std::unique_ptr<Model> bossArmRModel_Pa;
	std::unique_ptr<Model> bossArmRModel;

	bool isAtkArmSwing = false;
	int atkStartTime = 0;

	bool isAfter = false;
	float rootRotRad = 0;
	int TornadoFlame = 0;
	bool isLastAtkStart = false;
	int lastAtkCount = 0;
	EasingData easeRotArm;//腕振り制御のイージングチE�Eタ
	Vector3 dataRotArm[2];	//イージング用回転角データ
	Vector3 dataRotElbow[2];
	Vector3 dataRotShoulder[2];
	Vector3 dummyTargetPos;
	Vector3 swordPos[2];

	Matrix4 matBossDir;
	int atkArmSwingTime=0;

	float swordRad = 0.0f;

	float intervalFrame=0;
	float maxIntervalFrame=120;

	//引数などでもらってくる変数
	Vector3 targetPos = { 0,0,0 };

	Attack attack=Attack::StandBy;

	Attack oldAttack = Attack::StandBy;

	BossAttackPhase bossAttackPhase = BossAttackPhase::Before;

	EasingData attackEasing;

	std::unique_ptr<Model> ModelSpere;

	WorldTransform modelSpere[BossWarrierPart::Boss2PartMax];

	//���̓����蔻��
	BaseCollider* BossWarrier[BossWarrierPart::Boss2PartMax];
	float BossWarrierRadius[BossWarrierPart::Boss2PartMax];

	float rdi3 = 3.0f;
	float rdi5 = 5.0f;
	float rdi8 = 8.0f;

	//

	//第二phaseの剣の投げAttackの剣の最大数
	static const int MAXSWROD = 5;
	WorldTransform w[MAXSWROD];
	//生�Eしてから剣を飛�Eすまでの時間
	int phase2AttackCoolTime = 70;
	bool t;
	bool t2;
	WorldTransform pPos[MAXSWROD];
	WorldTransform num[MAXSWROD];
	int interval = 10;
	float moveSpeed = 0.2f;
	bool isSat = false;
	bool isSat2 = false;
	bool isOn = false;
	bool isShot[MAXSWROD];
	const int MAXSHOTTIME = 40;
	int shotTime = MAXSHOTTIME;

	bool kenrot[MAXSWROD];

	//���̓����蔻��
	BaseCollider* AttackCollider[MAXSWROD];

	WorldTransform AttackColliderWorldTrans[MAXSWROD];

	float AttackRadius = 4.0f;


	std::unique_ptr<Model> boss2TornadeModel;
	WorldTransform boss2TornadoTransform[2];
	float TornadoRotY[2];
	float TornadoSpeedRotY = 5;
	bool isTornado = false;

	//���̓����蔻��
	BaseCollider* Tornado;
	float TornadoRadius = 1.0f;

	std::unique_ptr<ParticleManager> spawnParticle;
	int particleCreateTime = 0;
	int spawnAnimationTime = 120;
	int deathParticleInterval = 0;
	const int deathParticleCooltime = 30;
	EasingData easeBossRoot;
	EasingData easePart[(int)BossDeathPhase::BossDeathPhaseMax];;
	BossDeathPhase bossDeathPhase;
	Vector3 partPos[(int)MovePartIndex::MovePartMax];

	bool isAlive = false;

#pragma region Hp�܂��̕ϐ�
	// Hp�̏��
	float maxHealth = 100;
	// ����Hp
	float health = maxHealth;

	std::unique_ptr<Sprite> healthSprite;    // HP�̃X�v���C�g
	std::unique_ptr<Sprite> healthAlfaSprite;// HP�̉��̕����̃X�v���C�g

	Vector2 hpSize;                       // Hp�̃X�v���C�g�̃T�C�Y
	Vector2 hpAlfaSize = { 553.0f,25.0f };// Hp�̉��̃X�v���C�g�̃T�C�Y
	bool IsHpAlfa = false;                // Hp�̉��̂��̂�����邩�ǂ���
	int hpAlfaTimer = 0;                  // Hp�̉��̂��̂������o���܂ł̃^�C�}�[

	std::unique_ptr<Sprite> HP_barSprite; // Hp�̃o�[�̃X�v���C�g
#pragma endregion


	

#pragma region �Z�̑ҋ@���[�V�����W�i�S�Ă̍U���͂��ꂩ��n�܂肱��ɏI���悤�ɓ�������j

	const Vector3 StandByNeck = { 0,0,0 };
	const Vector3 StandByShoulderL = { 0,0,-PI/4 };
	const Vector3 StandByShoulderR = { 0,0,PI/4 };
	const Vector3 StandByElbowL = { 0,0,-PI/4 };
	const Vector3 StandByElbowR = { 0,0,PI/4 };
	const Vector3 StandByWaist = { 0,0,0 };
#pragma endregion

private:
	//�r�U��U���̏�����
	void InitAtkArmSwing();

	//�r�U��U���X�V
	void UpdateAtkArmSwing();

	//���U��U���̏�����
	void InitAtkSwordSwing();

	//���U��U���X�V
	void UpdateAtkSwordSwing();

	//�X�|�[�����̍X�V����
	void UpdateSpawn();

	//���S���[�V����������
	void InitDeath();

	//���S���[�V�����X�V
	void UpdateDeath();

	//�e���ʂ̗����n�_�v�Z
	void CalcFallPos();

};

//Matrix4 CreateMatRot(const Vector3& pos, const Vector3& target);

float convertDegreeToRadian(float degree);

float convertRadianToDegree(float radian);

Vector3 convertDegreeToRadian(const Vector3& degree);

Vector3 convertRadianToDegree(const Vector3& radian);