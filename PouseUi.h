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

	bool GetisPouse() { return isPouse; }

	//ゲームシーン移行要
	Scene scene = Scene::Title;

	Scene oldScene = Scene::Title;


private:

	//static PouseUi* PouseUi_;
	//シーン選択用
	Input* input_ = nullptr;

	const int MIN = 1;
	const int MAX = 5;
	const int SMAX = 3;
	int select = MIN;
	//画像の最終座標位置
	Vector2 printXY = { spriteSizeX,spriteSizeY };//画像最初地点
	Vector2 printXY2 = { spriteSizeX,spriteSizeY * 2 + size };
	Vector2 printXY3 = { spriteSizeX,spriteSizeY * 3 + size * 2 };
	Vector2 printXY4 = { spriteSizeX,spriteSizeY * 4 + size * 3 };
	Vector2 printXY5 = { spriteSizeX,spriteSizeY * 5 + size * 4 };

	//スプライトの座標を入れる箱
	Vector2 sp;
	Vector2 sp2;
	Vector2 sp3;
	Vector2 sp4;
	Vector2 sp5;
	//決定させたやつを代入
	int decided = MIN;
	
	//フラグ
	bool OK = false;
	bool isPouse = false;
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

	float spriteSizeX = 640.0f;
	float spriteSizeY = 64.0f;

	float size = 32.0f;
};

