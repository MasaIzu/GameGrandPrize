#include "BossWarrier.h"
#include"Input.h"
#include"ImGuiManager.h"
#include"BossFish.h"

void BossWarrier::Initialize()
{
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

	//�{�X���`�Ԃ̊e���ʏ�����
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Model[i].Transform.Initialize();
	}

	//���͑�{��e�Ɏ���
	boss2Model[BossWarrierPart::Chest].Transform.parent_ = &boss2Model[BossWarrierPart::Root].Transform;

	//�e�q�֌W�c���[(�e���q)
	//�@����@����
	//�����ҁ@����
	//�@�����������r�����I������


	boss2Model[BossWarrierPart::Neck].Transform.parent_ = &boss2Model[BossWarrierPart::Chest].Transform;
	boss2Model[BossWarrierPart::Head].Transform.parent_ = &boss2Model[BossWarrierPart::Neck].Transform;
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
	boss2Model[BossWarrierPart::Root].Transform.scale_ = { 5,5,5 };
	boss2Model[BossWarrierPart::Root].Transform.translation_ = { 0,50,0 };
	boss2Model[BossWarrierPart::Root].Transform.SetRot({ 0,1.57,0 });
	//���ꂼ��̕��ʂ̈ʒu���Z�b�g
	boss2Model[BossWarrierPart::Neck].Transform.translation_ = { 0,3,0 };
	boss2Model[BossWarrierPart::Head].Transform.translation_ = { 0,1.5,0 };
	boss2Model[BossWarrierPart::ShoulderL].Transform.translation_ = { 0,1.5,3 };
	boss2Model[BossWarrierPart::ArmL].Transform.translation_ = { 0,0,2 };
	boss2Model[BossWarrierPart::ShoulderR].Transform.translation_ = { 0,1.5,-3 };
	boss2Model[BossWarrierPart::ArmR].Transform.translation_ = { 0,0,-2 };
	boss2Model[BossWarrierPart::elbowL].Transform.translation_ = { 0,0,2 };
	boss2Model[BossWarrierPart::HandL].Transform.translation_ = { 0,0,2 };
	boss2Model[BossWarrierPart::elbowR].Transform.translation_ = { 0,0,-2 };
	boss2Model[BossWarrierPart::HandR].Transform.translation_ = { 0,0,-2 };
	boss2Model[BossWarrierPart::Crotch].Transform.translation_ = { 0,-3,0 };
	boss2Model[BossWarrierPart::Waist].Transform.translation_ = { 0,-1.5,0 };

	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Model[i].Transform.TransferMatrix();
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

}

void BossWarrier::Update(const Vector3& targetPos)
{
	//�����������o�ɃR�s�[
	this->targetPos = targetPos;

#pragma region �g���l�[�h����(�e�X�g�L�[��L)

	if (Input::GetInstance()->TriggerKey(DIK_L))
	{
		isTornado = true;
	}
	if (isTornado == true)
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

		boss2TornadoTransform[0].SetRot({ 0,TornadoRotY[0],0 });
		boss2TornadoTransform[0].TransferMatrix();
		boss2TornadoTransform[1].SetRot({ 0,TornadoRotY[1],0 });
		boss2TornadoTransform[1].TransferMatrix();
		if (TornadoFlame >= 170)
		{
			isTornado = false;
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

		}
	}

#pragma endregion 

#pragma region �r�U��U������(�e�X�g�L�[��8)

	if (Input::GetInstance()->TriggerKey(DIK_8)) {
		//����������
		InitAtkArmSwing();
	}

	//�r�U��U���t���O��true
	if (isAtkArmSwing) {
		ImGui::Text("boss atack ArmSwing");

		UpdateAtkArmSwing();
	}

#pragma endregion

	boss2TornadoTransform[0].TransferMatrix();
	boss2TornadoTransform[1].TransferMatrix();
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Model[i].Transform.TransferMatrix();
	}
}

void BossWarrier::Draw(const ViewProjection& viewProMat)
{
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		if (boss2Model[i].isDraw == true)
		{
			boss2Model[i].model->Draw(boss2Model[i].Transform, viewProMat);
		}
	}

	boss2TornadeModel->Draw(boss2TornadoTransform[0], viewProMat);
	boss2TornadeModel->Draw(boss2TornadoTransform[1], viewProMat);
}

void BossWarrier::InitAtkArmSwing()
{
	//�r�U��U���t���O��true
	isAtkArmSwing = true;
	//�r�U��^�C�}�[���J�n(30f)�ŏI���悤�ɂ��A30f�o���ƂɃ��[�v������
	easeRotArm.Start(30);

	dataRotArm[0] = { 0,PI / 3.0f - (PI / 2.0f),0 };
	dataRotArm[1] = { 0,-PI / 3.0f - (PI / 2.0f),0 };
	dataRotShoulder[0] = { 0,PI / 6.0f,0 };
	dataRotShoulder[1] = { 0,PI / 3.0f,0 };
	dataRotElbow[0] = { 0,PI / 6.0f,0 };
	dataRotElbow[1] = { 0,PI / 4.0f * 3.0f,0 };
	dummyTargetPos = targetPos;

	isLastAtkStart = false;
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

	//�C�[�W���O�f�[�^�X�V
	easeRotArm.Update();
	//�C�[�W���O���I��������(timeRate��1.0�ȏ�)�C�[�W���O�̃p�����[�^�����ւ��Ă܂��C�[�W���O�J�n
	if (!easeRotArm.GetActive()) {
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
				isAtkArmSwing = false;
			}
		}

	}


	

	//�e�֐߂̉�]����
	Vector3 rotArm, rotShoulderL, rotShoulderR, rotElbowL, rotElbowR;
	rotArm = EaseOutVec3(dataRotArm[0], dataRotArm[1], easeRotArm.GetTimeRate()); 
	rotShoulderL = Lerp(dataRotShoulder[1], dataRotShoulder[0], easeRotArm.GetTimeRate());
	rotShoulderR = Lerp(-dataRotShoulder[0], -dataRotShoulder[1], easeRotArm.GetTimeRate());
	rotElbowL = Lerp(dataRotElbow[1], dataRotElbow[0], easeRotArm.GetTimeRate());
	rotElbowR = Lerp(-dataRotElbow[0], -dataRotElbow[1], easeRotArm.GetTimeRate());

	//	���@��������]�s��A�̂���]�������]�s������ꂼ��쐬
	Matrix4 matBossDir,matBodyRot,matBossRot;

	//�i�s�����Ɍ����������̂Ń_�~�[�̎��@�������Ă��炤
	matBossDir = CreateMatRot(boss2Model[BossWarrierPart::Root].Transform.translation_, dummyTargetPos);

	matBodyRot = CreateMatRot(rotArm);

	matBossRot =  matBodyRot*matBossDir;

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
