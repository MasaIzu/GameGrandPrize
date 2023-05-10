#pragma once

#include "DirectXCore.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin.h"
#include"ParticleManager.h"

#include "BaseScene.h"
#include "SceneManager.h"

#include "FbxModel.h"

#include<memory>
#include<vector>

#include "Skydome.h"

#include "BoxCollision.h"

#include "Player.h"
#include "GameCamera.h"
#include"Boss.h"
#include <CollisionManager.h>

#include "UserApplication/Ground/Ground.h"

#include <objbase.h>


class TouchableObject;

//struct ground
//{
//	WorldTransform pos;
//
//};

#include"MiniFish.h"
#include"FbxAnimation.h"

enum class Scene {
	Title,
	Game,
	GameOver,
	Result
};

enum class GamePhase {
	GameTutorial,	//チュートリアル
	GameMovie1,		//ムービー1(チュートリアル→ボス戦への遷移)
	GameBoss1,		//ボス戦第一形態
	GameMovie2,		//ムービー2(ボスの形態変化)
	GameBoss2,		//ボス戦第二形態
	GameMovie3,		//ムービー3(ボスが死ぬとき)
};


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

public:


	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	void TitleUpdate();

	void GameUpdate();

	void GameOverUpdate();

	void ResultUpdate();




	void PostEffectDraw() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	void Reset();

	// 終了処理
	void Finalize() override;

	//void CreateGround();




private: // メンバ変数
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;


	//ビュープロジェクション
	ViewProjection viewProjection_;
	//ワールド変換データ
	WorldTransform worldTransform_;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデル
	std::unique_ptr<Model> model_;

	std::unique_ptr<Player> player;
	std::unique_ptr<GameCamera> gameCamera;

	//当たり判定
	CollisionManager* collisionManager = nullptr;

	//ボックス当たり判定
	std::unique_ptr <BoxCollision> boxCollision;

	//シーンマネージャー
	SceneManager* sceneManager_ = nullptr;


	Boss* boss = nullptr;
	//デバッグによる生成用
	float newFishPosY = 0;

	bool isEnemyHit = false;
	bool isAttackHit = false;

	int playerAttackHitNumber = 0;

	GamePhase gamePhase = GamePhase::GameTutorial;

	//小魚関係(チュートリアル用)
	MiniFish minifishes[10];
	int deadMinFishCount = 0;	//倒された小魚のカウント
	std::unique_ptr<ParticleManager> gayserParticle;
	bool isTutorialEnd = false;
	bool isStartBossBattle = false;
	bool isMovie = false;

	const float gayserMaxFlame = 240;
	float gayserFlame = 0;

	int fishSpawnInterval = 0;
	int fishSpawnCount = 0;

	//ステージ関係
	float stageRadius = 50;	//ステージの半径
	Vector3 stagePos{ 0,0,0 };//ステージの中心座標


	// 間欠泉関連
	Vector3 gayserPos[5];	//小魚が吹き出る間欠泉座標
	std::unique_ptr<Model> gayserModel_[5];
	WorldTransform gayserW[5];

	bool isAllFishLeave = false;

	// 3Dモデル
	std::unique_ptr<Model> stageModel_;
	//ワールド変換データ
	WorldTransform stageWorldTransform_;

	//天球
	std::unique_ptr<Model> skyModel = nullptr;
	std::unique_ptr<Skydome> skydome_;

	////地面作成
	std::unique_ptr<Model>groundModel = nullptr;
	std::vector<ground> objects;

	static const int groundMaxCount = 10;

	std::unique_ptr<Model> modeltable[groundMaxCount];

	Ground ground;

	ViewProjection movieCamera;

	//現在使っているビュープロ
	ViewProjection nowViewProjection;

	Scene scene = Scene::Title;

	std::unique_ptr<Sprite> gameoverFont;
	std::unique_ptr<Sprite> gameClearFont;
	std::unique_ptr<Sprite> gameover;

	// シーンチェンジ用のスプライト
	std::unique_ptr<Sprite> sceneChageBlack[5];
	Vector2 sceneChagePos[5]= {
		{1280,0},
		{1280,144},
		{1280,288},
		{1280,432},
		{1280,576},
	};
	float sceneChageTimer[5] = { 0,0,0,0,0 };
	float sceneChageTimerMax = 40;
	bool IsSceneChange = false;
	bool IsHalf = false;
	Vector2 startPos[5] = {
		{1280,0},
		{1280,144},
		{1280,288},
		{1280,432},
		{1280,576},
	};
	Vector2 endPos[5] = {
		{0,0},
		{0,144},
		{0,288},
		{0,432},
		{0,576},
	};
	Vector2 backStartPos[5] = {
		{0,0},
		{0,144},
		{0,288},
		{0,432},
		{0,576},
	};
	Vector2 backEndPos[5] = {
		{1280,0},
		{1280,144},
		{1280,288},
		{1280,432},
		{1280,576},
	};

	Scene oldScene = Scene::Title;

	// スプライト
	Vector2 titlePos = { 350,200 };
	std::unique_ptr <Sprite> titlerogo = nullptr;

private://プライベート関数

	void CheckAllFishLeave();

	//生きている小魚の数を更新
	int GetMiniFishAlive();

	// シーンチェンジ関数
	void SceneChageFirst();

	void SceneChageRast();

	void SceneChageUpdate();
};