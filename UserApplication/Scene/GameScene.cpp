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
	worldTransform_.translation_ = { 0,0,100 };
	worldTransform_.rotation_ = { 0,0,0 };
	worldTransform_.scale_ = { 0.1f,0.1f,0.1f };
	worldTransform_.TransferMatrix();


	boxCollision = std::make_unique<BoxCollision>();



	player = std::make_unique<Player>();
	player->Initialize(model_.get(), 1280, 720);
	//player->worldTransform_.translation = { 0,0,-100 };

	gameCamera = std::make_unique<GameCamera>(1280, 720);
	gameCamera->Initialize();


	//model_->SetPolygonExplosion({ 0.0f,1.0f,0.0f,0.0f });

	stageWorldTransform_.Initialize();

	stageModel_.reset(Model::CreateFromOBJ("stage", true));

	stageWorldTransform_.scale_ = { 52,50,52 };

	stageWorldTransform_.translation_ = Vector3(0, -2.1f, 0);

	stageWorldTransform_.TransferMatrix();


	skyModel.reset(Model::CreateFromOBJ("skydome", true));

	//groundModel = std::make_unique<Model>();
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skyModel.get());

	////地面の描画
	ground.Initialize();

	for (int i = 0; i < (ground.MaxCount); i++)
	{
		ground.CreateGround();
	};
	for (int i = 0; i < (ground.MaxCount); i++)
	{
		ground.CreateBlock();
	};


	//間欠泉の座標設定
	for (int i = 0; i < 5; i++) {
		float gayserPosRad = 360.0f / 5.0f * i;
		gayserPos[i].x = sin(gayserPosRad * PI / 180.0f) * stageRadius * 0.8f;
		gayserPos[i].z = cos(gayserPosRad * PI / 180.0f) * stageRadius * 0.8f;
	}
	gayserParticle = std::make_unique<ParticleManager>();

	gayserParticle->Initialize();

	gayserParticle->SetTextureHandle(TextureManager::Load("effect2.png"));

	boss.Initialize();

	//ムービー用カメラの初期化
	movieCamera.Initialize();


	boss.fishParent.pos.translation_ = { 0,25,100 };
	boss.fishParent.pos.TransferMatrix();

	/*for (int i = 0; i < boss.fishMaxCount; i++) {
		boss.CreateFish(gayserPos[i % 5]);
	}*/

	for (int i = 0; i < 10; i++) {
		Vector3 pos;
		pos = { Random(-stageRadius,  stageRadius) / 2, 0, Random(-stageRadius,  stageRadius) / 2 };
		pos += stagePos;
		minifishes[i].Initialize(pos, COLLISION_ATTR_WEAKENEMYS1 + i);
	}

	boss.Update({ 0,0,0 });

	// 間欠泉の初期化
	for (int i = 0; i < 5; i++) {
		gayserModel_[i].reset(Model::CreateFromOBJ("geyser", true));
		gayserW[i].Initialize();
		gayserW[i].translation_.x = gayserPos[i].x;
		gayserW[i].translation_.y = -1.2f;
		gayserW[i].translation_.z = gayserPos[i].z;
		gayserW[i].scale_ = { 2,2,2 };
		gayserW[i].TransferMatrix();
	}

	gameCamera->SetPlayerMoveMent(player->GetPlayerMoveMent());
	gameCamera->SetSpaceInput(player->GetSpaceInput());
	gameCamera->SetCameraPosition(player->GetWorldPosition());
	//gameCamera->SetCameraPosition({0,0,-100});
	gameCamera->InitializeCameraPosition();

	//カメラは最後にアプデ
	viewProjection_.target = gameCamera->GetTarget();
	//viewProjection_.target = boss.fishParent.pos.translation_;
	viewProjection_.eye = gameCamera->GetEye();
	viewProjection_.fovAngleY = gameCamera->GetFovAngle();
	viewProjection_.UpdateMatrix();

	gameoverFont = std::make_unique<Sprite>();

	gameover = std::make_unique<Sprite>();

	gameoverFont = Sprite::Create(TextureManager::Load("GameOverFont.png"));

	gameover = Sprite::Create(TextureManager::Load("gameover.png"));

	gameClearFont = std::make_unique<Sprite>();

	gameClearFont = Sprite::Create(TextureManager::Load("GameClearFont.png"));


	titlerogo = Sprite::Create(TextureManager::Load("AtomsFont.png"));
	titlerogo->SetAnchorPoint({ 0,0 });
}

