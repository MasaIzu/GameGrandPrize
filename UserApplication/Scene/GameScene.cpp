#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>
#include "FbxLoader.h"
#include"ImGuiManager.h"
#include <CollisionAttribute.h>
#include "Collision.h"
#include "Easing.h"
#include <math.h>

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
	skydomeTitle_.reset(Model::CreateFromOBJ("skydomeTitle", true));
	//groundModel = std::make_unique<Model>();
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeTitle_.get());

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

	boss = new Boss();
	boss->Initialize();

	//ムービー用カメラの初期化
	movieCamera.Initialize();


	

	/*for (int i = 0; i < boss.fishMaxCount; i++) {
		boss.CreateFish(gayserPos[i % 5]);
	}*/

	for (int i = 0; i < 10; i++) {
		Vector3 pos;
		pos = { Random(-stageRadius,  stageRadius) / 2, 0, Random(-stageRadius,  stageRadius) / 2 };
		pos += stagePos;
		minifishes[i].Initialize(pos, COLLISION_ATTR_WEAKENEMYS1 + i);
	}

	boss->Update({ 0,0,0 },stagePos,stageRadius);

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



	gameClearFont = std::make_unique<Sprite>();

	gameClearFont = Sprite::Create(TextureManager::Load("GameClearFont.png"));

	for (int i = 0; i < 5; i++) {
		sceneChageBlack[i] = Sprite::Create(TextureManager::Load("SceneChageBlack.png"));
		sceneChageBlack[i].get()->SetAnchorPoint({0,0});
		sceneChageBlack[i].get()->SetPosition(startPos[i]);
	}

	// タイトルのスプライトの初期化
	titleStartFont[0] = Sprite::Create(TextureManager::Load("StartFont.png"));
	titleStartFont[0]->SetTextureRect({ 0,0 }, { 432,208 });
	titleStartFont[0]->SetSize(titleFontSiza);
	titleStartFont[0]->SetAnchorPoint({ 0.5f,0.5f });
	titleStartFont[1] = Sprite::Create(TextureManager::Load("StartFont.png"));
	titleStartFont[1]->SetTextureRect({ 432,0 }, { 432,208 });
	titleStartFont[1]->SetSize(titleFontSiza);
	titleStartFont[1]->SetAnchorPoint({ 0.5f,0.5f });
	//titleBackGround = Sprite::Create(TextureManager::Load("titleBackGround.jpg"));
	//titleBackGround.get()->SetSize({ 1280,720 });
	//titleBackGround.get()->SetAnchorPoint({ 0, 0 });

	// タイトルのビューの初期化
	titleView.Initialize();

	// タイトルのオブジェクトの初期化
	AFontModel_.reset(Model::CreateFromOBJ("A_Font"));
	TFontModel_.reset(Model::CreateFromOBJ("T_Font"));
	OFontModel_.reset(Model::CreateFromOBJ("O_Font"));
	MFontModel_.reset(Model::CreateFromOBJ("M_Font"));
	SFontModel_.reset(Model::CreateFromOBJ("S_Font"));

	TitileParticle = std::make_unique<ParticleManager>();

	TitileParticle->Initialize();

	TitileParticle->SetTextureHandle(TextureManager::Load("effect4.png"));

	// タイトルのオブジェクトのワールドトランスフォームの初期化
	rotationY = -35.0f;
	sowrdRotationY = -35.0f;

	AFontWorld_.Initialize();
	AFontWorld_.scale_ = { 1,1,1 };
	AFontWorld_.translation_ = {+7.0f,+10.5f,+180};

	TFontWorld_.Initialize();
	TFontWorld_.scale_ = { 1,1,1 };
	TFontWorld_.translation_ = { +5.8f,+10.5f,+179 };

	OFontWorld_.Initialize();
	OFontWorld_.scale_ = { 1,1,1 };
	OFontWorld_.translation_ = { +4.8f,+10.5f,+178 };

	MFontWorld_.Initialize();
	MFontWorld_.scale_ = { 1,1,1 };
	MFontWorld_.translation_ = { +3.2f,+10.5f,+177 };

	SFontWorld_.Initialize();
	SFontWorld_.scale_ = { 1,1,1 };
	SFontWorld_.translation_ = { +1.6f,+10.5f,+176 };

	AFontWorld_.rotation_.y = DegreeToRadian(rotationY);
	TFontWorld_.rotation_.y = DegreeToRadian(sowrdRotationY);
	OFontWorld_.rotation_.y = DegreeToRadian(rotationY);
	MFontWorld_.rotation_.y = DegreeToRadian(rotationY);
	SFontWorld_.rotation_.y = DegreeToRadian(rotationY);

	AFontWorld_.SetRot(AFontWorld_.rotation_);
	TFontWorld_.SetRot(TFontWorld_.rotation_);
	OFontWorld_.SetRot(OFontWorld_.rotation_);
	MFontWorld_.SetRot(MFontWorld_.rotation_);
	SFontWorld_.SetRot(SFontWorld_.rotation_);

	AFontWorld_.TransferMatrix();
	TFontWorld_.TransferMatrix();
	OFontWorld_.TransferMatrix();
	MFontWorld_.TransferMatrix();
	SFontWorld_.TransferMatrix();
	
	// ゲームオーバーの初期化
	GameOverInit();

}

