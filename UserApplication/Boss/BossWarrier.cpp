#include "BossWarrier.h"
#include"Input.h"
#include"ImGuiManager.h"
#include"BossFish.h"
#include <CollisionManager.h>
#include <CollisionAttribute.h>

void BossWarrier::Initialize()
{
	swordModel.reset(Model::CreateFromOBJ("BigSowrd", true));
	input_->GetInstance();
	//�{�X2�̃��f��������
	//Root�͑�{�œ������e�p�Ȃ̂ŋ�f�[�^��OK
	boss2Model[BossWarrierPart::Chest].model.reset(Model::CreateFromOBJ("Boss_Body", true));
	boss2Model[BossWarrierPart::Chest].isDraw = true;
	boss2Model[BossWarrierPart::Head].model.reset(Model::CreateFromOBJ("Boss_Head", true));
	boss2Model[BossWarrierPart::Head].isDraw = true;
	boss2Model[BossWarrierPart::Waist].model.reset(Model::CreateFromOBJ("Boss_Waist", true));
	boss2Model[BossWarrierPart::Waist].isDraw = true;
	boss2Model[BossWarrierPart::ArmL].model.reset(Model::CreateFromOBJ("Boss_ShoulderL", true));
	boss2Model[BossWarrierPart::ArmL].isDraw = true;
	boss2Model[BossWarrierPart::ArmR].model.reset(Model::CreateFromOBJ("Boss_ShoulderR", true));
	boss2Model[BossWarrierPart::ArmR].isDraw = true;
	boss2Model[BossWarrierPart::HandL].model.reset(Model::CreateFromOBJ("Boss_ArmL", true));
	boss2Model[BossWarrierPart::HandL].isDraw = true;
	boss2Model[BossWarrierPart::HandR].model.reset(Model::CreateFromOBJ("Boss_ArmR", true));
	boss2Model[BossWarrierPart::HandR].isDraw = true;

	ModelSpere.reset(Model::CreateFromOBJ("sphere", true));

	//�{�X���`�Ԃ̊e���ʏ�����
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Model[i].Transform.Initialize();

		modelSpere[i].Initialize();

		// �R���W�����}�l�[�W���ɒǉ�
		BossWarrier[i] = new SphereCollider(Vector4(0, BossWarrierRadius, 0, 0), BossWarrierRadius);
		CollisionManager::GetInstance()->AddCollider(BossWarrier[i]);
		BossWarrier[i]->SetAttribute(COLLISION_ATTR_NOTATTACK);
		BossWarrier[i]->Update(boss2Model[i].Transform.matWorld_);

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
	boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot({ 0,0,-PI/4 });
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

		// �R���W�����}�l�[�W���ɒǉ�
		AttackCollider[i] = new SphereCollider(Vector4(0, AttackRadius, 0, 0), AttackRadius);
		CollisionManager::GetInstance()->AddCollider(AttackCollider[i]);
		AttackCollider[i]->SetAttribute(COLLISION_ATTR_NOTATTACK);
		AttackCollider[i]->Update(w[i].matWorld_);
	}

	Tornado = new SphereCollider(Vector4(0, TornadoRadius, 0, 0), TornadoRadius);
	CollisionManager::GetInstance()->AddCollider(Tornado);
	Tornado->SetAttribute(COLLISION_ATTR_ENEMYTORNADOATTACK);
	Tornado->Update(boss2TornadoTransform[0].matWorld_, TornadoRadius);

}

void BossWarrier::Update(const Vector3& targetPos)
{
	//�����������o�ɃR�s�[
	this->targetPos = targetPos;

	Matrix4 matBossDir = CreateMatRot(boss2Model[BossWarrierPart::Root].Transform.translation_, targetPos);

	boss2Model[BossWarrierPart::Root].Transform.SetMatRot(matBossDir);

	for (int i = 0; i < MAXSWROD; i++)
	{
		AttackCollider[i]->SetOldEnemyAttackPos(w[i].matWorld_);
	}

	//swordRad = convertRadianToDegree(swordRad);
	//ImGui::SliderFloat("swordAngle", &swordRad, 0.0f, 360.0f);
	swordRad = convertDegreeToRadian(130);

	Matrix4 bossDir;
	switch (attack)
	{
	case Attack::StandBy:
		ImGui::Text("attack stand");

		if (Input::GetInstance()->TriggerKey(DIK_8)) {
			//����������
			InitAtkArmSwing();
			attackEasing.Start(30);
			bossAttackPhase = BossAttackPhase::Before;
		}
		if (Input::GetInstance()->TriggerKey(DIK_9))
		{
			attackEasing.Start(30);
			bossAttackPhase = BossAttackPhase::Before;
			attack = Attack::Tornado;
		}
		//if (Input::GetInstance()->TriggerKey(DIK_L))
		//{
		//	attackEasing.Start(30);
		//	attack = Attack::MultiLaunchSword;
		//	bossAttackPhase = BossAttackPhase::Before;
		//}
		//if (Input::GetInstance()->TriggerKey(DIK_K))
		//{
		//	attack = Attack::LaunchSword;
		//	attackEasing.Start(30);
		//	bossAttackPhase = BossAttackPhase::Before;
		//}
		//if (Input::GetInstance()->TriggerKey(DIK_0)) {
		//	InitAtkSwordSwing();
		//}

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
			}
			else
			{
				attack = Attack::StandBy;
			}
			break;
		default:
			break;
		}
		attackEasing.Update();
