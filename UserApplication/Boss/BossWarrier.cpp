#include "BossWarrier.h"
#include"Input.h"
#include"ImGuiManager.h"
#include"BossFish.h"
#include <CollisionManager.h>
#include <CollisionAttribute.h>
#include<random>

BossWarrier::~BossWarrier()
{
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		if (i != BossWarrierPart::HandL && i != BossWarrierPart::HandR)
		{
			delete boss2Model[i].model;
		}
	}
}

void BossWarrier::Initialize()
{
	swordModel.reset(Model::CreateFromOBJ("BigSowrd", true));
	input_->GetInstance();
	//ボス2のモデル初期化
	//Rootは大本で動かす親用なので空データでOK
	boss2Model[BossWarrierPart::Chest].model = Model::CreateFromOBJ("Boss_Body", true);
	boss2Model[BossWarrierPart::Chest].isDraw = true;
	boss2Model[BossWarrierPart::Head].model = Model::CreateFromOBJ("Boss_Head", true);
	boss2Model[BossWarrierPart::Head].isDraw = true;
	boss2Model[BossWarrierPart::Waist].model = Model::CreateFromOBJ("Boss_Waist", true);
	boss2Model[BossWarrierPart::Waist].isDraw = true;
	boss2Model[BossWarrierPart::ArmL].model = Model::CreateFromOBJ("Boss_ShoulderL", true);
	boss2Model[BossWarrierPart::ArmL].isDraw = true;
	boss2Model[BossWarrierPart::ArmR].model = Model::CreateFromOBJ("Boss_ShoulderR", true);
	boss2Model[BossWarrierPart::ArmR].isDraw = true;

	bossArmLModel_Gu.reset(Model::CreateFromOBJ("Boss_ArmL_Gu", true));
	bossArmLModel_Pa.reset(Model::CreateFromOBJ("Boss_ArmL_Pa", true));
	bossArmLModel.reset(Model::CreateFromOBJ("Boss_ArmL", true));
	bossArmRModel_Gu.reset(Model::CreateFromOBJ("Boss_ArmR_Gu", true));
	bossArmRModel_Pa.reset(Model::CreateFromOBJ("Boss_ArmR_Pa", true));
	bossArmRModel.reset(Model::CreateFromOBJ("Boss_ArmR", true));

	boss2Model[BossWarrierPart::HandL].model = bossArmLModel.get();
	boss2Model[BossWarrierPart::HandL].isDraw = true;
	boss2Model[BossWarrierPart::HandR].model = bossArmRModel.get();
	boss2Model[BossWarrierPart::HandR].isDraw = true;

	ModelSpere.reset(Model::CreateFromOBJ("sphere", true));

	//ボス第二形態の各部位初期化
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Model[i].Transform.Initialize();

		modelSpere[i].Initialize();

	}

	{//当たり判定半径
		//1
		BossWarrierRadius[BossWarrierPart::Root] = rdi3;
		//2
		BossWarrierRadius[BossWarrierPart::Chest] = rdi8 + 4.0f;
		//3
		BossWarrierRadius[BossWarrierPart::Head] = rdi3;
		//4
		BossWarrierRadius[BossWarrierPart::ShoulderL] = rdi3;
		//5
		BossWarrierRadius[BossWarrierPart::ArmL] = rdi3;
		//6
		BossWarrierRadius[BossWarrierPart::elbowL] = rdi3;
		//7
		BossWarrierRadius[BossWarrierPart::HandL] = rdi8;
		//8
		BossWarrierRadius[BossWarrierPart::ShoulderR] = rdi3;
		//9
		BossWarrierRadius[BossWarrierPart::ArmR] = rdi3;
		//10
		BossWarrierRadius[BossWarrierPart::elbowR] = rdi3;
		//11
		BossWarrierRadius[BossWarrierPart::HandR] = rdi8;
		//12
		BossWarrierRadius[BossWarrierPart::Crotch] = rdi5;
		//13
		BossWarrierRadius[BossWarrierPart::Waist] = rdi8;


		for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
			BossWarrier[i] = new SphereCollider(Vector4(0, BossWarrierRadius[i], 0, 0), BossWarrierRadius[i]);
			CollisionManager::GetInstance()->AddCollider(BossWarrier[i]);
			BossWarrier[i]->SetAttribute(COLLISION_ATTR_ENEMYRECEPTION);
			BossWarrier[i]->Update(boss2Model[i].Transform.matWorld_);
		}

	}

	//胸は大本を親に持つ
	boss2Model[BossWarrierPart::Chest].Transform.parent_ = &boss2Model[BossWarrierPart::Root].Transform;

	//親子関係ツリー(親→子)
	//	→頭
	//胸→股　→腰
	//　→両肩→両腕→両肘→両手


	boss2Model[BossWarrierPart::Head].Transform.parent_ = &boss2Model[BossWarrierPart::Chest].Transform;
	//腰→股→胸
	boss2Model[BossWarrierPart::Crotch].Transform.parent_ = &boss2Model[BossWarrierPart::Chest].Transform;
	boss2Model[BossWarrierPart::Waist].Transform.parent_ = &boss2Model[BossWarrierPart::Crotch].Transform;
	//両手→両肘→両腕→両肩→胸
	boss2Model[BossWarrierPart::ShoulderL].Transform.parent_ = &boss2Model[BossWarrierPart::Chest].Transform;
	boss2Model[BossWarrierPart::ArmL].Transform.parent_ = &boss2Model[BossWarrierPart::ShoulderL].Transform;
	boss2Model[BossWarrierPart::ShoulderR].Transform.parent_ = &boss2Model[BossWarrierPart::Chest].Transform;
	boss2Model[BossWarrierPart::ArmR].Transform.parent_ = &boss2Model[BossWarrierPart::ShoulderR].Transform;
	boss2Model[BossWarrierPart::elbowL].Transform.parent_ = &boss2Model[BossWarrierPart::ArmL].Transform;
	boss2Model[BossWarrierPart::HandL].Transform.parent_ = &boss2Model[BossWarrierPart::elbowL].Transform;
	boss2Model[BossWarrierPart::elbowR].Transform.parent_ = &boss2Model[BossWarrierPart::ArmR].Transform;
	boss2Model[BossWarrierPart::HandR].Transform.parent_ = &boss2Model[BossWarrierPart::elbowR].Transform;

	//ボスのスケールを5倍に
	boss2Model[BossWarrierPart::Root].Transform.scale_ = { 15,15,15 };
	boss2Model[BossWarrierPart::Root].Transform.translation_ = { 50,20,50 };
	//boss2Model[BossWarrierPart::Root].Transform.SetRot({1.57,0,0});
	//それぞれの部位の位置をセット
	boss2Model[BossWarrierPart::Head].Transform.translation_ = { 0,0.9,0.5 };
	boss2Model[BossWarrierPart::ShoulderL].Transform.translation_ = { 0.9,0.3,0 };
	boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot({ 0,0,-PI / 4 });
	boss2Model[BossWarrierPart::ArmL].Transform.translation_ = { 0.2,0,0 };
	boss2Model[BossWarrierPart::ShoulderR].Transform.translation_ = { -0.9,0.3,0 };
	boss2Model[BossWarrierPart::ShoulderR].Transform.SetRot({ 0,0,PI / 4 });
	boss2Model[BossWarrierPart::ArmR].Transform.translation_ = { -0.2,0,0 };
	boss2Model[BossWarrierPart::elbowL].Transform.translation_ = { 0.2,0,0 };
	boss2Model[BossWarrierPart::elbowL].Transform.SetRot({ 0,0,-PI / 4 });
	boss2Model[BossWarrierPart::HandL].Transform.translation_ = { 0.7,0,0 };
	boss2Model[BossWarrierPart::elbowR].Transform.translation_ = { -0.2,0,0 };
	boss2Model[BossWarrierPart::elbowR].Transform.SetRot({ 0,0,PI / 4 });
	boss2Model[BossWarrierPart::HandR].Transform.translation_ = { -0.7,0,0 };
	boss2Model[BossWarrierPart::Crotch].Transform.translation_ = { 0,-0.5,0 };
	boss2Model[BossWarrierPart::Waist].Transform.translation_ = { 0,-0.5,0 };

	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Model[i].Transform.TransferMatrix();
		BossWarrier[i]->Update(boss2Model[i].Transform.matWorld_);
		modelSpere[i].translation_ = MyMath::GetWorldTransform(boss2Model[i].Transform.matWorld_);
		modelSpere[i].scale_ = Vector3{ BossWarrierRadius[i],BossWarrierRadius[i] ,BossWarrierRadius[i] };
		modelSpere[i].TransferMatrix();
	}

	boss2TornadeModel.reset(Model::CreateFromOBJ("tornadoGame", true));

	boss2TornadoTransform[0].Initialize();
	boss2TornadoTransform[0].scale_ = { 1,10,1 };
	boss2TornadoTransform[0].translation_ = { 0,-10,0 };
	boss2TornadoTransform[0].TransferMatrix();
	boss2TornadoTransform[1].Initialize();
	boss2TornadoTransform[1].scale_ = { 1,10,1 };
	boss2TornadoTransform[1].translation_ = { 0,-10,0 };
	boss2TornadoTransform[1].TransferMatrix();

	boss2TornadoTransform[0].alpha = 0.6;
	boss2TornadoTransform[1].alpha = 0.6;

	TornadoRotY[0] = 0;

	TornadoRotY[1] = 3.14;

	for (int i = 0; i < MAXSWROD; i++)
	{
		w[i].Initialize();
		num[i].Initialize();
		AttackColliderWorldTrans[i].Initialize();
		// コリジョンマネージャに追加
		if (AttackCollider[i] == nullptr) {
			AttackCollider[i] = new SphereCollider(Vector4(0, AttackRadius, 0, 0), AttackRadius);
			CollisionManager::GetInstance()->AddCollider(AttackCollider[i]);
		}
		AttackColliderWorldTrans[i].matWorld_ = w[i].matWorld_;

		AttackCollider[i]->SetAttribute(COLLISION_ATTR_NOTATTACK);
		AttackCollider[i]->Update(AttackColliderWorldTrans[i].matWorld_);
	}

	if (Tornado == nullptr) {
		Tornado = new SphereCollider(Vector4(0, TornadoRadius, 0, 0), TornadoRadius);
		CollisionManager::GetInstance()->AddCollider(Tornado);
	}
	Tornado->SetAttribute(COLLISION_ATTR_ENEMYTORNADOATTACK);
	Tornado->Update(boss2TornadoTransform[0].matWorld_, TornadoRadius);


	// エネルギーの初期化
	for (int i = 0; i < energyNum; i++) {
		energyL[i].WorldTrans.Initialize();
		energyL[i].WorldTrans.parent_ = &boss2Model[BossWarrierPart::Root].Transform;
		energyL[i].WorldTrans.scale_ = { energyScale,energyScale,energyScale };
		energyL[i].WorldTrans.alpha = 0.9f;
		energyL[i].model.reset(Model::CreateFromOBJ("sphere", true));
		energyL[i].startTaiming = Random(3, 30);
	}
	for (int i = 0; i < energyNum; i++) {
		energyR[i].WorldTrans.Initialize();
		energyR[i].WorldTrans.parent_ = &boss2Model[BossWarrierPart::Root].Transform;
		energyR[i].WorldTrans.scale_ = { energyScale,energyScale,energyScale };
		energyR[i].WorldTrans.alpha = 0.9f;
		energyR[i].model.reset(Model::CreateFromOBJ("sphere", true));
		energyR[i].startTaiming = Random(3, 30);
	}
	//IsKingDrop = true;
	//IsKingUp = true;

	spawnParticle = std::make_unique<ParticleManager>();
	spawnParticle->Initialize();
	spawnParticle->SetTextureHandle(TextureManager::Load("effect4.png"));

	swordModel->SetPolygonExplosion({ 0.0f,1.0f,6.28,600.0f });

	Vector2 HP_barSize = { 742.0f ,58.0f };

	//体力の画像読み込み
	healthSprite = Sprite::Create(TextureManager::Load("Hp_inside.png"));
	healthSprite->SetAnchorPoint({ 0,0 });

	healthAlfaSprite = Sprite::Create(TextureManager::Load("Hp_insideAlfa.png"));
	healthAlfaSprite->SetAnchorPoint({ 0,0 });

	HP_barSprite = Sprite::Create(TextureManager::Load("bossBar.png"));
	HP_barSprite->SetAnchorPoint({ 0.5,0.5 });

	// サイズをセットする
	healthAlfaSprite->SetSize(hpAlfaSize);
	HP_barSprite->SetSize(HP_barSize);

	// エネルギーの弾の生成するときの中間点のベクトルの初期化
	EnergyVelL = createEnergyEndPos - createEnergyStartLPos;
	EnergyVelL /= 2;
	energyVelHozonL = EnergyVelL;

	EnergyVelR = createEnergyEndPos - createEnergyStartRPos;
	EnergyVelR /= 2;
	energyVelHozonR = EnergyVelR;
	// エネルギーの巨大弾の生成するときのポジション
	energyBallPos = createEnergyEndPos;
	energyBigBall.model.reset(Model::CreateFromOBJ("sphere", true));
	energyBigBall.WorldTrans.Initialize();
	energyBigBall.WorldTrans.parent_ = &boss2Model[BossWarrierPart::Root].Transform;
	energyBigBall.WorldTrans.translation_ = energyBallPos;
	energyBigBall.WorldTrans.scale_ = energyBallScale;
	energyBigBall.WorldTrans.alpha = 0.95f;
	energyBigBall.WorldTrans.TransferMatrix();

	energyBigBallSub.model.reset(Model::CreateFromOBJ("sphere", true));
	energyBigBallSub.WorldTrans.Initialize();
	energyBigBallSub.WorldTrans.parent_ = &boss2Model[BossWarrierPart::Root].Transform;
	energyBigBallSub.WorldTrans.translation_ = energyBallPos;
	energyBigBallSub.WorldTrans.scale_ = energyBallScale / 2;
	energyBigBallSub.WorldTrans.alpha = 1.0f;
	energyBigBallSub.WorldTrans.TransferMatrix();
}