void GameScene::Update() {

	switch (scene)
	{
	case Scene::Title:
		TitleUpdate();
		break;
	case Scene::Game:
		GameUpdate();
		break;
	case Scene::GameOver:
		GameOverUpdate();
		break;
	case Scene::Result:
		ResultUpdate();
		break;
	default:
		break;
	}

}

void GameScene::TitleUpdate()
{
	nowViewProjection = viewProjection_;
	if (input_->TriggerKey(DIK_SPACE)) {
		scene = Scene::Game;
	}
}

void GameScene::GameUpdate()
{
	gayserFlame++;
	if (ImGui::Button("break")) {
		static int a = 0;
		a++;
	}

	//ランダムな小魚が自機の攻撃に当たったことにする
	if (ImGui::Button("col minifish to pAtk")) {
		int randNum = Random(0, 9);
		minifishes[randNum].OnCollision();
	}

	//生きている小魚の数が5匹以下になったら魚が逃げ出す
	if (!isTutorialEnd) {
		if (GetMiniFishAlive() < 5) {
			isTutorialEnd = true;
			isMovie = true;
			for (int i = 0; i < 10; i++) {
				minifishes[i].LeaveGayser(gayserPos[i / 2]);
			}
		}
	}



	//チュートリアルと最初のムービーでだけ小魚を動かす
	if (gamePhase == GamePhase::GameTutorial || gamePhase == GamePhase::GameMovie1) {

		//小魚の更新

		for (int i = 0; i < 10; i++) {
			minifishes[i].Update(stagePos, stageRadius);
		}

	}

	CheckAllFishLeave();
	if (isTutorialEnd) {
		ImGui::Text("tutorial end!");
	}

	if (isAllFishLeave) {
		ImGui::Text("all fishes leave!");
	}

	//チュートリアルが終了かつ、魚が間欠泉に逃げ終わっているなら
	if (!isStartBossBattle) {
		if (isTutorialEnd && isAllFishLeave) {
			//ボスの生成を開始
			fishSpawnCount = 20;
			isStartBossBattle = true;
			//小魚を全員殺す
			for (int i = 0; i < 10; i++) {
				minifishes[i].OnCollision();
			}
		}
	}

	fishSpawnInterval--;



	if (isStartBossBattle) {
		ImGui::Text("boss battle start!");
		if (fishSpawnCount > 0 && fishSpawnInterval <= 0) {
			fishSpawnCount--;
			fishSpawnInterval = 5;
			for (int i = 0; i < boss.fishMaxCount / 20; i++) {
				boss.CreateFish(gayserPos[i % 5]);
			}
		}
	}

	//小魚スポーンカウントが0でボス戦開始フラグがtrueならムービー終了
	if (isStartBossBattle && fishSpawnCount == 0) {
		isMovie = false;
	}



	//チュートリアルが終わっていて、魚が移動し終わっていないならカメラを上からの見下ろしに
	if (isTutorialEnd && !isAllFishLeave) {
		movieCamera.eye = { 0,125,-150 };
		movieCamera.target = { 0,0,0 };
	}

	//ボス生成フェーズになったらカメラをボスに向ける
	if (isStartBossBattle) {
		movieCamera.target = boss.fishParent.pos.translation_;
	}

	//ムービーカメラの更新
	movieCamera.UpdateMatrix();

	if (collisionManager->GetIsEnemyHit()) {
		gameCamera->Collision();
		Matrix4 a = collisionManager->GetEnemyWorldPos();
		player->SetEnemyPos(collisionManager->GetEnemyWorldPos());
		player->Collision(10);
	}

	if (collisionManager->GetIsWakeEnemyHit()) {
		gameCamera->Collision();
		Matrix4 a = collisionManager->GetEnemyWorldPos();
		player->SetEnemyPos(collisionManager->GetEnemyWorldPos());
		player->Collision(5);
	}

	ImGui::Text("EnemyWorldPosX : %f", MyMath::GetWorldTransform(collisionManager->GetEnemyWorldPos()).x);
	ImGui::Text("EnemyWorldPosY : %f", MyMath::GetWorldTransform(collisionManager->GetEnemyWorldPos()).y);
	ImGui::Text("EnemyWorldPosZ : %f", MyMath::GetWorldTransform(collisionManager->GetEnemyWorldPos()).z);

	//剣と自機の当たり判定
	if (collisionManager->GetEnemySwordHit()) {
		gameCamera->Collision();
		Matrix4 a = collisionManager->GetEnemyWorldPos();
		player->SetEnemyPos(collisionManager->GetEnemyWorldPos());
		player->Collision(20);
	}


	if (collisionManager->GetIsAttackHit()) {
		isAttackHit = true;
		gameCamera->Collision();
		player->SetParticlePos(collisionManager->GetAttackHitWorldPos());
		boss.Damage(2);
	}

	if (collisionManager->GetIsWakeEnemyAttackHit()) {
		isAttackHit = true;
		playerAttackHitNumber = collisionManager->GetHitNumber() - 1;

		minifishes[playerAttackHitNumber].SetAttribute(COLLISION_ATTR_WEAKENEMYS_DEI);

		minifishes[playerAttackHitNumber].OnCollision();
	}


	// ボスフェーズ１のHPが０になったら
	if (boss.bossHealth <= 0) {
		boss.Death();
	}



	ImGui::Begin("Phase");

	ImGui::Text("minifishesX:%f", MyMath::GetWorldTransform(minifishes[0].GetWorldTransform().matWorld_).x);
	ImGui::Text("minifishesY:%f", MyMath::GetWorldTransform(minifishes[0].GetWorldTransform().matWorld_).y);
	ImGui::Text("minifishesZ:%f", MyMath::GetWorldTransform(minifishes[0].GetWorldTransform().matWorld_).z);

	ImGui::End();


	boss.Update(player->GetWorldPosition());
	viewProjection_.UpdateMatrix();

	player->SetIsEnemyHit(isEnemyHit);
	player->SetIsAttackHit(isAttackHit);
	isAttackHit = false;
	player->SetAngle(gameCamera->GetCameraAngle());
	player->SetCameraRot(gameCamera->GetCameraRotVec3());
	player->SetCameraLook(viewProjection_.cameraLook);
	player->Update(viewProjection_);




	gameCamera->SetPlayerMoveMent(player->GetPlayerMoveMent());
	gameCamera->SetSpaceInput(player->GetSpaceInput());
	gameCamera->SetCameraPosition(player->GetWorldPosition());
	//gameCamera->SetCameraPosition({0,0,-100});
	gameCamera->Update(&viewProjection_);
	//	viewProjection_.eye = gameCamera->GetEye();


	nowViewProjection = viewProjection_;

	//ムービーフラグがオンならカメラをムービー用に
	if (isMovie) {
		nowViewProjection = movieCamera;
	}

	Vector3 pWith(1, 1, 1);
	Vector3 eWith(0.6f, 9, 1);


	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();

	gayserParticle->Update();


	//カメラは最後にアプデ
	viewProjection_.target = gameCamera->GetTarget();
	//viewProjection_.target = boss.fishParent.pos.translation_;
	viewProjection_.eye = gameCamera->GetEye();
	viewProjection_.fovAngleY = gameCamera->GetFovAngle();
	viewProjection_.UpdateMatrix();
	//ParticleMan->Update();

	if (boss.GetIsDeathEnd()) {
		scene = Scene::Result;
	}
	if (player->GetAlive() == false)
	{
		scene = Scene::GameOver;
	}
}

