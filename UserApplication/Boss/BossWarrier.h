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
	Neck,		//馁E関節)
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
	std::unique_ptr<Model> model;
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
	void BossTornado();

	void LaunchSwordDraw(ViewProjection viewProMat);

	void Rota();
private:

	std::unique_ptr<Model> swordModel = nullptr;	//剣のモチE��チE�Eタ

	Input* input_ = nullptr;
	Player* pl = nullptr;
	BossWarrierModel boss2Model[BossWarrierPart::Boss2PartMax];	//ボス第二形態�EモチE��

	WorldTransform boss2TornadoTransform[2];
	float TornadoRotY[2];
	float TornadoSpeedRotY = 5;
	bool isTornado = false;
	bool isAtkArmSwing = false;
	float rootRotRad = 0;
	int TornadoFlame = 0;
	bool isLastAtkStart = false;
	int lastAtkCount = 0;
	EasingData easeRotArm;//腕振り制御のイージングチE�Eタ
	Vector3 dataRotArm[2];	//イージング用回転角データ
	Vector3 dataRotElbow[2];
	Vector3 dataRotShoulder[2];
	Vector3 dummyTargetPos;

	int atkArmSwingTime=0;

	//引数などでもらってくる変数
	Vector3 targetPos = { 0,0,0 };

	Attack attack=Attack::StandBy;

	BossAttackPhase bossAttackPhase = BossAttackPhase::Before;

	EasingData attackEasing;

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
	float AttackRadius = 4.0f;


	std::unique_ptr<Model> boss2TornadeModel;
	WorldTransform boss2TornadoTransform[2];
	float TornadoRotY[2];
	float TornadoSpeedRotY = 5;
	bool isTornado = false;

	//���̓����蔻��
	BaseCollider* Tornado;
	float TornadoRadius = 1.0f;



#pragma region �Z�̑ҋ@���[�V�����W�i�S�Ă̍U���͂��ꂩ��n�܂肱��ɏI���悤�ɓ�������j

	const Vector3 StandByNeck = { 0,0,0 };
	const Vector3 StandByShoulderL = { 0,0,-PI/4 };
	const Vector3 StandByShoulderR = { 0,0,PI/4 };
	const Vector3 StandByElbowL = { 0,0,-PI/4 };
	const Vector3 StandByElbowR = { 0,0,PI/4 };
	const Vector3 StandByWaist = { 0,0,0 };
#pragma endregion

private:
	//腕振り攻撁E�E初期匁E
	void InitAtkArmSwing();

	//腕振り攻撁E�E更新
	void UpdateAtkArmSwing();

};

//Matrix4 CreateMatRot(const Vector3& pos, const Vector3& target);