void BossWarrier::Spawn(const Vector3& boss1Pos)
{
	//フェーズをスポーンに変更
	attack = Attack::Spawm;
	//スケールを0に
	boss2Model[BossWarrierPart::Root].Transform.scale_ = { 0,0,0 };
	boss2Model[BossWarrierPart::Root].Transform.translation_ = boss1Pos;
	//イージング開始
	easeRotArm.Start(spawnAnimationTime);
	//パーティクルの生成数はイージング時間-20に
	particleCreateTime = spawnAnimationTime -20;
	//生存フラグとHPのリセット
	health = maxHealth;
	isAlive = true;
}

void BossWarrier::Update(const Vector3& targetPos)
{
	srand(time(NULL));
	ImGui::Begin("Warrier");

	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		BossWarrier[i]->SetAttribute(COLLISION_ATTR_ENEMYS);
	}

	//引数をメンバにコピー
	this->targetPos = targetPos;

	matBossDir = CreateMatRot({ boss2Model[BossWarrierPart::Root].Transform.translation_.x,0,boss2Model[BossWarrierPart::Root].Transform.translation_.z },{ targetPos.x,0,targetPos.z });

	boss2Model[BossWarrierPart::Root].Transform.SetMatRot(matBossDir);

	if (health <= 0) {
		//死亡
		isAlive = false;
	}

	ImGui::Text("health %d", health);


	for (int i = 0; i < MAXSWROD; i++)
	{
		AttackCollider[i]->SetOldEnemyAttackPos(w[i].matWorld_);
	}

	//swordRad = convertRadianToDegree(swordRad);
	//ImGui::SliderFloat("swordAngle", &swordRad, 0.0f, 360.0f);
	swordRad = convertDegreeToRadian(130);

	Matrix4 bossDir;

	float range = (abs(targetPos.x - boss2Model->Transform.translation_.x) + abs(targetPos.z - boss2Model->Transform.translation_.z)) / 2;

	Vector3 speed;
	switch (attack)
	{
	case Attack::StandBy:
		ImGui::Text("attack stand");

		if (intervalFrame <= maxIntervalFrame)
		{
			intervalFrame++;
		}
		else
		{
			intervalFrame = 0;
			int randInterval = rand() % 3;
			//randAttack %= 100;
			if (randInterval == 0)
			{
				maxIntervalFrame = 60;
			}
			if (randInterval == 1)
			{
				maxIntervalFrame = 120;
			}
			if (randInterval == 2)
			{
				maxIntervalFrame = 180;
			}
			if (range <= 35)
			{
				do
				{
					int randAttack = rand() % 4;
					//randAttack %= 100;
					if (randAttack == 0)
					{
						attack = Attack::SwordSwing;
						attackEasing.Start(60);
						bossAttackPhase = BossAttackPhase::Before;
						InitAtkSwordSwing();
					}
					if (randAttack == 1)
					{
						attackEasing.Start(60);
						bossAttackPhase = BossAttackPhase::Before;
						attack = Attack::Tornado;
						boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Pa.get();
					}
					if (randAttack == 2|| randAttack == 3)
					{
						//初期化処理
						InitAtkArmSwing();
						attackEasing.Start(60);
						bossAttackPhase = BossAttackPhase::Before;
						attack = Attack::ArmSwing;
						boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Gu.get();
						boss2Model[BossWarrierPart::HandR].model = bossArmRModel_Gu.get();
						boss2Model[BossWarrierPart::HandL].Transform.scale_ = { 1.5,2,2 };
						boss2Model[BossWarrierPart::HandR].Transform.scale_ = { 1.5,2,2 };
						boss2Model[BossWarrierPart::HandL].Transform.translation_ = { 1,0,0 };
						boss2Model[BossWarrierPart::HandR].Transform.translation_ = { -1,0,0 };
					}
				} while (attack == oldAttack);
			}
			else
			{
				do
				{
					int randAttack = rand() % 5;
					//randAttack %= 100;
					if (randAttack == 0)
					{
						attack = Attack::LaunchSword;
						attackEasing.Start(60);
						bossAttackPhase = BossAttackPhase::Before;
						boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Pa.get();
						StartLaunchSword();
					}
					if (randAttack == 1)
					{
						attackEasing.Start(60);
						attack = Attack::MultiLaunchSword;
						bossAttackPhase = BossAttackPhase::Before;
						boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Pa.get();
						StartMultiLaunchSword();
					}
					if (randAttack == 2)
					{
						attack = Attack::SwordSwing;
						attackEasing.Start(60);
						bossAttackPhase = BossAttackPhase::Before;
						InitAtkSwordSwing();
					}
					if (randAttack == 3|| randAttack == 4)
					{
						attack = Attack::Approach;
					}
				} while (attack == oldAttack);
			}
		}

		oldAttack = attack;
		ImGui::Text("%d", attack);


		if (Input::GetInstance()->TriggerKey(DIK_8)) {
			//初期化処理
			InitAtkArmSwing();
			attackEasing.Start(60);
			bossAttackPhase = BossAttackPhase::Before;
			attack = Attack::ArmSwing;
			boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Gu.get();
			boss2Model[BossWarrierPart::HandR].model = bossArmRModel_Gu.get();
			boss2Model[BossWarrierPart::HandL].Transform.scale_ = { 1.5,2,2 };
			boss2Model[BossWarrierPart::HandR].Transform.scale_ = { 1.5,2,2 };
			boss2Model[BossWarrierPart::HandL].Transform.translation_ = { 1,0,0 };
			boss2Model[BossWarrierPart::HandR].Transform.translation_ = { -1,0,0 };

		}
		if (Input::GetInstance()->TriggerKey(DIK_9))
		{
			attackEasing.Start(60);
			bossAttackPhase = BossAttackPhase::Before;
			attack = Attack::Tornado;
			boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Pa.get();
		}
		if (Input::GetInstance()->TriggerKey(DIK_L))
		{
			attackEasing.Start(60);
			attack = Attack::MultiLaunchSword;
			bossAttackPhase = BossAttackPhase::Before;
			boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Pa.get();
			StartMultiLaunchSword();
		}
		if (Input::GetInstance()->TriggerKey(DIK_K))
		{
			attack = Attack::LaunchSword;
			attackEasing.Start(60);
			bossAttackPhase = BossAttackPhase::Before;
			boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Pa.get();
			StartLaunchSword();
		}
		if (Input::GetInstance()->TriggerKey(DIK_0)) {
			attack = Attack::SwordSwing;
			attackEasing.Start(60);
			bossAttackPhase = BossAttackPhase::Before;
			InitAtkSwordSwing();
		}

		break;
	case Attack::ArmSwing:
#pragma region 腕振り攻撃処理(テストキーは8)

		ImGui::Text("boss atack ArmSwing");

		for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
			BossWarrier[i]->SetAttribute(COLLISION_ATTR_ENEMYS);
		}

		switch (bossAttackPhase)
		{
		case BossAttackPhase::Before:
			if (attackEasing.GetActive())
			{
				Vector3 rotShoulderL = Lerp(StandByShoulderL, { PI / 4,PI / -6.0f,0 }, attackEasing.GetTimeRate());
				Vector3 rotElbowL = Lerp(StandByElbowL, { 0,PI / -6.0f,0 }, attackEasing.GetTimeRate());

				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(rotShoulderL);
				boss2Model[BossWarrierPart::elbowL].Transform.SetRot(rotElbowL);

				Vector3 rotShoulderR = Lerp(StandByShoulderR, { PI / 4,-PI / -3.0f,0 }, attackEasing.GetTimeRate());
				Vector3 rotElbowR = Lerp(StandByElbowR, { 0,PI / 4.0f * 3.0f,0 }, attackEasing.GetTimeRate());

				boss2Model[BossWarrierPart::ShoulderR].Transform.SetRot(rotShoulderR);
				boss2Model[BossWarrierPart::elbowR].Transform.SetRot(rotElbowR);

				Vector3 scaleHand = Lerp({ 1,1,1 }, { 1.5,2,2 }, attackEasing.GetTimeRate());
				Vector3 transHandL = Lerp({ 0.7,0,0 }, { 1.0f,0,0 }, attackEasing.GetTimeRate());

				boss2Model[BossWarrierPart::HandL].Transform.scale_ = scaleHand;
				boss2Model[BossWarrierPart::HandR].Transform.scale_ = scaleHand;
				boss2Model[BossWarrierPart::HandL].Transform.translation_ = transHandL;
				boss2Model[BossWarrierPart::HandR].Transform.translation_ = -transHandL;
			}
			else
			{
				bossAttackPhase = BossAttackPhase::Attack;
			}
			break;
		case BossAttackPhase::Attack:
			UpdateAtkArmSwing();
			break;
		case BossAttackPhase::After:
			if (attackEasing.GetActive())
			{

				Vector3 rotShoulderL = Lerp({ PI / 4,-PI / -6.0f,0 }, StandByShoulderL, attackEasing.GetTimeRate());
				Vector3 rotElbowL = Lerp({ 0,PI / -6.0f,0 }, StandByElbowL, attackEasing.GetTimeRate());

				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(rotShoulderL);
				boss2Model[BossWarrierPart::elbowL].Transform.SetRot(rotElbowL);

				Vector3 rotShoulderR = Lerp({ PI / 4,-PI / -3.0f,0 }, StandByShoulderR, attackEasing.GetTimeRate());
				Vector3 rotElbowR = Lerp({ 0,PI / 4.0f * 3.0f,0 }, StandByElbowR, attackEasing.GetTimeRate());

				boss2Model[BossWarrierPart::ShoulderR].Transform.SetRot(rotShoulderR);
				boss2Model[BossWarrierPart::elbowR].Transform.SetRot(rotElbowR);

				Vector3 scaleHand = Lerp({ 1.5,2,2 }, { 1,1,1 }, attackEasing.GetTimeRate());
				Vector3 transHandL = Lerp({ 1.0f,0,0 }, { 0.7,0,0 }, attackEasing.GetTimeRate());

				boss2Model[BossWarrierPart::HandL].Transform.scale_ = scaleHand;
				boss2Model[BossWarrierPart::HandR].Transform.scale_ = scaleHand;
				boss2Model[BossWarrierPart::HandL].Transform.translation_ = transHandL;
				boss2Model[BossWarrierPart::HandR].Transform.translation_ = -transHandL;
			}
			else
			{
				attack = Attack::StandBy;
				boss2Model[BossWarrierPart::HandL].model = bossArmLModel.get();
			}
			break;
		default:
			break;
		}
