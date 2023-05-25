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
	//(éª¨)ã¯æç”»ã™ã‚‹
	//(é–¢ç¯€)ã¯æç”»ã—ãªãE
	//éª¨ã¯å‹•ã‹ã•ãšé–¢ç¯€ã®å›è»¢ã§å‹•ã‹ãE
	//enumã®é E•ªã¯å­ã«ãªã‚‹ã«ã¤ã‚Œã¦ä¸‹ã«è¡Œãã‚ˆã†ã«æ±ºã‚ãŸ
	Root,		//å¤§æœ¬(é–¢ç¯€)
	Chest,		//èƒ¸(éª¨)
	Neck,		//é¦Eé–¢ç¯€)
	Head,		//é ­(éª¨)
	ShoulderL,	//å·¦è‚©(é–¢ç¯€)
	ArmL,		//å·¦è…Eéª¨)
	elbowL,		//å·¦è‚Eé–¢ç¯€)
	HandL,		//å·¦æ‰Eéª¨)
	ShoulderR,	//å³è‚©(é–¢ç¯€)
	ArmR,		//å³è…Eéª¨)
	elbowR,		//å³è‚Eé–¢ç¯€)
	HandR,		//å³æ‰Eéª¨)
	Crotch,		//è‚¡(é–¢ç¯€)
	Waist,		//è…°(éª¨)
	Boss2PartMax,	//ãƒœã‚¹ã®éƒ¨ä½æ•°
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

	void Spawn();

	void Update(const Vector3& targetPos);

	void Draw(const ViewProjection& viewProMat);
	void DrawParticle(const ViewProjection& viewProMat);
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
private:

	std::unique_ptr<Model> swordModel = nullptr;	//å‰£ã®ãƒ¢ãƒEƒ«ãƒEEã‚¿

	Input* input_ = nullptr;
	Player* pl = nullptr;
	BossWarrierModel boss2Model[BossWarrierPart::Boss2PartMax];	//ãƒœã‚¹ç¬¬äºŒå½¢æ…‹ãEãƒ¢ãƒEƒ«
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
	EasingData easeRotArm;//è…•æŒ¯ã‚Šåˆ¶å¾¡ã®ã‚¤ãƒ¼ã‚¸ãƒ³ã‚°ãƒEEã‚¿
	Vector3 dataRotArm[2];	//ã‚¤ãƒ¼ã‚¸ãƒ³ã‚°ç”¨å›è»¢è§’ãƒ‡ãƒ¼ã‚¿
	Vector3 dataRotElbow[2];
	Vector3 dataRotShoulder[2];
	Vector3 dummyTargetPos;
	Vector3 swordPos[2];

	Matrix4 matBossDir;
	int atkArmSwingTime=0;

	float swordRad = 0.0f;

	//å¼•æ•°ãªã©ã§ã‚‚ã‚‰ã£ã¦ãã‚‹å¤‰æ•°
	Vector3 targetPos = { 0,0,0 };

	Attack attack=Attack::StandBy;

	BossAttackPhase bossAttackPhase = BossAttackPhase::Before;

	EasingData attackEasing;

	std::unique_ptr<Model> ModelSpere;

	WorldTransform modelSpere[BossWarrierPart::Boss2PartMax];

	//•—‚Ì“–‚½‚è”»’è
	BaseCollider* BossWarrier[BossWarrierPart::Boss2PartMax];
	float BossWarrierRadius = 1.0f;

	//

	//ç¬¬äºŒphaseã®å‰£ã®æŠ•ã’Attackã®å‰£ã®æœ€å¤§æ•°
	static const int MAXSWROD = 5;
	WorldTransform w[MAXSWROD];
	//ç”ŸæEã—ã¦ã‹ã‚‰å‰£ã‚’é£›ãEã™ã¾ã§ã®æ™‚é–“
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

	//Œ•‚Ì“–‚½‚è”»’è
	BaseCollider* AttackCollider[MAXSWROD];
	float AttackRadius = 4.0f;


	std::unique_ptr<Model> boss2TornadeModel;
	WorldTransform boss2TornadoTransform[2];
	float TornadoRotY[2];
	float TornadoSpeedRotY = 5;
	bool isTornado = false;

	//•—‚Ì“–‚½‚è”»’è
	BaseCollider* Tornado;
	float TornadoRadius = 1.0f;

	std::unique_ptr<ParticleManager> spawnParticle;
	int particleCreateTime = 0;
	int spawnAnimationTime = 120;

	bool isAlive = false;;

	int health = 0;

#pragma region ŠZ‚Ì‘Ò‹@ƒ‚[ƒVƒ‡ƒ“Wi‘S‚Ä‚ÌUŒ‚‚Í‚±‚ê‚©‚çn‚Ü‚è‚±‚ê‚ÉI‚í‚é‚æ‚¤‚É“®ì‚ğì‚éj

	const Vector3 StandByNeck = { 0,0,0 };
	const Vector3 StandByShoulderL = { 0,0,-PI/4 };
	const Vector3 StandByShoulderR = { 0,0,PI/4 };
	const Vector3 StandByElbowL = { 0,0,-PI/4 };
	const Vector3 StandByElbowR = { 0,0,PI/4 };
	const Vector3 StandByWaist = { 0,0,0 };
#pragma endregion

private:
	//˜rU‚èUŒ‚‚Ì‰Šú‰»
	void InitAtkArmSwing();

	//˜rU‚èUŒ‚XV
	void UpdateAtkArmSwing();

	//Œ•U‚èUŒ‚‚Ì‰Šú‰»
	void InitAtkSwordSwing();

	//Œ•U‚èUŒ‚XV
	void UpdateAtkSwordSwing();

	//ƒXƒ|[ƒ“‚ÌXVˆ—
	void UpdateSpawn();

};

//Matrix4 CreateMatRot(const Vector3& pos, const Vector3& target);

float convertDegreeToRadian(float degree);

float convertRadianToDegree(float radian);

Vector3 convertDegreeToRadian(const Vector3& degree);

Vector3 convertRadianToDegree(const Vector3& radian);