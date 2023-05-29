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

struct BossKingDropEnergy
{
	std::unique_ptr<Model> model;
	WorldTransform WorldTrans;
	// ���̂������̃G�l���M�[���ړ�����t���O
	bool IsKingEnergyMoce = false;

	bool IsZurasi = false;

	float startTaiming;
	float startTimer = 0;

	float easingTimer = 0;
	float easingTimeMax = 80;
	float easingTimeRate = 0;
	Vector3 colPoint;
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
	KingDrop,
};

enum class BossAttackPhase
{
	Before,
	Attack,
	After,
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

#pragma region ���̂������֘A�̕ϐ�
	// ���̂�������ł�
	bool IsKingDrop = false;
	// ���̂�������ł|�W�V�����ɂ��邩�ǂ���
	bool IsKingDropPos = false;
	// ���̂�������łO�̃|�W�V����
	bool IsBeforPos = false;
	// �ړ�����Ƃ��̃t���O
	bool IsMoveBefor = false;
	// �ړ��̂��Ƃ̃t���O
	bool IsMoveAfter = false;
	// ���̂������̃|�W�V�����ɍs���܂ł̃t���O
	bool IsKingUp = false;
	// ���̂������̃G�l���M�[���W�߂�t���O
	bool IsKingEnergy = false;
	// ���̂������̃G�l���M�[�̒e�𐶐����Ă邩�̃t���O
	bool IsKingEnergyBall = false;
	
	// ���̂������̒e��ł����낷���ǂ����̃t���O
	bool IsKingDown = false;

	// ���̂�������łO�̃|�W�V����


	// ���̂�������łƂ��̃|�W�V����
	Vector3 kingDropPos = { 0,20,-250 };

	// ���̂������̈ړ�����Ƃ��̃��̃}�C�i�X�ƃv���X
	float kingDropMoveAlphaM = 0.01;
	float kingDropMoveAlphaP = 0.01;

	//���̂������̓����蔻��
	BaseCollider* KingsDrop;
	float KingAttackRadius = 0.5f;

	// ��������̂Ɏg���G�l���M�[
	int energyNum = 25;
	float energyScale = 0.05f;
	BossKingDropEnergy energyL[25];
	BossKingDropEnergy energyR[25];
	BossKingDropEnergy energyBigBall;
	// �G�l���M�[�e�̒��ɂ�����̂���
	BossKingDropEnergy energyBigBallSub;

	// �G�l���M�[�����̊J�n�ʒu�ƏI�_�ʒu
	Vector3 createEnergyStartLPos = { 1.120f,1.120f,2.17f };
	Vector3 createEnergyStartRPos = { -1.120f,1.120f,2.17f };
	Vector3 createEnergyEndPos = { 0,8.0f,15.84f };
	Vector3 energyVelHozonL;
	Vector3 energyVelHozonR;
	Vector3 EnergyVelL;
	Vector3 EnergyVelR;
	Vector3 energyVelZurasi;

	// �G�l���M�[�̒e�𐶐�����^�C�~���O�����炷�^�C�}�[
	float ballZurasiTimer = 0;
	float ballZurasiTimeMax = 90;
	// �G�l���M�[���̃|�W�V����
	Vector3 energyBallPos;
	// �G�l���M�[���W�߂Đ������鋅�̃X�P�[��
	Vector3 energyBallScale = { 0,0,0 };
	// �G�l���M�[���̐�������Ƃ��̃}�b�N�X�X�P�[��
	Vector3 energyBallMaxScale = { 4.8f,4.8f, 4.8f };
	// �G�l���M�[�̒e����t���[���ɑ�����X�P�[���̗�
	Vector3 energyBallPlusScale = {0.01f,0.01f, 0.01f};

	bool isKingDownAfter=false;


	// �̂̏㏸���E
	float bodyDefultY = 20;
	float bodyUpMaxY = bodyDefultY + 20;

	// �r���΂߂ɏグ�鎞�̊p�x
	Vector3 diagonalRotaL;
	Vector3 diagonalRotaR;