#pragma endregion
		break;
	case Attack::Tornado:
		switch (bossAttackPhase)
		{
		case BossAttackPhase::Before:
			if (attackEasing.GetActive())
			{
				Vector3 rotShoulderL = Lerp(StandByShoulderL, { 0,-PI / 2,0 }, attackEasing.GetTimeRate());
				Vector3 rotElbowL = Lerp(StandByElbowL, { 0,0,0 }, attackEasing.GetTimeRate());
				Vector3 rotRoot = Lerp({ 0,0,0 }, { 0,100,0 }, attackEasing.GetTimeRate());
				boss2Model[BossWarrierPart::Root].Transform.SetMatRot(CreateMatRot({
							   boss2Model[BossWarrierPart::Root].Transform.translation_.x,
							   rotRoot.y ,
							   boss2Model[BossWarrierPart::Root].Transform.translation_.z },
					{ targetPos.x,0,targetPos.z }));

				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(rotShoulderL);
				boss2Model[BossWarrierPart::elbowL].Transform.SetRot(rotElbowL);

				Vector3 boss2Pos = Lerp({ 0,20,0 }, { 0,50,0 }, attackEasing.GetTimeRate());
				boss2Model[BossWarrierPart::Root].Transform.translation_.y = boss2Pos.y;
			}
			else
			{
				bossAttackPhase = BossAttackPhase::Attack;
			}
			break;
		case BossAttackPhase::Attack:
			boss2Model[BossWarrierPart::Root].Transform.SetMatRot(CreateMatRot({ boss2Model[BossWarrierPart::Root].Transform.translation_.x,
							   100 ,
							   boss2Model[BossWarrierPart::Root].Transform.translation_.z },
				{ targetPos.x,0,targetPos.z }));
			BossTornado();
			break;
		case BossAttackPhase::After:
			if (attackEasing.GetActive())
			{
				Vector3 rotShoulderL = Lerp({ 0,-PI / 2,0 }, StandByShoulderL, attackEasing.GetTimeRate());
				Vector3 rotElbowL = Lerp({ 0,0,0 }, StandByElbowL, attackEasing.GetTimeRate());
				Vector3 rotRoot = Lerp({ 0,100,0 }, { 0,0,0 }, attackEasing.GetTimeRate());
				boss2Model[BossWarrierPart::Root].Transform.SetMatRot(CreateMatRot({
							   boss2Model[BossWarrierPart::Root].Transform.translation_.x,
							   rotRoot.y ,
							   boss2Model[BossWarrierPart::Root].Transform.translation_.z },
					{ targetPos.x,0,targetPos.z }));

				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(rotShoulderL);
				boss2Model[BossWarrierPart::elbowL].Transform.SetRot(rotElbowL);

				Vector3 boss2Pos = Lerp({ 0,50,0 }, { 0,20,0 }, attackEasing.GetTimeRate());
				boss2Model[BossWarrierPart::Root].Transform.translation_.y = boss2Pos.y;
			}
			else
			{
				attack = Attack::StandBy;
				boss2Model[BossWarrierPart::HandL].model = bossArmLModel.get();
			}
			break;
		default:
			break;
		}
		break;
	case Attack::MultiLaunchSword:
		switch (bossAttackPhase)
		{
		case BossAttackPhase::Before:
			if (attackEasing.GetActive())
			{
				Model::ConstBufferPolygonExplosion polygon = swordModel->GetPolygonExplosion();

				Vector3 polygonEasing = Lerp({ 1,0,5 }, { 0,1,1 }, attackEasing.GetTimeRate());
				Vector3 rotShoulderL = Lerp(StandByShoulderL, { 0,-PI / 2,0 }, attackEasing.GetTimeRate());
				Vector3 rotElbowL = Lerp(StandByElbowL, { 0,0,0 }, attackEasing.GetTimeRate());

				for (int i = 0; i < MAXSWROD; i++)
				{
					w[i].alpha = polygonEasing.y;
				}
				swordModel->SetPolygonExplosion({ polygonEasing.x,polygonEasing.z,polygon._RotationFactor,polygon._PositionFactor });
				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(rotShoulderL);
				boss2Model[BossWarrierPart::elbowL].Transform.SetRot(rotElbowL);
			}
			else
			{
				for (int i = 0; i < MAXSWROD; i++)
				{
					w[i].alpha = 1;
				}
				Model::ConstBufferPolygonExplosion polygon = swordModel->GetPolygonExplosion();
				swordModel->SetPolygonExplosion({ 0,1,polygon._RotationFactor,polygon._PositionFactor });
				bossAttackPhase = BossAttackPhase::Attack;
			}
			break;
		case BossAttackPhase::Attack:
			MultiLaunchSword();
			break;
		case BossAttackPhase::After:
			if (attackEasing.GetActive())
			{
				Vector3 rotShoulderL = Lerp({ 0,-PI / 2,0 }, StandByShoulderL, attackEasing.GetTimeRate());
				Vector3 rotElbowL = Lerp({ 0,0,0 }, StandByElbowL, attackEasing.GetTimeRate());

				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(rotShoulderL);
				boss2Model[BossWarrierPart::elbowL].Transform.SetRot(rotElbowL);
			}
			else
			{
				attack = Attack::StandBy;
				boss2Model[BossWarrierPart::HandL].model = bossArmLModel.get();
			}
			break;
		default:
			break;
		}
		break;
	case Attack::LaunchSword:
		switch (bossAttackPhase)
		{
		case BossAttackPhase::Before:
			if (attackEasing.GetActive())
			{
				Model::ConstBufferPolygonExplosion polygon = swordModel->GetPolygonExplosion();

				Vector3 polygonEasing = Lerp({ 1,0,5 }, { 0,1,1 }, attackEasing.GetTimeRate());
				Vector3 rotShoulderL = Lerp(StandByShoulderL, { 0,-PI / 2,0 }, attackEasing.GetTimeRate());
				Vector3 rotElbowL = Lerp(StandByElbowL, { 0,0,0 }, attackEasing.GetTimeRate());

				for (int i = 0; i < MAXSWROD; i++)
				{
					w[i].alpha = polygonEasing.y;
				}
				swordModel->SetPolygonExplosion({ polygonEasing.x,polygonEasing.z,polygon._RotationFactor,polygon._PositionFactor });
				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(rotShoulderL);
				boss2Model[BossWarrierPart::elbowL].Transform.SetRot(rotElbowL);
			}
			else
			{
				for (int i = 0; i < MAXSWROD; i++)
				{
					w[i].alpha = 1;
				}
				Model::ConstBufferPolygonExplosion polygon = swordModel->GetPolygonExplosion();
				swordModel->SetPolygonExplosion({ 0,1,polygon._RotationFactor,polygon._PositionFactor });
				bossAttackPhase = BossAttackPhase::Attack;
			}
			break;
		case BossAttackPhase::Attack:
			LaunchSword();
			break;
		case BossAttackPhase::After:
			if (attackEasing.GetActive())
			{
				Vector3 rotShoulderL = Lerp({ 0,-PI / 2,0 }, StandByShoulderL, attackEasing.GetTimeRate());
				Vector3 rotElbowL = Lerp({ 0,0,0 }, StandByElbowL, attackEasing.GetTimeRate());

				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(rotShoulderL);
				boss2Model[BossWarrierPart::elbowL].Transform.SetRot(rotElbowL);
			}
			else
			{
				attack = Attack::StandBy;
				boss2Model[BossWarrierPart::HandL].model = bossArmLModel.get();
			}
			break;
		default:
			break;
		}
		break;
	case Attack::SwordSwing:
		; switch (bossAttackPhase)
		{
		case BossAttackPhase::Before:
			if (attackEasing.GetActive())
			{
				Vector3 rootRot, shoulderRotL, shoulderRotR, elbowRotR;
				rootRot = { 0,15,0 };
				shoulderRotL = { 180,30,30 };
				shoulderRotR = { 0,30,95 };
				elbowRotR = { 30,20,95 };
				rootRot = convertDegreeToRadian(rootRot);
				shoulderRotL = convertDegreeToRadian(shoulderRotL);
				shoulderRotR = convertDegreeToRadian(shoulderRotR);
				elbowRotR = convertDegreeToRadian(elbowRotR);
				Model::ConstBufferPolygonExplosion polygon = swordModel->GetPolygonExplosion();

				Vector3 polygonEasing = Lerp({ 1,0,5 }, { 0,1,1 }, attackEasing.GetTimeRate());
				shoulderRotL = Lerp(StandByShoulderL, shoulderRotL, attackEasing.GetTimeRate());
				shoulderRotR = Lerp(StandByShoulderR, shoulderRotR, attackEasing.GetTimeRate());
				elbowRotR = Lerp(StandByElbowR, elbowRotR, attackEasing.GetTimeRate());
				rootRot = Lerp({ 0,0,0 }, rootRot, attackEasing.GetTimeRate());

				Matrix4 rotRoot = CreateMatRot(rootRot);

				for (int i = 0; i < MAXSWROD; i++)
				{
					w[i].alpha = 1;
				}
				swordModel->SetPolygonExplosion({ polygonEasing.x,1,polygon._RotationFactor,polygon._PositionFactor });
				boss2Model[BossWarrierPart::Root].Transform.SetMatRot(matBossDir * rotRoot);
				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(shoulderRotL);
				boss2Model[BossWarrierPart::ShoulderR].Transform.SetRot(shoulderRotR);
				boss2Model[BossWarrierPart::elbowR].Transform.SetRot(elbowRotR);
			}
			else
			{
				for (int i = 0; i < MAXSWROD; i++)
				{
					w[i].alpha = 1;
				}
				Model::ConstBufferPolygonExplosion polygon = swordModel->GetPolygonExplosion();
				swordModel->SetPolygonExplosion({ 0,1, polygon._RotationFactor,polygon._PositionFactor });
				bossAttackPhase = BossAttackPhase::Attack;
			}
			break;
		case BossAttackPhase::Attack:
			UpdateAtkSwordSwing();
			break;
		case BossAttackPhase::After:
			if (attackEasing.GetActive())
			{
				Vector3 rootRot, shoulderRotL, shoulderRotR, elbowRotR;
				rootRot = { 0,15,0 };
				shoulderRotL = { 180,30,30 };
				shoulderRotR = { 0,30,95 };
				elbowRotR = { 30,20,95 };
				rootRot = convertDegreeToRadian(rootRot);
				shoulderRotL = convertDegreeToRadian(shoulderRotL);
				shoulderRotR = convertDegreeToRadian(shoulderRotR);
				elbowRotR = convertDegreeToRadian(elbowRotR);

				shoulderRotL = Lerp(shoulderRotL, StandByShoulderL, attackEasing.GetTimeRate());
				shoulderRotR = Lerp(shoulderRotR, StandByShoulderR, attackEasing.GetTimeRate());
				elbowRotR = Lerp(elbowRotR, StandByElbowR, attackEasing.GetTimeRate());
				rootRot = Lerp(rootRot, { 0,0,0 }, attackEasing.GetTimeRate());

				Matrix4 rotRoot = CreateMatRot(rootRot);

				boss2Model[BossWarrierPart::Root].Transform.SetMatRot(matBossDir * rotRoot);
				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(shoulderRotL);
				boss2Model[BossWarrierPart::ShoulderR].Transform.SetRot(shoulderRotR);
				boss2Model[BossWarrierPart::elbowR].Transform.SetRot(elbowRotR);
			}
			else
			{
				attack = Attack::StandBy;
			}
			break;
		default:
			break;
		}
		break;
	case Attack::Spawm:
		UpdateSpawn();
		break;

	case Attack::Approach:

		speed = targetPos - boss2Model->Transform.translation_;

		speed = speed.normalize();

		speed.y = 0;

		boss2Model[BossWarrierPart::Root].Transform.translation_ += speed * 3;

		if (range <= 20)
		{
			maxIntervalFrame = 30;
			attack = Attack::StandBy;
		}

		break;

	default:

		break;
	}

	attackEasing.Update();

	boss2TornadoTransform[0].TransferMatrix();
	boss2TornadoTransform[1].TransferMatrix();
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Model[i].Transform.TransferMatrix();

		BossWarrier[i]->Update(boss2Model[i].Transform.matWorld_);

		modelSpere[i].translation_ = MyMath::GetWorldTransform(boss2Model[i].Transform.matWorld_);
		modelSpere[i].TransferMatrix();
	}
	if (IsKingDrop == false) {
		if (Input::GetInstance()->TriggerKey(DIK_2)) {
			KingDropReset();
		}
	}
	// 一旦仮に王のしずくの流れを確認用に
	for (int i = 0; i < MAXSWROD; i++)
	{
		w[i].TransferMatrix();
	}

	KingDropUpdate();


	ImGui::Text("TornadoRadius:%f", TornadoRadius);

	ImGui::Text("BossAttack:%d", attack);

	ImGui::Text("maxintervalFrame:%f", maxIntervalFrame);

	ImGui::Text("intervalFrame:%d", intervalFrame);

	ImGui::End();

}

