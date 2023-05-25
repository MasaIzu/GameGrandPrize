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

	//�����������o�ɃR�s�[
	this->targetPos = targetPos;

	matBossDir = CreateMatRot(boss2Model[BossWarrierPart::Root].Transform.translation_, targetPos);

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
					int randAttack = rand() % 3;
					//randAttack %= 100;
					if (randAttack == 0)
					{
						//����������
						InitAtkArmSwing();
						attackEasing.Start(30);
						bossAttackPhase = BossAttackPhase::Before;
						attack = Attack::ArmSwing;
						boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Gu.get();
						boss2Model[BossWarrierPart::HandR].model = bossArmRModel_Gu.get();
						boss2Model[BossWarrierPart::HandL].Transform.scale_ = { 1.5,2,2 };
						boss2Model[BossWarrierPart::HandR].Transform.scale_ = { 1.5,2,2 };
						boss2Model[BossWarrierPart::HandL].Transform.translation_ = { 1,0,0 };
						boss2Model[BossWarrierPart::HandR].Transform.translation_ = { -1,0,0 };
					}
					if (randAttack == 1)
					{
						attackEasing.Start(30);
						bossAttackPhase = BossAttackPhase::Before;
						attack = Attack::Tornado;
						boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Pa.get();
					}
					if (randAttack == 2)
					{
						attack = Attack::SwordSwing;
						attackEasing.Start(30);
						bossAttackPhase = BossAttackPhase::Before;
						InitAtkSwordSwing();
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
						attackEasing.Start(30);
						bossAttackPhase = BossAttackPhase::Before;
						boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Pa.get();
						StartLaunchSword();
					}
					if (randAttack == 1)
					{
						attackEasing.Start(30);
						attack = Attack::MultiLaunchSword;
						bossAttackPhase = BossAttackPhase::Before;
						boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Pa.get();
						StartMultiLaunchSword();
					}
					if (randAttack == 2)
					{
						attack = Attack::SwordSwing;
						attackEasing.Start(30);
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
			attackEasing.Start(30);
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
			attackEasing.Start(30);
			bossAttackPhase = BossAttackPhase::Before;
			attack = Attack::Tornado;
			boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Pa.get();
		}
		if (Input::GetInstance()->TriggerKey(DIK_L))
		{
			attackEasing.Start(30);
			attack = Attack::MultiLaunchSword;
			bossAttackPhase = BossAttackPhase::Before;
			boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Pa.get();
			StartMultiLaunchSword();
		}
		if (Input::GetInstance()->TriggerKey(DIK_K))
		{
			attack = Attack::LaunchSword;
			attackEasing.Start(30);
			bossAttackPhase = BossAttackPhase::Before;
			boss2Model[BossWarrierPart::HandL].model = bossArmLModel_Pa.get();
			StartLaunchSword();
		}
		if (Input::GetInstance()->TriggerKey(DIK_0)) {
			attack = Attack::SwordSwing;
			attackEasing.Start(30);
			bossAttackPhase = BossAttackPhase::Before;
			InitAtkSwordSwing();
		}

		break;
	case Attack::ArmSwing:
#pragma region �r�U��U������(�e�X�g�L�[��8)

		ImGui::Text("boss atack ArmSwing");

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
	for (int i = 0; i < MAXSWROD; i++)
	{
		w[i].TransferMatrix();
	}

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

	for (int i = 0; i < 5; i++) {

		ModelSpere->Draw(AttackColliderWorldTrans[i], viewProMat);
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
		attackEasing.Start(30);
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
			attackEasing.Start(30);
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
		attackEasing.Start(30);
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

		attackEasing.Start(30);

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
				attack = Attack::StandBy;
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
	matBossDir = CreateMatRot(boss2Model[BossWarrierPart::Root].Transform.translation_, dummyTargetPos);

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
		attackEasing.Start(30);
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
