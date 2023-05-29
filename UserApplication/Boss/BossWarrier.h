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

struct BossKingDropEnergy
{
	std::unique_ptr<Model> model;
	WorldTransform WorldTrans;
	// ‰¤‚Ì‚µ‚¸‚­‚ÌƒGƒlƒ‹ƒM[‚ªˆÚ“®‚·‚éƒtƒ‰ƒO
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
	/// ‘Ì—Í‚Ì•`‰æ
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

	float intervalFrame=0;
	float maxIntervalFrame=120;

	//å¼•æ•°ãªã©ã§ã‚‚ã‚‰ã£ã¦ãã‚‹å¤‰æ•°
	Vector3 targetPos = { 0,0,0 };

	Attack attack=Attack::StandBy;

	Attack oldAttack = Attack::StandBy;

	BossAttackPhase bossAttackPhase = BossAttackPhase::Before;

	EasingData attackEasing;

	std::unique_ptr<Model> ModelSpere;

	WorldTransform modelSpere[BossWarrierPart::Boss2PartMax];

	//•—‚Ì“–‚½‚è”»’è
	BaseCollider* BossWarrier[BossWarrierPart::Boss2PartMax];
	float BossWarrierRadius[BossWarrierPart::Boss2PartMax];

	float rdi3 = 3.0f;
	float rdi5 = 5.0f;
	float rdi8 = 8.0f;

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

	WorldTransform AttackColliderWorldTrans[MAXSWROD];

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

	bool isAlive = false;

#pragma region Hp‚Ü‚í‚è‚Ì•Ï”
	// Hp‚ÌãŒÀ
	float maxHealth = 100;
	// ¡‚ÌHp
	float health = maxHealth;

	std::unique_ptr<Sprite> healthSprite;    // HP‚ÌƒXƒvƒ‰ƒCƒg
	std::unique_ptr<Sprite> healthAlfaSprite;// HP‚Ì‰º‚Ì•”•ª‚ÌƒXƒvƒ‰ƒCƒg

	Vector2 hpSize;                       // Hp‚ÌƒXƒvƒ‰ƒCƒg‚ÌƒTƒCƒY
	Vector2 hpAlfaSize = { 553.0f,25.0f };// Hp‚Ì‰º‚ÌƒXƒvƒ‰ƒCƒg‚ÌƒTƒCƒY
	bool IsHpAlfa = false;                // Hp‚Ì‰º‚Ì‚à‚Ì‚ªŒ»‚ê‚é‚©‚Ç‚¤‚©
	int hpAlfaTimer = 0;                  // Hp‚Ì‰º‚Ì‚à‚Ì‚ª“®‚«o‚·‚Ü‚Å‚Ìƒ^ƒCƒ}[

	std::unique_ptr<Sprite> HP_barSprite; // Hp‚Ìƒo[‚ÌƒXƒvƒ‰ƒCƒg
#pragma endregion


	

#pragma region ŠZ‚Ì‘Ò‹@ƒ‚[ƒVƒ‡ƒ“Wi‘S‚Ä‚ÌUŒ‚‚Í‚±‚ê‚©‚çn‚Ü‚è‚±‚ê‚ÉI‚í‚é‚æ‚¤‚É“®ì‚ğì‚éj

