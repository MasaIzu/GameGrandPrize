#include "GameClear.h"

void GameClear::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
}

//result‚ÌŒ‹‰Ê‚ÌŒvŽZ‚È‚ÇA–”‚ÍsceneˆÚs
void GameClear::Update()
{

	if (input->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
	}
}

//resultŒ‹‰Ê‚Ì•`‰æ
void GameClear::Draw()
{
}

void GameClear::Finalize()
{
}