void GameScene::GameOverUpdate()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		scene = Scene::Title;
		Reset();
	}
}

void GameScene::ResultUpdate()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		scene = Scene::Title;
		Reset();
	}
}

void GameScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	model_->Draw(worldTransform_, viewProjection_);

	stageModel_->Draw(stageWorldTransform_, nowViewProjection);


	//stageModel_->Draw(stageWorldTransform_,viewProjection_);

	ground.Draw(nowViewProjection);



	//チュートリアルと最初のムービーでだけ小魚を描画
	if (gamePhase == GamePhase::GameTutorial || gamePhase == GamePhase::GameMovie1) {

		for (int i = 0; i < 10; i++) {
			//minifishes[i].Draw(viewProjection_);
			if (minifishes[i].GetAlive()) {
				boss.fishBodyModel->Draw(minifishes[i].GetWorldTransform(), nowViewProjection);
				boss.fishEyeModel->Draw(minifishes[i].GetWorldTransform(), nowViewProjection);
			}
		}
	}

	//ボス出現ムービーとボス変身ムービーの間で描画
	boss.Draw(nowViewProjection);

	player->Draw(nowViewProjection);

	// 間欠泉の描画
	for (int i = 0; i < 5; i++) {
		gayserModel_[i]->Draw(gayserW[i], nowViewProjection);
	}

	//3Dオブジェクト描画後処理
	Model::PostDraw();



	FbxModel::PreDraw(commandList);

	player->PlayerFbxDraw(nowViewProjection);

	FbxModel::PostDraw();

	ParticleManager::PreDraw(commandList);

	player->ParticleDraw(nowViewProjection);

	if (isMovie) {


		gayserParticle->Draw(nowViewProjection);

	}



	ParticleManager::PostDraw();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画

	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画


