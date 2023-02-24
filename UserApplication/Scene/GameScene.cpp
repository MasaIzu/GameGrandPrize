#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>
#include "FbxLoader.h"
#include"ImGuiManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	//model_.reset();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	//model_.reset(Model::CreateFromOBJ("UFO", true));

	sceneManager_ = SceneManager::GetInstance();

	viewProjection_.Initialize();
	viewProjection_.eye = { 0,0,-100 };
	viewProjection_.UpdateMatrix();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,0 };
	worldTransform_.rotation_ = { 0,0,0 };
	//worldTransform_.scale_ = { 0.1f,0.1f,0.1f };
	worldTransform_.TransferMatrix();

	fbxmodel = std::make_unique<FbxModel>();
	
	fbxmodel.reset(FbxLoader::GetInstance()->LoadModelFromFile("lowpoliHitokunBoss"));
	fbxmodel->Initialize();

	boss.Initialize();

	for (int i = 0; i < 500; i++) {
		boss.CreateFish(Random(-boss.fishParent.radius, boss.fishParent.radius));
	}

}

void GameScene::Update() {
	
	if (input_->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
	}



	//ImGui::Begin("Create Fish");
	//

	////ImGui::SliderFloat("posY", &newFishPosY, -boss.fishParent.radius, boss.fishParent.radius);

	//ImGui::Text("enemy Count %d",boss.fishes.size());

	//if (ImGui::Button("Create")) {
	//	boss.CreateFish(newFishPosY);
	//}

	//if (ImGui::Button("create 10")) {
	//	for (int i = 0; i < 100; i++) {
	//		boss.CreateFish(Random(-boss.fishParent.radius, boss.fishParent.radius));
	//	}
	//}

	//if (ImGui::Button("create 100")) {
	//	for (int i = 0; i < 100; i++) {
	//		boss.CreateFish(Random(-boss.fishParent.radius, boss.fishParent.radius));
	//	}
	//}
	//Vector3 parentPos = boss.fishParent.pos.translation_;

	//ImGui::SliderFloat("Parent posX", &parentPos.x, -boss.fishParent.radius, boss.fishParent.radius);
	//ImGui::SliderFloat("Parent posY", &parentPos.y, -boss.fishParent.radius, boss.fishParent.radius);
	//ImGui::SliderFloat("Parent posZ", &parentPos.z, -boss.fishParent.radius, boss.fishParent.radius);
	//if (ImGui::Button("parent Reset")) {
	//	parentPos = {0,0,0};
	//}

	//boss.fishParent.pos.translation_ = parentPos;

	//ImGui::End();

	//boss.Update();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	//model_->Draw(worldTransform_, viewProjection_);

	/*for (int i = 0; i < boss.fishes.size(); i++) {
		model_->Draw(boss.fishes[i].pos, viewProjection_);
	}*/

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	FbxModel::PreDraw(commandList);

	fbxmodel->Draw(worldTransform_, viewProjection_);

	FbxModel::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	


#pragma endregion
}

void GameScene::Finalize()
{
}