void BossWarrier::Draw(const ViewProjection& viewProMat)
{
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		if (boss2Model[i].isDraw == true)
		{
			boss2Model[i].model->Draw(boss2Model[i].Transform, viewProMat);

			//ModelSpere->Draw(modelSpere[i], viewProMat);
		}
	}
	if (attack == Attack::Tornado)
	{
		boss2TornadeModel->Draw(boss2TornadoTransform[0], viewProMat);
		boss2TornadeModel->Draw(boss2TornadoTransform[1], viewProMat);
	}

	//ModelSpere->Draw(boss2TornadoTransform[0], viewProMat);

	//剣振り攻撃なら剣を一本描画
	if (attack == Attack::SwordSwing) {
		swordModel->Draw(w[0], viewProMat);
	}

	LaunchSwordDraw(viewProMat);

	//for (int i = 0; i < 5; i++) {

	//	ModelSpere->Draw(AttackColliderWorldTrans[i], viewProMat);
	//}
	// 王のしずくの描画
	if (IsKingDrop == true) {
		// 王のしずくのエネルギーの生成の粒の描画
		if (IsKingEnergy == true) {
			for (int i = 0; i < energyNum; i++) {
				energyL[i].model->Draw(energyL[i].WorldTrans, viewProMat);
				energyR[i].model->Draw(energyR[i].WorldTrans, viewProMat);
			}
			// 王のしずくが落ちたら
			energyBigBallSub.model->Draw(energyBigBallSub.WorldTrans, viewProMat);
			// 王のしずくのエネルギーの巨大弾の描画
			energyBigBall.model->Draw(energyBigBall.WorldTrans, viewProMat);
		}
	}
}

void BossWarrier::DrawParticle(const ViewProjection& viewProMat)
{
	spawnParticle->Draw(viewProMat);
}

void BossWarrier::DrawHealth()
{
	// HPのセット
	float nowHp = health / maxHealth;
	hpSize = { 714.0f * nowHp,27.0f };
	healthSprite->SetSize(hpSize);

	// Hpの下の部分を減らす処理
	if (IsHpAlfa) {
		// 攻撃を受けてから 30 フレーム下のHpは動かない
		if (hpAlfaTimer < 30) {
			hpAlfaTimer++;
		}
		else {
			// 赤ゲージよりサイズが大きいなら減らす
			if (hpSize.x < hpAlfaSize.x) {
				hpAlfaSize.x -= 2.0f;
				healthAlfaSprite->SetSize(hpAlfaSize);
			}
			// 赤ゲージよりサイズが小さくなったら減らすのをやめ、赤ゲージのサイズに合わせる
			// 下のゲージのフラグをオフにする
			else if (hpSize.x >= hpAlfaSize.x) {
				hpAlfaTimer = 0;
				healthAlfaSprite->SetSize(hpSize);
				IsHpAlfa = false;
			}
		}
	}



	//Vector2 size = { 48.0f * bossHealth,48.0f };

	Vector2 pos = { WinApp::window_width / 2 - 358,WinApp::window_height / 2 + 236 };

	Vector2 HP_barPos = { WinApp::window_width / 2,WinApp::window_height / 2 + 250 };

	//healthSprite->SetSize(size);

	// スプライト描画
	healthAlfaSprite->Draw(pos, { 1,1,1,1 });

	healthSprite->Draw(pos, { 1,1,1,1 });

	HP_barSprite->Draw(HP_barPos, { 1,1,1,1 });
}



void BossWarrier::MultiLaunchSword()
{

	//攻撃開始
	if (t)
	{
		phase2AttackCoolTime--;
		if (phase2AttackCoolTime <= 0)
		{
			//生成した剣を飛ばすシーン
			for (int i = 0; i < MAXSWROD; i++)
			{


				//
				// 
				//計算したベクトル方向に動かす
				w[i].translation_ += num[i].translation_ * 10;
				w[i].TransferMatrix();

				AttackCollider[i]->Update(w[i].matWorld_);

			}
			//フラグ解除
			if (phase2AttackCoolTime <= -40)
			{

				t = false;

			}
		}
	}
	else
	{
		phase2AttackCoolTime = 70;
		isSat = false;
		attackEasing.Start(60);
		bossAttackPhase = BossAttackPhase::After;
	}


}

void BossWarrier::StartMultiLaunchSword()
{
	//生成時の座標設定
	for (int i = 0; i < MAXSWROD; i++)
	{

		w[i].translation_ =
		{
			//BOSS第二形態の真ん中の座標
			boss2Model[BossWarrierPart::Root].Transform.translation_.x - interval * 2 + i * interval,
			boss2Model[BossWarrierPart::Root].Transform.translation_.y + 35 ,
			boss2Model[BossWarrierPart::Root].Transform.translation_.z
		};
		Rota();
		w[i].TransferMatrix();
		AttackCollider[i]->SetAttribute(COLLISION_ATTR_ENEMYSOWRDATTACK);
		AttackCollider[i]->Update(w[i].matWorld_);
		w[i].alpha = 0;
	}

	for (int i = 0; i < MAXSWROD; i++)
	{
		//
		//剣からプレイヤーへのベクトル計算,飛ばす用
		pPos[i].translation_ = targetPos + Vector3(0, 5, 0);
		num[i].translation_ = pPos[i].translation_ - w[i].translation_;
		num[i].translation_.normalize();

		num[i].TransferMatrix();


	}
	Model::ConstBufferPolygonExplosion polygon = swordModel->GetPolygonExplosion();
	swordModel->SetPolygonExplosion({ 1,polygon._ScaleFactor,polygon._RotationFactor,polygon._PositionFactor });


	t = true;
	isSat = true;
	Rota();
}

void BossWarrier::LaunchSword()
{
	//一個ずつ飛ばす


	//攻撃開始
	if (t2)
	{
		shotTime--;
		for (int i = 0; i < 5; i++)
		{
			if (shotTime <= 0)
			{
				if (isShot[i] == false)
				{

					isShot[i] = true;
					//剣からプレイヤーへのベクトル計算,飛ばす用
					pPos[i].translation_ = targetPos + Vector3(0, 5, 0);
					num[i].translation_ = pPos[i].translation_ - w[i].translation_;
					num[i].translation_.normalize();
					num[i].TransferMatrix();
					//Rota();
					shotTime = MAXSHOTTIME;
					break;
				}

			}
		}


		//生成した剣を飛ばすシーン
		for (int i = 0; i < MAXSWROD; i++)
		{

			//計算したベクトル方向に動かす
			if (isShot[i] == true)
			{

				w[i].translation_ += num[i].translation_ * 10;
				w[i].TransferMatrix();
				AttackCollider[i]->SetAttribute(COLLISION_ATTR_ENEMYSOWRDATTACK);
				AttackCollider[i]->Update(w[i].matWorld_);
			}
		}
		if (shotTime <= -40)
		{
			shotTime = 70;
			t2 = false;
			attackEasing.Start(60);
			bossAttackPhase = BossAttackPhase::After;
		}

	}
	else
	{
		phase2AttackCoolTime = 70;
		isSat2 = false;
		attack = Attack::StandBy;
	}
	//向きを変える
	for (int i = 0; i < 5; i++)
	{
		if (isShot[i] == false)
		{

			WorldTransform plWorldTransform;

			plWorldTransform.translation_ = targetPos;

			Matrix4 mat;
			mat = CreateMatRot(w[i].translation_, plWorldTransform.translation_);

			w[i].SetMatRot(mat);
			w[i].SetLookMatRot(mat);
			w[i].TransferMatrix();
			AttackCollider[i]->SetAttribute(COLLISION_ATTR_ENEMYSOWRDATTACK);
			AttackCollider[i]->Update(w[i].matWorld_,AttackRadius);
		}
	}
}

void BossWarrier::StartLaunchSword()
{
	//生成時の座標設定
	for (int i = 0; i < MAXSWROD; i++)
	{

		w[i].translation_ =
		{
			//BOSS第二形態の真ん中の座標
			boss2Model[BossWarrierPart::Root].Transform.translation_.x - interval * 2 + i * interval,
			boss2Model[BossWarrierPart::Root].Transform.translation_.y + 35 ,
			boss2Model[BossWarrierPart::Root].Transform.translation_.z
		};

		w[i].TransferMatrix();
		AttackCollider[i]->SetAttribute(COLLISION_ATTR_ENEMYSOWRDATTACK);
		AttackCollider[i]->Update(w[i].matWorld_, AttackRadius);
	}


	for (int i = 0; i < MAXSWROD; i++)
	{
		//
		////剣からプレイヤーへのベクトル計算,飛ばす用
		//pPos[i].translation_ = pl->GetWorldPosition();
		//num[i].translation_ = pPos[i].translation_ - w[i].translation_;
		//num[i].translation_.normalize();

		//num[i].TransferMatrix();

		isShot[i] = false;
		kenrot[i] = false;
	}


	t2 = true;
	isSat2 = true;
	Rota();
}

void BossWarrier::BossTornado()
{
	TornadoFlame++;
	TornadoRotY[0] += 3.14 / 180 * TornadoSpeedRotY;
	TornadoRotY[1] += 3.14 / 180 * TornadoSpeedRotY;

	if (TornadoFlame <= 100)
	{
		if (boss2TornadoTransform[0].scale_.x <= 50)
		{
			boss2TornadoTransform[0].scale_.x += 0.5;
			boss2TornadoTransform[0].scale_.z += 0.5;
			TornadoRadius += 0.5f;
		}
		if (boss2TornadoTransform[1].scale_.x <= 45)
		{
			boss2TornadoTransform[1].scale_.x += 0.5;
			boss2TornadoTransform[1].scale_.z += 0.5;
		}
	}
	else if (TornadoFlame >= 160)
	{
		if (boss2TornadoTransform[0].alpha > 0)
		{
			boss2TornadoTransform[0].alpha -= 0.05;
			boss2TornadoTransform[1].alpha -= 0.05;
		}
	}
	if (boss2TornadoTransform[1].scale_.y <= 50)
	{
		boss2TornadoTransform[1].scale_.y += 0.5;
		boss2TornadoTransform[0].scale_.y += 0.5;
	}
	boss2TornadoTransform[0].translation_.x = boss2Model[BossWarrierPart::Root].Transform.translation_.x;
	boss2TornadoTransform[0].translation_.z = boss2Model[BossWarrierPart::Root].Transform.translation_.z;
	boss2TornadoTransform[0].SetRot({ 0,TornadoRotY[0],0 });
	boss2TornadoTransform[0].TransferMatrix();
	boss2TornadoTransform[1].translation_.x = boss2Model[BossWarrierPart::Root].Transform.translation_.x;
	boss2TornadoTransform[1].translation_.z = boss2Model[BossWarrierPart::Root].Transform.translation_.z;
	boss2TornadoTransform[1].SetRot({ 0,TornadoRotY[1],0 });
	boss2TornadoTransform[1].TransferMatrix();
	if (TornadoFlame >= 170)
	{
		bossAttackPhase = BossAttackPhase::After;
		attackEasing.Start(60);
		TornadoFlame = 0;
		boss2TornadoTransform[0].scale_.x = 1;
		boss2TornadoTransform[0].scale_.z = 1;
		boss2TornadoTransform[1].scale_.x = 1;
		boss2TornadoTransform[1].scale_.z = 1;
		boss2TornadoTransform[1].scale_.y = 10;
		boss2TornadoTransform[0].scale_.y = 10;
		TornadoRotY[0] += 0;
		TornadoRotY[1] += 3.14;
		boss2TornadoTransform[0].alpha = 0.6;
		boss2TornadoTransform[1].alpha = 0.6;
		TornadoRadius = 1.0f;
	}
	Tornado->Update(boss2TornadoTransform[0].matWorld_, TornadoRadius);
}

void BossWarrier::LaunchSwordDraw(ViewProjection viewProMat)
{
	if (isSat || isSat2)
	{
		for (int i = 0; i < MAXSWROD; i++)
		{
			swordModel->Draw(w[i], viewProMat);
		}
	}
}

void BossWarrier::Rota()
{
	WorldTransform plWorldTransform;

	plWorldTransform.translation_ = targetPos;

	for (int i = 0; i < MAXSWROD; i++)
	{
		Matrix4 mat;
		mat = CreateMatRot(w[i].translation_, plWorldTransform.translation_);
		w[i].SetMatRot(mat);
		w[i].SetLookMatRot(mat);
	}
}

void BossWarrier::Damage(int damage)
{
	health -= damage;
	IsHpAlfa = true;
	hpAlfaSize = hpSize;

	if (health < 0)
	{
		health = 0;
	}
}