void GameScene::Update() {
	SceneChageUpdate();

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

	ImGui::Begin("Font");

	ImGui::Text("eye:%f,%f,%f", nowViewProjection.eye.x,nowViewProjection.eye.y, nowViewProjection.eye.z);
	ImGui::InputFloat("RotationY : %f", &rotationY);
	ImGui::InputFloat("S_RotationY : %f", &sowrdRotationY);

	ImGui::InputFloat2("startFontPos : %f", &titlePos.x);

	ImGui::InputFloat3("AFont:%f,%f,%f", &AFontWorld_.translation_.x);
	ImGui::InputFloat3("TFont:%f,%f,%f", &TFontWorld_.translation_.x);
	ImGui::InputFloat3("OFont:%f,%f,%f", &OFontWorld_.translation_.x);
	ImGui::InputFloat3("MFont:%f,%f,%f", &MFontWorld_.translation_.x);
	ImGui::InputFloat3("SFont:%f,%f,%f", &SFontWorld_.translation_.x);

	ImGui::InputFloat3("AFont:%f,%f,%f", &AFontWorld_.translation_.x);

	ImGui::End();

	// スプライトの位置調整

	// 文字の浮遊

	// ずらしタイマーのプラスと制限
	shiftTimer++;
	if (shiftTimer >= shiftTimeMax) {
		shiftTimer = shiftTimeMax;
	}
	// 浮遊タイマーのプラスと制限
	
	// 文字をずらして浮遊
	flyTimer[0]++;
	if (shiftTimer >= shiftTimeOneSet * 1) {
		flyTimer[1]++;
	}
	if (shiftTimer >= shiftTimeOneSet * 2) {
		flyTimer[2]++;
	}
	if (shiftTimer >= shiftTimeOneSet * 3) {
		flyTimer[3]++;
	}
	if (shiftTimer >= shiftTimeOneSet * 4) {
		flyTimer[4]++;
	}

	for (int i = 0; i < 5; i++) {
		if (flyTimer[i] >= flyMax) {
			flyTimer[i] = 0;
		}
	}
	ParticleFlame++;
	if (ParticleFlame>=10)
	{
		ParticleFlame = 0;
		for (int i = 0; i < 2; i++)
		{
			//消えるまでの時間
			const float rnd_life = 200.0f;
			//最低限のライフ
			const float constlife = 100;
			float life = (float)rand() / RAND_MAX * rnd_life + constlife;

			//XYZの広がる距離
			const float rnd_pos = 15.0f;
			//Y方向には最低限の飛ぶ距離
			const float constPosY = 15;
			Vector3 pos{};
			pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2;
			pos.y = -0;
			pos.z = 181;

			Vector3 startPos = pos;

			Vector3 endPos = { pos.x,8.0f,pos.z };

			//消えるまでの時間
			const float rndScale = 0.2f;
			//最低限のライフ
			const float constScale = 0.3f;
			float scale = (float)rand() / RAND_MAX * rndScale + constScale;
			//追加
			TitileParticle->Add(ParticleManager::Type::Normal, life, false, startPos, { 0,0,0 }, endPos, scale,scale, { 5,5,0,1 }, { 5,5,0,0.5 });
		}
	}


	AFontWorld_.translation_.y = Sin_ZeroToOne(10.5f, flyMax, flyTimer[0], 0.2f);
	AFontWorld_.TransferMatrix();

	TFontWorld_.translation_.y = Sin_ZeroToOne(10.5f, flyMax, flyTimer[1], 0.2f);
	TFontWorld_.TransferMatrix();

	OFontWorld_.translation_.y = Sin_ZeroToOne(10.5f, flyMax, flyTimer[2], 0.2f);
	OFontWorld_.TransferMatrix();

	MFontWorld_.translation_.y = Sin_ZeroToOne(10.5f, flyMax, flyTimer[3], 0.2f);
	MFontWorld_.TransferMatrix();

	SFontWorld_.translation_.y = Sin_ZeroToOne(10.5f, flyMax, flyTimer[4], 0.2f);
	SFontWorld_.TransferMatrix();

	TitileParticle->Update();

	if (input_->TriggerKey(DIK_Q)) {
		IsSceneChange = true;
	}
	if (input_->TriggerKey(DIK_SPACE)) {
		IsRotaStart = true;
		//scene = Scene::Game;
	}

	// 文字が回転する
	if (IsRotaStart == true) {
		// ソードの回転タイマーがマックスになったら
		if (sowrdRotaTimer >= sowrdRotaTimerMax) {
			sowrdRotaTimer = sowrdRotaTimerMax;
			IsRotaStart = false;
			IsRotaEnd = true;
		}

		sowrdRotaTimer++;

		// 回転にイージングをかける
		sowrdRotationY = float(Easing::In(startrota, endRota, sowrdRotaTimer, sowrdRotaTimerMax));
		TFontWorld_.rotation_.y = DegreeToRadian(sowrdRotationY);
		TFontWorld_.TransferMatrix();
		TFontWorld_.SetRot(TFontWorld_.rotation_);


	}

	// 文字の剣が回転終わった後シーンチェンジ
	if (IsRotaEnd == true) {

		oldScene = Scene::Title;
		IsSceneChange = true;
		TitileParticle->AllDelete();
	}

	if (input_->TriggerKey(DIK_K)) {
		scene = Scene::GameOver;
		Reset();
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
			//ボスをスポーンさせる
			for (int i = 0; i < boss->bossFish->fishMaxCount / 20; i++) {
				boss->bossFish->CreateFish(gayserPos[i % 5]);
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
		movieCamera.target = boss->bossFish->GetParentPos();
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
		boss->bossFish->Damage(2);
	}

	if (collisionManager->GetIsWakeEnemyAttackHit()) {
		isAttackHit = true;
		playerAttackHitNumber = collisionManager->GetHitNumber() - 1;

		minifishes[playerAttackHitNumber].SetAttribute(COLLISION_ATTR_WEAKENEMYS_DEI);

		minifishes[playerAttackHitNumber].OnCollision();
	}


	// ボスフェーズ１のHPが０になったら
	if (boss->bossFish->GetHealth() <= 0) {
		boss->bossFish->Death();
	}






	boss->Update(player->GetWorldPosition(),stagePos,stageRadius);
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

	if (boss->bossFish->GetIsDeathEnd()) {
		scene = Scene::Result;
	}
	if (player->GetAlive() == false)
	{
		scene = Scene::GameOver;
	}
}

void GameScene::GameOverUpdate()
{
	ImGui::Begin("Font");
	ImGui::InputFloat2("selectSize : %f", &selectButtonSize.x);
	ImGui::InputFloat2("replayFontSize : %f", &replayFontSize.x);
	ImGui::InputFloat2("backTitleFontSize : %f", &backTitleFontSize.x);
	ImGui::InputFloat2("selectButtonPos : %f", &selectButtonPos.x);
	ImGui::InputFloat2("replayFontPos : %f", &replayFontPos.x);
	ImGui::InputFloat2("backTitleFontPos : %f", &backTitleFontPos.x);

	ImGui::End();


	// αの出し方
	if (alphaTimer < alphaTimeMax) {
		alphaTimer++;
	}

	// 一段階目
	if (alpha[0] < 1.0f) {
		alpha[0] += alphaPlus;
	}
	// 二段階目
	if (alphaTimer >= alphaTimeOneSet * 0.5f) {
		if (alpha[1] < 1.0f) {
			alpha[1] += alphaPlus;
			
		}
	}
	// 三段階目
	if (alphaTimer >= alphaTimeOneSet * 2) {
		if (alpha[2] < 1.0f) {
			alpha[2] += alphaPlus;
		}
	}
	if (input_->TriggerKey(DIK_A)) {
		selectButtonPos = { 250,510 };
	}
	if (input_->TriggerKey(DIK_D)) {
		selectButtonPos = { 760,510 };
	}
	if (selectButtonPos.x < 640) {
		if (input_->TriggerKey(DIK_SPACE)) {
			oldScene = Scene::GameOver;
			IsSceneChange = true;
			IsRetry = true;
		}
	}
	else {
		if (input_->TriggerKey(DIK_SPACE)) {
			oldScene = Scene::GameOver;
			IsSceneChange = true;
			IsRetry = false;
		}
	}

}

void GameScene::ResultUpdate()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		oldScene = Scene::Result;
		IsSceneChange = true;
		
	}
}

