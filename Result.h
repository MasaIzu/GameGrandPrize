#pragma once
#include "BaseScene.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "Input.h"
#include "GameScene.h"
#include "SceneManager.h"

class Result : public BaseScene
{
public:
	Result();
	~Result();

	void Initialize() override;

	void Update() override;

	void PostEffectDraw() override;

	void Draw() override;

	void Finalize() override;

	void Select();

private:
	Input* input = nullptr;
	SceneManager* sceneManager_ = nullptr;
	GameScene* gameScene = nullptr;

	uint32_t clearTexture_ = 0;
	std::unique_ptr<Sprite> clearSprite_ = nullptr;
	uint32_t overTexture_ = 0;
	std::unique_ptr<Sprite> overSprite_ = nullptr;
	
	int selectNum = 0;
	int production = 0;

};