void BossWarrier::reset()
{
	isAtkArmSwing = false;
	atkStartTime = 0;

	isAfter = false;
	rootRotRad = 0;
	TornadoFlame = 0;
	isLastAtkStart = false;
	lastAtkCount = 0;

	atkArmSwingTime = 0;

	swordRad = 0.0f;

	intervalFrame = 0;
	maxIntervalFrame = 120;

	//蠑墓焚縺ｪ縺ｩ縺ｧ繧ゅｉ縺｣縺ｦ縺上ｋ螟画焚
	targetPos = { 0,0,0 };

	attack = Attack::StandBy;

	oldAttack = Attack::StandBy;

	bossAttackPhase = BossAttackPhase::Before;

	rdi3 = 3.0f;
	rdi5 = 5.0f;
	rdi8 = 8.0f;

	//

	phase2AttackCoolTime = 70;
	interval = 10;
	moveSpeed = 0.2f;
	isSat = false;
	isSat2 = false;
	isOn = false;
	shotTime = MAXSHOTTIME;

	TornadoSpeedRotY = 5;
	isTornado = false;

	particleCreateTime = 0;
	spawnAnimationTime = 120;

	isAlive = false;

	health = 0;

	//ボスのスケールを5倍に
	boss2Model[BossWarrierPart::Root].Transform.scale_ = { 15,15,15 };
	boss2Model[BossWarrierPart::Root].Transform.translation_ = { 50,50,50 };
	//boss2Model[BossWarrierPart::Root].Transform.SetRot({1.57,0,0});
	//それぞれの部位の位置をセット
	boss2Model[BossWarrierPart::Head].Transform.translation_ = { 0,0.9,0.5 };
	boss2Model[BossWarrierPart::ShoulderL].Transform.translation_ = { 0.9,0.3,0 };
	boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot({ 0,0,-PI / 4 });
	boss2Model[BossWarrierPart::ArmL].Transform.translation_ = { 0.2,0,0 };
	boss2Model[BossWarrierPart::ShoulderR].Transform.translation_ = { -0.9,0.3,0 };
	boss2Model[BossWarrierPart::ShoulderR].Transform.SetRot({ 0,0,PI / 4 });
	boss2Model[BossWarrierPart::ArmR].Transform.translation_ = { -0.2,0,0 };
	boss2Model[BossWarrierPart::elbowL].Transform.translation_ = { 0.2,0,0 };
	boss2Model[BossWarrierPart::elbowL].Transform.SetRot({ 0,0,-PI / 4 });
	boss2Model[BossWarrierPart::HandL].Transform.translation_ = { 0.7,0,0 };
	boss2Model[BossWarrierPart::HandL].Transform.scale_ = { 1,1,1 };
	boss2Model[BossWarrierPart::elbowR].Transform.translation_ = { -0.2,0,0 };
	boss2Model[BossWarrierPart::elbowR].Transform.SetRot({ 0,0,PI / 4 });
	boss2Model[BossWarrierPart::HandR].Transform.translation_ = { -0.7,0,0 };
	boss2Model[BossWarrierPart::HandR].Transform.scale_ = { 1,1,1 };
	boss2Model[BossWarrierPart::Crotch].Transform.translation_ = { 0,-0.5,0 };
	boss2Model[BossWarrierPart::Waist].Transform.translation_ = { 0,-0.5,0 };

	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Model[i].Transform.TransferMatrix();
		BossWarrier[i]->Update(boss2Model[i].Transform.matWorld_);
		modelSpere[i].translation_ = MyMath::GetWorldTransform(boss2Model[i].Transform.matWorld_);
		modelSpere[i].scale_ = Vector3{ BossWarrierRadius[i],BossWarrierRadius[i] ,BossWarrierRadius[i] };
		modelSpere[i].TransferMatrix();
	}

	boss2TornadoTransform[0].scale_ = { 1,10,1 };
	boss2TornadoTransform[0].translation_ = { 0,-10,0 };
	boss2TornadoTransform[0].TransferMatrix();
	boss2TornadoTransform[1].scale_ = { 1,10,1 };
	boss2TornadoTransform[1].translation_ = { 0,-10,0 };
	boss2TornadoTransform[1].TransferMatrix();

	boss2TornadoTransform[0].alpha = 0.6;
	boss2TornadoTransform[1].alpha = 0.6;

	TornadoRotY[0] = 0;

	TornadoRotY[1] = 3.14;


	swordModel->SetPolygonExplosion({ 0.0f,1.0f,6.28,600.0f });
}

void BossWarrier::InitAtkArmSwing()
{
	//腕振り攻撃フラグをtrue
	attack = Attack::ArmSwing;
	//腕振りタイマーを開始(30f)で終わるようにし、30f経つごとにループさせる
	easeRotArm.Start(30);

	dataRotArm[0] = { 0,PI / -3.0f,0 };
	dataRotArm[1] = { 0,-PI / -3.0f ,0 };
	dataRotShoulder[0] = { PI / 4,PI / -6.0f,0 };
	dataRotShoulder[1] = { PI / 4,PI / -3.0f,0 };
	dataRotElbow[0] = { 0,PI / -6.0f,0 };
	dataRotElbow[1] = { 0,PI / -4.0f * 3.0f,0 };
	dummyTargetPos = targetPos;

	isLastAtkStart = false;

	atkArmSwingTime = 0;
}

void BossWarrier::UpdateAtkArmSwing()
{
	//移動処理を最初に

	//ボスは30fの間決まったダミーの座標に向かって移動する
	Vector3 bossMoveVec;
	bossMoveVec = dummyTargetPos - boss2Model[BossWarrierPart::Root].Transform.translation_;
	bossMoveVec.normalize();
	//XZ移動なのでYは0
	bossMoveVec.y = 0;

	//ボスの移動速度(スカラー)は1秒に5進むくらい(1fで 5 /60)
	float bossSpdScalar = 5.0f / 10.0f;
	//正規化した移動ベクトルにボス速度を掛け算
	bossMoveVec *= bossSpdScalar;

	ImGui::Text("bossMoveVec:%f,%f,%f", bossMoveVec.x, bossMoveVec.y, bossMoveVec.z);
	ImGui::Text("atkCount:%d", lastAtkCount);

	//大本のモデルを移動
	boss2Model[BossWarrierPart::Root].Transform.translation_ += bossMoveVec;

	if (atkArmSwingTime >= 14)
	{
		bossAttackPhase = BossAttackPhase::After;

		attackEasing.Start(60);

		return;
	}

	//イージングデータ更新
	easeRotArm.Update();
	//イージングが終了したら(timeRateが1.0以上)イージングのパラメータを入れ替えてまたイージング開始
	if (!easeRotArm.GetActive()) {
		atkArmSwingTime++;

		// それぞれの回転データをスワップ
		Vector3 data = dataRotArm[0];
		dataRotArm[0] = dataRotArm[1];
		dataRotArm[1] = data;
		data = dataRotElbow[0];
		dataRotElbow[0] = dataRotElbow[1];
		dataRotElbow[1] = data;
		data = dataRotShoulder[0];
		dataRotShoulder[0] = dataRotShoulder[1];
		dataRotShoulder[1] = data;

		//腕振り開始
		easeRotArm.Start(30);

		//ダミー座標更新
		//攻撃終了フラグが経っているならダミーはの座標は進行方向に
		if (isLastAtkStart) {
			dummyTargetPos += bossMoveVec;
		}
		else {
			dummyTargetPos = targetPos;
		}

		//ボスの現在の座標と自機座標が近かったら攻撃終了カウント開始
		Vector3 bossToTarget = boss2Model->Transform.translation_ - targetPos;
		bossToTarget.y = 0;
		ImGui::Text("length bossToPlayer:%f", bossToTarget.length());
		if (bossToTarget.length() <= 25.0f && !isLastAtkStart) {
			isLastAtkStart = true;
			//残りの移動回数は合計距離が 攻撃終了カウントの計算に使った距離の二倍になるように
			lastAtkCount = 6;
		}
		//攻撃終了カウントがあるなら減らす
		if (isLastAtkStart) {
			lastAtkCount--;
			//攻撃終了カウントが0未満ならモーション終了
			if (lastAtkCount < 0) {
				bossAttackPhase = BossAttackPhase::After;

				attackEasing.Start(60);
			}
		}

	}




	//各関節の回転処理
	Vector3 rotArm, rotShoulderL, rotShoulderR, rotElbowL, rotElbowR;
	rotArm = EaseOutVec3(dataRotArm[0], dataRotArm[1], easeRotArm.GetTimeRate());
	rotShoulderL = Lerp(dataRotShoulder[1], dataRotShoulder[0], easeRotArm.GetTimeRate());
	rotShoulderR = Lerp({ dataRotShoulder[0].x ,-dataRotShoulder[0].y ,-dataRotShoulder[0].z }, { dataRotShoulder[1].x ,-dataRotShoulder[1].y ,-dataRotShoulder[1].z }, easeRotArm.GetTimeRate());
	rotElbowL = Lerp(dataRotElbow[1], dataRotElbow[0], easeRotArm.GetTimeRate());
	rotElbowR = Lerp(-dataRotElbow[0], -dataRotElbow[1], easeRotArm.GetTimeRate());

	//	自機を向く回転行列、体を回転させる回転行列をそれぞれ作成
	Matrix4 matBossDir, matBodyRot, matBossRot;

	//進行方向に向かせたいのでダミーの自機を向いてもらう
	matBossDir = CreateMatRot({ boss2Model[BossWarrierPart::Root].Transform.translation_.x,0,boss2Model[BossWarrierPart::Root].Transform.translation_.z }, {dummyTargetPos.x,0,dummyTargetPos .z} );

	matBodyRot = CreateMatRot(rotArm);

	matBossRot = matBossDir * matBodyRot;

	boss2Model[BossWarrierPart::Root].Transform.SetMatRot(matBossRot);
	//boss2Model[BossWarrierPart::Root].Transform.translation_.x += 5.0f / 60.0f;
	//if (boss2Model[BossWarrierPart::Root].Transform.translation_.x > 100) {
	//	boss2Model[BossWarrierPart::Root].Transform.translation_.x = 0;
	//	isAtkArmSwing = false;
	//}
	boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(rotShoulderL);
	boss2Model[BossWarrierPart::ShoulderR].Transform.SetRot(rotShoulderR);
	boss2Model[BossWarrierPart::elbowL].Transform.SetRot(rotElbowL);
	boss2Model[BossWarrierPart::elbowR].Transform.SetRot(rotElbowR);

}