void GameScene::SceneChageUpdate()
{
	if (IsSceneChange == true) {
		switch (scene)
		{
		case Scene::Title:
			if (oldScene == Scene::Title) {
				// 最初のスライダーのイン
				SceneChageFirst();
			}
			else {
				// 閉じきったら消す、スライダーのアウト
				SceneChageRast();
			}
			break;
		case Scene::Game:
			if (oldScene == Scene::Title || oldScene == Scene::GameOver) {
				// 閉じきったら消す、スライダーのアウト
				SceneChageRast();
			}
			break;
		case Scene::GameOver:
			if (oldScene == Scene::GameOver) {
				// 最初のスライダーのイン
				SceneChageFirst();
			}
			break;
		case Scene::Result:
			if (oldScene == Scene::Result) {
				// 最初のスライダーのイン
				SceneChageFirst();
			}
			break;
		default:
			break;
		}

		

	}
	
}

float GameScene::DegreeToRadian(float degree)
{
	float PI = 3.141592f;
	float result = degree * (PI / 180);

	return result;
}

float GameScene::Sin_ZeroToOne(float pos, float maxCount, float nowCount, float swingWidth)
{
	float PI = 3.141592f;
	float result = pos + sin(PI * 2 / maxCount * nowCount) * swingWidth;
	return result;
}

