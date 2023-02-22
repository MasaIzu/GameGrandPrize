#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>
#include "FbxLoader.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	model_.reset(Model::CreateFromOBJ("UFO", true));

	sceneManager_ = SceneManager::GetInstance();

	viewProjection_.Initialize();
	viewProjection_.eye = { 0,0,-100 };
	viewProjection_.UpdateMatrix();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,0 };
	worldTransform_.rotation_ = { 0,0,0 };
	worldTransform_.scale_ = { 0.1f,0.1f,0.1f };
	worldTransform_.TransferMatrix();

	fbxmodel = new FbxModel();
	
	fbxmodel = FbxLoader::GetInstance()->LoadModelFromFile("lowpoliHitokunBoss");
	fbxmodel->Initialize();

}

void GameScene::Update() {
	
	if (input_->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
	}


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

	model_->Draw(worldTransform_, viewProjection_);


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
