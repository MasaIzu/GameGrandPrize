#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>
#include "FbxLoader.h"
#include"ImGuiManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	model_.reset();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	model_.reset(Model::CreateFromOBJ("UFO", true));

	sceneManager_ = SceneManager::GetInstance();

	viewProjection_.Initialize();
	viewProjection_.eye = { 0,0,-10 };
	viewProjection_.UpdateMatrix();

	worldTransform_.Initialize();
	//worldTransform_.translation_ = { 0,0,100 };
	//worldTransform_.rotation_ = { 0,0,0 };
	//worldTransform_.scale_ = { 0.1f,0.1f,0.1f };
	//worldTransform_.TransferMatrix();

	/*fbxmodel = std::make_unique<FbxModel>();
	
	fbxmodel.reset(FbxLoader::GetInstance()->LoadModelFromFile("lowpoliHitokunBoss"));
	fbxmodel->Initialize();*/

	boss.Initialize();

	for (int i = 0; i < 500; i++) {
		boss.CreateFish(Random(-boss.fishParent.radius, boss.fishParent.radius));
	}

	model_->SetPolygonExplosion({1.0f,1.0f,1.0f,10.0f});
}

void GameScene::Update() {
	
	if (input_->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
	}
	

	//ImGui::SliderFloat("posY", &newFishPosY, -boss.fishParent.radius, boss.fishParent.radius);

	Model::ConstBufferPolygonExplosion parentPos = model_->GetPolygonExplosion();

	ImGui::SliderFloat("Parent Destruction", &parentPos._Destruction, 1.0f, 0.0f);
	ImGui::SliderFloat("Parent ScaleFactor", &parentPos._ScaleFactor, 1.0f, 0.0f);
	ImGui::SliderFloat("Parent RotationFactor", &parentPos._RotationFactor, 100.0f, 0.0f);
	ImGui::SliderFloat("Parent PositionFactor", &parentPos._PositionFactor, 100.0f, 0.0f);

	model_->SetPolygonExplosion(parentPos);

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
	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	model_->Draw(worldTransform_, viewProjection_);

	//for (int i = 0; i < boss.fishes.size(); i++) {
	//	model_->Draw(boss.fishes[i].pos, viewProjection_);
	//}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	FbxModel::PreDraw(commandList);

	//fbxmodel->Draw(worldTransform_, viewProjection_);

	FbxModel::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	


#pragma endregion
}

void GameScene::Finalize()
{
}