void BossWarrier::KingDropUpdate()
{
	// 王のしずくだったら
	if (IsKingDrop == true) {
		// 王のしずくを打つポジションに移動する
		if (IsKingDropPos == false) {
			ImGui::Begin("Pos");
			ImGui::SliderFloat3("Boss Pos", &kingDropPos.x, -250, 180);
			ImGui::End();
			if (IsMoveBefor == false) {
				boss2Model[BossWarrierPart::Chest].Transform.alpha -= kingDropMoveAlphaM;
				boss2Model[BossWarrierPart::Head].Transform.alpha -= kingDropMoveAlphaM;
				boss2Model[BossWarrierPart::Waist].Transform.alpha -= kingDropMoveAlphaM;
				boss2Model[BossWarrierPart::ArmL].Transform.alpha -= kingDropMoveAlphaM;
				boss2Model[BossWarrierPart::ArmR].Transform.alpha -= kingDropMoveAlphaM;
				boss2Model[BossWarrierPart::HandL].Transform.alpha -= kingDropMoveAlphaM;
				boss2Model[BossWarrierPart::HandR].Transform.alpha -= kingDropMoveAlphaM;

				if (boss2Model[BossWarrierPart::Chest].Transform.alpha <= 0.0f) {
					IsMoveBefor = true;
					boss2Model[BossWarrierPart::Root].Transform.translation_ = kingDropPos;
					boss2Model[BossWarrierPart::Root].Transform.TransferMatrix();
				}
			}
			if (IsMoveBefor == true) {
				if (IsMoveAfter == false) {
					boss2Model[BossWarrierPart::Chest].Transform.alpha += kingDropMoveAlphaP;
					boss2Model[BossWarrierPart::Head].Transform.alpha += kingDropMoveAlphaP;
					boss2Model[BossWarrierPart::Waist].Transform.alpha += kingDropMoveAlphaP;
					boss2Model[BossWarrierPart::ArmL].Transform.alpha += kingDropMoveAlphaP;
					boss2Model[BossWarrierPart::ArmR].Transform.alpha += kingDropMoveAlphaP;
					boss2Model[BossWarrierPart::HandL].Transform.alpha += kingDropMoveAlphaP;
					boss2Model[BossWarrierPart::HandR].Transform.alpha += kingDropMoveAlphaP;
				}
				if (boss2Model[BossWarrierPart::Chest].Transform.alpha >= 1.0f) {
					IsMoveAfter = true;
					IsKingUp = true;
					IsKingDropPos = true;
				}
			}
		}
		// 体や腕の移動処理
		if (IsKingUp == true) {
			// 体の位置を上げる処理
			if (bodyUpMaxY > boss2Model[BossWarrierPart::Root].Transform.translation_.y) {
				boss2Model[BossWarrierPart::Root].Transform.translation_.y += 0.05f;
			}
			// 腕を上げる処理---------------Todo
			if (armUpTimer < armUpTimeMax) {
				armUpTimer++;
			}

			ImGui::Begin("Font");
			ImGui::SliderFloat3("BossSho L Rot", &shoulderL_RotaEnd.x, -180, 180);
			ImGui::SliderFloat3("BossElbowL Rot", &elbowL_RotaEnd.x, -180, 180);
			ImGui::SliderFloat3("BossSho R Rot", &shoulderR_RotaEnd.x, -180, 180);
			ImGui::SliderFloat3("BossElbowR Rot", &elbowR_RotaEnd.x, -180, 180);
			ImGui::SliderFloat3("Boss L Pos", &zurasi_L_Pos.x, -5, 5);
			ImGui::SliderFloat3("Boss R Pos", &zurasi_R_Pos.x, -5, 5);
			ImGui::End();
			//boss2Model[BossWarrierPart::elbowR].Transform.translation_ = zurasi_R_Pos;
			//boss2Model[BossWarrierPart::elbowL].Transform.translation_ = zurasi_L_Pos;

			//boss2Model[BossWarrierPart::ShoulderR].Transform.SetRot(DegreeToRadianVec3(shoulderR_RotaEnd));
			//boss2Model[BossWarrierPart::elbowR].Transform.SetRot(DegreeToRadianVec3(elbowR_RotaEnd));
			//boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(DegreeToRadianVec3(shoulderL_RotaEnd));
			//boss2Model[BossWarrierPart::elbowL].Transform.SetRot(DegreeToRadianVec3(elbowL_RotaEnd));
			// イージングで腕を回転と位置を動かす
			boss2Model[BossWarrierPart::elbowR].Transform.translation_ = Easing::InOutVec3(defuPos, zurasi_R_Pos, armUpTimer, armUpTimeMax);
			boss2Model[BossWarrierPart::elbowL].Transform.translation_ = Easing::InOutVec3(defuPos, zurasi_L_Pos, armUpTimer, armUpTimeMax);

			boss2Model[BossWarrierPart::ShoulderR].Transform.SetRot(Easing::InOutVec3(StandByShoulderR, DegreeToRadianVec3(shoulderR_RotaEnd), armUpTimer, armUpTimeMax));
			boss2Model[BossWarrierPart::elbowR].Transform.SetRot(Easing::InOutVec3(StandByElbowR, DegreeToRadianVec3(elbowR_RotaEnd), armUpTimer, armUpTimeMax));
			boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(Easing::InOutVec3(StandByShoulderL, DegreeToRadianVec3(shoulderL_RotaEnd), armUpTimer, armUpTimeMax));
			boss2Model[BossWarrierPart::elbowL].Transform.SetRot(Easing::InOutVec3(StandByElbowL, DegreeToRadianVec3(elbowL_RotaEnd), armUpTimer, armUpTimeMax));

			// ボスの位置が上がりきってとイージングが終了したら
			// エネルギーのフラグをオンにする
			if (boss2Model[BossWarrierPart::Root].Transform.translation_.y >= bodyUpMaxY &&
				armUpTimer >= armUpTimeMax) {
				IsKingEnergy = true;

				IsKingUp = false;

			}
		}

		// エネルギーのフラグがオンの時
		if (IsKingEnergy == true) {

			//ImGui::Begin("Energy");
			//ImGui::SliderFloat3("EnergyStartL Pos", &createEnergyStartLPos.x, -10, 10);
			//ImGui::SliderFloat3("EnergyStartR Pos", &createEnergyStartRPos.x, -10, 10);
			//ImGui::SliderFloat3("EnergyEnd Pos", &createEnergyEndPos.x, -250, 250);
			//ImGui::InputFloat("energyposZ", &energyL[0].WorldTrans.translation_.z);
			//ImGui::InputFloat("energyBallScale", &energyBallScale.x);
			//ImGui::End();
			for (int i = 0; i < energyNum; i++) {
				if (energyL[i].IsKingEnergyMoce == false) {
					// エネルギーの移動するタイミングをずらす
					if (energyL[i].startTimer < energyL[i].startTaiming) {
						energyL[i].startTimer++;
					}

					// エネルギーのずらしタイミングに到達したら
					// イージングを始める
					if (energyL[i].startTimer >= energyL[i].startTaiming) {
						// イージングのタイマーをプラス
						if (energyL[i].easingTimer < energyL[i].easingTimeMax) {
							energyL[i].easingTimer += 1;
							energyL[i].easingTimeRate = energyL[i].easingTimer / energyL[i].easingTimeMax;
						}
						// 中間点をずらすのを一回行う
						if (energyL[i].IsZurasi == false) {
							energyL[i].IsZurasi = true;
							energyVelZurasi.x = Random(-5.0f, 5.0f);
							energyVelZurasi.y = Random(-5.0f, 5.0f);
							energyVelZurasi.z = Random(-5.0f, 5.0f);
							EnergyVelL = energyVelHozonL;
							EnergyVelL += energyVelZurasi;
							energyL[i].colPoint = EnergyVelL;
						}

						//energy[0].WorldTrans.translation_ = createEnergyEndPos;
						// ベジエ
						energyL[i].WorldTrans.translation_ = LerpBezireQuadratic(createEnergyStartLPos, energyL[i].colPoint, createEnergyEndPos, energyL[i].easingTimeRate);

						// イージングが完了したらリセットして再度始める準備
						if (energyL[i].easingTimer >= energyL[i].easingTimeMax) {
							energyL[i].startTimer = 0;
							energyL[i].easingTimer = 0;
							energyL[i].IsZurasi = false;
							energyL[i].WorldTrans.translation_ = createEnergyStartLPos;
						}
					}
				}
				if (energyR[i].IsKingEnergyMoce == false) {
					// エネルギーの移動するタイミングをずらす
					if (energyR[i].startTimer < energyR[i].startTaiming) {
						energyR[i].startTimer++;
					}

					// エネルギーのずらしタイミングに到達したら
					// イージングを始める
					if (energyR[i].startTimer >= energyR[i].startTaiming) {
						// イージングのタイマーをプラス
						if (energyR[i].easingTimer < energyR[i].easingTimeMax) {
							energyR[i].easingTimer += 1;
							energyR[i].easingTimeRate = energyR[i].easingTimer / energyR[i].easingTimeMax;
						}
						// 中間点をずらすのを一回行う
						if (energyR[i].IsZurasi == false) {
							energyR[i].IsZurasi = true;
							energyVelZurasi.x = Random(-5.0f, 5.0f);
							energyVelZurasi.y = Random(-5.0f, 5.0f);
							energyVelZurasi.z = Random(-5.0f, 5.0f);
							EnergyVelR = energyVelHozonR;
							EnergyVelR += energyVelZurasi;
							energyR[i].colPoint = EnergyVelR;
						}

						//energy[0].WorldTrans.translation_ = createEnergyEndPos;
						// ベジエ
						energyR[i].WorldTrans.translation_ = LerpBezireQuadratic(createEnergyStartRPos, energyR[i].colPoint, createEnergyEndPos, energyR[i].easingTimeRate);

						// イージングが完了したらリセットして再度始める準備
						if (energyR[i].easingTimer >= energyR[i].easingTimeMax) {
							energyR[i].startTimer = 0;
							energyR[i].easingTimer = 0;
							energyR[i].IsZurasi = false;
							energyR[i].WorldTrans.translation_ = createEnergyStartRPos;
						}
					}
				}
				energyL[i].WorldTrans.TransferMatrix();
				energyR[i].WorldTrans.TransferMatrix();

			}

			// エネルギーの弾を生成する処理
			if (IsKingEnergyBall == false) {
				if (ballZurasiTimer < ballZurasiTimeMax) {
					ballZurasiTimer++;
				}
				// 弾のずらしタイマーに達したら
				if (ballZurasiTimer >= ballZurasiTimeMax) {
					// スケールを徐々に増やす
					energyBallScale += energyBallPlusScale;
					if (energyBallScale.x >= energyBallMaxScale.x) {
						IsKingEnergyBall = true;
					}

					// スケールの代入
					energyBigBall.WorldTrans.scale_ = energyBallScale;
					energyBigBallSub.WorldTrans.scale_ = energyBigBall.WorldTrans.scale_;
					// 行列の更新
					energyBigBall.WorldTrans.TransferMatrix();
					energyBigBallSub.WorldTrans.TransferMatrix();
				}
			}

			// エネルギー弾の生成が完了したら小さい弾の処理を止める
			if (IsKingEnergyBall == true) {
				// 徐々にエネルギーの粒を消す
				for (int i = 0; i < energyNum; i++) {
					energyL[i].WorldTrans.alpha -= 0.01f;
					energyR[i].WorldTrans.alpha -= 0.01f;
				}
				// エネルギーの粒のアルファ値が0以下になったらエネルギーの動く処理を止める
				if (energyL[24].WorldTrans.alpha <= 0.0f) {
					IsKingEnergy = false;
				}

			}
			// エネルギー弾の生成が終わった後
			if (IsKingEnergyBall == true && IsKingEnergy == false) {
				// エネルギー弾が一瞬大きくする処理に入る
				IsEnergyBallBig = true;
				BigEnergyPlusScale = (BigEnergyBallScaleMax - BigEnergyBallScale) / armBigTimeMax;
			}
		}

		// エネルギー弾を一瞬大きくする処理
		if (IsEnergyBallBig == true) {
			//ImGui::Begin("BigMotion");

			//ImGui::SliderFloat3("BossElbowL Rot", &bigElbowLRot.x, -180, 180);
			//ImGui::SliderFloat3("BossElbowR Rot", &bigElbowRRot.x, -180, 180);
			//ImGui::SliderFloat3("Boss L Pos", &bigElbowLPos.x, -5, 5);
			//ImGui::SliderFloat3("Boss R Pos", &bigElbowRPos.x, -5, 5);
			//ImGui::End();

			// イージングのタイマーを進めるのとスケールを同時に大きくする
			if (armBigTimer < armBigTimeMax) {
				armBigTimer++;

			}

			boss2Model[BossWarrierPart::elbowR].Transform.translation_ = Easing::InOutVec3(zurasi_R_Pos, bigElbowRPos, armBigTimer, armBigTimeMax);
			boss2Model[BossWarrierPart::elbowL].Transform.translation_ = Easing::InOutVec3(zurasi_L_Pos, bigElbowLPos, armBigTimer, armBigTimeMax);
			boss2Model[BossWarrierPart::elbowL].Transform.SetRot(Easing::InOutVec3(DegreeToRadianVec3(elbowL_RotaEnd), DegreeToRadianVec3(bigElbowLRot), armBigTimer, armBigTimeMax));
			boss2Model[BossWarrierPart::elbowR].Transform.SetRot(Easing::InOutVec3(DegreeToRadianVec3(elbowR_RotaEnd), DegreeToRadianVec3(bigElbowRRot), armBigTimer, armBigTimeMax));


			// スケールの代入
			energyBigBall.WorldTrans.scale_ = Easing::InOutVec3(BigEnergyBallScale, BigEnergyBallScaleMax, armBigTimer, armBigTimeMax);
			energyBigBallSub.WorldTrans.scale_ = energyBigBall.WorldTrans.scale_;
			// 行列の更新
			energyBigBall.WorldTrans.TransferMatrix();
			energyBigBallSub.WorldTrans.TransferMatrix();
			// 弾のスケールが大きくなったら処理を止め、圧縮の処理に移行
			if (energyBigBall.WorldTrans.scale_.x >= BigEnergyBallScaleMax.x) {
				bigWaitTimer++;
			}

			if (bigWaitTimer >= bigWaitTimeMax) {
				IsEnergyBallBig = false;
				IsEnergyBallCompression = true;

				// 圧縮時のマイナスのあたいをここで求める
				comEnergyMinusScale = (comEnergyBallScaleMax - comEnergyBallScale) / armCompressionTimeMax;
			}
		}

		// エネルギー弾を圧縮する処理
		if (IsEnergyBallCompression == true) {
			//ImGui::Begin("comMotion");
			//ImGui::SliderFloat3("BossElbowL Rot", &comElbowLRot.x, -180, 180);
			//ImGui::SliderFloat3("BossElbowR Rot", &comElbowRRot.x, -180, 180);
			//ImGui::SliderFloat3("Boss L Pos", &comElbowLPos.x, -5, 5);
			//ImGui::SliderFloat3("Boss R Pos", &comElbowRPos.x, -5, 5);
			//ImGui::End();

			// イージングのタイマーを進めるのとスケールを同時に小さくする
			if (armCompressionTimer < armCompressionTimeMax) {
				armCompressionTimer++;
			}

			boss2Model[BossWarrierPart::elbowL].Transform.translation_ = Easing::InOutVec3(bigElbowLPos, comElbowLPos, armCompressionTimer, armCompressionTimeMax);
			boss2Model[BossWarrierPart::elbowR].Transform.translation_ = Easing::InOutVec3(bigElbowRPos, comElbowRPos, armCompressionTimer, armCompressionTimeMax);
			boss2Model[BossWarrierPart::elbowL].Transform.SetRot(Easing::InOutVec3(DegreeToRadianVec3(bigElbowLRot), DegreeToRadianVec3(comElbowLRot), armCompressionTimer, armCompressionTimeMax));
			boss2Model[BossWarrierPart::elbowR].Transform.SetRot(Easing::InOutVec3(DegreeToRadianVec3(bigElbowRRot), DegreeToRadianVec3(comElbowRRot), armCompressionTimer, armCompressionTimeMax));
			//boss2Model[BossWarrierPart::elbowL].Transform.translation_ = comElbowLPos;
			//boss2Model[BossWarrierPart::elbowR].Transform.translation_ = comElbowRPos;
			//boss2Model[BossWarrierPart::elbowL].Transform.SetRot(DegreeToRadianVec3(comElbowLRot));
			//boss2Model[BossWarrierPart::elbowR].Transform.SetRot(DegreeToRadianVec3(comElbowRRot));

			comEnergyBallScale += comEnergyMinusScale;
			// 弾のスケールが小さくなったら処理を止め、腕の振り上げの処理に移行
			if (comEnergyBallScale.x <= comEnergyBallScaleMax.x) {
				IsEnergyBallCompression = false;
				IsEnergyBallFallUp = true;
			}
			// スケールの代入
			energyBigBall.WorldTrans.scale_ = comEnergyBallScale;
			energyBigBallSub.WorldTrans.scale_ = energyBigBall.WorldTrans.scale_;
			// 行列の更新
			energyBigBall.WorldTrans.TransferMatrix();
			energyBigBallSub.WorldTrans.TransferMatrix();
		}

		// エネルギー弾をおろす前動作
		if (IsEnergyBallFallUp == true) {
			// イージングのタイマーを進める
			if (armFallUpTimer < armFallUpTimeMax) {
				armFallUpTimer++;
			}

			boss2Model[BossWarrierPart::elbowL].Transform.translation_ = Easing::InOutVec3(comElbowLPos, fallUpElbowLPos, armFallUpTimer, armFallUpTimeMax);
			boss2Model[BossWarrierPart::elbowR].Transform.translation_ = Easing::InOutVec3(comElbowRPos, fallUpElbowRPos, armFallUpTimer, armFallUpTimeMax);
			boss2Model[BossWarrierPart::elbowL].Transform.SetRot(Easing::InOutVec3(DegreeToRadianVec3(comElbowLRot), DegreeToRadianVec3(fallUpElbowLRot), armFallUpTimer, armFallUpTimeMax));
			boss2Model[BossWarrierPart::elbowR].Transform.SetRot(Easing::InOutVec3(DegreeToRadianVec3(comElbowRRot), DegreeToRadianVec3(fallUpElbowRRot), armFallUpTimer, armFallUpTimeMax));

			// 腕の振り上げの処理のイージングが終了したら、処理を止め、腕の振りおろしに移行
			if (armFallUpTimer >= armFallUpTimeMax) {
				IsEnergyBallFallUp = false;
				IsEnergyBallFallDown = true;
			}
		}

		// エネルギー弾をおろす動作
		if (IsEnergyBallFallDown == true) {
			//ImGui::Begin("fallDown");
			//ImGui::SliderFloat3("energyBallPos", &energyBallFallDownPos.x, -25, 25);
			//ImGui::End();

			// イージングのタイマーを進める
			if (armFallDownTimer < armFallDownTimeMax) {
				armFallDownTimer++;
				armFallDownTimeRate = armFallDownTimer / armFallDownTimeMax;
			}

			boss2Model[BossWarrierPart::elbowL].Transform.translation_ = Easing::InOutVec3(fallUpElbowLPos, fallDownElbowLPos, armFallDownTimer, armFallDownTimeMax);
			boss2Model[BossWarrierPart::elbowR].Transform.translation_ = Easing::InOutVec3(fallUpElbowRPos, fallDownElbowRPos, armFallDownTimer, armFallDownTimeMax);
			boss2Model[BossWarrierPart::elbowL].Transform.SetRot(Easing::InOutVec3(DegreeToRadianVec3(fallUpElbowLRot), DegreeToRadianVec3(fallDownElbowLRot), armFallDownTimer, armFallDownTimeMax));
			boss2Model[BossWarrierPart::elbowR].Transform.SetRot(Easing::InOutVec3(DegreeToRadianVec3(fallUpElbowRRot), DegreeToRadianVec3(fallDownElbowRRot), armFallDownTimer, armFallDownTimeMax));

			// イージングより少し遅れてエネルギー弾が落ちてくる
			if (armFallDownTimeRate >= 0.2f) {
				if (energyBallDownTimer < energyBallDownTimeMax) {
					energyBallDownTimer++;
				}
				energyBigBall.WorldTrans.translation_ = Easing::InOutVec3(createEnergyEndPos, energyBallFallDownPos, energyBallDownTimer, energyBallDownTimeMax);
				energyBigBallSub.WorldTrans.translation_ = energyBigBall.WorldTrans.translation_;

				// 行列の更新
				energyBigBall.WorldTrans.TransferMatrix();
				energyBigBallSub.WorldTrans.TransferMatrix();
			}

			// 弾のイージングが終わったら弾の落下後のうにょうにょ動く処理に移行
			if (energyBallDownTimer >= energyBallDownTimeMax) {
				// 動きの前に圧縮したときのスケール情報を代入
				energyFirstTransScale = comEnergyBallScaleMax;

				//energyBigBallSub.WorldTrans.parent_ = nullptr;
				//energyBigBallSub.WorldTrans.matWorld_ *= boss2Model[BossWarrierPart::Root].Transform.matWorld_;
				//energyBigBallSub.WorldTrans.TransferMatrix();
				IsEnergyBallFallDown = false;
				IsEnergyBallPyupyu = true;
			}


		}

		// エネルギー弾がうにょうにょする動きの処理
		if (IsEnergyBallPyupyu == true) {
			// 最初は普通に拡大させる
			if (IsEnergyScaleFirstTrans == false) {
				if (energyBallUnyoTimer < energyBallFirstTransTimeMax) {
					energyBallUnyoTimer++;
				}
				energyBigBall.WorldTrans.scale_ = Easing::InOutVec3(energyFirstTransScale, energyUnyoUnyoScaleDefu, energyBallUnyoTimer, energyBallFirstTransTimeMax);
				energyBigBallSub.WorldTrans.scale_ = energyBigBall.WorldTrans.scale_ / 3 * 2;

				// 行列の更新
				energyBigBall.WorldTrans.TransferMatrix();
				energyBigBallSub.WorldTrans.TransferMatrix();
				// イージングが終了したらタイマーのリセットして、最初のスケール変形を終了
				if (energyBallUnyoTimer >= energyBallFirstTransTimeMax) {
					energyBallUnyoTimer = 0;
					IsEnergyScaleFirstTrans = true;
				}
			}

			// うにょうにょの動きのカウントがマックスになっていなかったら
			if (energyUnyoUnyoCount < energyUnyoUnyoCountMax &&
				IsEnergyScaleFirstTrans == true) {
				// エネルギー弾の今のスケールを判断して、縮小もしくは拡大
				// 今のスケールがデフォルトの大きさだったらきもい方にイージング
				if (IsEnergyScaleKimo == false) {
					if (energyBallUnyoTimer < energyBallUnyoTimeMax) {
						energyBallUnyoTimer++;
					}
					energyBigBall.WorldTrans.scale_ = Easing::InOutVec3(energyUnyoUnyoScaleDefu, energyUnyoUnyoScaleKimo, energyBallUnyoTimer, energyBallUnyoTimeMax);
					energyBigBallSub.WorldTrans.scale_ = energyBigBall.WorldTrans.scale_ / 3 * 2;

					// 行列の更新
					energyBigBall.WorldTrans.TransferMatrix();
					energyBigBallSub.WorldTrans.TransferMatrix();
					// イージングが終了したらタイマーのリセットして、きもい方の動きに変更
					if (energyBallUnyoTimer >= energyBallUnyoTimeMax) {
						energyBallUnyoTimer = 0;
						IsEnergyScaleKimo = true;
						IsEnergyScaleDefu = false;
					}
				}
				// きもい場合はデフォルトの方にイージング
				else {
					if (energyBallUnyoTimer < energyBallUnyoTimeMax) {
						energyBallUnyoTimer++;
					}
					energyBigBall.WorldTrans.scale_ = Easing::InOutVec3(energyUnyoUnyoScaleKimo, energyUnyoUnyoScaleDefu, energyBallUnyoTimer, energyBallUnyoTimeMax);
					energyBigBallSub.WorldTrans.scale_ = energyBigBall.WorldTrans.scale_ / 3 * 2;

					// 行列の更新
					energyBigBall.WorldTrans.TransferMatrix();
					energyBigBallSub.WorldTrans.TransferMatrix();
					// イージングが終了したらタイマーのリセットして、デフォルトの動きに変更
					if (energyBallUnyoTimer >= energyBallUnyoTimeMax) {
						energyBallUnyoTimer = 0;
						IsEnergyScaleKimo = false;
						IsEnergyScaleDefu = true;
						// きもい方からデフォルトにイージングした場合、ワンセットとみなしカウントを一つ増やす
						energyUnyoUnyoCount += 1;
					}
				}
			}

			// うにょうにょのセットがマックス以上になったら、拡大の処理に移行
			if (energyUnyoUnyoCount >= energyUnyoUnyoCountMax) {
				IsEnergyScaleFirstTrans = false;
				IsEnergyBallPyupyu = false;
				IsEnergyBallFallBig = true;
			}
		}

		// エネルギー弾が拡大して、消えるまでの処理
		if (IsEnergyBallFallBig == true) {
			// 弾のスケールを拡大していく処理
			if (energyScaleTransTimer < energyScaleTransTimeMax) {
				energyScaleTransTimer++;
				energyScaleTransTimeRate = energyScaleTransTimer / energyScaleTransTimeMax;
			}
			energyBigBall.WorldTrans.scale_ = Easing::InOutVec3(energyUnyoUnyoScaleDefu, energyTransScaleToEnd, energyScaleTransTimer, energyScaleTransTimeMax);
			energyBigBallSub.WorldTrans.scale_ = energyBigBall.WorldTrans.scale_ / 3 * 2;


			// スケールのタイムレイトが4割に到達したら、外側の球のアルファ値を下げる
			if (energyScaleTransTimeRate >= 0.4f) {
				if (energyAlphaTransTimer < energyAlphaTransTimeMax) {
					energyAlphaTransTimer++;
				}
				energyBigBall.WorldTrans.alpha = Easing::InOut(energyBallAlphaDefu, energyBallAlphaTrans, energyAlphaTransTimer, energyAlphaTransTimeMax);

			}

			// スケールのタイムレイトが8割りに到達したら、エネルギー弾のすべてのアルファ値を下げる
			if (energyScaleTransTimeRate >= 0.8f) {
				if (energyAlphaEndTimer < energyAlphaEndTimeMax) {
					energyAlphaEndTimer++;
				}
				energyBigBall.WorldTrans.alpha = Easing::InOut(energyBallAlphaTrans, energyBallAlphaTransEnd, energyAlphaEndTimer, energyAlphaEndTimeMax);
				energyBigBallSub.WorldTrans.alpha = Easing::InOut(energyBallAlphaInsideEnd, energyBallAlphaTransEnd, energyAlphaEndTimer, energyAlphaEndTimeMax);

			}

			// 行列の更新
			energyBigBall.WorldTrans.TransferMatrix();
			energyBigBallSub.WorldTrans.TransferMatrix();

			// イージングが終了したら、全ての王のしずくの処理を終了
			if (energyScaleTransTimer >= energyScaleTransTimeMax) {
				IsEnergyBallFallBig = false;
				IsKingDrop = false;
			}
		}
	}

}