#pragma endregion
		break;
	case Attack::Tornado:
		switch (bossAttackPhase)
		{
		case BossAttackPhase::Before:
			if (attackEasing.GetActive())
			{
				Vector3 rotShoulderL = Lerp(StandByShoulderL, { 0,-PI/2,0 }, attackEasing.GetTimeRate());
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
				Vector3 rotShoulderL = Lerp({ 0,-PI / 2,0 },StandByShoulderL, attackEasing.GetTimeRate());
				Vector3 rotElbowL = Lerp({ 0,0,0 },StandByElbowL, attackEasing.GetTimeRate());
				Vector3 rotRoot = Lerp({0,100,0 }, { 0,0,0 }, attackEasing.GetTimeRate());
				boss2Model[BossWarrierPart::Root].Transform.SetMatRot(CreateMatRot({ 
					           boss2Model[BossWarrierPart::Root].Transform.translation_.x,
							   rotRoot.y ,
							   boss2Model[BossWarrierPart::Root].Transform.translation_.z },
					{ targetPos.x,0,targetPos.z }));

				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(rotShoulderL);
				boss2Model[BossWarrierPart::elbowL].Transform.SetRot(rotElbowL);

				Vector3 boss2Pos = Lerp({ 0,50,0 }, {0,20,0}, attackEasing.GetTimeRate());
				boss2Model[BossWarrierPart::Root].Transform.translation_.y=boss2Pos.y;
			}
			else
			{
				attack = Attack::StandBy;
			}
			break;
		default:
			break;
		}
		attackEasing.Update();
		break;
	case Attack::MultiLaunchSword:
		switch (bossAttackPhase)
		{
		case BossAttackPhase::Before:
			if (attackEasing.GetActive())
			{
				Vector3 rotShoulderL = Lerp(StandByShoulderL, { 0,-PI / 2,0 }, attackEasing.GetTimeRate());
				Vector3 rotElbowL = Lerp(StandByElbowL, { -PI / 2,0,0 }, attackEasing.GetTimeRate());

				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(rotShoulderL);
				boss2Model[BossWarrierPart::elbowL].Transform.SetRot(rotElbowL);
			}
			else
			{
				StartMultiLaunchSword();
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
				Vector3 rotElbowL = Lerp({ -PI / 2,0,0 }, StandByElbowL, attackEasing.GetTimeRate());

				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(rotShoulderL);
				boss2Model[BossWarrierPart::elbowL].Transform.SetRot(rotElbowL);
			}
			else
			{
				attack = Attack::StandBy;
			}
			break;
		default:
			break;
		}
		attackEasing.Update();
		break;
	case Attack::LaunchSword:
		switch (bossAttackPhase)
		{
		case BossAttackPhase::Before:
			if (attackEasing.GetActive())
			{
				Vector3 rotShoulderL = Lerp(StandByShoulderL, { 0,-PI / 2,0 }, attackEasing.GetTimeRate());
				Vector3 rotElbowL = Lerp(StandByElbowL, { -PI / 2,0,0 }, attackEasing.GetTimeRate());

				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(rotShoulderL);
				boss2Model[BossWarrierPart::elbowL].Transform.SetRot(rotElbowL);
			}
			else
			{
				StartLaunchSword();
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
				Vector3 rotElbowL = Lerp({ -PI / 2,0,0 }, StandByElbowL, attackEasing.GetTimeRate());

				boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(rotShoulderL);
				boss2Model[BossWarrierPart::elbowL].Transform.SetRot(rotElbowL);
			}
			else
			{
				attack = Attack::StandBy;
			}
			break;
		default:
			break;
		}
		attackEasing.Update();
		break;

	case Attack::SwordSwing:
		UpdateAtkSwordSwing();
		break;

	default:
		break;
	}

	boss2TornadoTransform[0].TransferMatrix();
	boss2TornadoTransform[1].TransferMatrix();
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Model[i].Transform.TransferMatrix();

		BossWarrier[i]->Update(boss2Model[i].Transform.matWorld_);

		modelSpere[i].translation_ = MyMath::GetWorldTransform(boss2Model[i].Transform.matWorld_);
		modelSpere[i].TransferMatrix();
	}

	ImGui::Begin("Warrier");

	ImGui::Text("TornadoRadius:%f", TornadoRadius);

	ImGui::End();

}

