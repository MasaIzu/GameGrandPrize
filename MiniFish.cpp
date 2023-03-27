#include "MiniFish.h"
#include"ImGuiManager.h"

void MiniFish::Initialize(const Vector3& pos)
{
	//ワールド変換の初期化
	world.Initialize();
	//座標を引数で設定
	positions[3] = pos;
	world.translation_ = pos;
	world.TransferMatrix();

	//魚のモデル初期化
	//bodyModel.reset(Model::CreateFromOBJ("FishBody", true));
	//eyeModel.reset(Model::CreateFromOBJ("FishMedama", true));
}

static int count = 0;
void MiniFish::Update(const Vector3& stagePos, float stageRadius)
{
	float spdPerSec = 120.0f;
	//イージング更新
	easeMove.Update();


	//イージングが半分
	if (!easeMove.GetActive()) {
		//3点をランダムに決定、始点は前回の終点
		positions[0] = positions[3];

		

		for (int i = 1; i < 4; i++) {

			//ベジエ曲線のため速度を分割
			float spd = spdPerSec / 4.0f;

			//ステージの外に行かないための座標設定
			//	ImGui::Text("not active...");
				float rad = Random(0, 360);
				positions[i].x = sin(rad * PI / 180) * spd;
				positions[i].z = cos(rad * PI / 180) * spd;
				positions[i].y = stagePos.y;
				positions[i] += positions[i - 1];
				Vector3 vecPosToStage = stagePos - positions[i];
				if (stageRadius < vecPosToStage.length()) {
					//ステージ中心から移動座標への距離がステージの半径よりおおきいならもう一回for文回す
					i--;
				}
			
		}

		//イージング開始
		easeMove.Start(60);
	}

	


	ImGui::Text("timeRate:%f", easeMove.GetTimeRate());

	if (easeMove.GetActive()) {
		ImGui::Text("active!");
		for (int i = 0; i < 4; i++) {
			ImGui::Text("pos[%d]:%f,%f,%f", i, positions[i].x, positions[i].y, positions[i].z);
		}

	}

	Vector3 pos;
	pos = LerpBezireCubic(positions[0], positions[1], positions[2], positions[3], easeMove.GetTimeRate());
	//現在の座標と次に代入される座標から正面を向く回転行列を作成
	Matrix4 matRot = CreateMatRot(world.translation_, pos);
	world.SetMatRot(matRot);
	world.translation_ = pos;
	world.TransferMatrix();

}

void MiniFish::Draw(ViewProjection viewPro)
{
	//bodyModel->Draw(world, viewPro);
	//eyeModel->Draw(world, viewPro);
}
