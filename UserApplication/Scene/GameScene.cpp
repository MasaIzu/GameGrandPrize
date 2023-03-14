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

	//当たり判定
	collisionManager = CollisionManager::GetInstance();

	model_.reset(Model::CreateFromOBJ("UFO", true));

	viewProjection_.eye = { 0,10,-10 };

	sceneManager_ = SceneManager::GetInstance();

	viewProjection_.Initialize();
	viewProjection_.UpdateMatrix();

	worldTransform_.Initialize();
	//worldTransform_.translation_ = { 0,0,100 };
	//worldTransform_.rotation_ = { 0,0,0 };
	//worldTransform_.scale_ = { 0.1f,0.1f,0.1f };
	//worldTransform_.TransferMatrix();

	/*fbxmodel = std::make_unique<FbxModel>();

	fbxmodel.reset(FbxLoader::GetInstance()->LoadModelFromFile("lowpoliHitokunBoss"));
	fbxmodel->Initialize();*/

	boxCollision = std::make_unique<BoxCollision>();

	boss.Initialize();

	for (int i = 0; i < boss.fishMaxCount; i++) {
		boss.CreateFish(Random(-boss.fishParent.radius, boss.fishParent.radius));
	}

	boss.fishParent.pos.translation_ = { 0,0,100 };
	boss.Update({ 0,0,0 });

	player = std::make_unique<Player>();
	player->Initialize(model_.get(), 1280, 720);
	//player->worldTransform_.translation = { 0,0,-100 };

	gameCamera = std::make_unique<GameCamera>(1280, 720);
	gameCamera->Initialize();


	model_->SetPolygonExplosion({ 0.0f,1.0f,0.0f,0.0f });
	ParticleMan = std::make_unique<ParticleManager>();
	ParticleMan->Initialize();

	UINT tex = TextureManager::GetInstance()->Load("effect1.png");
	ParticleMan->SetTextureHandle(tex);
}

void GameScene::Update() {

	/*if (input_->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
	}*/

	if (collisionManager->GetIsHit()) {
		isHit = true;
	}


	ImGui::Begin("Phase");

	ImGui::Text("ParticleListSize:%d", ParticleMan->GetParticlesListSize());


	ImGui::End();

	boss.Update(player->GetWorldPosition());
	viewProjection_.UpdateMatrix();


	//スペースキーを押していたら
	for (int i = 0; i < 5; i++)
	{
		const float rnd_life = 290.0f;
		float life = (float)rand() / RAND_MAX * rnd_life - rnd_life / 2.0f + 10;

		//X,Y,Z全て[-5.0,+5.0f]でランダムに分布
		const float rnd_pos = 30.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = abs((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + 200;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//追加
		ParticleMan->OutAdd(life, { 0,50,0 }, pos, 1, 1, { 1,0.75,0.5,0 }, { 1,1,1,1 });
	}
	////スペースキーを押していたら
	//for (int i = 0; i < 50; i++)
	//{

	//	//X,Y,Z全て[-5.0,+5.0f]でランダムに分布
	//	const float rnd_pos = 10.0f;
	//	Vector3 pos{};
	//	pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	//	pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	//	pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	//	//追加
	//	ParticleMan->InAdd(60, pos, {0,0,0}, 1.0f, 1.0f, { 1,1,0,0.5 }, { 1,1,1,1 });
	//}

	player->SetCameraRot(gameCamera->GetCameraRotVec3());
	player->SetCameraLook(viewProjection_.cameraLook);
	player->Update(viewProjection_);


	gameCamera->SetIsHit(isHit);
	gameCamera->SetSpaceInput(player->GetSpaceInput());
	gameCamera->SetCameraPosition(player->GetWorldPosition());
	//gameCamera->SetCameraPosition({0,0,-100});
	gameCamera->Update(&viewProjection_);
	isHit = gameCamera->GetIsHit();

	//	viewProjection_.eye = gameCamera->GetEye();
	ImGui::Begin("Camera");
	ImGui::SliderFloat("PosX", &viewProjection_.eye.x, -100.0f, 200.0f);
	ImGui::SliderFloat("PosY", &viewProjection_.eye.y, -100.0f, 100.0f);
	ImGui::SliderFloat("PosZ", &viewProjection_.eye.z, -100.0f, 200.0f);
	ImGui::End();

	Vector3 pWith(1, 1, 1);
	Vector3 eWith(0.6f, 9, 1);

	if (boxCollision->BoxCollision_1(player->GetWorldPosition(),MyMath::GetWorldTransform(boss.swordTransform.matWorld_),pWith,eWith)) {

	}

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();


	//カメラは最後にアプデ
	viewProjection_.target = gameCamera->GetTarget();
	//viewProjection_.target = boss.fishParent.pos.translation_;
	viewProjection_.eye = gameCamera->GetEye();
	//viewProjection_.fovAngleY = viewProjection_.ToRadian(x);
	viewProjection_.UpdateMatrix();
	ParticleMan->Update();


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



	model_->Draw(boss.swordTransform, viewProjection_);

	boss.Draw(viewProjection_);

	player->Draw(viewProjection_);

	//3Dオブジェクト描画後処理
	Model::PostDraw();

	FbxModel::PreDraw(commandList);

	//fbxmodel->Draw(worldTransform_, viewProjection_);

	FbxModel::PostDraw();

	ParticleManager::PreDraw(commandList);

	ParticleMan->Draw(viewProjection_);

	ParticleManager::PostDraw();


#pragma endregion

#pragma region 前景スプライト描画



#pragma endregion
}

void GameScene::Finalize()
{
}