void BossWarrier::Draw(const ViewProjection& viewProMat)
{
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		if (boss2Model[i].isDraw == true)
		{
			boss2Model[i].model->Draw(boss2Model[i].Transform, viewProMat);

			ModelSpere->Draw(modelSpere[i], viewProMat);
		}
	}

	boss2TornadeModel->Draw(boss2TornadoTransform[0], viewProMat);
	boss2TornadeModel->Draw(boss2TornadoTransform[1], viewProMat);

	//ModelSpere->Draw(boss2TornadoTransform[0], viewProMat);

	//���U��U���Ȃ猕����{�`��
	if (attack == Attack::SwordSwing) {
		swordModel->Draw(w[0], viewProMat);
	}

	LaunchSwordDraw(viewProMat);

	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {

		ModelSpere->Draw(modelSpere[i], viewProMat);
	}

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
			w[i].TransferMatrix();
			AttackCollider[i]->SetAttribute(COLLISION_ATTR_ENEMYSOWRDATTACK);
			AttackCollider[i]->Update(w[i].matWorld_);
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
		AttackCollider[i]->Update(w[i].matWorld_);
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
	}
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
	attack = Attack::SwordSwing;
	//�e���ʂ̉�]�p��ݒ�
	//�����̉�]:x180�x,y30�x,z30�x
	//�E���̉�]:x0�x,y30�x,z95�x
	//�E�I�̉�]:x30�x,y20�x,z95�x
	Vector3 rootRot, shoulderRotL, shoulderRotR, elbowRotR;
	rootRot = { 0,15,0 };
	shoulderRotL = { 180,30,30 };
	shoulderRotR = { 0,30,95 };
	elbowRotR = { 30,20,95 };
	rootRot = convertDegreeToRadian(rootRot);
	shoulderRotL = convertDegreeToRadian(shoulderRotL);
	shoulderRotR = convertDegreeToRadian(shoulderRotR);
	elbowRotR = convertDegreeToRadian(elbowRotR);

	boss2Model[BossWarrierPart::Root].Transform.SetRot(rootRot);
	boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(shoulderRotL);
	boss2Model[BossWarrierPart::ShoulderR].Transform.SetRot(shoulderRotR);
	boss2Model[BossWarrierPart::elbowR].Transform.SetRot(elbowRotR);

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
	rotaV[0] *=75.0f;
	rotaV[1] *=75.0f;

	Matrix4 bossDir = CreateMatRot(targetPos, boss2Model[BossWarrierPart::Root].Transform.translation_);
	swordPos[0] = bossDir.transform(rotaV[0], bossDir) + targetPos;
	swordPos[1] = bossDir.transform(rotaV[1], bossDir) + targetPos;

	//���̑傫����4�{��
	w[0].scale_ = { 4,4,4 };
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
		attack = Attack::StandBy;
		w[0].scale_ = { 1,1,1 };
	}


	//�C�[�W���O
	float ease = pow(easeRotArm.GetTimeRate(), 5);
	rootRot = Lerp(dataRootRot[0], dataRootRot[1], easeRotArm.GetTimeRate());
	shoulderRotL = Lerp(dataRotShoulderL[0], dataRotShoulderL[1], easeRotArm.GetTimeRate());

	//�p�x���Z�b�g
	boss2Model[BossWarrierPart::Root].Transform.SetRot(rootRot);
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


	w[0].translation_ = LerpBezireQuadratic(swordPos[1],targetPos, swordPos[0], ease);
	w[0].SetMatRot(CreateMatRot(bossY0,w[0].translation_));
	w[0].TransferMatrix();
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
