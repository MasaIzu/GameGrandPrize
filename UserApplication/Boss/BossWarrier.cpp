#include "BossWarrier.h"
#include"Input.h"

void BossWarrier::Initialize()
{
	//ボス2のモデル初期化
	//Rootは大本で動かす親用なので空データでOK
	boss2Model[BossWarrierPart::Chest].reset(Model::CreateFromOBJ("Boss_Body", true));
	boss2Model[BossWarrierPart::Head].reset(Model::CreateFromOBJ("Boss_Head", true));
	boss2Model[BossWarrierPart::Waist].reset(Model::CreateFromOBJ("Boss_Waist", true));
	boss2Model[BossWarrierPart::ArmL].reset(Model::CreateFromOBJ("Boss_ShoulderL", true));
	boss2Model[BossWarrierPart::ArmR].reset(Model::CreateFromOBJ("Boss_ShoulderR", true));
	boss2Model[BossWarrierPart::HandL].reset(Model::CreateFromOBJ("Boss_ArmL", true));
	boss2Model[BossWarrierPart::HandR].reset(Model::CreateFromOBJ("Boss_ArmR", true));

	//ボス第二形態の各部位初期化
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Transform[i].Initialize();
	}

	//胸は大本を親に持つ
	boss2Transform[BossWarrierPart::Chest].parent_ = &boss2Transform[BossWarrierPart::Root];
	//頭、腰、両腕は胸を親に持つ
	boss2Transform[BossWarrierPart::Head].parent_ = &boss2Transform[BossWarrierPart::Chest];
	boss2Transform[BossWarrierPart::Waist].parent_ = &boss2Transform[BossWarrierPart::Chest];
	boss2Transform[BossWarrierPart::ArmL].parent_ = &boss2Transform[BossWarrierPart::Chest];
	boss2Transform[BossWarrierPart::ArmR].parent_ = &boss2Transform[BossWarrierPart::Chest];
	//それぞれの手はそれぞれの腕を親に持つ
	boss2Transform[BossWarrierPart::HandL].parent_ = &boss2Transform[BossWarrierPart::ArmL];
	boss2Transform[BossWarrierPart::HandR].parent_ = &boss2Transform[BossWarrierPart::ArmR];

	//ボスのスケールを5倍に
	boss2Transform[BossWarrierPart::Root].scale_ = { 5,5,5 };

	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Transform[i].TransferMatrix();
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

void BossWarrier::Update()
{
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

	boss2TornadoTransform[0].TransferMatrix();
	boss2TornadoTransform[1].TransferMatrix();
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Transform[i].TransferMatrix();
	}
}

void BossWarrier::Draw(const ViewProjection& viewProMat)
{
	for (int i = BossWarrierPart::Chest; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Model[i]->Draw(boss2Transform[i], viewProMat);
	}

	boss2TornadeModel->Draw(boss2TornadoTransform[0],viewProMat);
	boss2TornadeModel->Draw(boss2TornadoTransform[1], viewProMat);
}
