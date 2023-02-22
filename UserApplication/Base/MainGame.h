#pragma once
#include "Framework.h"

// ゲーム全体
class MainGame : public Framework
{
public: // メンバ関数
	MainGame() = default;
	~MainGame() = default;

	// 初期化
	void Initialize() override;
	// 終了
	void Finalize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;

private:
	// ゲーム終了フラグ
	bool endRequest_ = false;
};