	const Vector3 StandByNeck = { 0,0,0 };
	const Vector3 StandByShoulderL = { 0,0,-PI/4 };
	const Vector3 StandByShoulderR = { 0,0,PI/4 };
	const Vector3 StandByElbowL = { 0,0,-PI/4 };
	const Vector3 StandByElbowR = { 0,0,PI/4 };
	const Vector3 StandByWaist = { 0,0,0 };
#pragma endregion

#pragma region ‰¤‚Ì‚µ‚¸‚­ŠÖ˜A‚Ì•Ï”
	// ‰¤‚Ì‚µ‚¸‚­‚ğ‘Å‚Â‚©
	bool IsKingDrop = false;
	// ‰¤‚Ì‚µ‚¸‚­‚ğ‘Å‚Âƒ|ƒWƒVƒ‡ƒ“‚É‚¢‚é‚©‚Ç‚¤‚©
	bool IsKingDropPos = false;
	// ‰¤‚Ì‚µ‚¸‚­‚ğ‘Å‚Â‘O‚Ìƒ|ƒWƒVƒ‡ƒ“
	bool IsBeforPos = false;
	// ˆÚ“®‚·‚é‚Æ‚«‚Ìƒtƒ‰ƒO
	bool IsMoveBefor = false;
	// ˆÚ“®‚Ì‚ ‚Æ‚Ìƒtƒ‰ƒO
	bool IsMoveAfter = false;
	// ‰¤‚Ì‚µ‚¸‚­‚Ìƒ|ƒWƒVƒ‡ƒ“‚És‚­‚Ü‚Å‚Ìƒtƒ‰ƒO
	bool IsKingUp = false;
	// ‰¤‚Ì‚µ‚¸‚­‚ÌƒGƒlƒ‹ƒM[‚ğW‚ß‚éƒtƒ‰ƒO
	bool IsKingEnergy = false;
	// ‰¤‚Ì‚µ‚¸‚­‚ÌƒGƒlƒ‹ƒM[‚Ì’e‚ğ¶¬‚µ‚Ä‚é‚©‚Ìƒtƒ‰ƒO
	bool IsKingEnergyBall = false;
	
	// ‰¤‚Ì‚µ‚¸‚­‚Ì’e‚ğ‘Å‚¿‰º‚ë‚·‚©‚Ç‚¤‚©‚Ìƒtƒ‰ƒO
	bool IsKingDown = false;

	// ‰¤‚Ì‚µ‚¸‚­‚ğ‘Å‚Â‘O‚Ìƒ|ƒWƒVƒ‡ƒ“


	// ‰¤‚Ì‚µ‚¸‚­‚ğ‘Å‚Â‚Æ‚«‚Ìƒ|ƒWƒVƒ‡ƒ“
	Vector3 kingDropPos = { 0,20,-250 };

	// ‰¤‚Ì‚µ‚¸‚­‚ÌˆÚ“®‚·‚é‚Æ‚«‚Ìƒ¿‚Ìƒ}ƒCƒiƒX‚Æƒvƒ‰ƒX
	float kingDropMoveAlphaM = 0.01;
	float kingDropMoveAlphaP = 0.01;

	//‰¤‚Ì‚µ‚¸‚­‚Ì“–‚½‚è”»’è
	BaseCollider* KingsDrop;
	float KingAttackRadius = 0.5f;

	// ¶¬‚·‚é‚Ì‚Ég‚¤ƒGƒlƒ‹ƒM[
	int energyNum = 25;
	float energyScale = 0.05f;
	BossKingDropEnergy energyL[25];
	BossKingDropEnergy energyR[25];
	BossKingDropEnergy energyBigBall;
	// ƒGƒlƒ‹ƒM[’e‚Ì’†‚É‚à‚¤ˆê‚Â‚Ì‚à‚Ì
	BossKingDropEnergy energyBigBallSub;

	// ƒGƒlƒ‹ƒM[¶¬‚ÌŠJnˆÊ’u‚ÆI“_ˆÊ’u
	Vector3 createEnergyStartLPos = { 1.120f,1.120f,2.17f };
	Vector3 createEnergyStartRPos = { -1.120f,1.120f,2.17f };
	Vector3 createEnergyEndPos = { 0,8.0f,15.84f };
	Vector3 energyVelHozonL;
	Vector3 energyVelHozonR;
	Vector3 EnergyVelL;
	Vector3 EnergyVelR;
	Vector3 energyVelZurasi;

