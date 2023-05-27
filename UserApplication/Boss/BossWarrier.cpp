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
	//�{�X2�̃��f��������
	//Root�͑�{�œ������e�p�Ȃ̂ŋ�f�[�^��OK
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

	//�{�X���`�Ԃ̊e���ʏ�����
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Model[i].Transform.Initialize();

		modelSpere[i].Initialize();

	}

	{//�����蔻�蔼�a
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

	//���͑�{��e�Ɏ���
	boss2Model[BossWarrierPart::Chest].Transform.parent_ = &boss2Model[BossWarrierPart::Root].Transform;

	//�e�q�֌W�c���[(�e���q)
	//	����
	//�����ҁ@����
	//�@�����������r�����I������


	boss2Model[BossWarrierPart::Head].Transform.parent_ = &boss2Model[BossWarrierPart::Chest].Transform;
	//�����ҁ���
	boss2Model[BossWarrierPart::Crotch].Transform.parent_ = &boss2Model[BossWarrierPart::Chest].Transform;
	boss2Model[BossWarrierPart::Waist].Transform.parent_ = &boss2Model[BossWarrierPart::Crotch].Transform;
	//���聨���I�����r����������
	boss2Model[BossWarrierPart::ShoulderL].Transform.parent_ = &boss2Model[BossWarrierPart::Chest].Transform;
	boss2Model[BossWarrierPart::ArmL].Transform.parent_ = &boss2Model[BossWarrierPart::ShoulderL].Transform;
	boss2Model[BossWarrierPart::ShoulderR].Transform.parent_ = &boss2Model[BossWarrierPart::Chest].Transform;
	boss2Model[BossWarrierPart::ArmR].Transform.parent_ = &boss2Model[BossWarrierPart::ShoulderR].Transform;
	boss2Model[BossWarrierPart::elbowL].Transform.parent_ = &boss2Model[BossWarrierPart::ArmL].Transform;
	boss2Model[BossWarrierPart::HandL].Transform.parent_ = &boss2Model[BossWarrierPart::elbowL].Transform;
	boss2Model[BossWarrierPart::elbowR].Transform.parent_ = &boss2Model[BossWarrierPart::ArmR].Transform;
	boss2Model[BossWarrierPart::HandR].Transform.parent_ = &boss2Model[BossWarrierPart::elbowR].Transform;

	//�{�X�̃X�P�[����5�{��
	boss2Model[BossWarrierPart::Root].Transform.scale_ = { 15,15,15 };
	boss2Model[BossWarrierPart::Root].Transform.translation_ = { 50,20,50 };
	//boss2Model[BossWarrierPart::Root].Transform.SetRot({1.57,0,0});
	//���ꂼ��̕��ʂ̈ʒu���Z�b�g
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
		// �R���W�����}�l�[�W���ɒǉ�
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


	// �G�l���M�[�̏�����
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

	//�̗͂̉摜�ǂݍ���
	healthSprite = Sprite::Create(TextureManager::Load("Hp_inside.png"));
	healthSprite->SetAnchorPoint({ 0,0 });

	healthAlfaSprite = Sprite::Create(TextureManager::Load("Hp_insideAlfa.png"));
	healthAlfaSprite->SetAnchorPoint({ 0,0 });

	HP_barSprite = Sprite::Create(TextureManager::Load("bossBar.png"));
	HP_barSprite->SetAnchorPoint({ 0.5,0.5 });

	// �T�C�Y���Z�b�g����
	healthAlfaSprite->SetSize(hpAlfaSize);
	HP_barSprite->SetSize(HP_barSize);

	// �G�l���M�[�̒e�̐�������Ƃ��̒��ԓ_�̃x�N�g���̏�����
	EnergyVelL = createEnergyEndPos - createEnergyStartLPos;
	EnergyVelL /= 2;
	energyVelHozonL = EnergyVelL;

	EnergyVelR = createEnergyEndPos - createEnergyStartRPos;
	EnergyVelR /= 2;
	energyVelHozonR = EnergyVelR;
	// �G�l���M�[�̋���e�̐�������Ƃ��̃|�W�V����
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
	//�t�F�[�Y���X�|�[���ɕύX
	attack = Attack::Spawm;
	//�X�P�[����0��
	boss2Model[BossWarrierPart::Root].Transform.scale_ = { 0,0,0 };
	boss2Model[BossWarrierPart::Root].Transform.translation_ = boss1Pos;
	//�C�[�W���O�J�n
	easeRotArm.Start(spawnAnimationTime);
	//�p�[�e�B�N���̐������̓C�[�W���O����-20��
	particleCreateTime = spawnAnimationTime -20;
	//�����t���O��HP�̃��Z�b�g
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

	//�����������o�ɃR�s�[
	this->targetPos = targetPos;

	matBossDir = CreateMatRot({ boss2Model[BossWarrierPart::Root].Transform.translation_.x,0,boss2Model[BossWarrierPart::Root].Transform.translation_.z },{ targetPos.x,0,targetPos.z });

	boss2Model[BossWarrierPart::Root].Transform.SetMatRot(matBossDir);

	if (health <= 0) {
		//���S
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
						//����������
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
			//����������
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
#pragma region �r�U��U������(�e�X�g�L�[��8)

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
	// ��U���ɉ��̂������̗�����m�F�p��
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

	//���U��U���Ȃ猕����{�`��
	if (attack == Attack::SwordSwing) {
		swordModel->Draw(w[0], viewProMat);
	}

	LaunchSwordDraw(viewProMat);

	//for (int i = 0; i < 5; i++) {

	//	ModelSpere->Draw(AttackColliderWorldTrans[i], viewProMat);
	//}
	// ���̂������̕`��
	if (IsKingDrop == true) {
		// ���̂������̃G�l���M�[�̐����̗��̕`��
		if (IsKingEnergy == true) {
			for (int i = 0; i < energyNum; i++) {
				energyL[i].model->Draw(energyL[i].WorldTrans, viewProMat);
				energyR[i].model->Draw(energyR[i].WorldTrans, viewProMat);
			}
			// ���̂���������������
			energyBigBallSub.model->Draw(energyBigBallSub.WorldTrans, viewProMat);
			// ���̂������̃G�l���M�[�̋���e�̕`��
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
	// HP�̃Z�b�g
	float nowHp = health / maxHealth;
	hpSize = { 714.0f * nowHp,27.0f };
	healthSprite->SetSize(hpSize);

	// Hp�̉��̕��������炷����
	if (IsHpAlfa) {
		// �U�����󂯂Ă��� 30 �t���[������Hp�͓����Ȃ�
		if (hpAlfaTimer < 30) {
			hpAlfaTimer++;
		}
		else {
			// �ԃQ�[�W���T�C�Y���傫���Ȃ猸�炷
			if (hpSize.x < hpAlfaSize.x) {
				hpAlfaSize.x -= 2.0f;
				healthAlfaSprite->SetSize(hpAlfaSize);
			}
			// �ԃQ�[�W���T�C�Y���������Ȃ����猸�炷�̂���߁A�ԃQ�[�W�̃T�C�Y�ɍ��킹��
			// ���̃Q�[�W�̃t���O���I�t�ɂ���
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

	// �X�v���C�g�`��
	healthAlfaSprite->Draw(pos, { 1,1,1,1 });

	healthSprite->Draw(pos, { 1,1,1,1 });

	HP_barSprite->Draw(HP_barPos, { 1,1,1,1 });
}



void BossWarrier::MultiLaunchSword()
{

	//�U���J�n
	if (t)
	{
		phase2AttackCoolTime--;
		if (phase2AttackCoolTime <= 0)
		{
			//�������������΂��V�[��
			for (int i = 0; i < MAXSWROD; i++)
			{


				//
				// 
				//�v�Z�����x�N�g�������ɓ�����
				w[i].translation_ += num[i].translation_ * 10;
				w[i].TransferMatrix();

				AttackCollider[i]->Update(w[i].matWorld_);

			}
			//�t���O����
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
	//�������̍��W�ݒ�
	for (int i = 0; i < MAXSWROD; i++)
	{

		w[i].translation_ =
		{
			//BOSS���`�Ԃ̐^�񒆂̍��W
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
		//������v���C���[�ւ̃x�N�g���v�Z,��΂��p
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
	//�����΂�


	//�U���J�n
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
					//������v���C���[�ւ̃x�N�g���v�Z,��΂��p
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


		//�������������΂��V�[��
		for (int i = 0; i < MAXSWROD; i++)
		{

			//�v�Z�����x�N�g�������ɓ�����
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
	//������ς���
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
	//�������̍��W�ݒ�
	for (int i = 0; i < MAXSWROD; i++)
	{

		w[i].translation_ =
		{
			//BOSS���`�Ԃ̐^�񒆂̍��W
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
		////������v���C���[�ւ̃x�N�g���v�Z,��΂��p
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

	//引数などでもらってくる変数
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

	//�{�X�̃X�P�[����5�{��
	boss2Model[BossWarrierPart::Root].Transform.scale_ = { 15,15,15 };
	boss2Model[BossWarrierPart::Root].Transform.translation_ = { 50,50,50 };
	//boss2Model[BossWarrierPart::Root].Transform.SetRot({1.57,0,0});
	//���ꂼ��̕��ʂ̈ʒu���Z�b�g
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
	//�r�U��U���t���O��true
	attack = Attack::ArmSwing;
	//�r�U��^�C�}�[���J�n(30f)�ŏI���悤�ɂ��A30f�o���ƂɃ��[�v������
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
	//�ړ��������ŏ���

	//�{�X��30f�̊Ԍ��܂����_�~�[�̍��W�Ɍ������Ĉړ�����
	Vector3 bossMoveVec;
	bossMoveVec = dummyTargetPos - boss2Model[BossWarrierPart::Root].Transform.translation_;
	bossMoveVec.normalize();
	//XZ�ړ��Ȃ̂�Y��0
	bossMoveVec.y = 0;

	//�{�X�̈ړ����x(�X�J���[)��1�b��5�i�ނ��炢(1f�� 5 /60)
	float bossSpdScalar = 5.0f / 10.0f;
	//���K�������ړ��x�N�g���Ƀ{�X���x���|���Z
	bossMoveVec *= bossSpdScalar;

	ImGui::Text("bossMoveVec:%f,%f,%f", bossMoveVec.x, bossMoveVec.y, bossMoveVec.z);
	ImGui::Text("atkCount:%d", lastAtkCount);

	//��{�̃��f�����ړ�
	boss2Model[BossWarrierPart::Root].Transform.translation_ += bossMoveVec;

	if (atkArmSwingTime >= 14)
	{
		bossAttackPhase = BossAttackPhase::After;

		attackEasing.Start(60);

		return;
	}

	//�C�[�W���O�f�[�^�X�V
	easeRotArm.Update();
	//�C�[�W���O���I��������(timeRate��1.0�ȏ�)�C�[�W���O�̃p�����[�^�����ւ��Ă܂��C�[�W���O�J�n
	if (!easeRotArm.GetActive()) {
		atkArmSwingTime++;

		// ���ꂼ��̉�]�f�[�^���X���b�v
		Vector3 data = dataRotArm[0];
		dataRotArm[0] = dataRotArm[1];
		dataRotArm[1] = data;
		data = dataRotElbow[0];
		dataRotElbow[0] = dataRotElbow[1];
		dataRotElbow[1] = data;
		data = dataRotShoulder[0];
		dataRotShoulder[0] = dataRotShoulder[1];
		dataRotShoulder[1] = data;

		//�r�U��J�n
		easeRotArm.Start(30);

		//�_�~�[���W�X�V
		//�U���I���t���O���o���Ă���Ȃ�_�~�[�͂̍��W�͐i�s������
		if (isLastAtkStart) {
			dummyTargetPos += bossMoveVec;
		}
		else {
			dummyTargetPos = targetPos;
		}

		//�{�X�̌��݂̍��W�Ǝ��@���W���߂�������U���I���J�E���g�J�n
		Vector3 bossToTarget = boss2Model->Transform.translation_ - targetPos;
		bossToTarget.y = 0;
		ImGui::Text("length bossToPlayer:%f", bossToTarget.length());
		if (bossToTarget.length() <= 25.0f && !isLastAtkStart) {
			isLastAtkStart = true;
			//�c��̈ړ��񐔂͍��v������ �U���I���J�E���g�̌v�Z�Ɏg���������̓�{�ɂȂ�悤��
			lastAtkCount = 6;
		}
		//�U���I���J�E���g������Ȃ猸�炷
		if (isLastAtkStart) {
			lastAtkCount--;
			//�U���I���J�E���g��0�����Ȃ烂�[�V�����I��
			if (lastAtkCount < 0) {
				bossAttackPhase = BossAttackPhase::After;

				attackEasing.Start(60);
			}
		}

	}




	//�e�֐߂̉�]����
	Vector3 rotArm, rotShoulderL, rotShoulderR, rotElbowL, rotElbowR;
	rotArm = EaseOutVec3(dataRotArm[0], dataRotArm[1], easeRotArm.GetTimeRate());
	rotShoulderL = Lerp(dataRotShoulder[1], dataRotShoulder[0], easeRotArm.GetTimeRate());
	rotShoulderR = Lerp({ dataRotShoulder[0].x ,-dataRotShoulder[0].y ,-dataRotShoulder[0].z }, { dataRotShoulder[1].x ,-dataRotShoulder[1].y ,-dataRotShoulder[1].z }, easeRotArm.GetTimeRate());
	rotElbowL = Lerp(dataRotElbow[1], dataRotElbow[0], easeRotArm.GetTimeRate());
	rotElbowR = Lerp(-dataRotElbow[0], -dataRotElbow[1], easeRotArm.GetTimeRate());

	//	���@��������]�s��A�̂���]�������]�s������ꂼ��쐬
	Matrix4 matBossDir, matBodyRot, matBossRot;

	//�i�s�����Ɍ����������̂Ń_�~�[�̎��@�������Ă��炤
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
	// ���̂�������������
	if (IsKingDrop == true) {
		// ���̂�������ł|�W�V�����Ɉړ�����
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
		// �̂�r�̈ړ�����
		if (IsKingUp == true) {
			// �̂̈ʒu���グ�鏈��
			if (bodyUpMaxY > boss2Model[BossWarrierPart::Root].Transform.translation_.y) {
				boss2Model[BossWarrierPart::Root].Transform.translation_.y += 0.05f;
			}
			// �r���グ�鏈��---------------Todo
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
			// �C�[�W���O�Řr����]�ƈʒu�𓮂���
			boss2Model[BossWarrierPart::elbowR].Transform.translation_ = Easing::InOutVec3(defuPos, zurasi_R_Pos, armUpTimer, armUpTimeMax);
			boss2Model[BossWarrierPart::elbowL].Transform.translation_ = Easing::InOutVec3(defuPos, zurasi_L_Pos, armUpTimer, armUpTimeMax);

			boss2Model[BossWarrierPart::ShoulderR].Transform.SetRot(Easing::InOutVec3(StandByShoulderR, DegreeToRadianVec3(shoulderR_RotaEnd), armUpTimer, armUpTimeMax));
			boss2Model[BossWarrierPart::elbowR].Transform.SetRot(Easing::InOutVec3(StandByElbowR, DegreeToRadianVec3(elbowR_RotaEnd), armUpTimer, armUpTimeMax));
			boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(Easing::InOutVec3(StandByShoulderL, DegreeToRadianVec3(shoulderL_RotaEnd), armUpTimer, armUpTimeMax));
			boss2Model[BossWarrierPart::elbowL].Transform.SetRot(Easing::InOutVec3(StandByElbowL, DegreeToRadianVec3(elbowL_RotaEnd), armUpTimer, armUpTimeMax));

			// �{�X�̈ʒu���オ�肫���ĂƃC�[�W���O���I��������
			// �G�l���M�[�̃t���O���I���ɂ���
			if (boss2Model[BossWarrierPart::Root].Transform.translation_.y >= bodyUpMaxY &&
				armUpTimer >= armUpTimeMax) {
				IsKingEnergy = true;

				IsKingUp = false;

			}
		}

		// �G�l���M�[�̃t���O���I���̎�
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
					// �G�l���M�[�̈ړ�����^�C�~���O�����炷
					if (energyL[i].startTimer < energyL[i].startTaiming) {
						energyL[i].startTimer++;
					}

					// �G�l���M�[�̂��炵�^�C�~���O�ɓ��B������
					// �C�[�W���O���n�߂�
					if (energyL[i].startTimer >= energyL[i].startTaiming) {
						// �C�[�W���O�̃^�C�}�[���v���X
						if (energyL[i].easingTimer < energyL[i].easingTimeMax) {
							energyL[i].easingTimer += 1;
							energyL[i].easingTimeRate = energyL[i].easingTimer / energyL[i].easingTimeMax;
						}
						// ���ԓ_�����炷�̂����s��
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
						// �x�W�G
						energyL[i].WorldTrans.translation_ = LerpBezireQuadratic(createEnergyStartLPos, energyL[i].colPoint, createEnergyEndPos, energyL[i].easingTimeRate);

						// �C�[�W���O�����������烊�Z�b�g���čēx�n�߂鏀��
						if (energyL[i].easingTimer >= energyL[i].easingTimeMax) {
							energyL[i].startTimer = 0;
							energyL[i].easingTimer = 0;
							energyL[i].IsZurasi = false;
							energyL[i].WorldTrans.translation_ = createEnergyStartLPos;
						}
					}
				}
				if (energyR[i].IsKingEnergyMoce == false) {
					// �G�l���M�[�̈ړ�����^�C�~���O�����炷
					if (energyR[i].startTimer < energyR[i].startTaiming) {
						energyR[i].startTimer++;
					}

					// �G�l���M�[�̂��炵�^�C�~���O�ɓ��B������
					// �C�[�W���O���n�߂�
					if (energyR[i].startTimer >= energyR[i].startTaiming) {
						// �C�[�W���O�̃^�C�}�[���v���X
						if (energyR[i].easingTimer < energyR[i].easingTimeMax) {
							energyR[i].easingTimer += 1;
							energyR[i].easingTimeRate = energyR[i].easingTimer / energyR[i].easingTimeMax;
						}
						// ���ԓ_�����炷�̂����s��
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
						// �x�W�G
						energyR[i].WorldTrans.translation_ = LerpBezireQuadratic(createEnergyStartRPos, energyR[i].colPoint, createEnergyEndPos, energyR[i].easingTimeRate);

						// �C�[�W���O�����������烊�Z�b�g���čēx�n�߂鏀��
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

			// �G�l���M�[�̒e�𐶐����鏈��
			if (IsKingEnergyBall == false) {
				if (ballZurasiTimer < ballZurasiTimeMax) {
					ballZurasiTimer++;
				}
				// �e�̂��炵�^�C�}�[�ɒB������
				if (ballZurasiTimer >= ballZurasiTimeMax) {
					// �X�P�[�������X�ɑ��₷
					energyBallScale += energyBallPlusScale;
					if (energyBallScale.x >= energyBallMaxScale.x) {
						IsKingEnergyBall = true;
					}

					// �X�P�[���̑��
					energyBigBall.WorldTrans.scale_ = energyBallScale;
					energyBigBallSub.WorldTrans.scale_ = energyBigBall.WorldTrans.scale_;
					// �s��̍X�V
					energyBigBall.WorldTrans.TransferMatrix();
					energyBigBallSub.WorldTrans.TransferMatrix();
				}
			}

			// �G�l���M�[�e�̐��������������珬�����e�̏������~�߂�
			if (IsKingEnergyBall == true) {
				// ���X�ɃG�l���M�[�̗�������
				for (int i = 0; i < energyNum; i++) {
					energyL[i].WorldTrans.alpha -= 0.01f;
					energyR[i].WorldTrans.alpha -= 0.01f;
				}
				// �G�l���M�[�̗��̃A���t�@�l��0�ȉ��ɂȂ�����G�l���M�[�̓����������~�߂�
				if (energyL[24].WorldTrans.alpha <= 0.0f) {
					IsKingEnergy = false;
				}

			}
			// �G�l���M�[�e�̐������I�������
			if (IsKingEnergyBall == true && IsKingEnergy == false) {
				// �G�l���M�[�e����u�傫�����鏈���ɓ���
				IsEnergyBallBig = true;
				BigEnergyPlusScale = (BigEnergyBallScaleMax - BigEnergyBallScale) / armBigTimeMax;
			}
		}

		// �G�l���M�[�e����u�傫�����鏈��
		if (IsEnergyBallBig == true) {
			//ImGui::Begin("BigMotion");

			//ImGui::SliderFloat3("BossElbowL Rot", &bigElbowLRot.x, -180, 180);
			//ImGui::SliderFloat3("BossElbowR Rot", &bigElbowRRot.x, -180, 180);
			//ImGui::SliderFloat3("Boss L Pos", &bigElbowLPos.x, -5, 5);
			//ImGui::SliderFloat3("Boss R Pos", &bigElbowRPos.x, -5, 5);
			//ImGui::End();

			// �C�[�W���O�̃^�C�}�[��i�߂�̂ƃX�P�[���𓯎��ɑ傫������
			if (armBigTimer < armBigTimeMax) {
				armBigTimer++;

			}

			boss2Model[BossWarrierPart::elbowR].Transform.translation_ = Easing::InOutVec3(zurasi_R_Pos, bigElbowRPos, armBigTimer, armBigTimeMax);
			boss2Model[BossWarrierPart::elbowL].Transform.translation_ = Easing::InOutVec3(zurasi_L_Pos, bigElbowLPos, armBigTimer, armBigTimeMax);
			boss2Model[BossWarrierPart::elbowL].Transform.SetRot(Easing::InOutVec3(DegreeToRadianVec3(elbowL_RotaEnd), DegreeToRadianVec3(bigElbowLRot), armBigTimer, armBigTimeMax));
			boss2Model[BossWarrierPart::elbowR].Transform.SetRot(Easing::InOutVec3(DegreeToRadianVec3(elbowR_RotaEnd), DegreeToRadianVec3(bigElbowRRot), armBigTimer, armBigTimeMax));


			// �X�P�[���̑��
			energyBigBall.WorldTrans.scale_ = Easing::InOutVec3(BigEnergyBallScale, BigEnergyBallScaleMax, armBigTimer, armBigTimeMax);
			energyBigBallSub.WorldTrans.scale_ = energyBigBall.WorldTrans.scale_;
			// �s��̍X�V
			energyBigBall.WorldTrans.TransferMatrix();
			energyBigBallSub.WorldTrans.TransferMatrix();
			// �e�̃X�P�[�����傫���Ȃ����珈�����~�߁A���k�̏����Ɉڍs
			if (energyBigBall.WorldTrans.scale_.x >= BigEnergyBallScaleMax.x) {
				bigWaitTimer++;
			}

			if (bigWaitTimer >= bigWaitTimeMax) {
				IsEnergyBallBig = false;
				IsEnergyBallCompression = true;

				// ���k���̃}�C�i�X�̂������������ŋ��߂�
				comEnergyMinusScale = (comEnergyBallScaleMax - comEnergyBallScale) / armCompressionTimeMax;
			}
		}

		// �G�l���M�[�e�����k���鏈��
		if (IsEnergyBallCompression == true) {
			//ImGui::Begin("comMotion");
			//ImGui::SliderFloat3("BossElbowL Rot", &comElbowLRot.x, -180, 180);
			//ImGui::SliderFloat3("BossElbowR Rot", &comElbowRRot.x, -180, 180);
			//ImGui::SliderFloat3("Boss L Pos", &comElbowLPos.x, -5, 5);
			//ImGui::SliderFloat3("Boss R Pos", &comElbowRPos.x, -5, 5);
			//ImGui::End();

			// �C�[�W���O�̃^�C�}�[��i�߂�̂ƃX�P�[���𓯎��ɏ���������
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
			// �e�̃X�P�[�����������Ȃ����珈�����~�߁A�r�̐U��グ�̏����Ɉڍs
			if (comEnergyBallScale.x <= comEnergyBallScaleMax.x) {
				IsEnergyBallCompression = false;
				IsEnergyBallFallUp = true;
			}
			// �X�P�[���̑��
			energyBigBall.WorldTrans.scale_ = comEnergyBallScale;
			energyBigBallSub.WorldTrans.scale_ = energyBigBall.WorldTrans.scale_;
			// �s��̍X�V
			energyBigBall.WorldTrans.TransferMatrix();
			energyBigBallSub.WorldTrans.TransferMatrix();
		}

		// �G�l���M�[�e�����낷�O����
		if (IsEnergyBallFallUp == true) {
			// �C�[�W���O�̃^�C�}�[��i�߂�
			if (armFallUpTimer < armFallUpTimeMax) {
				armFallUpTimer++;
			}

			boss2Model[BossWarrierPart::elbowL].Transform.translation_ = Easing::InOutVec3(comElbowLPos, fallUpElbowLPos, armFallUpTimer, armFallUpTimeMax);
			boss2Model[BossWarrierPart::elbowR].Transform.translation_ = Easing::InOutVec3(comElbowRPos, fallUpElbowRPos, armFallUpTimer, armFallUpTimeMax);
			boss2Model[BossWarrierPart::elbowL].Transform.SetRot(Easing::InOutVec3(DegreeToRadianVec3(comElbowLRot), DegreeToRadianVec3(fallUpElbowLRot), armFallUpTimer, armFallUpTimeMax));
			boss2Model[BossWarrierPart::elbowR].Transform.SetRot(Easing::InOutVec3(DegreeToRadianVec3(comElbowRRot), DegreeToRadianVec3(fallUpElbowRRot), armFallUpTimer, armFallUpTimeMax));

			// �r�̐U��グ�̏����̃C�[�W���O���I��������A�������~�߁A�r�̐U�肨�낵�Ɉڍs
			if (armFallUpTimer >= armFallUpTimeMax) {
				IsEnergyBallFallUp = false;
				IsEnergyBallFallDown = true;
			}
		}

		// �G�l���M�[�e�����낷����
		if (IsEnergyBallFallDown == true) {
			//ImGui::Begin("fallDown");
			//ImGui::SliderFloat3("energyBallPos", &energyBallFallDownPos.x, -25, 25);
			//ImGui::End();

			// �C�[�W���O�̃^�C�}�[��i�߂�
			if (armFallDownTimer < armFallDownTimeMax) {
				armFallDownTimer++;
				armFallDownTimeRate = armFallDownTimer / armFallDownTimeMax;
			}

			boss2Model[BossWarrierPart::elbowL].Transform.translation_ = Easing::InOutVec3(fallUpElbowLPos, fallDownElbowLPos, armFallDownTimer, armFallDownTimeMax);
			boss2Model[BossWarrierPart::elbowR].Transform.translation_ = Easing::InOutVec3(fallUpElbowRPos, fallDownElbowRPos, armFallDownTimer, armFallDownTimeMax);
			boss2Model[BossWarrierPart::elbowL].Transform.SetRot(Easing::InOutVec3(DegreeToRadianVec3(fallUpElbowLRot), DegreeToRadianVec3(fallDownElbowLRot), armFallDownTimer, armFallDownTimeMax));
			boss2Model[BossWarrierPart::elbowR].Transform.SetRot(Easing::InOutVec3(DegreeToRadianVec3(fallUpElbowRRot), DegreeToRadianVec3(fallDownElbowRRot), armFallDownTimer, armFallDownTimeMax));

			// �C�[�W���O��菭���x��ăG�l���M�[�e�������Ă���
			if (armFallDownTimeRate >= 0.2f) {
				if (energyBallDownTimer < energyBallDownTimeMax) {
					energyBallDownTimer++;
				}
				energyBigBall.WorldTrans.translation_ = Easing::InOutVec3(createEnergyEndPos, energyBallFallDownPos, energyBallDownTimer, energyBallDownTimeMax);
				energyBigBallSub.WorldTrans.translation_ = energyBigBall.WorldTrans.translation_;

				// �s��̍X�V
				energyBigBall.WorldTrans.TransferMatrix();
				energyBigBallSub.WorldTrans.TransferMatrix();
			}

			// �e�̃C�[�W���O���I�������e�̗�����̂��ɂ傤�ɂ哮�������Ɉڍs
			if (energyBallDownTimer >= energyBallDownTimeMax) {
				// �����̑O�Ɉ��k�����Ƃ��̃X�P�[��������
				energyFirstTransScale = comEnergyBallScaleMax;

				//energyBigBallSub.WorldTrans.parent_ = nullptr;
				//energyBigBallSub.WorldTrans.matWorld_ *= boss2Model[BossWarrierPart::Root].Transform.matWorld_;
				//energyBigBallSub.WorldTrans.TransferMatrix();
				IsEnergyBallFallDown = false;
				IsEnergyBallPyupyu = true;
			}


		}

		// �G�l���M�[�e�����ɂ傤�ɂ傷�铮���̏���
		if (IsEnergyBallPyupyu == true) {
			// �ŏ��͕��ʂɊg�傳����
			if (IsEnergyScaleFirstTrans == false) {
				if (energyBallUnyoTimer < energyBallFirstTransTimeMax) {
					energyBallUnyoTimer++;
				}
				energyBigBall.WorldTrans.scale_ = Easing::InOutVec3(energyFirstTransScale, energyUnyoUnyoScaleDefu, energyBallUnyoTimer, energyBallFirstTransTimeMax);
				energyBigBallSub.WorldTrans.scale_ = energyBigBall.WorldTrans.scale_ / 3 * 2;

				// �s��̍X�V
				energyBigBall.WorldTrans.TransferMatrix();
				energyBigBallSub.WorldTrans.TransferMatrix();
				// �C�[�W���O���I��������^�C�}�[�̃��Z�b�g���āA�ŏ��̃X�P�[���ό`���I��
				if (energyBallUnyoTimer >= energyBallFirstTransTimeMax) {
					energyBallUnyoTimer = 0;
					IsEnergyScaleFirstTrans = true;
				}
			}

			// ���ɂ傤�ɂ�̓����̃J�E���g���}�b�N�X�ɂȂ��Ă��Ȃ�������
			if (energyUnyoUnyoCount < energyUnyoUnyoCountMax &&
				IsEnergyScaleFirstTrans == true) {
				// �G�l���M�[�e�̍��̃X�P�[���𔻒f���āA�k���������͊g��
				// ���̃X�P�[�����f�t�H���g�̑傫���������炫�������ɃC�[�W���O
				if (IsEnergyScaleKimo == false) {
					if (energyBallUnyoTimer < energyBallUnyoTimeMax) {
						energyBallUnyoTimer++;
					}
					energyBigBall.WorldTrans.scale_ = Easing::InOutVec3(energyUnyoUnyoScaleDefu, energyUnyoUnyoScaleKimo, energyBallUnyoTimer, energyBallUnyoTimeMax);
					energyBigBallSub.WorldTrans.scale_ = energyBigBall.WorldTrans.scale_ / 3 * 2;

					// �s��̍X�V
					energyBigBall.WorldTrans.TransferMatrix();
					energyBigBallSub.WorldTrans.TransferMatrix();
					// �C�[�W���O���I��������^�C�}�[�̃��Z�b�g���āA���������̓����ɕύX
					if (energyBallUnyoTimer >= energyBallUnyoTimeMax) {
						energyBallUnyoTimer = 0;
						IsEnergyScaleKimo = true;
						IsEnergyScaleDefu = false;
					}
				}
				// �������ꍇ�̓f�t�H���g�̕��ɃC�[�W���O
				else {
					if (energyBallUnyoTimer < energyBallUnyoTimeMax) {
						energyBallUnyoTimer++;
					}
					energyBigBall.WorldTrans.scale_ = Easing::InOutVec3(energyUnyoUnyoScaleKimo, energyUnyoUnyoScaleDefu, energyBallUnyoTimer, energyBallUnyoTimeMax);
					energyBigBallSub.WorldTrans.scale_ = energyBigBall.WorldTrans.scale_ / 3 * 2;

					// �s��̍X�V
					energyBigBall.WorldTrans.TransferMatrix();
					energyBigBallSub.WorldTrans.TransferMatrix();
					// �C�[�W���O���I��������^�C�}�[�̃��Z�b�g���āA�f�t�H���g�̓����ɕύX
					if (energyBallUnyoTimer >= energyBallUnyoTimeMax) {
						energyBallUnyoTimer = 0;
						IsEnergyScaleKimo = false;
						IsEnergyScaleDefu = true;
						// ������������f�t�H���g�ɃC�[�W���O�����ꍇ�A�����Z�b�g�Ƃ݂Ȃ��J�E���g������₷
						energyUnyoUnyoCount += 1;
					}
				}
			}

			// ���ɂ傤�ɂ�̃Z�b�g���}�b�N�X�ȏ�ɂȂ�����A�g��̏����Ɉڍs
			if (energyUnyoUnyoCount >= energyUnyoUnyoCountMax) {
				IsEnergyScaleFirstTrans = false;
				IsEnergyBallPyupyu = false;
				IsEnergyBallFallBig = true;
			}
		}

		// �G�l���M�[�e���g�債�āA������܂ł̏���
		if (IsEnergyBallFallBig == true) {
			// �e�̃X�P�[�����g�債�Ă�������
			if (energyScaleTransTimer < energyScaleTransTimeMax) {
				energyScaleTransTimer++;
				energyScaleTransTimeRate = energyScaleTransTimer / energyScaleTransTimeMax;
			}
			energyBigBall.WorldTrans.scale_ = Easing::InOutVec3(energyUnyoUnyoScaleDefu, energyTransScaleToEnd, energyScaleTransTimer, energyScaleTransTimeMax);
			energyBigBallSub.WorldTrans.scale_ = energyBigBall.WorldTrans.scale_ / 3 * 2;


			// �X�P�[���̃^�C�����C�g��4���ɓ��B������A�O���̋��̃A���t�@�l��������
			if (energyScaleTransTimeRate >= 0.4f) {
				if (energyAlphaTransTimer < energyAlphaTransTimeMax) {
					energyAlphaTransTimer++;
				}
				energyBigBall.WorldTrans.alpha = Easing::InOut(energyBallAlphaDefu, energyBallAlphaTrans, energyAlphaTransTimer, energyAlphaTransTimeMax);

			}

			// �X�P�[���̃^�C�����C�g��8����ɓ��B������A�G�l���M�[�e�̂��ׂẴA���t�@�l��������
			if (energyScaleTransTimeRate >= 0.8f) {
				if (energyAlphaEndTimer < energyAlphaEndTimeMax) {
					energyAlphaEndTimer++;
				}
				energyBigBall.WorldTrans.alpha = Easing::InOut(energyBallAlphaTrans, energyBallAlphaTransEnd, energyAlphaEndTimer, energyAlphaEndTimeMax);
				energyBigBallSub.WorldTrans.alpha = Easing::InOut(energyBallAlphaInsideEnd, energyBallAlphaTransEnd, energyAlphaEndTimer, energyAlphaEndTimeMax);

			}

			// �s��̍X�V
			energyBigBall.WorldTrans.TransferMatrix();
			energyBigBallSub.WorldTrans.TransferMatrix();

			// �C�[�W���O���I��������A�S�Ẳ��̂������̏������I��
			if (energyScaleTransTimer >= energyScaleTransTimeMax) {
				IsEnergyBallFallBig = false;
				IsKingDrop = false;
			}
		}
	}

}

void BossWarrier::KingDropReset()
{
	// �t���O�̃��Z�b�g
	// ���̂��������N��
	IsKingDrop = true;

	// �N�����̃t���O�֘A���܂Ƃ߂ă��Z�b�g
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


	// �G�l���M�[�e�̃p�����[�^�����Z�b�g
	energyBallScale = { 0,0,0 };

	energyBigBall.WorldTrans.translation_ = energyBallPos;
	energyBigBall.WorldTrans.scale_ = energyBallScale;
	energyBigBall.WorldTrans.alpha = 0.95f;
	energyBigBallSub.WorldTrans.translation_ = energyBallPos;
	energyBigBallSub.WorldTrans.scale_ = energyBallScale / 2;
	energyBigBallSub.WorldTrans.alpha = 1.0f;

	// �G�l���M�[�̐����̗��̃p�����[�^���Z�b�g
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

	// �^�C�}�[�֘A�̃��Z�b�g
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
	//�e���ʂ̉�]�p��ݒ�
	//�����̉�]:x180�x,y30�x,z30�x
	//�E���̉�]:x0�x,y30�x,z95�x
	//�E�I�̉�]:x30�x,y20�x,z95�x


	atkStartTime = 60;
	isAfter = false;

	//���̍��W�����߂�(���ߕ��͋��Q�̎��̌��U�肩��Q��)
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

	//���̑傫����4�{��
	w[0].scale_ = { 4,4,4 };

	AttackRadius = 16.0f;

	//���̈ړ��Ɖ�]
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

	//��]�Ɏg���p�Ƃ���𐧌䂷��f�[�^
	Vector3 rootRot, shoulderRotL, shoulderRotR, elbowRotR;
	Vector3 dataRootRot[2], dataRotShoulderL[2];

	dataRootRot[0] = { 0,15,0 };
	dataRootRot[1] = { 0,-15,0 };
	dataRotShoulderL[0] = { 180,30,30 };
	dataRotShoulderL[1] = { 300,230,30 };
	dataRotShoulderL[1] = { 300,-130,30 };
	//�x���@�ɕϊ�
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

	//���U��J�n�̃J�E���g�_�E��
	if (atkStartTime > 0) {
		atkStartTime--;
		//�p�x�͌��U��O�̌`��
		rootRot = dataRootRot[0];
		shoulderRotL = dataRotShoulderL[0];
		easeRotArm.Start(30);
	}
	//�J�E���g�_�E��0�ōU���J�n
	else if (atkStartTime == 0) {
		easeRotArm.Update();
	}

	if (!easeRotArm.GetActive()) {
		attackEasing.Start(60);
		bossAttackPhase = BossAttackPhase::After;
		w[0].scale_ = { 1,1,1 };
		AttackRadius = 4.0f;
	}


	//�C�[�W���O
	float ease = pow(easeRotArm.GetTimeRate(), 5);
	rootRot = Lerp(dataRootRot[0], dataRootRot[1], easeRotArm.GetTimeRate());
	shoulderRotL = Lerp(dataRotShoulderL[0], dataRotShoulderL[1], easeRotArm.GetTimeRate());

	Matrix4 rotRoot = CreateMatRot(rootRot);

	//�p�x���Z�b�g
	boss2Model[BossWarrierPart::Root].Transform.SetMatRot(matBossDir * rotRoot);
	boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(shoulderRotL);
	boss2Model[BossWarrierPart::ShoulderR].Transform.SetRot(shoulderRotR);
	boss2Model[BossWarrierPart::elbowR].Transform.SetRot(elbowRotR);

	//���̈ړ��Ɖ�]
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
	//�C�[�W���O�X�V
	easeRotArm.Update();

	//�X�P�[�������񂾂�傫��
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
