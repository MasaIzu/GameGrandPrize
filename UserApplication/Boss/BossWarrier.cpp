#include "BossWarrier.h"
#include"Input.h"
#include"ImGuiManager.h"

void BossWarrier::Initialize()
{
	swordModel.reset(Model::CreateFromOBJ("BigSowrd", true));
	input_->GetInstance();
	//ボス2のモデル初期化
	//Rootは大本で動かす親用なので空データでOK
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

	//ボス第二形態の各部位初期化
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Model[i].Transform.Initialize();
	}

	//胸は大本を親に持つ
	boss2Model[BossWarrierPart::Chest].Transform.parent_ = &boss2Model[BossWarrierPart::Root].Transform;

	//親子関係ツリー(親→子)
	//　→首　→頭
	//胸→股　→腰
	//　→両肩→両腕→両肘→両手


	boss2Model[BossWarrierPart::Neck].Transform.parent_ = &boss2Model[BossWarrierPart::Chest].Transform;
	boss2Model[BossWarrierPart::Head].Transform.parent_ = &boss2Model[BossWarrierPart::Neck].Transform;
	//腰→股→胸
	boss2Model[BossWarrierPart::Crotch ].Transform.parent_ = &boss2Model[BossWarrierPart::Chest].Transform;
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
	boss2Model[BossWarrierPart::Root].Transform.scale_ = { 5,5,5 };
	boss2Model[BossWarrierPart::Root].Transform.translation_ = { 0,50,0 };
	boss2Model[BossWarrierPart::Root].Transform.SetRot({ 0,1.57,0 });
	//それぞれの部位の位置をセット
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

void BossWarrier::Update()
{
#pragma region トルネード処理(テストキーはL)

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

#pragma region 腕振り攻撃処理(テストキーは8)

	if (Input::GetInstance()->TriggerKey(DIK_8)) {
		//初期化処理
		InitAtkArmSwing();
	}

	//腕振り攻撃フラグがtrue
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
		if (boss2Model[i].isDraw==true)
		{
			boss2Model[i].model->Draw(boss2Model[i].Transform, viewProMat);
		}
	}

	boss2TornadeModel->Draw(boss2TornadoTransform[0],viewProMat);
	boss2TornadeModel->Draw(boss2TornadoTransform[1], viewProMat);
}

void BossWarrier::phase2Attack()
{
	

//確認用の攻撃キー
if (input_->PushKey(DIK_L))
{
	//isOn = true;



	//生成時の座標設定
	for (int i = 0; i < MAXSWROD; i++)
	{

		w[i].translation_ = 
		{
			//BOSS第二形態の真ん中の座標
			//fishParent.pos.translation_.x - interval * 2 + i * interval,
			//fishParent.pos.translation_.y + 10 ,
			//fishParent.pos.translation_.z 
		};

		w[i].TransferMatrix();
		
	}

	for (int i = 0; i < MAXSWROD; i++)
	{
		//
		//剣からプレイヤーへのベクトル計算,飛ばす用
		pPos[i].translation_ = pl->GetWorldPosition();
		num[i].translation_ = pPos[i].translation_ - w[i].translation_;
		num[i].translation_.normalize();
		
		num[i].TransferMatrix();
		

	}
		

	t = true;
	isSat = true;
	Rota();
}

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

		

		}
		//フラグ解除
		if (phase2AttackCoolTime <= -40)
		{
			
			//t = false;

		}
	}
}
else
{
	phase2AttackCoolTime = 70;
	isSat = false;
}


}

void BossWarrier::phase2AttackP2()
{
	//一個ずつ飛ばす
//確認用の攻撃キー
	if (input_->PushKey(DIK_K))
	{
		//isOn = true;



		//生成時の座標設定
		for (int i = 0; i < MAXSWROD; i++)
		{

			w[i].translation_ =
			{
				//BOSS第二形態の真ん中の座標
				//fishParent.pos.translation_.x - interval * 2 + i * interval,
				//fishParent.pos.translation_.y + 10 ,
				//fishParent.pos.translation_.z
			};

			w[i].TransferMatrix();
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
		//Rota();
	}


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
					pPos[i].translation_ = pl->GetWorldPosition();
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
			}
		}
		if (phase2AttackCoolTime <= -40)
		{
			phase2AttackCoolTime = 70;
			//t2 = false;

		}

	}
	else
	{
		phase2AttackCoolTime = 70;
		isSat2 = false;
	}
	//向きを変える
	for (int i = 0; i < 5; i++)
	{
		if (isShot[i] == false)
		{

			WorldTransform plWorldTransform;

			plWorldTransform.translation_ = pl->GetWorldPosition();

			Matrix4 mat;
			mat = CreateMatRot(w[i].translation_, plWorldTransform.translation_);

			w[i].SetMatRot(mat);
			w[i].TransferMatrix();
		}
	}
}

void BossWarrier::phase2AttackDraw(ViewProjection viewProMat)
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

	plWorldTransform.translation_ = pl->GetWorldPosition();

	for (int i = 0; i < MAXSWROD; i++)
	{
		Matrix4 mat;
		mat = CreateMatRot(w[i].translation_, plWorldTransform.translation_);

		w[i].SetMatRot(mat);
	}


}

void BossWarrier::InitAtkArmSwing()
{
	//腕振り攻撃フラグをtrue
	isAtkArmSwing = true;
}

void BossWarrier::UpdateAtkArmSwing()
{
	//大本の回転角の増減
	rootRotRad+= 8.0f;
	if (rootRotRad >= 360.0f) {
		rootRotRad -= rootRotRad;
	}

	Vector3 rootRad = { 0,sin(rootRotRad * PI / 180.0f) * PI / 2.0f,0 };




	//関節の回転幅
	Vector3 jointRot = { 0,PI / 6.0f,0 };
	boss2Model[BossWarrierPart::Root].Transform.SetRot(rootRad);
	boss2Model[BossWarrierPart::Root].Transform.translation_.x++;
	if (boss2Model[BossWarrierPart::Root].Transform.translation_.x > 100) {
		boss2Model[BossWarrierPart::Root].Transform.translation_.x = 0;
		isAtkArmSwing = false;
	}
	boss2Model[BossWarrierPart::ShoulderL].Transform.SetRot(jointRot);
	boss2Model[BossWarrierPart::ShoulderR].Transform.SetRot(-jointRot);
	boss2Model[BossWarrierPart::elbowL].Transform.SetRot(jointRot);
	boss2Model[BossWarrierPart::elbowR].Transform.SetRot(-jointRot);

}

Matrix4 CreateMatRot(const Vector3& pos, const Vector3& target)
{
	Vector3 matRotX, matRotY, matRotZ;
	Vector3 up{ 0,1,0 };

	matRotZ = target - pos;
	matRotZ.normalize();
	matRotX = up.cross(matRotZ);
	matRotX.normalize();
	matRotY = matRotZ.cross(matRotX);
	matRotY.normalize();

	Matrix4 matRot{
				matRotX.x,matRotX.y,matRotX.z,0,
				matRotY.x,matRotY.y,matRotY.z,0,
				matRotZ.x,matRotZ.y,matRotZ.z,0,
				0,0,0,1
	};

	return matRot;
}