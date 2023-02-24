#include "GameClear.h"

void GameClear::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
}

//result�̌��ʂ̌v�Z�ȂǁA����scene�ڍs
void GameClear::Update()
{

	if (input->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
	}
}

//result���ʂ̕`��
void GameClear::Draw()
{
}

void GameClear::Finalize()
{
}
