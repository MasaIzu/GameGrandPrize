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
#include "Sound.h"
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

	Scene oldScene = Scene::Title;
	std::unique_ptr<Sprite> gameClearFont;


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


	
	// スプライト


	
#pragma region タイトルオブジェクト関連
	// タイトルビュープロジェクション
	ViewProjection titleView;

	// タイトル用のオブジェクトのモデル
	std::unique_ptr<Model> skydomeTitle_;
	std::unique_ptr<Model> AFontModel_;
	std::unique_ptr<Model> TFontModel_;
	std::unique_ptr<Model> OFontModel_;
	std::unique_ptr<Model> MFontModel_;
	std::unique_ptr<Model> SFontModel_;

	std::unique_ptr<ParticleManager> TitileParticle;

	// オブジェクトのワールドトランスフォーム
	WorldTransform AFontWorld_;
	WorldTransform TFontWorld_;
	WorldTransform OFontWorld_;
	WorldTransform MFontWorld_;
	WorldTransform SFontWorld_;

	// タイトルの浮遊に使う変数
	float flyTimer[5];
	float flyMax = 60 * 4;

	// 浮かせるタイミングをずらすためのタイマー
	float shiftTimer = 0;
	float shiftTimeOneSet = 20;
	float shiftTimeMax = shiftTimeOneSet * 5;

	// 文字の角度
	float rotationY = 0;
	float sowrdRotationY = 0;

	// 回転の開始と終わり
	float startrota = -35.0f;
	float endRota = -360 * 3;
	bool IsRotaStart = false;
	bool IsRotaEnd = false;

	// 回転するときのタイマー
	float sowrdRotaTimer = 0;
	float sowrdRotaTimerMax = 60 * 1.5;

	// タイトルのスタートの文字スプライト
	Vector2 titlePos = { 950,460 };
	Vector2 titleFontSiza = { 259,124 };
	std::unique_ptr <Sprite> titleStartFont[2];

	// タイトルの背景のスプライト
	std::unique_ptr <Sprite> titleBackGround;

	int ParticleFlame=0;

#pragma endregion

#pragma region gameOver関連
	bool IsRetry = false;
	float alpha[3];

	float alphaTimer = 0;
	float alphaTimeOneSet = 20;
	float alphaTimeMax = 100;
	float alphaPlus = 0.02f;

	// スプライト
	std::unique_ptr<Sprite> gameoverFont;
	std::unique_ptr<Sprite> gameover;
	std::unique_ptr<Sprite> selectButton;
	std::unique_ptr<Sprite> replayFont;
	std::unique_ptr<Sprite> backTitleFont;

	// スプライトのサイズ
	Vector2 selectButtonSize={40,80};
	Vector2 replayFontSize={252,92};
	Vector2 backTitleFontSize={282,100};

	// スプライトのポジション
	Vector2 selectButtonPos={250,510};
	Vector2 replayFontPos={400,520};
	Vector2 backTitleFontPos={900,520};
#pragma endregion

#pragma region gameStartカメラ関連
	// ゲームスタートのカメラになっているかフラグ
	bool IsFirst = false;
	// ゲームスタート時かめら
	ViewProjection firstCamera;
	// エネミーが生成し終わっているかどうか
	bool IsEnemySpon = false;
	// かめら移動速度
	Vector3 moveSpeed;
	// スタート時のカメラが終わっているかどうか
	bool IsFirstCameraEnd = false;
	// 最初のスタート位置
	Vector3 FirstStartPos = { 0,60,120 };
	Vector3 FirstStartTarget = { 0,-5,0 };
	// 最初のエンド位置
	Vector3 FirstEndPos = { 0,8,195 };
	Vector3 FirstEndTarget = { 0,8,150 };

	// 仮のタイマー
	float timer = 0;
	float timerMax = 240;

	// 移動タイマー
	float firstCameraTimer = 0;
	float firstCameraTimeMax = 180;

#pragma endregion

	#pragma region  gameのBGM
	Sound titleBGM;
	Sound battle01BGM;
	Sound battle02BGM;
	Sound gameOverBGM;
	Sound gameClearBGM;

	bool IsTitleBGM = false;
	bool IsBattle01BGM = false;
	bool IsBattle02BGM = false;
	bool IsGameOverBGM = false;
	bool IsGameClearBGM = false;
	#pragma endregion


private://プライベート関数

	void CheckAllFishLeave();

	//生きている小魚の数を更新
	int GetMiniFishAlive();

	// シーンチェンジ関数
	void SceneChageFirst();

	void SceneChageRast();

	void SceneChageUpdate();

	// ゲームオーバーの初期化
	void GameOverInit();

	void GameOverReset();

	// 最初のカメラの更新処理
	void FirstCameraUpdate();

	// サウンドのロード
	void SoundLoadInit();

	// 度数からラジアン
	float DegreeToRadian(float degree);

	// sin波の動きを作る関数
	float Sin_ZeroToOne(float pos, float maxCount, float nowCount, float swingWidth);
};