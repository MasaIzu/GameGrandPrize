#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>
#include "FbxLoader.h"
#include"ImGuiManager.h"
#include <CollisionAttribute.h>
#include "Collision.h"

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

	stageWorldTransform_.Initialize();

	stageModel_.reset(Model::CreateFromOBJ("stage", true));

	stageWorldTransform_.scale_ = { 52,50,52 };

	stageWorldTransform_.translation_ = Vector3(0, -2.1f, 0);

	stageWorldTransform_.TransferMatrix();
}

void GameScene::Update() {

	/*if (input_->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
	}*/
	isAttackHit = false;

	if (collisionManager->GetIsEnemyHit()) {
		isEnemyHit = true;
		player->SetEnemyPos(collisionManager->GetEnemyWorldPos());
	}

	if (player->GetColliderAttribute() == COLLISION_ATTR_ALLIES) {
		if (Collision::CheckRectSphere(MyMath::GetWorldTransform(boss.swordTransform.matWorld_), boss.GetSwordCollisionCube1(), boss.GetSwordCollisionCube2(),
			player->GetWorldPosition(), player->GetRadius())) {

			isEnemyHit = true;
			player->SetEnemyPos(boss.GetSwordWorldPos());
		}
	}

	if (collisionManager->GetIsAttackHit()) {
		isAttackHit = true;
		player->SetParticlePos(collisionManager->GetAttackHitWorldPos());
	}



	boss.Update(player->GetWorldPosition());
	viewProjection_.UpdateMatrix();

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

	player->SetIsEnemyHit(isEnemyHit);
	player->SetIsAttackHit(isAttackHit);
	player->SetAngle(gameCamera->GetCameraAngle());
	player->SetCameraRot(gameCamera->GetCameraRotVec3());
	player->SetCameraLook(viewProjection_.cameraLook);
	player->Update(viewProjection_);


	gameCamera->SetIsHit(isAttackHit);
	gameCamera->SetSpaceInput(player->GetSpaceInput());
	gameCamera->SetCameraPosition(player->GetWorldPosition());
	//gameCamera->SetCameraPosition({0,0,-100});
	gameCamera->Update(&viewProjection_);
	isEnemyHit = gameCamera->GetIsHit();

	//	viewProjection_.eye = gameCamera->GetEye();



	Vector3 pWith(1, 1, 1);
	Vector3 eWith(0.6f, 9, 1);


	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();


	//カメラは最後にアプデ
	viewProjection_.target = gameCamera->GetTarget();
	//viewProjection_.target = boss.fishParent.pos.translation_;
	viewProjection_.eye = gameCamera->GetEye();
	//viewProjection_.fovAngleY = viewProjection_.ToRadian(x);
	viewProjection_.UpdateMatrix();
	//ParticleMan->Update();


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

	//model_->Draw(worldTransform_, viewProjection_);


	stageModel_->Draw(stageWorldTransform_,viewProjection_);

	model_->Draw(boss.swordTransform, viewProjection_);

	boss.Draw(viewProjection_);

	player->Draw(viewProjection_);

	//3Dオブジェクト描画後処理
	Model::PostDraw();

	FbxModel::PreDraw(commandList);

	//fbxmodel->Draw(worldTransform_, viewProjection_);

	FbxModel::PostDraw();

	ParticleManager::PreDraw(commandList);

	//ParticleMan->Draw(viewProjection_);
	player->ParticleDraw(viewProjection_);

	ParticleManager::PostDraw();


#pragma endregion

#pragma region 前景スプライト描画



#pragma endregion
}

void GameScene::Finalize()
{
}

