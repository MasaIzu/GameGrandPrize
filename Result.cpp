#include "Result.h"

Result::Result()
{
	
}

Result::~Result()
{

}

void Result::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
	gameScene = new GameScene;

	clearTexture_ = TextureManager::Load("mario.jpg");
	clearSprite_ = Sprite::Create(clearTexture_);

	overTexture_ = TextureManager::Load("mario.jpg");
	overSprite_ = Sprite::Create(overTexture_);
}

void Result::Update()
{
	Select();
}

void Result::PostEffectDraw()
{
}

void Result::Draw()
{
	if (gameScene->GetClear())
	{
		clearSprite_->Draw({ 640,360 }, { 1,1,1,1 });
	}
	if (gameScene->GetOver())
	{
		overSprite_->Draw({ 640,360 }, { 1,1,1,1 });
	}
}

void Result::Finalize()
{
}

void Result::Select()
{
	if (input->PushKey(DIK_UP))
	{
		selectNum = 0;
	}
	if (input->PushKey(DIK_DOWN))
	{
		selectNum = 1;
	}

	if (input->PushKey(DIK_L))
	{
		production = selectNum;
		if (production == 0)
		{
			sceneManager_->ChangeScene("TITLE");
		}
		if (production == 1)
		{
			sceneManager_->ChangeScene("GAMEPLAY");
		}
	}

}
