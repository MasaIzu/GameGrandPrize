#include "GameClear.h"

void GameClear::Initialize()
{
	input = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
}

//resultの結果の計算など、又はscene移行
void GameClear::Update()
{

	if (input->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
	}
}

//result結果の描画
void GameClear::Draw()
{
}

void GameClear::Finalize()
{
}
