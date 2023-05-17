#include "BossWarrier.h"
#include"Input.h"
#include"ImGuiManager.h"
#include"BossFish.h"

void BossWarrier::Initialize()
{
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

void BossWarrier::Update(const Vector3& targetPos)
{
	//引数をメンバにコピー
	this->targetPos = targetPos;

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
	//腕振り攻撃フラグをtrue
	isAtkArmSwing = true;
	//腕振りタイマーを開始(30f)で終わるようにし、30f経つごとにループさせる
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

	//イージングデータ更新
	easeRotArm.Update();
	//イージングが終了したら(timeRateが1.0以上)イージングのパラメータを入れ替えてまたイージング開始
	if (!easeRotArm.GetActive()) {
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
				isAtkArmSwing = false;
			}
		}

	}


	

	//各関節の回転処理
	Vector3 rotArm, rotShoulderL, rotShoulderR, rotElbowL, rotElbowR;
	rotArm = EaseOutVec3(dataRotArm[0], dataRotArm[1], easeRotArm.GetTimeRate()); 
	rotShoulderL = Lerp(dataRotShoulder[1], dataRotShoulder[0], easeRotArm.GetTimeRate());
	rotShoulderR = Lerp(-dataRotShoulder[0], -dataRotShoulder[1], easeRotArm.GetTimeRate());
	rotElbowL = Lerp(dataRotElbow[1], dataRotElbow[0], easeRotArm.GetTimeRate());
	rotElbowR = Lerp(-dataRotElbow[0], -dataRotElbow[1], easeRotArm.GetTimeRate());

	//	自機を向く回転行列、体を回転させる回転行列をそれぞれ作成
	Matrix4 matBossDir,matBodyRot,matBossRot;

	//進行方向に向かせたいのでダミーの自機を向いてもらう
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