	// ƒGƒlƒ‹ƒM[‚Ì’e‚ğ¶¬‚·‚éƒ^ƒCƒ~ƒ“ƒO‚ğ‚¸‚ç‚·ƒ^ƒCƒ}[
	float ballZurasiTimer = 0;
	float ballZurasiTimeMax = 90;
	// ƒGƒlƒ‹ƒM[‹…‚Ìƒ|ƒWƒVƒ‡ƒ“
	Vector3 energyBallPos;
	// ƒGƒlƒ‹ƒM[‚ğW‚ß‚Ä¶¬‚·‚é‹…‚ÌƒXƒP[ƒ‹
	Vector3 energyBallScale = { 0,0,0 };
	// ƒGƒlƒ‹ƒM[‹…‚Ì¶¬‚·‚é‚Æ‚«‚Ìƒ}ƒbƒNƒXƒXƒP[ƒ‹
	Vector3 energyBallMaxScale = { 4.8f,4.8f, 4.8f };
	// ƒGƒlƒ‹ƒM[‚Ì’e‚ªˆêƒtƒŒ[ƒ€‚É‘‚¦‚éƒXƒP[ƒ‹‚Ì—Ê
	Vector3 energyBallPlusScale = {0.01f,0.01f, 0.01f};

	bool isKingDownAfter=false;


	// ‘Ì‚Ìã¸ŒÀŠE
	float bodyDefultY = 20;
	float bodyUpMaxY = bodyDefultY + 20;

	// ˜r‚ğÎ‚ß‚Éã‚°‚é‚ÌŠp“x
	Vector3 diagonalRotaL;
	Vector3 diagonalRotaR;

	// ˜r‚ğƒ|ƒWƒVƒ‡ƒ“‚¸‚ç‚µ
	Vector3 defuPos = { 0,0,0 };
	Vector3 zurasi_R_Pos = { -0.875,0.25f,-0.125f };
	Vector3 zurasi_L_Pos = { 0.875,0.25f,-0.125f };
	// ˜r‚ÌƒC[ƒWƒ“ƒO
	float armUpTimer = 0;
	float armUpTimeMax = 360;

	// ˜rã‚°‚ÌƒC[ƒWƒ“ƒOƒ|ƒWƒVƒ‡ƒ“
	Vector3 shoulderR_RotaEnd = { 0,90,0 };
	Vector3 elbowR_RotaEnd = { -103.5f,-13.5f,-22.5 };
	Vector3 shoulderL_RotaEnd = { 0,-90,0 };
	Vector3 elbowL_RotaEnd = { -103.5f,13.5f,22.5 };

	// ˜r‚Ì‰º‚°‚é‚Æ‚«‚Ìƒ|ƒWƒVƒ‡ƒ“
	Vector3 downShoulder_R_RotaEnd;
	Vector3 downElbow_R_RotaEnd;
	Vector3 downShoulder_L_RotaEnd;
	Vector3 downElbow_L_RotaEnd;

	#pragma region ˜r‚ğU‚è‰º‚ë‚µ‚½ŒãŠÖ˜A‚Ì•Ï”
	// ˜r‚ğˆêuL‚°‚ÄAƒGƒlƒ‹ƒM[’e‚Ì‘å‚«‚³‚àˆêu‘å‚«‚­‚·‚éƒtƒ‰ƒO
	bool IsEnergyBallBig = false;

	// ƒGƒlƒ‹ƒM[’e‚ğˆ³k‚·‚éƒtƒ‰ƒOi˜r‚àˆê‚É“à‘¤‚ÉˆÚ“®j
	bool IsEnergyBallCompression = false;

	// ˜r‚ğU‚èã‚°‚ÄAƒGƒlƒ‹ƒM[’e‚ğ—‚Æ‚·€”õ‚Ìƒtƒ‰ƒO
	bool IsEnergyBallFallUp = false;

	// ˜r‚ğU‚è‰º‚ë‚µ‚ÄAƒGƒlƒ‹ƒM[’e‚ğ—‚Æ‚·ƒtƒ‰ƒO
	bool IsEnergyBallFallDown = false;

	// ƒGƒlƒ‹ƒM[’e‚ª—‰ºŒã‚É‚Õ‚É‚å‚Õ‚É‚ã‚·‚éƒtƒ‰ƒO
	bool IsEnergyBallPyupyu = false;