#pragma endregion

#pragma region ポストエフェクトの描画

#pragma endregion

#pragma region 前景スプライト描画



	if (scene==Scene::Title ) {

		titlerogo->Draw(titlePos, { 1,1,1,1 });
	}

	if (scene == Scene::Game)
	{
		boss.DrawHealth();
		player->DrawHealth();
		boss.DrawHealth();
		player->DrawHealth();

	}
	else if (scene == Scene::Result) {
		gameClearFont->Draw({ 640,300 }, { 1,1,1,1 });
	}
	if (scene == Scene::GameOver)
	{
		gameover->Draw({ 640,360 }, { 1,1,1,1 });
		gameoverFont->Draw({ 640,300 }, { 1,1,1,1 });
	}

#pragma endregion
}

void GameScene::Reset()
{

	viewProjection_.eye = { 0,10,-10 };
	viewProjection_.UpdateMatrix();

	worldTransform_.translation_ = { 0,0,100 };
	worldTransform_.rotation_ = { 0,0,0 };
	worldTransform_.scale_ = { 0.1f,0.1f,0.1f };
	worldTransform_.TransferMatrix();

	player->Reset();

	gameCamera->Initialize();


	//model_->SetPolygonExplosion({ 0.0f,1.0f,0.0f,0.0f });


	//間欠泉の座標設定
	for (int i = 0; i < 5; i++) {
		float gayserPosRad = 360.0f / 5.0f * i;
		gayserPos[i].x = sin(gayserPosRad * PI / 180.0f) * stageRadius * 0.8f;
		gayserPos[i].z = cos(gayserPosRad * PI / 180.0f) * stageRadius * 0.8f;
	}

	boss.Reset();

	//ムービー用カメラの初期化
	movieCamera.Initialize();


	boss.fishParent.pos.translation_ = { 0,25,100 };
	boss.fishParent.pos.TransferMatrix();

	for (int i = 0; i < 10; i++) {
		Vector3 pos;
		pos = { Random(-stageRadius,  stageRadius) / 2, 0, Random(-stageRadius,  stageRadius) / 2 };
		pos += stagePos;
		minifishes[i].Initialize(pos, COLLISION_ATTR_WEAKENEMYS1 + i);
	}

	boss.Update({ 0,0,0 });

	// 間欠泉の初期化
	for (int i = 0; i < 5; i++) {
		gayserW[i].translation_.x = gayserPos[i].x;
		gayserW[i].translation_.y = -1.2f;
		gayserW[i].translation_.z = gayserPos[i].z;
		gayserW[i].scale_ = { 2,2,2 };
		gayserW[i].TransferMatrix();
	}

	gameCamera->SetPlayerMoveMent(player->GetPlayerMoveMent());
	gameCamera->SetSpaceInput(player->GetSpaceInput());
	gameCamera->SetCameraPosition(player->GetWorldPosition());
	//gameCamera->SetCameraPosition({0,0,-100});
	gameCamera->InitializeCameraPosition();

	//カメラは最後にアプデ
	viewProjection_.target = gameCamera->GetTarget();
	//viewProjection_.target = boss.fishParent.pos.translation_;
	viewProjection_.eye = gameCamera->GetEye();
	viewProjection_.fovAngleY = gameCamera->GetFovAngle();
	viewProjection_.UpdateMatrix();

	isTutorialEnd = false;
	isStartBossBattle = false;
}

void GameScene::Finalize()
{
}





int GameScene::GetMiniFishAlive() {
	int count = 0;
	for (int i = 0; i < 10; i++) {

		if (minifishes[i].GetAlive()) {
			count++;
		}
	}
	return count;
}

void GameScene::CheckAllFishLeave() {
	for (int i = 0; i < 10; i++) {
		//	minifishes[i].easeMove.Update();
		if (minifishes[i].easeMove.GetActive()) {
			isAllFishLeave = false;
			return;
		}
	}
	isAllFishLeave = true;
}

