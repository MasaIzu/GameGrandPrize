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

	model_.reset(Model::CreateFromOBJ("sphere", true));

	viewProjection_.eye = { 0,10,-10 };

	sceneManager_ = SceneManager::GetInstance();

	viewProjection_.Initialize();
	viewProjection_.farZ = 1500;
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
	for (int i = 0; i < gayserPos.size(); i++) {
		float gayserPosRad = 360.0f / 5.0f * i;
		gayserPos[i].x = sin(gayserPosRad * PI / 180.0f) * stageRadius * 0.8f;
		gayserPos[i].z = cos(gayserPosRad * PI / 180.0f) * stageRadius * 0.8f;
	}
	gayserParticle = std::make_unique<ParticleManager>();

	gayserParticle->Initialize();

	gayserParticle->SetTextureHandle(TextureManager::Load("effect2.png"));

	boss = new Boss();
	boss->Initialize();
	boss->SetPlayer(player.get());
	//ムービー用カメラの初期化
	movieCamera.Initialize();




	/*for (int i = 0; i < boss.fishMaxCount; i++) {
		boss.CreateFish(gayserPos[i % 5]);
	}*/

	//for (int i = 0; i < 10; i++) {
	//	Vector3 pos;
	//	pos = { Random(-stageRadius,  stageRadius) / 2, 0, Random(-stageRadius,  stageRadius) / 2 };
	//	pos += stagePos;
	//	minifishes[i].Initialize(pos, COLLISION_ATTR_WEAKENEMYS1 + i);
	//}

	boss->Update({ 0,0,0 }, stagePos, stageRadius);

	// 間欠泉の初期化
	for (int i = 0; i < gayserModel_.size(); i++) {
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

	spaceKeyFont = Sprite::Create(TextureManager::Load("spaceKey.png"));
	spaceKeyFont->SetAnchorPoint({ 0.5f,0.5f });

	for (int i = 0; i < sceneChageBlack.size(); i++) {
		sceneChageBlack[i] = Sprite::Create(TextureManager::Load("SceneChageBlack.png"));
		sceneChageBlack[i].get()->SetAnchorPoint({ 0,0 });
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
	AFontWorld_.translation_ = { +7.0f,+16.5f,+180 };

	TFontWorld_.Initialize();
	TFontWorld_.scale_ = { 1,1,1 };
	TFontWorld_.translation_ = { +5.8f,+16.5f,+179 };

	OFontWorld_.Initialize();
	OFontWorld_.scale_ = { 1,1,1 };
	OFontWorld_.translation_ = { +4.8f,+16.5f,+178 };

	MFontWorld_.Initialize();
	MFontWorld_.scale_ = { 1,1,1 };
	MFontWorld_.translation_ = { +3.2f,+16.5f,+177 };

	SFontWorld_.Initialize();
	SFontWorld_.scale_ = { 1,1,1 };
	SFontWorld_.translation_ = { +1.6f,+16.5f,+176 };

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

	firstCamera.Initialize();
	// 最初のカメラ
	firstCamera.eye = { 0,15,100 };
	firstCamera.target = { 0,0,0 };

	// サウンドの初期化
	SoundLoadInit();
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

	ImGui::Text("eye:%f,%f,%f", nowViewProjection.eye.x, nowViewProjection.eye.y, nowViewProjection.eye.z);
	ImGui::Text("target:%f,%f,%f", nowViewProjection.target.x, nowViewProjection.target.y, nowViewProjection.target.z);

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

	// BGＭを鳴らす
	if (IsTitleBGM == false) {
		titleBGM.SoundPlayWave(true, 0.5f);
		IsTitleBGM = true;
	}

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

	for (int i = 0; i < flyTimer.size(); i++) {
		if (flyTimer[i] >= flyMax) {
			flyTimer[i] = 0;
		}
	}
	ParticleFlame++;
	if (ParticleFlame >= 10)
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
			pos.y = 6;
			pos.z = 181;

			Vector3 startPos = pos;

			Vector3 endPos = { pos.x,14.0f,pos.z };

			//消えるまでの時間
			const float rndScale = 0.2f;
			//最低限のライフ
			const float constScale = 0.3f;
			float scale = (float)rand() / RAND_MAX * rndScale + constScale;
			//追加
			TitileParticle->Add(ParticleManager::Type::Normal, life, false, startPos, { 0,0,0 }, endPos, scale, scale, { 5,5,0,1 }, { 5,5,0,0.0 });
		}
	}


	AFontWorld_.translation_.y = Sin_ZeroToOne(16.5f, flyMax, flyTimer[0], 0.2f);
	AFontWorld_.TransferMatrix();

	TFontWorld_.translation_.y = Sin_ZeroToOne(16.5f, flyMax, flyTimer[1], 0.2f);
	TFontWorld_.TransferMatrix();

	OFontWorld_.translation_.y = Sin_ZeroToOne(16.5f, flyMax, flyTimer[2], 0.2f);
	OFontWorld_.TransferMatrix();

	MFontWorld_.translation_.y = Sin_ZeroToOne(16.5f, flyMax, flyTimer[3], 0.2f);
	MFontWorld_.TransferMatrix();

	SFontWorld_.translation_.y = Sin_ZeroToOne(16.5f, flyMax, flyTimer[4], 0.2f);
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
		//イベントから始まるのでイベントフラグを立てる
		isMovie = true;

	}

	if (input_->TriggerKey(DIK_K)) {
		scene = Scene::GameOver;
		Reset();
	}
}