	// ƒGƒlƒ‹ƒM[’e‚ª—‰ºŒã‚É‘å‚«‚­‚È‚Á‚Ä‚¢‚­ƒtƒ‰ƒO
	bool IsEnergyBallFallBig = false;

	// ˜r‚ğˆêu‘å‚«‚­‚·‚é‚Æ‚«‚Ìƒ|ƒWƒVƒ‡ƒ“‚Æ‰ñ“]
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

	// ƒGƒlƒ‹ƒM[‚ğˆ³k‚·‚é‚Ì‚à‚Ì
	Vector3 comElbowLRot = { -103.5f,-23.988f,22.5f };
	Vector3 comElbowRRot = { -103.5f,23.988f,-22.5f };
	Vector3 comElbowLPos = { 0.875f,0.25f,0.320f };
	Vector3 comElbowRPos = { -0.875f,0.25f,0.320f };
	float armCompressionTimer = 0;
	float armCompressionTimeMax = 60 * 1;
	Vector3 comEnergyBallScaleMax = { 0.2f,0.2f, 0.2f };
	Vector3 comEnergyBallScale={ 5.8f,5.8f, 5.8f };
	Vector3 comEnergyMinusScale;

	// ƒGƒlƒ‹ƒM[‚Ì‘O“®ì‚Ì‚à‚Ì
	Vector3 fallUpElbowLRot = { -103.5f,-23.988f,63.245f };
	Vector3 fallUpElbowRRot = { -103.5f,23.988f,-63.245f };
	Vector3 fallUpElbowLPos = { 0.875f,0.586f,0.320f };
	Vector3 fallUpElbowRPos = { -0.875f,0.586f,0.320f };
	float armFallUpTimer = 0;
	float armFallUpTimeMax = 60 * 1;

	// ƒGƒlƒ‹ƒM[’e‚ğ‚¨‚ë‚·“®ì‚Ì‚à‚Ì
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

	// ƒGƒlƒ‹ƒM[’e‚ª‚¤‚É‚å‚¤‚É‚å‚·‚é“®‚«‚Ì‚à‚Ì
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

	// ˆêu‚Ì”’‚¢‰æ–Ê‚ÌƒXƒvƒ‰ƒCƒg
	std::unique_ptr<Sprite> whiteBack;
	std::unique_ptr<Sprite> whiteStarBack1;
	std::unique_ptr<Sprite> whiteStarBack2;
	float whiteTimer = 0;
	float whiteTimeMax = 2;
	int whiteCount = 0;
	Vector4 whiteOutColor = { 0.8f,0.8f,0.8f,0.6f };

	// ƒGƒlƒ‹ƒM[’e‚ªÁ‚¦‚é‚Ü‚Å‚Ì“®‚«‚Ì‚à‚Ì
	float energyScaleTransTimer = 0;
	float energyScaleTransTimeMax = 60 * 2;
	float energyScaleTransTimeRate;
	Vector3 energyTransScaleToEnd = { 11,11,11 };

	// ‘æˆê’iŠK‚Ìƒ¿‚ğ‰º‚°‚é‚à‚Ì
	float energyAlphaTransTimer = 0;
	float energyAlphaTransTimeMax = 5;
	float energyBallAlphaDefu = 0.95f;
	float energyBallAlphaTrans = 0.3f;

	// I‚í‚è‚ÉŠ‚é‚Ü‚Å‚Ìƒ¿‚ğ‰º‚°‚é‚à‚Ì
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
	//˜rU‚èUŒ‚‚Ì‰Šú‰»
	void InitAtkArmSwing();

	//˜rU‚èUŒ‚XV
	void UpdateAtkArmSwing();

	// ‰¤‚Ì‚µ‚¸‚­‚ÌXVˆ—
	void KingDropUpdate();

	// ‰¤‚Ì‚µ‚¸‚­‚Ì‰Šú‰»
	void KingDropReset();

	// “x”‚©‚çƒ‰ƒWƒAƒ“
	float DegreeToRadian(float degree);


	Vector3 DegreeToRadianVec3(Vector3 degree);
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