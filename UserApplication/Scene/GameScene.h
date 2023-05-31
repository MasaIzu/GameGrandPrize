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

#include "PouseUi.h"

#include <objbase.h>
#include "Sound.h"
#include<array>
class TouchableObject;

//struct ground
//{
//	WorldTransform pos;
//
//};

#include"MiniFish.h"
#include"FbxAnimation.h"
#include"EasingData.h"

//enum class Scene {
//	Title,
//	Game,
//	GameOver,
//	Result
//};

enum class EventPhase {

	MinifishSpawn,
	Boss1Spawn,
	BossPhaseChange,
	Boss2Death,
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

	bool IsBreak() override;


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

	//ポーズ
	PouseUi* pouseUi_ = nullptr;


	Boss* boss = nullptr;
	//デバッグによる生成用
	float newFishPosY = 0;

	bool isEnemyHit = false;
	bool isAttackHit = false;

	int playerAttackHitNumber = 0;

	EventPhase eventPhase = EventPhase::MinifishSpawn;

	//小魚関係(チュートリアル用)
	static const int miniFishMax = 10;
	std::array<MiniFish, miniFishMax> minifishes;
	int deadMinFishCount = 0;	//倒された小魚のカウント
	std::unique_ptr<ParticleManager> gayserParticle;
	bool isTutorialEnd = false;
	bool isStartBossBattle = false;
	bool isMovie = false;
	bool isActiveChangeEvent = false;
//	EasingData easeCameraEye{};

	const float gayserMaxFlame = 240;
	float gayserFlame = 0;

	int fishSpawnInterval = 0;
	int fishSpawnCount = 0;

	//ステージ関係
	float stageRadius = 50;	//ステージの半径
	Vector3 stagePos{ 0,0,0 };//ステージの中心座標


	// 間欠泉関連
	static const int gayserMax = 5;
	std::array<Vector3, gayserMax> gayserPos;	//小魚が吹き出る間欠泉座標
	std::array<std::unique_ptr<Model>, gayserMax> gayserModel_;
	std::array<WorldTransform, gayserMax> gayserW;

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
	int cameraStopTime = 0;

	//現在使っているビュープロ
	ViewProjection nowViewProjection;

	/*Scene scene = Scene::Title;

	Scene oldScene = Scene::Title;*/
	std::unique_ptr<Sprite> gameClearFont;


	// シーンチェンジ用のスプライト
	static const int sceneChangerSpriteMax = 5;
	std::array<std::unique_ptr<Sprite>, sceneChangerSpriteMax> sceneChageBlack;
	std::array<Vector2, sceneChangerSpriteMax> sceneChagePos = {
		Vector2{1280,0},
		Vector2{1280,144},
		Vector2{1280,288},
		Vector2{1280,432},
		Vector2{1280,576},
	};
	std::array<float, sceneChangerSpriteMax> sceneChageTimer = { 0,0,0,0,0 };
	float sceneChageTimerMax = 40;
	bool IsSceneChange = false;
	bool IsHalf = false;
	std::array< Vector2, sceneChangerSpriteMax> startPos = {
		Vector2{1280,0},
		Vector2{1280,144},
		Vector2{1280,288},
		Vector2{1280,432},
		Vector2{1280,576},
	};
	std::array < Vector2, sceneChangerSpriteMax> endPos = {
		Vector2{0,0},
		Vector2{0,144},
		Vector2{0,288},
		Vector2{0,432},
		Vector2{0,576},
	};
	std::array < Vector2, sceneChangerSpriteMax> backStartPos = {
		Vector2{0,0},
		Vector2{0,144},
		Vector2{0,288},
		Vector2{0,432},
		Vector2{0,576},
	};
	std::array < Vector2, sceneChangerSpriteMax> backEndPos = {
		Vector2{1280,0},
		Vector2{1280,144},
		Vector2{1280,288},
		Vector2{1280,432},
		Vector2{1280,576},
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
	static const int titleFontMax = 5;
	std::array<float,titleFontMax> flyTimer;
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

	int ParticleFlame = 0;

#pragma endregion

#pragma region gameOver関連
	bool IsRetry = false;
	static const int gameOverSpriteMax = 3;
	std::array<float,gameOverSpriteMax> alpha;

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
	Vector2 selectButtonSize = { 40,80 };
	Vector2 replayFontSize = { 252,92 };
	Vector2 backTitleFontSize = { 282,100 };

	// スプライトのポジション
	Vector2 selectButtonPos = { 250,510 };
	Vector2 replayFontPos = { 400,520 };
	Vector2 backTitleFontPos = { 900,520 };
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
	Vector3 FirstEndPos = { 0,14,195 };
	Vector3 FirstEndTarget = { 0,14,150 };

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

	bool one = FALSE;
#pragma endregion


private://プライベート関数

	void CheckAllFishLeave();

	//生きている小魚の数を更新
	int GetMiniFishAlive();

	//最初のムービー(小魚が10匹程生まれるムービー)の更新処理
	void FirstMovieUpdate();

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

	//ボスの変身開始関数
	void StartBossChangeEvent();

	//ボスの変身の更新
	void UpdateBossChangeEventCamera();
};

int MinMax(int param,int min, int max);