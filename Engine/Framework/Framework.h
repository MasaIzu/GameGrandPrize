#pragma once
#include "DirectXCore.h"
#include "GameScene.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "Input.h"
#include "FPS.h"
#include "ImGuiManager.h"

#include "SceneManager.h"
#include "FbxModel.h"

#include <memory>

class Framework {
public:
	//virtualメンバ関数
	//初期化
	virtual void Initialize();

	//更新
	virtual void Update();

	//描画
	virtual void Draw() = 0;

	//終了処理
	virtual void Finalize();

	//終了チェック
	virtual bool isPlayMyGame();

public://デストラクタ
	virtual ~Framework() = default;

public:
	//メンバ関数
	//実行
	void Run();

	WinApp* GetWinApp() { return winApp_; }
	DirectXCore* GetDirectXCore() { return directXCore_; }

protected:

	//シングルトン
	WinApp* winApp_ = nullptr;
	DirectXCore* directXCore_ = nullptr;
	Input* input_ = nullptr;
	SceneManager* sceneManager_;

	//シングルトン以外
	std::unique_ptr<ImGuiManager> imGui;
	std::unique_ptr <FPS> fps;
	std::unique_ptr <AbstractSceneFactory> sceneFactory_;
	std::unique_ptr<FbxModel> fbx;



	bool isEndRequst = false;
};