void GameScene::PostEffectDraw()
{
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	//// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// タイトルのオブジェクトの描画
	if (scene == Scene::Title) {
		AFontModel_.get()->Draw(AFontWorld_, nowViewProjection);
		TFontModel_.get()->Draw(TFontWorld_, nowViewProjection);
		OFontModel_.get()->Draw(OFontWorld_, nowViewProjection);
		MFontModel_.get()->Draw(MFontWorld_, nowViewProjection);
		SFontModel_.get()->Draw(SFontWorld_, nowViewProjection);
		
		skydome_.get()->Draw(nowViewProjection);
	}


	//model_->Draw(worldTransform_, viewProjection_);

	//stageModel_->Draw(stageWorldTransform_, nowViewProjection);

	//stageModel_->Draw(stageWorldTransform_,viewProjection_);

	// ゲームシーンのオブジェクトの描画
	else{
		ground.Draw(nowViewProjection);

		
		skydome_.get()->Draw(nowViewProjection);


		//チュートリアルと最初のムービーでだけ小魚を描画
		if (gamePhase == GamePhase::GameTutorial || gamePhase == GamePhase::GameMovie1) {

			for (int i = 0; i < 10; i++) {
				//minifishes[i].Draw(viewProjection_);
				if (minifishes[i].GetAlive()) {
					boss->bossFish->fishBodyModel->Draw(minifishes[i].GetWorldTransform(), nowViewProjection);
					boss->bossFish->fishEyeModel->Draw(minifishes[i].GetWorldTransform(), nowViewProjection);
				}
			}
		}

		//ボス出現ムービーとボス変身ムービーの間で描画
		boss->Draw(nowViewProjection);

		player->Draw(nowViewProjection);

		// 間欠泉の描画
		for (int i = 0; i < 5; i++) {
			gayserModel_[i]->Draw(gayserW[i], nowViewProjection);
		}
	}
	

	//3Dオブジェクト描画後処理
	Model::PostDraw();



	FbxModel::PreDraw(commandList);
	if (scene == Scene::Game) {
		player->PlayerFbxDraw(nowViewProjection);
	}
	FbxModel::PostDraw();

	ParticleManager::PreDraw(commandList);
	if (scene == Scene::Title) {
		TitileParticle->Draw(nowViewProjection);
	}
	if (scene == Scene::Game) {

		player->ParticleDraw(nowViewProjection);
	}



	ParticleManager::PostDraw();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	if (scene == Scene::Title) {
		
	}

	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画


#pragma endregion

#pragma region ポストエフェクトの描画

#pragma endregion

#pragma region 前景スプライト描画



	if (scene==Scene::Title ) {

		//titleStartFont->Draw(titlePos, { 1,1,1,1 });
		if (IsRotaStart==true) {
			titleStartFont[1].get()->Draw(titlePos,{1,1,1,1});
		}
		else {
			titleStartFont[0].get()->Draw(titlePos, { 1,1,1,1 });
		}

	}

	if (scene == Scene::Game)
	{
		boss->DrawHealth();
		player->DrawHealth();
		//boss->DrawHealth();
		//player->DrawHealth();

	}
	else if (scene == Scene::Result) {
		gameClearFont->Draw({ 640,300 }, { 1,1,1,1 });
	}
	if (scene == Scene::GameOver)
	{
		gameoverFont->Draw({ 640,300 }, { 1,1,1,alpha[0] });
		gameover->Draw({ 640,360 }, { 1,1,1,alpha[1]});
		

		selectButton->Draw(selectButtonPos, { 1,1,1,alpha[2] });
		replayFont->Draw(replayFontPos, { 1,1,1,alpha[2] });
		backTitleFont->Draw(backTitleFontPos, { 1,1,1,alpha[2] });
	}

	// シーンチェンジ用のブラックスライダーの描画
	if (IsSceneChange == true) {
		for (int i = 0; i < 5; i++) {
			sceneChageBlack[i].get()->Draw(sceneChagePos[i], { 1,1,1,1 });
		}
	}


#pragma endregion
}

