#pragma once
#include "Input.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Easing.h"
enum class Scene {
	Title,
	Game,
	GameOver,
	Result
};

class PouseUi
{
public:
	//コンストラクタ
	PouseUi();
	//デストラクタ
	~PouseUi();

	//初期化
	void Initialize();
	//処理
	void Update();
	//画像のイージング処理
	void DrawEasing();
	//描画
	void Draw();

	void Load();

	void Select();

	void Process();

	//リセット関数
	void Reset();


	void Destroy();

	void Back();

	void GameReset();

	void Title();

	void Setting();

	void Exit();

	void EasingReset();

	bool GetisPouse() { return isPouse; }

	bool GetGameReset() { return gameReset; }

	bool GetisEnd() { return isEnd; }

	//bool GetisTitle() { return isTitle; }

	//ゲームシーン移行要
	Scene scene = Scene::Title;

	Scene oldScene = Scene::Title;

	bool isTitle = false;
	bool gameReset = false;

private:


	Vector2 selectButtonSize = { 40,80 };
	//static PouseUi* PouseUi_;
	//シーン選択用
	Input* input_ = nullptr;

	const int MIN = 1;
	const int MAX = 3;
	const int SMAX = 3;
	int select = MIN;
	float size = 32.0f;
	float spriteSizeX = 640.0f;
	float spriteSizeY = 64.0f;
	
	int easingXTimer = 0;
	const int EASINGMAXTIME_X = 30;

	int easingYTimer = 0;
	int easingYTimer2 = 0;
	int easingYTimer3 = 0;
	int easingYTimer4 = 0;
	int easingYTimer5 = 0;
	const int EASINGMAXTIME_Y = 25;

	//画像の最終座標位置
	Vector2 printXY ;//画像最初地点
	Vector2 printXY2 ;
	Vector2 printXY3 ;
	Vector2 printXY4 ;
	Vector2 printXY5 ;

	//スプライトの座標を入れる箱
	Vector2 sp = { spriteSizeX,spriteSizeY };
	Vector2 sp2 = { spriteSizeX,spriteSizeY };
	Vector2 sp3 = { spriteSizeX,spriteSizeY };
	Vector2 sp4 = { spriteSizeX,spriteSizeY };
	Vector2 sp5 = { spriteSizeX,spriteSizeY };

	Vector2 mdlPos = { 650,520 };
	Vector2 sPos1 = { 400, 520 };
	Vector2 sPos2 = { 870,520 };
	Vector2 ent = mdlPos;
	int selectCheck = 1;
	int waitTimer = 0;
	CONST INT WAITMAXTIME = 40;

	//中心
	Vector2 mdl = { 1280 / 2, 720 / 2 };
	//決定させたやつを代入
	int decided = MIN;
	
	//フラグ
	bool OK = false;
	bool isPouse = false;
	bool easingOkX = false;
	bool isEnd = false;
	bool isCheck = false;
	bool non = FALSE;
	
	//bool isTitle = false;

	//bool showEnd = false;
	//テクスチャ
	//ポーズ画面の背景
	std::unique_ptr<Sprite> spriteUi_ = nullptr;
	uint32_t loserUi_ = 0;
	//戻る
	std::unique_ptr<Sprite> spriteBack_ = nullptr;
	uint32_t loserBack_ = 0;
	//リセット
	std::unique_ptr<Sprite> spriteReset_ = nullptr;
	uint32_t loserReset_ = 0;
	//タイトル
	std::unique_ptr<Sprite> spriteTitle_ = nullptr;
	uint32_t loserTitle_ = 0;
	//設定
	std::unique_ptr<Sprite> spriteSetting_ = nullptr;
	uint32_t loserSetting_ = 0;
	//終わる
	std::unique_ptr<Sprite> spriteExit_ = nullptr;
	uint32_t loserExit_ = 0;
	//セレクト
	std::unique_ptr<Sprite> spriteSelect_ = nullptr;
	uint32_t loserSelect_ = 0;

	//セレクト
	std::unique_ptr<Sprite> spriteSelect2_ = nullptr;

	//ゲーム終了確認
	std::unique_ptr<Sprite> spriteCheck_ = nullptr;
	uint32_t loserCheck_ = 0;
	
	
	
	
};

