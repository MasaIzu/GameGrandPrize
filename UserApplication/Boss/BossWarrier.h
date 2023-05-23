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
	std::unique_ptr<Model> model;
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
	void Initialize();

	void Update(const Vector3& targetPos);

	void Draw(const ViewProjection& viewProMat);
	void SetPlayer(Player* player_) { pl = player_; }

	void MultiLaunchSword();
	void StartMultiLaunchSword();
	void LaunchSword();
	void StartLaunchSword();

	void LaunchSwordDraw(ViewProjection viewProMat);

	void Rota();
private:

	std::unique_ptr<Model> swordModel = nullptr;	//å‰£ã®ãƒ¢ãƒEƒ«ãƒEEã‚¿

	Input* input_ = nullptr;
	Player* pl = nullptr;
	BossWarrierModel boss2Model[BossWarrierPart::Boss2PartMax];	//ãƒœã‚¹ç¬¬äºŒå½¢æ…‹ãEãƒ¢ãƒEƒ«
	std::unique_ptr<Model> boss2TornadeModel;

	std::unique_ptr<Model> ModelSpere;

	//•—‚Ì“–‚½‚è”»’è
	BaseCollider* BossWarrier[BossWarrierPart::Boss2PartMax];
	float BossWarrierRadius = 1.0f;
	WorldTransform modelSpere[BossWarrierPart::Boss2PartMax];

	WorldTransform boss2TornadoTransform[2];
	float TornadoRotY[2];
	float TornadoSpeedRotY = 5;
	bool isTornado = false;
	bool isAtkArmSwing = false;
	float rootRotRad = 0;
	int TornadoFlame = 0;
	bool isLastAtkStart = false;
	int lastAtkCount = 0;
	EasingData easeRotArm;//è…•æŒ¯ã‚Šåˆ¶å¾¡ã®ã‚¤ãƒ¼ã‚¸ãƒ³ã‚°ãƒEEã‚¿
	Vector3 dataRotArm[2];	//ã‚¤ãƒ¼ã‚¸ãƒ³ã‚°ç”¨å›è»¢è§’ãƒ‡ãƒ¼ã‚¿
	Vector3 dataRotElbow[2];
	Vector3 dataRotShoulder[2];
	Vector3 dummyTargetPos;

	//å¼•æ•°ãªã©ã§ã‚‚ã‚‰ã£ã¦ãã‚‹å¤‰æ•°
	Vector3 targetPos = { 0,0,0 };

	Attack attack=Attack::StandBy;

	BossAttackPhase bossAttackPhase = BossAttackPhase::Before;

	EasingData attackEasing;

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

	//•—‚Ì“–‚½‚è”»’è
	BaseCollider* Tornado;
	float TornadoRadius = 1.0f;

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

	// ¶¬‚·‚é‚Ì‚Ég‚¤ƒGƒlƒ‹ƒM[
	int energyNum = 25;
	float energyScale = 0.05f;
	BossKingDropEnergy energyL[25];
	BossKingDropEnergy energyR[25];
	BossKingDropEnergy energyBigBall;

	// ƒGƒlƒ‹ƒM[¶¬‚ÌŠJnˆÊ’u‚ÆI“_ˆÊ’u
	Vector3 createEnergyStartLPos = { 0,0.8f,1.85f };
	Vector3 createEnergyStartRPos = { 0,0.8f,1.85f };
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
	Vector3 energyBallMaxScale = { 2.8f,2.8f, 2.8f };
	// ƒGƒlƒ‹ƒM[‚Ì’e‚ªˆêƒtƒŒ[ƒ€‚É‘‚¦‚éƒXƒP[ƒ‹‚Ì—Ê
	Vector3 energyBallPlusScale = {0.005f,0.005f, 0.005f};


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

#pragma endregion

#pragma region 

#pragma endregion


private:
	//è…•æŒ¯ã‚Šæ”»æ’EEåˆæœŸåŒE
	void InitAtkArmSwing();

	//è…•æŒ¯ã‚Šæ”»æ’EEæ›´æ–°
	void UpdateAtkArmSwing();

	// ‰¤‚Ì‚µ‚¸‚­‚ÌXVˆ—
	void KingDropUpdate();

	// ‰¤‚Ì‚µ‚¸‚­‚Ì‰Šú‰»
	void KingDropInit();

	// “x”‚©‚çƒ‰ƒWƒAƒ“
	float DegreeToRadian(float degree);


	Vector3 DegreeToRadianVec3(Vector3 degree);
};

//Matrix4 CreateMatRot(const Vector3& pos, const Vector3& target);