void GameScene::Reset()
{
	// タイトルシーンのリセット
	if (scene == Scene::Title) {
		skydome_.get()->SetModel(skydomeTitle_.get());
		AFontWorld_.translation_ = { +7.0f,+10.5f,+180 };
		TFontWorld_.translation_ = { +5.8f,+10.5f,+179 };
		OFontWorld_.translation_ = { +4.8f,+10.5f,+178 };
		MFontWorld_.translation_ = { +3.2f,+10.5f,+177 };
		SFontWorld_.translation_ = { +1.6f,+10.5f,+176 };

		sowrdRotationY = -35.0f;
		TFontWorld_.rotation_.y = DegreeToRadian(sowrdRotationY);
		TFontWorld_.SetRot(TFontWorld_.rotation_);

		AFontWorld_.TransferMatrix();
		TFontWorld_.TransferMatrix();
		OFontWorld_.TransferMatrix();
		MFontWorld_.TransferMatrix();
		SFontWorld_.TransferMatrix();

		for (int i = 0; i < 5; i++) {
			flyTimer[i] = 0;
		}
		shiftTimer = 0;
		sowrdRotaTimer = 0;

		IsRotaStart = false;
		IsRotaEnd = false;

	}
	if (scene == Scene::Game) {
		skydome_.get()->SetModel(skyModel.get());
	}
	
	// ゲームオーバーのリセット
	GameOverReset();
	
	collisionManager->CheckAllCollisions();

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

	boss->Reset();

	//ムービー用カメラの初期化
	movieCamera.Initialize();



	for (int i = 0; i < 10; i++) {
		Vector3 pos;
		pos = { Random(-stageRadius,  stageRadius) / 2, 0, Random(-stageRadius,  stageRadius) / 2 };
		pos += stagePos;
		minifishes[i].Initialize(pos, COLLISION_ATTR_WEAKENEMYS1 + i);
	}

	boss->Update({ 0,0,0 },stagePos,stageRadius);

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

	collisionManager->CheckAllCollisions();
}