	// �r���|�W�V�������炵
	Vector3 defuPos = { 0,0,0 };
	Vector3 zurasi_R_Pos = { -0.875,0.25f,-0.125f };
	Vector3 zurasi_L_Pos = { 0.875,0.25f,-0.125f };
	// �r�̃C�[�W���O
	float armUpTimer = 0;
	float armUpTimeMax = 360;

	// �r�グ�̃C�[�W���O�|�W�V����
	Vector3 shoulderR_RotaEnd = { 0,90,0 };
	Vector3 elbowR_RotaEnd = { -103.5f,-13.5f,-22.5 };
	Vector3 shoulderL_RotaEnd = { 0,-90,0 };
	Vector3 elbowL_RotaEnd = { -103.5f,13.5f,22.5 };

	// �r�̉�����Ƃ��̃|�W�V����
	Vector3 downShoulder_R_RotaEnd;
	Vector3 downElbow_R_RotaEnd;
	Vector3 downShoulder_L_RotaEnd;
	Vector3 downElbow_L_RotaEnd;

	#pragma region �r��U�艺�낵����֘A�̕ϐ�
	// �r����u�L���āA�G�l���M�[�e�̑傫������u�傫������t���O
	bool IsEnergyBallBig = false;

	// �G�l���M�[�e�����k����t���O�i�r���ꏏ�ɓ����Ɉړ��j
	bool IsEnergyBallCompression = false;

	// �r��U��グ�āA�G�l���M�[�e�𗎂Ƃ������̃t���O
	bool IsEnergyBallFallUp = false;

	// �r��U�艺�낵�āA�G�l���M�[�e�𗎂Ƃ��t���O
	bool IsEnergyBallFallDown = false;

	// �G�l���M�[�e��������ɂՂɂ�Ղɂシ��t���O
	bool IsEnergyBallPyupyu = false;

	// �G�l���M�[�e��������ɑ傫���Ȃ��Ă����t���O
	bool IsEnergyBallFallBig = false;

	// �r����u�傫������Ƃ��̃|�W�V�����Ɖ�]
	Vector3 bigElbowLRot = { -103.5f,31.147f,22.5 };
	Vector3 bigElbowRRot = { -103.5f,-31.147f,-22.5 };
	Vector3 bigElbowLPos = { 0.875,0.25f,-0.221f };
	Vector3 bigElbowRPos = { -0.875,0.25f,-0.221f };
	float armBigTimer = 0;
	float armBigTimeMax = 30;
	Vector3 BigEnergyBallScaleMax = { 5.8f,5.8f, 5.8f };
	Vector3 BigEnergyBallScale = { 4.8f,4.8f, 4.8f };
	Vector3 BigEnergyPlusScale;
	float bigWaitTimer = 0;
	float bigWaitTimeMax = 60 * 0.5;

	// �G�l���M�[�����k���鎞�̂���
	Vector3 comElbowLRot = { -103.5f,-23.988f,22.5f };
	Vector3 comElbowRRot = { -103.5f,23.988f,-22.5f };
	Vector3 comElbowLPos = { 0.875f,0.25f,0.320f };
	Vector3 comElbowRPos = { -0.875f,0.25f,0.320f };
	float armCompressionTimer = 0;
	float armCompressionTimeMax = 60 * 1;
	Vector3 comEnergyBallScaleMax = { 0.2f,0.2f, 0.2f };
	Vector3 comEnergyBallScale={ 5.8f,5.8f, 5.8f };
	Vector3 comEnergyMinusScale;

	// �G�l���M�[�̑O����̂���
	Vector3 fallUpElbowLRot = { -103.5f,-23.988f,63.245f };
	Vector3 fallUpElbowRRot = { -103.5f,23.988f,-63.245f };
	Vector3 fallUpElbowLPos = { 0.875f,0.586f,0.320f };
	Vector3 fallUpElbowRPos = { -0.875f,0.586f,0.320f };
	float armFallUpTimer = 0;
	float armFallUpTimeMax = 60 * 1;