void BossWarrier::KingDropReset()
{
	// フラグのリセット
	// 王のしずくを起動
	IsKingDrop = true;

	// 起動時のフラグ関連をまとめてリセット
	IsKingDropPos = false;
	IsBeforPos = false;
	IsMoveBefor = false;
	IsMoveAfter = false;
	IsKingUp = false;
	IsKingEnergy = false;
	IsKingEnergyBall = false;
	IsKingDown = false;

	IsEnergyBallBig = false;
	IsEnergyBallCompression = false;
	IsEnergyBallFallUp = false;
	IsEnergyBallFallDown = false;
	IsEnergyBallPyupyu = false;
	IsEnergyBallFallBig = false;

	IsEnergyScaleFirstTrans = false;
	IsEnergyScaleDefu = false;
	IsEnergyScaleKimo = false;


	// エネルギー弾のパラメータをリセット
	energyBallScale = { 0,0,0 };

	energyBigBall.WorldTrans.translation_ = energyBallPos;
	energyBigBall.WorldTrans.scale_ = energyBallScale;
	energyBigBall.WorldTrans.alpha = 0.95f;
	energyBigBallSub.WorldTrans.translation_ = energyBallPos;
	energyBigBallSub.WorldTrans.scale_ = energyBallScale / 2;
	energyBigBallSub.WorldTrans.alpha = 1.0f;

	// エネルギーの生成の粒のパラメータリセット
	for (int i = 0; i < energyNum; i++) {
		energyL[i].IsKingEnergyMoce = false;
		energyL[i].IsZurasi = false;
		energyL[i].startTimer = 0;
		energyL[i].easingTimer = 0;
		energyL[i].WorldTrans.translation_ = createEnergyStartLPos;
		energyL[i].WorldTrans.alpha = 0.9f;

		energyR[i].IsKingEnergyMoce = false;
		energyR[i].IsZurasi = false;
		energyR[i].startTimer = 0;
		energyR[i].easingTimer = 0;
		energyR[i].WorldTrans.translation_ = createEnergyStartRPos;
		energyR[i].WorldTrans.alpha = 0.9f;

		energyL[i].WorldTrans.TransferMatrix();
		energyR[i].WorldTrans.TransferMatrix();
	}
	comEnergyBallScale = { 5.8f,5.8f, 5.8f };

	// タイマー関連のリセット
	ballZurasiTimer = 0;
	armUpTimer = 0;
	armBigTimer = 0;
	bigWaitTimer = 0;
	armCompressionTimer = 0;
	armFallUpTimer = 0;
	armFallDownTimer = 0;
	energyBallDownTimer = 0;
	energyUnyoUnyoCount = 0;
	energyBallUnyoTimer = 0;
	energyScaleTransTimer = 0;
	energyAlphaTransTimer = 0;
	energyAlphaEndTimer = 0;

}

