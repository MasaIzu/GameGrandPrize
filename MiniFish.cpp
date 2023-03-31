#include "MiniFish.h"
#include"ImGuiManager.h"
#include <CollisionManager.h>
#include <CollisionAttribute.h>

void MiniFish::Initialize(const Vector3& pos)
{
	//ワールド変換の初期化
	world.Initialize();
	//座標を引数で設定
	positions[3] = pos;
	world.translation_ = pos;
	world.TransferMatrix();


	// コリジョンマネージャに追加
	FishCollider = new SphereCollider(Vector4(0, fishRadius, 0, 0), fishRadius);
	CollisionManager::GetInstance()->AddCollider(FishCollider);
	FishCollider->SetAttribute(COLLISION_ATTR_WEAKENEMYS);
	

	//魚のモデル初期化
	//bodyModel.reset(Model::CreateFromOBJ("FishBody", true));
	//eyeModel.reset(Model::CreateFromOBJ("FishMedama", true));
}

static int count = 0;
void MiniFish::Update(const Vector3& stagePos, float stageRadius)
{

	//イージング更新
	easeMove.Update();

	Vector3 vecStageToPos = world.translation_ - stagePos;

	////イージングが終わっている
	//if (!easeMove.GetActive() || vecStageToPos.length() >= stageRadius) {
	//	SetMovePos(stagePos, stageRadius);
	//}




	ImGui::Text("timeRate:%f", easeMove.GetTimeRate());

	if (easeMove.GetActive()) {
		ImGui::Text("active!");
		for (int i = 0; i < 4; i++) {
			ImGui::Text("pos[%d]:%f,%f,%f", i, positions[i].x, positions[i].y, positions[i].z);
		}

	}


	if (speedResetCount == 0) {
		moveRad += Random(-15, 15);

		move.x = sin(moveRad * PI / 180.0f) * spdtemp;
		move.z = cos(moveRad * PI / 180.0f) * spdtemp;

		speedResetCount = spdResetInterval;
	}

	Vector3 pos;

	//pos = world.translation_;
	Vector3 nextPos = world.translation_ + move;
	if (nextPos.length() >= stageRadius) {
	/*	if (fabs(nextPos.x) > fabs(nextPos.z)) {
			move.x = -move.x;
		}
		else {
			move.z = -move.z;
		}*/
		//move = -move;
		moveRad += 180;
		spdtemp = Random(0.5f, 2.0f);
		move.x = sin(moveRad * PI / 180.0f) * spdtemp;
		move.z = cos(moveRad * PI / 180.0f) * spdtemp;
		speedResetCount = spdResetInterval;

	}

	if (moveRad > 360) {
		moveRad -= 360;
	}

	pos = LerpBezireCubic(positions[0], positions[1], positions[2], positions[3], easeMove.GetTimeRate());
	//現在の座標と次に代入される座標から正面を向く回転行列を作成
	Matrix4 matRot = CreateMatRot(world.translation_, world.translation_ + move);
	world.SetMatRot(matRot);
	world.translation_ += move;
	world.TransferMatrix();
	speedResetCount--;

	
	
	FishCollider->Update(world.matWorld_);
	

}

void MiniFish::Draw(ViewProjection viewPro)
{
	//bodyModel->Draw(world, viewPro);
	//eyeModel->Draw(world, viewPro);
}

void MiniFish::SetMovePos(const Vector3& stagePos, float stageRadius)
{
	//プレイヤーが最後にいた場所を始点とする
	positions[0] = world.translation_;

	for (int i = 1; i < 4; i++) {

		//ベジエ曲線のため速度を分割
		float spd = spdPerSec / 4.0f;

		//ステージの外に行かないための座標設定
		//	ImGui::Text("not active...");


		//iが1の時のみ(制御点1つめ)前回のradから前後90の物にする
		if (i == 1) {
			moveRad += Random(-90, 90);
		}
		else {
			moveRad = Random(0, 360);
		}

		positions[i].x = sin(moveRad * PI / 180) * spd;
		positions[i].z = cos(moveRad * PI / 180) * spd;
		positions[i].y = stagePos.y;
		positions[i] += positions[i - 1];
		Vector3 vecPosToStage = stagePos - positions[i];
		if (stageRadius < vecPosToStage.length()) {
			//ステージ中心から移動座標への距離がステージの半径よりおおきいならもう一回for文回す
			i--;
		}
	}
	//イージング開始
	easeMove.Start(120);
}