	// �G�l���M�[�e�����낷����̂���
	Vector3 fallDownElbowLRot = { -103.5f,-23.988f,-41.875f };
	Vector3 fallDownElbowRRot = { -103.5f,23.988f,41.875f };
	Vector3 fallDownElbowLPos = { 0.875f,-0.664f,0.320f };
	Vector3 fallDownElbowRPos = { -0.875f,-0.664f,0.320f };
	float armFallDownTimer = 0;
	float armFallDownTimeMax = 60 * 2;
	float armFallDownTimeRate;

	Vector3 energyBallDownMinus = { 0,-0.05f, 0 };
	Vector3 energyBallFallDownPos= { 0,-2.65f,15.84f };
	float energyBallDownTimer = 0;
	float energyBallDownTimeMax = 60 * 3;

	// �G�l���M�[�e�����ɂ傤�ɂ傷�铮���̂���
	int energyUnyoUnyoCount = 0;
	int energyUnyoUnyoCountMax = 2;
	Vector3 energyFirstTransScale;
	Vector3 energyUnyoUnyoScaleDefu = { 2,2,2 };
	Vector3 energyUnyoUnyoScaleKimo = { 1.8f,2.2f,1.8f };
	float energyBallUnyoTimer = 0;
	float energyBallFirstTransTimeMax = 60 * 0.5;
	float energyBallUnyoTimeMax = 2 * 1;
	bool IsEnergyScaleFirstTrans = false;
	bool IsEnergyScaleDefu = false;
	bool IsEnergyScaleKimo = false;

	// ��u�̔�����ʂ̃X�v���C�g
	std::unique_ptr<Sprite> whiteBack;
	std::unique_ptr<Sprite> whiteStarBack1;
	std::unique_ptr<Sprite> whiteStarBack2;
	float whiteTimer = 0;
	float whiteTimeMax = 2;
	int whiteCount = 0;
	Vector4 whiteOutColor = { 0.8f,0.8f,0.8f,0.6f };

	// �G�l���M�[�e��������܂ł̓����̂���
	float energyScaleTransTimer = 0;
	float energyScaleTransTimeMax = 60 * 2;
	float energyScaleTransTimeRate;
	Vector3 energyTransScaleToEnd = { 11,11,11 };

	// ���i�K�̃������������
	float energyAlphaTransTimer = 0;
	float energyAlphaTransTimeMax = 5;
	float energyBallAlphaDefu = 0.95f;
	float energyBallAlphaTrans = 0.3f;

	// �I���Ɏ���܂ł̃������������
	float energyAlphaEndTimer = 0;
	float energyAlphaEndTimeMax = 5;
	float energyBallAlphaInsideEnd = 1.0f;
	float energyBallAlphaTransEnd = 0.0f;

	Sound ArmSwingSE;
	Sound TornadoSE;
	Sound LaunchSwordSE;
	Sound SwordSwingSE;

	#pragma endregion


#pragma endregion

#pragma region 

#pragma endregion


private:
	//�r�U��U���̏�����
	void InitAtkArmSwing();

	//�r�U��U���X�V
	void UpdateAtkArmSwing();

	// ���̂������̍X�V����
	void KingDropUpdate();

	// ���̂������̏�����
	void KingDropReset();

	// �x�����烉�W�A��
	float DegreeToRadian(float degree);


	Vector3 DegreeToRadianVec3(Vector3 degree);
	//���U��U���̏�����
	void InitAtkSwordSwing();

	//���U��U���X�V
	void UpdateAtkSwordSwing();

	//�X�|�[�����̍X�V����
	void UpdateSpawn();

};

//Matrix4 CreateMatRot(const Vector3& pos, const Vector3& target);

float convertDegreeToRadian(float degree);

float convertRadianToDegree(float radian);

Vector3 convertDegreeToRadian(const Vector3& degree);

Vector3 convertRadianToDegree(const Vector3& radian);