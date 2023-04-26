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
	
	clearTexture_ = TextureManager::Load("GameClearFont.png");
	clearSprite_ = Sprite::Create(clearTexture_);

	overTexture_ = TextureManager::Load("GameOverFont.png");
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
	if (&GameScene::GetClear)
	{
		clearSprite_->Draw({ 360,360 }, { 1,1,1,1 });
	}
	if (&GameScene::GetOver)
	{
		overSprite_->Draw({ 360,360 }, { 1,1,1,1 });
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