float BossWarrier::DegreeToRadian(float degree)
{
	float PI = 3.141592f;
	float result = degree * (PI / 180);

	return result;
}

Vector3 BossWarrier::DegreeToRadianVec3(Vector3 degree)
{
	float PI = 3.141592f;
	Vector3 result;
	result.x = degree.x * (PI / 180);
	result.y = degree.y * (PI / 180);
	result.z = degree.z * (PI / 180);

	return result;
}


void BossWarrier::InitAtkSwordSwing()
{
	//各部位の回転角を設定
	//左肩の回転:x180度,y30度,z30度
	//右肩の回転:x0度,y30度,z95度
	//右肘の回転:x30度,y20度,z95度


	atkStartTime = 60;
	isAfter = false;

	//剣の座標を決める(決め方は魚群の時の剣振りから参照)
	Vector3 rotaV[2];
	rotaV[0].x = sin(swordRad);
	rotaV[0].z = cos(swordRad);
	rotaV[0].normalize();
	rotaV[1].x = -sin(swordRad);
	rotaV[1].z = -cos(swordRad);
	rotaV[1].normalize();
	rotaV[0] *= 75.0f;
	rotaV[1] *= 75.0f;

	Matrix4 bossDir = CreateMatRot(targetPos, boss2Model[BossWarrierPart::Root].Transform.translation_);
	swordPos[0] = bossDir.transform(rotaV[0], bossDir) + targetPos;
	swordPos[1] = bossDir.transform(rotaV[1], bossDir) + targetPos;

	//剣の大きさを4倍に
	w[0].scale_ = { 4,4,4 };

	AttackRadius = 16.0f;

	//剣の移動と回転
	Vector3 bossY0;
	bossY0 = boss2Model[BossWarrierPart::Root].Transform.translation_;
	bossY0.y = targetPos.y;
	Vector3 target = targetPos;
	target.y += 10;
	swordPos[0].y = target.y;
	swordPos[1].y = target.y;

	float ease = pow(0, 5);

	w[0].translation_ = LerpBezireQuadratic(swordPos[1], targetPos, swordPos[0], ease);
	w[0].SetMatRot(CreateMatRot(bossY0, w[0].translation_));
	w[0].SetLookMatRot(CreateMatRot(bossY0, w[0].translation_));
	w[0].TransferMatrix();

	Vector3 Pos = w[0].lookRight - w[0].translation_;
	Pos.normalize();

	AttackColliderWorldTrans[0].translation_ = w[0].translation_;
	AttackColliderWorldTrans[1].translation_ = w[0].translation_ + (Pos * 8);
	AttackColliderWorldTrans[2].translation_ = w[0].translation_ + (Pos * 16);
	AttackColliderWorldTrans[3].translation_ = w[0].translation_ + (Pos * 24);
	AttackColliderWorldTrans[4].translation_ = w[0].translation_ + (Pos * 32);
	

	AttackRadius = 12.0f;

	for (int i = 0; i < MAXSWROD; i++)
	{
		AttackColliderWorldTrans[i].scale_ = Vector3(AttackRadius, AttackRadius, AttackRadius);
		AttackColliderWorldTrans[i].TransferMatrix();
		AttackCollider[i]->SetAttribute(COLLISION_ATTR_ENEMYSOWRDATTACK);
		AttackCollider[i]->Update(AttackColliderWorldTrans[i].matWorld_, AttackRadius);
	}

}

void BossWarrier::UpdateAtkSwordSwing()
{
	Vector3 rot = boss2Model[BossWarrierPart::ShoulderL].Transform.rotation_;
	rot = convertRadianToDegree(rot);
	ImGui::Text("rot %f %f %f", rot.x, rot.y, rot.z);
	ImGui::SliderFloat("rotX", &rot.x, 0.0f, 360.0f);
	ImGui::SliderFloat("rotY", &rot.y, 0.0f, 360.0f);
	ImGui::SliderFloat("rotZ", &rot.z, 0.0f, 360.0f);
	rot = convertDegreeToRadian(rot);

	//回転に使う角とそれを制御するデータ
	Vector3 rootRot, shoulderRotL, shoulderRotR, elbowRotR;
	Vector3 dataRootRot[2], dataRotShoulderL[2];

	dataRootRot[0] = { 0,15,0 };
	dataRootRot[1] = { 0,-15,0 };
	dataRotShoulderL[0] = { 180,30,30 };
	dataRotShoulderL[1] = { 300,230,30 };
	dataRotShoulderL[1] = { 300,-130,30 };
	//度数法に変換
	for (int i = 0; i < 2; i++) {
		dataRootRot[i] = convertDegreeToRadian(dataRootRot[i]);
		dataRotShoulderL[i] = convertDegreeToRadian(dataRotShoulderL[i]);
	}

	rootRot = { 0,15,0 };
	shoulderRotL = { 180,30,30 };
	shoulderRotR = { 0,30,95 };
	elbowRotR = { 30,20,95 };
	shoulderRotR = convertDegreeToRadian(shoulderRotR);
	elbowRotR = convertDegreeToRadian(elbowRotR);

	//剣振り開始のカウントダウン
	if (atkStartTime > 0) {
		atkStartTime--;
		//角度は剣振り前の形に
		rootRot = dataRootRot[0];
		shoulderRotL = dataRotShoulderL[0];
		easeRotArm.Start(30);
	}
	//カウントダウン0で攻撃開始
	else if (atkStartTime == 0) {
		easeRotArm.Update();
	}

	if (!easeRotArm.GetActive()) {
		attackEasing.Start(60);
		bossAttackPhase = BossAttackPhase::After;
		w[0].scale_ = { 1,1,1 };
		AttackRadius = 4.0f;
	}


	//イージング
	float ease = pow(easeRotArm.GetTimeRate(), 5);
	rootRot = Lerp(dataRootRot[0], dataRootRot[1], easeRotArm.GetTimeRate());
	shoulderRotL = Lerp(dataRotShoulderL[0], dataRotShoulderL[1], easeRotArm.GetTimeRate());

	Matrix4 rotRoot = CreateMatRot(rootRot);

	//角度をセット
	boss2Model[BossWarrierPart::Root].Transform.SetMatRot(matBossDir * rotRoot);
	boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(shoulderRotL);
	boss2Model[BossWarrierPart::ShoulderR].Transform.SetRot(shoulderRotR);
	boss2Model[BossWarrierPart::elbowR].Transform.SetRot(elbowRotR);

	//剣の移動と回転
	Vector3 bossY0;
	bossY0 = boss2Model[BossWarrierPart::Root].Transform.translation_;
	bossY0.y = targetPos.y;
	Vector3 target = targetPos;
	target.y += 10;
	swordPos[0].y = target.y;
	swordPos[1].y = target.y;


	w[0].translation_ = LerpBezireQuadratic(swordPos[1], targetPos, swordPos[0], ease);
	w[0].SetMatRot(CreateMatRot(bossY0, w[0].translation_));
	w[0].SetLookMatRot(CreateMatRot(bossY0, w[0].translation_));
	w[0].TransferMatrix();

	Vector3 Pos = w[0].lookRight - w[0].translation_;
	Pos.normalize();

	AttackColliderWorldTrans[0].translation_ = w[0].translation_;
	AttackColliderWorldTrans[1].translation_ = w[0].translation_ + (Pos * 8);
	AttackColliderWorldTrans[2].translation_ = w[0].translation_ + (Pos * 16);
	AttackColliderWorldTrans[3].translation_ = w[0].translation_ + (Pos * 24);
	AttackColliderWorldTrans[4].translation_ = w[0].translation_ + (Pos * 32);


	AttackRadius = 12.0f;

	for (int i = 0; i < MAXSWROD; i++)
	{
		AttackColliderWorldTrans[i].scale_ = Vector3(AttackRadius, AttackRadius, AttackRadius);
		AttackColliderWorldTrans[i].TransferMatrix();
		AttackCollider[i]->SetAttribute(COLLISION_ATTR_ENEMYSOWRDATTACK);
		AttackCollider[i]->Update(AttackColliderWorldTrans[i].matWorld_, AttackRadius);
	}

}

void BossWarrier::UpdateSpawn()
{
	//イージング更新
	easeRotArm.Update();

	//スケールをだんだん大きく
	boss2Model[BossWarrierPart::Root].Transform.scale_ = Lerp({ 0,0,0 }, { 15,15,15 }, easeRotArm.GetTimeRate());

	Vector3 spawnPos = { Random(-200,200),Random(0,45) ,Random(-200,200) };
	Vector3 controll = { Random(-45,45),Random(0,45) ,Random(-45,45) };
	float scale = Random(2.0f, 6.0f);

	if (particleCreateTime > 0) {
		spawnParticle->Add(ParticleManager::Type::Out, 20, true, spawnPos, controll, boss2Model[BossWarrierPart::Root].Transform.translation_, scale, scale, Vector4(0, 0, 0, 1), Vector4(45.0f / 256.0f, 0, 45.0f / 256.0f, 1));
		particleCreateTime--;
	}

	spawnParticle->Update();


	if (!easeRotArm.GetActive()) {
		attack = Attack::StandBy;
		spawnParticle->AllDelete();
	}

}

float convertDegreeToRadian(float degree)
{
	return degree * PI / 180.0f;
}

float convertRadianToDegree(float radian)
{
	return radian * 180.0f / PI;
}

Vector3 convertDegreeToRadian(const Vector3& degree)
{
	Vector3 result = degree;
	result.x = convertDegreeToRadian(result.x);
	result.y = convertDegreeToRadian(result.y);
	result.z = convertDegreeToRadian(result.z);
	return result;
}

Vector3 convertRadianToDegree(const Vector3& radian)
{
	Vector3 result = radian;
	result.x = convertRadianToDegree(result.x);
	result.y = convertRadianToDegree(result.y);
	result.z = convertRadianToDegree(result.z);
	return result;
}
