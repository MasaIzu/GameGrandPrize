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
}

void Result::Update()
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

void Result::PostEffectDraw()
{
}

void Result::Draw()
{

}

void Result::Finalize()
{
}