void GameScene::GameUpdate()
{
	if (ImGui::Button("break")) {
		static int a = 0;
		a++;
	}

	if (isMovie) {
		// イベントシーンの更新
		// 最初のカメラと魚の更新
		FirstCameraUpdate();
		FirstMovieUpdate();


		//チュートリアルが終了かつ、魚が間欠泉に逃げ終わっているなら
		if (!isStartBossBattle) {
			if (isTutorialEnd && isAllFishLeave) {
				//ボスの生成を開始
				fishSpawnCount = 20;
				isStartBossBattle = true;
				//小魚を全員殺す
				for (int i = 0; i < minifishes.size(); i++) {
					minifishes[i].SetAttribute(COLLISION_ATTR_WEAKENEMYS_DEI);
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
					int gayserIndex = MinMax(i % 5, 0, gayserPos.size());
					boss->bossFish->CreateFish(gayserPos[gayserIndex]);
				}
			}
		}

		//魚がすべて間欠泉に逃げたかチェック
		CheckAllFishLeave();

		//チュートリアルが終わっていて、魚が移動し終わっていないならカメラを上からの見下ろしに
		if (isTutorialEnd && !isAllFishLeave) {
			movieCamera.eye = { 0,125,-150 };
			movieCamera.target = { 0,0,0 };
		}

		//ボス生成フェーズになったらカメラをボスに向ける
		if (isStartBossBattle) {
			movieCamera.target = boss->bossFish->GetParentPos();
		}

		//小魚スポーンカウントが0でボス戦開始フラグがtrueならムービー終了
		if (isStartBossBattle && fishSpawnCount == 0) {
			if (eventPhase == EventPhase::Boss1Spawn) {
				isMovie = false;
			}
		}

		//ボスの変身のカメラ処理
		UpdateBossChangeEventCamera();

		//ボス第二形態死亡のカメラ処理
		UpdateBossDeathEvent();

	}
	else {
		//ゲーム本編の更新処理

			//生きている小魚の数が5匹以下になったら魚が逃げ出す
		if (!isTutorialEnd && !IsFirst) {
			if (GetMiniFishAlive() < 5) {
				isTutorialEnd = true;
				isMovie = true;
				eventPhase = EventPhase::Boss1Spawn;
				for (int i = 0; i < minifishes.size(); i++) {
					int gayserIndex = MinMax(i / 2, 0, gayserPos.size());
					minifishes[i].LeaveGayser(gayserPos[gayserIndex]);
				}
			}

		}

		//各当たり判定とそのコールバック
		if (collisionManager->GetIsEnemyHit()) {
			gameCamera->Collision();
			Matrix4 a = collisionManager->GetEnemyWorldPos();
			player->SetEnemyPos(collisionManager->GetEnemyWorldPos());
			if (player->GetIsAwakening() == false) {
				player->Collision(10);
			}

		}

		if (collisionManager->GetIsWakeEnemyHit()) {
			gameCamera->Collision();
			Matrix4 a = collisionManager->GetEnemyWorldPos();
			player->SetEnemyPos(collisionManager->GetEnemyWorldPos());
			if (player->GetIsAwakening() == false) {
				player->Collision(5);
			}
		}
		player->EnemyNotAttackCollision(collisionManager->GetIsEnemyReception(), collisionManager->GetPlayerPos());


		ImGui::Text("EnemyWorldPosX : %f", MyMath::GetWorldTransform(collisionManager->GetEnemyWorldPos()).x);
		ImGui::Text("EnemyWorldPosY : %f", MyMath::GetWorldTransform(collisionManager->GetEnemyWorldPos()).y);
		ImGui::Text("EnemyWorldPosZ : %f", MyMath::GetWorldTransform(collisionManager->GetEnemyWorldPos()).z);

		//剣と自機の当たり判定
		if (collisionManager->GetEnemySwordHit()) {
			gameCamera->Collision();
			player->SetEnemyPos(collisionManager->GetEnemyWorldPos());
			player->Collision(20);
		}

		if (collisionManager->GetIsEnemyKingDrop()) {
			gameCamera->Collision();
			player->SetEnemyPos(collisionManager->GetEnemyWorldPos());
			player->Collision(2,1);
		}

		if (collisionManager->GetIsAttackHit()) {
			isAttackHit = true;
			gameCamera->Collision();
			player->SetParticlePos(collisionManager->GetAttackHitWorldPos());
			if (player->GetUltState() == false) {
				boss->bossFish->Damage(2);
				if (boss->bossWarrier->GetAlive()) {
					boss->bossWarrier->Damage(2);
				}
			}
			else {
				boss->bossFish->Damage(4);
				if (boss->bossWarrier->GetAlive()) {
					boss->bossWarrier->Damage(4);
				}
			}

			player->AddUltCount(60);
		}

		if (collisionManager->GetIsWakeEnemyAttackHit()) {
			isAttackHit = true;
			playerAttackHitNumber = collisionManager->GetHitNumber() - 1;
			playerAttackHitNumber = MinMax(playerAttackHitNumber, 0, minifishes.size());

			minifishes[playerAttackHitNumber].SetAttribute(COLLISION_ATTR_WEAKENEMYS_DEI);

			minifishes[playerAttackHitNumber].OnCollision();

			player->AddUltCount(50);
		}

		// ボスフェーズ１のHPが０になったら
		if (boss->bossFish->GetHealth() <= 0) {
			boss->bossFish->Death();

			//イベントシーンが進んでいなければ進める
			if (eventPhase == EventPhase::Boss1Spawn) {
				eventPhase = EventPhase::BossPhaseChange;
				StartBossChangeEvent();
			}

		}

		//テスト用
		if (Input::GetInstance()->TriggerKey(DIK_H)) {
			InitBossDeathEvent();
		}

		if (boss->bossFish->GetIsDeathEnd() && boss->bossWarrier->GetIdDeadEnd()) {

			InitBossDeathEvent();

			//scene = Scene::Result;
		}
		if (player->GetAlive() == false)
		{
			scene = Scene::GameOver;
		}

	}

	//イベントと本編両方で必要な更新
		//小魚の更新
	for (int i = 0; i < minifishes.size(); i++) {
		minifishes[i].Update(stagePos, stageRadius);
	}

	boss->Update(player->GetWorldPosition(), stagePos, stageRadius);

	if (boss->GetBossFishIsDeathEnd() && IsBattle02BGM == false && IsBattle01BGM == true)
	{
		battle01BGM.StopWave();
		IsBattle01BGM = false;
		battle02BGM.SoundPlayWave(true, 0.5);
		IsBattle02BGM = true;
	}


	if (isTutorialEnd) {
		ImGui::Text("tutorial end!");
	}

	if (isAllFishLeave) {
		ImGui::Text("all fishes leave!");
	}

	player->SetIsEnemyHit(isEnemyHit);
	player->SetIsAttackHit(isAttackHit);
	isAttackHit = false;
	player->SetAngle(gameCamera->GetCameraAngle());
	player->SetCameraRot(gameCamera->GetCameraRotVec3());
	player->SetCameraLook(viewProjection_.cameraLook);


	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();

	gayserParticle->Update();



	//カメラは最後にアプデ
		//ムービーフラグがオンならカメラをムービー用に
	if (isMovie) {
		movieCamera.UpdateMatrix();
		nowViewProjection = movieCamera;
	}
	else {
		player->Update(viewProjection_);
		nowViewProjection = viewProjection_;
		gameCamera->SetPlayerMoveMent(player->GetPlayerMoveMent());
		gameCamera->SetSpaceInput(player->GetSpaceInput());
		gameCamera->SetCameraPosition(player->GetWorldPosition());
		gameCamera->Update(&viewProjection_);
	}

	viewProjection_.target = gameCamera->GetTarget();
	//viewProjection_.target = boss.fishParent.pos.translation_;
	viewProjection_.eye = gameCamera->GetEye();
	viewProjection_.fovAngleY = gameCamera->GetFovAngle();
	viewProjection_.UpdateMatrix();
	//ParticleMan->Update();



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

	// ゲームオーバーのBGMを鳴らす
	if (IsGameOverBGM == false) {
		battle02BGM.StopWave();
		IsBattle02BGM = false;
		gameOverBGM.SoundPlayWave(true, 0.5f);
		IsGameOverBGM = true;
	}

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

			//小魚を全員殺す
			for (int i = 0; i < minifishes.size(); i++) {
				minifishes[i].SetAttribute(COLLISION_ATTR_WEAKENEMYS_DEI);
				minifishes[i].OnCollision();
			}

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

	backTitleFontPos = { 650,480 };
	// 徐々にゲームクリアのスプライトを出す処理
	if (gameClearSpriteAlpha < 1.0f) {
		gameClearSpriteAlpha += 0.02;
	}

	// ゲームオーバーのBGMを鳴らす
	if (IsGameClearBGM == false) {
		battle02BGM.StopWave();
		IsBattle02BGM = false;
		gameClearBGM.SoundPlayWave(true, 0.5f);
		IsGameClearBGM = true;
	}
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
	else {
		ground.Draw(nowViewProjection);


		skydome_.get()->Draw(nowViewProjection);


		//チュートリアルと最初のムービーでだけ小魚を描画


		for (int i = 0; i < minifishes.size(); i++) {
			//minifishes[i].Draw(viewProjection_);
			if (minifishes[i].GetAlive()) {
				boss->bossFish->fishBodyModel->Draw(minifishes[i].GetWorldTransform(), nowViewProjection);
				boss->bossFish->fishEyeModel->Draw(minifishes[i].GetWorldTransform(), nowViewProjection);
				model_->Draw(minifishes[i].GetWorldTransform(), nowViewProjection);
			}
		}



		//ボス出現ムービーとボス変身ムービーの間で描画
		boss->Draw(nowViewProjection);

		player->Draw(nowViewProjection);

		// 間欠泉の描画
		for (int i = 0; i < gayserModel_.size(); i++) {
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

	boss->bossWarrier->DrawParticle(nowViewProjection);

	gayserParticle->Draw(nowViewProjection);

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



	if (scene == Scene::Title) {

		//titleStartFont->Draw(titlePos, { 1,1,1,1 });
		if (IsRotaStart == true) {
			titleStartFont[1].get()->Draw(titlePos, { 1,1,1,1 });
		}
		else {
			titleStartFont[0].get()->Draw(titlePos, { 1,1,1,1 });
		}

	}

	if (scene == Scene::Game)
	{
		if (!isMovie) {

			boss->DrawHealth();
			player->DrawHealth();
			if (boss->bossFish->GetIsDeathEnd()) {
				boss->bossWarrier->DrawHealth();
			}

		}
		else {
			//ここにムービー用の黒の縁の描画を入れる

		}

		//boss->DrawHealth();
		//player->DrawHealth();

	}
	else if (scene == Scene::Result) {
		gameClearFont->Draw(gameClearFontPos, { 1,1,1,gameClearSpriteAlpha });
		spaceKeyFont->Draw(spaceKeyFontPos, { 1,1,1,gameClearSpriteAlpha });
		backTitleFont->Draw(backTitleFontPos, { 1,1,1,gameClearSpriteAlpha });
	}
	if (scene == Scene::GameOver)
	{
		gameoverFont->Draw({ 640,300 }, { 1,1,1,alpha[0] });
		gameover->Draw({ 640,360 }, { 1,1,1,alpha[1] });


		selectButton->Draw(selectButtonPos, { 1,1,1,alpha[2] });
		replayFont->Draw(replayFontPos, { 1,1,1,alpha[2] });
		backTitleFont->Draw(backTitleFontPos, { 1,1,1,alpha[2] });
	}

	// シーンチェンジ用のブラックスライダーの描画
	if (IsSceneChange == true) {
		for (int i = 0; i < sceneChageBlack.size(); i++) {
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
		AFontWorld_.translation_ = { +7.0f,+16.5f,+180 };
		TFontWorld_.translation_ = { +5.8f,+16.5f,+179 };
		OFontWorld_.translation_ = { +4.8f,+16.5f,+178 };
		MFontWorld_.translation_ = { +3.2f,+16.5f,+177 };
		SFontWorld_.translation_ = { +1.6f,+16.5f,+176 };

		sowrdRotationY = -35.0f;
		TFontWorld_.rotation_.y = DegreeToRadian(sowrdRotationY);
		TFontWorld_.SetRot(TFontWorld_.rotation_);

		AFontWorld_.TransferMatrix();
		TFontWorld_.TransferMatrix();
		OFontWorld_.TransferMatrix();
		MFontWorld_.TransferMatrix();
		SFontWorld_.TransferMatrix();

		for (int i = 0; i < flyTimer.size(); i++) {
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
	for (int i = 0; i < gayserPos.size(); i++) {
		float gayserPosRad = 360.0f / 5.0f * i;
		gayserPos[i].x = sin(gayserPosRad * PI / 180.0f) * stageRadius * 0.8f;
		gayserPos[i].z = cos(gayserPosRad * PI / 180.0f) * stageRadius * 0.8f;
	}

	boss->Reset();

	//ムービー用カメラの初期化
	movieCamera.Initialize();

	isMovie = false;

	//for (int i = 0; i < 10; i++) {
	//	Vector3 pos;
	//	pos = { Random(-stageRadius,  stageRadius) / 2, 0, Random(-stageRadius,  stageRadius) / 2 };
	//	pos += stagePos;
	//	minifishes[i].Initialize(pos, COLLISION_ATTR_WEAKENEMYS1 + i);
	//}

	boss->Update({ 0,0,0 }, stagePos, stageRadius);

	// 間欠泉の初期化
	for (int i = 0; i < gayserW.size(); i++) {
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

	// ゲームのスタート時のカメラリセット
	IsFirst = true;
	isMovie = true;
	//IsFirstCameraEnd = false;

	isTutorialEnd = false;
	isStartBossBattle = false;
	isAllFishLeave = false;

	collisionManager->CheckAllCollisions();

	eventPhase = EventPhase::MinifishSpawn;
}

void GameScene::Finalize()
{
	delete boss;
}

int GameScene::GetMiniFishAlive() {
	int count = 0;
	for (int i = 0; i < minifishes.size(); i++) {

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
		for (int i = 1; i < sceneChageTimer.size(); i++) {
			if (sceneChageTimer[i - 1] / sceneChageTimerMax >= 0.5f) {
				if (sceneChageTimer[i] < sceneChageTimerMax) {
					sceneChageTimer[i]++;
				}
				sceneChagePos[i] = { float(Easing::easeOutCubic(startPos[i].x, endPos[i].x, sceneChageTimer[i], sceneChageTimerMax)),startPos[i].y };
			}
		}
		// ハーフタイムになったら
		if (sceneChageTimer[4] / sceneChageTimerMax >= 1.0f) {
			for (int i = 0; i < sceneChageTimer.size(); i++) {
				sceneChageTimer[i] = 0;
			}
			IsHalf = true;
			TitileParticle->AllDelete();
			// 次がなんのシーンかチェックする
			switch (scene)
			{
			case Scene::Title:
				scene = Scene::Game;
				IsTitleBGM = false;
				titleBGM.StopWave();
				battle01BGM.SoundPlayWave(true, 0.5);
				IsBattle01BGM = true;
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
				IsGameOverBGM = false;
				gameOverBGM.StopWave();
				Reset();
				break;
			case Scene::Result:
				scene = Scene::Title;
				IsGameClearBGM = false;
				gameClearBGM.StopWave();
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
		for (int i = 1; i < sceneChageTimer.size(); i++) {
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
			for (int i = 0; i < sceneChageTimer.size(); i++) {
				sceneChageTimer[i] = 0;
			}
			// フラグ関連のリセット
			IsHalf = false;
			IsSceneChange = false;
		}
	}
}

void GameScene::CheckAllFishLeave() {
	if (!isTutorialEnd) {
		return;
	}

	for (int i = 0; i < minifishes.size(); i++) {
		//	minifishes[i].easeMove.Update();
		if (minifishes[i].easeMove.GetActive() && minifishes[i].GetAlive()) {
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

	selectButton = Sprite::Create(TextureManager::Load("selectButton.png"));
	replayFont = Sprite::Create(TextureManager::Load("RetryFont.png"));
	backTitleFont = Sprite::Create(TextureManager::Load("backTitleFont.png"));

	// サイズのセット
	selectButton.get()->SetSize(selectButtonSize);
	replayFont.get()->SetSize(replayFontSize);
	backTitleFont.get()->SetSize(backTitleFontSize);
}

void GameScene::GameOverReset()
{
	alphaTimer = 0;
	IsRetry = false;
	selectButtonPos = { 250,510 };
	for (int i = 0; i < alpha.size(); i++) {
		alpha[i] = 0;
	}
}

void GameScene::SoundLoadInit()
{

	titleBGM.SoundLoadWave("titleBGM.wav");
	battle01BGM.SoundLoadWave("Battle-Conflict_loop.wav");
	battle02BGM.SoundLoadWave("battle-dark_loop.wav");
	gameOverBGM.SoundLoadWave("Rest-in-Peace_loop.wav");
	gameClearBGM.SoundLoadWave("Loop02.wav");
}

void GameScene::StartBossChangeEvent()
{
	if (isActiveChangeEvent) {
		return;
	}

	isActiveChangeEvent = true;
	isMovie = true;
	nowViewProjection = movieCamera;
}

void GameScene::UpdateBossChangeEventCamera() {
	if (!isActiveChangeEvent) {
		return;
	}



	float cameraRadian = boss->bossWarrier->GetEasingData().GetTimeRate();
	float cameraDistance = 120.0f;


	//注視点はボスに
	movieCamera.target = boss->bossWarrier->GetRootTransform().translation_;

	if (cameraStopTime > 0) {
		cameraStopTime--;
		if (cameraStopTime == 0) {
			isMovie = false;
			isActiveChangeEvent = false;
		}
		return;
	}
	
	//カメラ座標をボスのイージングを使って回転させる
	cameraRadian *= 360.0f;
	cameraRadian -= 90.0f;
	cameraRadian = convertDegreeToRadian(cameraRadian);


	//ImGui::Text("cameraRadian %f", cameraRadian *180 / PI);

	Vector3 eye = movieCamera.eye;
	eye.y = 20.0f;
	eye.x = sin(cameraRadian) * cameraDistance + stagePos.x;
	eye.z = cos(cameraRadian) * cameraDistance + stagePos.z;
	movieCamera.eye = eye;



	if (boss->bossWarrier->GetEasingData().GetTimeRate() >= 1.0f) {

		cameraStopTime = 30;
	}


}

void GameScene::FirstCameraUpdate()
{
	ImGui::Begin("camera");

	ImGui::InputFloat3("firstEye:%f,%f,%f", &firstCamera.eye.x);
	ImGui::InputFloat3("firstCamera.target:%f,%f,%f", &firstCamera.target.x);

	ImGui::End();

	// 最初のカメラがオンだったら
	if (IsFirst == true) {
		// エネミーがスポーンする前
		if (IsEnemySpon == false) {
			timer++;
			if (timer >= timerMax) {
				IsEnemySpon = true;
			}
			// 
			movieCamera.eye = { 0,60,120 };
			movieCamera.target = { 0,-5,0 };


		}
		else {
			firstCameraTimer++;
			// カメラ移動
			movieCamera.eye = Easing::InOutVec3(FirstStartPos, FirstEndPos, firstCameraTimer, firstCameraTimeMax);
			movieCamera.target = Easing::InOutVec3(FirstStartTarget, FirstEndTarget, firstCameraTimer, firstCameraTimeMax);


			if (firstCameraTimer >= firstCameraTimeMax) {
				timer = 0;
				IsEnemySpon = false;
				IsFirst = false;
				isTutorialEnd = false;
				isAllFishLeave = false;
				isMovie = false;
				gameCamera->MousePositionReset();
			}
		}
	}
}

void GameScene::FirstMovieUpdate()
{
	//フラグが経ってなければ処理を終了
	if (!IsFirst) {
		return;
	}

	//敵のスポーンタイミング
	int enemySpawnTiming = timerMax / 10 - 1;

	//最初のカメラの固定時間で10体出す
	if ((int)timer % enemySpawnTiming == 0 && !IsEnemySpon) {
		int enemyIndex = timer / enemySpawnTiming - 1;
		enemyIndex = MinMax(enemyIndex, 0, minifishes.size());
		int gayserIndex = MinMax(enemyIndex % 5, 0, gayserPos.size());

		Vector3 pos;




		pos = { Random(-stageRadius,  stageRadius) / 2, 0, Random(-stageRadius,  stageRadius) / 2 };
		pos += stagePos;
		minifishes[enemyIndex].Initialize(pos, gayserPos[gayserIndex], enemySpawnTiming * 2, COLLISION_ATTR_WEAKENEMYS1 + enemyIndex);

		Vector3 particleAfterPos;
		for (int i = 0; i < 10; i++) {
			//パーティクルを出す
			particleAfterPos.x = Random(-10.0f, 10.0f);
			particleAfterPos.y = Random(12.5f, 20.0f);
			particleAfterPos.z = Random(-10.0f, 10.0f);
			particleAfterPos += gayserPos[gayserIndex];

			gayserParticle->Add(ParticleManager::Type::Normal, enemySpawnTiming, false, gayserPos[gayserIndex], gayserPos[gayserIndex], particleAfterPos, 0.0f, 10.0f, { 0,0,0,1, }, { 0,0,0,0 });
		}
	}
}

void GameScene::InitBossDeathEvent() {

	isMovie = true;
	//ムービーフェーズをボス2死亡に
	eventPhase = EventPhase::Boss2Death;

	//カメラを指定の位置にセット
	movieCamera.target = boss->bossWarrier->GetRootTransform().translation_;

	player->SetPosition({ 0,0,120 });
	player->SetRotation({ 0,PI,0 });
	player->Update(viewProjection_);

}


void GameScene::UpdateBossDeathEvent() {

	if (eventPhase != EventPhase::Boss2Death) {
		return;
	}
	float timerate = boss->bossWarrier->GetEasingData().GetTimeRate();
	Vector3 eyeBefore;
	Vector3 eyeAfter;

	ImGui::Text("timerate %f", timerate);

	//ボスが上昇するときにカメラも上に上げる
	if (boss->bossWarrier->GetBossDeathPhase() == BossDeathPhase::RiseBody) {
		eyeBefore = { 0,0,100 };
		eyeAfter = { 0,45,100 };

		movieCamera.eye = Lerp(eyeBefore, eyeAfter, timerate);
		movieCamera.target = boss->bossWarrier->GetRootTransform().translation_;
	}//頭落下のフェーズでtargetは頭に
	else if (boss->bossWarrier->GetBossDeathPhase() == BossDeathPhase::BreakBody) {
		movieCamera.target = boss->bossWarrier->GetRootTransform().translation_;
	}
	else if (boss->bossWarrier->GetBossDeathPhase() == BossDeathPhase::FallHead) {
		movieCamera.target = boss->bossWarrier->GetHeadTransform().translation_;
	}//モーションの終わり(頭が消えてく)時にtargetを頭→自機に
	else if (boss->bossWarrier->GetBossDeathPhase() == BossDeathPhase::MotionEnd) {
		Vector3 afterTarget = player->GetWorldPosition();
		afterTarget.y += 10.0f;

		movieCamera.target = Lerp(movieCamera.target = boss->bossWarrier->GetHeadTransform().translation_,afterTarget , timerate);
		eyeBefore = { 0,45,100 };
		eyeAfter = player->GetWorldPosition();
		eyeAfter.z += 30;
		eyeAfter.y += 5;
		movieCamera.eye = Lerp(eyeBefore, eyeAfter, timerate);

		//イージング終わってるならシーン以降
		if (timerate >= 1.0f) {
			scene = Scene::Result;
		}

	}


}

int MinMax(int param, int min, int max)
{
	if (param > max)return max;
	else if (param < min)return min;

	return param;
}