void GameScene::Finalize()
{
	delete boss;
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

void GameScene::SceneChageFirst()
{
	// 最初のスライダーのイン
	if (IsHalf == false) {
		if (sceneChageBlack[0].get()->GetPosition().x >= 0) {
			if (sceneChageTimer[0] < sceneChageTimerMax) {
				sceneChageTimer[0]++;
			}

			sceneChagePos[0] = { float(Easing::easeOutCubic(startPos[0].x, endPos[0].x, sceneChageTimer[0], sceneChageTimerMax)),startPos[0].y };
		}
		for (int i = 1; i < 5; i++) {
			if (sceneChageTimer[i - 1] / sceneChageTimerMax >= 0.5f) {
				if (sceneChageTimer[i] < sceneChageTimerMax) {
					sceneChageTimer[i]++;
				}
				sceneChagePos[i] = { float(Easing::easeOutCubic(startPos[i].x, endPos[i].x, sceneChageTimer[i], sceneChageTimerMax)),startPos[i].y };
			}
		}
		// ハーフタイムになったら
		if (sceneChageTimer[4] / sceneChageTimerMax >= 1.0f) {
			for (int i = 0; i < 5; i++) {
				sceneChageTimer[i] = 0;
			}
			IsHalf = true;

			// 次がなんのシーンかチェックする
			switch (scene)
			{
			case Scene::Title:
				scene = Scene::Game;
				Reset();
				break;
			case Scene::Game:
				
				break;
			case Scene::GameOver:
				if (IsRetry == false) {
					scene = Scene::Title;
				}
				else if (IsRetry == true) {
					scene = Scene::Game;
				}
				Reset();
				break;
			case Scene::Result:
				scene = Scene::Title;
				Reset();
				break;
			default:
				break;
			}
		}
	}
}

void GameScene::SceneChageRast()
{
	if (IsHalf == true) {
		if (sceneChageBlack[0].get()->GetPosition().x >= 0) {
			if (sceneChageTimer[0] < sceneChageTimerMax) {
				sceneChageTimer[0]++;
			}
			sceneChagePos[0] = { float(Easing::easeOutCubic(endPos[0].x, startPos[0].x, sceneChageTimer[0], sceneChageTimerMax)),startPos[0].y };
		}
		for (int i = 1; i < 5; i++) {
			if (sceneChageTimer[i - 1] / sceneChageTimerMax >= 0.5f) {
				if (sceneChageTimer[i] < sceneChageTimerMax) {
					sceneChageTimer[i]++;
				}
				sceneChagePos[i] = { float(Easing::easeOutCubic(endPos[i].x, startPos[i].x, sceneChageTimer[i], sceneChageTimerMax)),startPos[i].y };
			}
		}
		// スライダーがすべて消えたらフラグをオフ
		if (sceneChageTimer[4] / sceneChageTimerMax >= 1.0f) {
			// タイマーのリセット
			for (int i = 0; i < 5; i++) {
				sceneChageTimer[i] = 0;
			}
			// フラグ関連のリセット
			IsHalf = false;
			IsSceneChange = false;
		}
	}
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

void GameScene::GameOverInit()
{
	// スプライトのロード
	gameoverFont = std::make_unique<Sprite>();

	gameover = std::make_unique<Sprite>();

	gameoverFont = Sprite::Create(TextureManager::Load("GameOverFont.png"));

	gameover = Sprite::Create(TextureManager::Load("gameover.png"));

	selectButton= Sprite::Create(TextureManager::Load("selectButton.png"));
	replayFont = Sprite::Create(TextureManager::Load("RetryFont.png"));
	backTitleFont= Sprite::Create(TextureManager::Load("backTitleFont.png"));

	// サイズのセット
	selectButton.get()->SetSize(selectButtonSize);
	replayFont.get()->SetSize(replayFontSize);
	backTitleFont.get()->SetSize(backTitleFontSize);
}

void GameScene::GameOverReset()
{
	alphaTimer = 0;
	IsRetry = false;
	selectButtonPos={250,510};
	for (int i = 0; i < 3; i++) {
		alpha[i] = 0;
	}
}

