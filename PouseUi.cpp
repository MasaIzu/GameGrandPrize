#include "PouseUi.h"


PouseUi::PouseUi()
{
}

PouseUi::~PouseUi()
{
	
}

//PouseUi* PouseUi_ = nullptr;

//PouseUi* PouseUi::GetInstance()
//{
//	/*if (PouseUi_ == nullptr)
//	{
//		PouseUi_ = new PouseUi;
//	}
//	return PouseUi_;*/
//}

void PouseUi::Destroy()
{
	//delete PouseUi_;
}

void PouseUi::Initialize()
{
	input = Input::GetInstance();

	//UI背景
	loserUi_ = TextureManager::Load("UI/UIPouse.png");
	spriteUi_ = Sprite::Create(loserUi_);
	//戻る
	loserBack_ = TextureManager::Load("UI/Back.png");
	spriteBack_ = Sprite::Create(loserBack_);

	//リセット
	loserReset_ = TextureManager::Load("UI/Restart.png");
	spriteReset_ = Sprite::Create(loserReset_);

	//タイトル
	loserTitle_ = TextureManager::Load("UI/Title.png");
	spriteTitle_ = Sprite::Create(loserTitle_);

	//設定
	loserSetting_ = TextureManager::Load("UI/Setting.png");
	spriteSetting_ = Sprite::Create(loserSetting_);

	//終わる
	loserExit_ = TextureManager::Load("UI/Exit.png");
	spriteExit_ = Sprite::Create(loserExit_);

	//SELECT
	loserSelect_ = TextureManager::Load("UI/Select.png");
	spriteSelect_ = Sprite::Create(loserSelect_);
}

void PouseUi::Update()
{
	//下選択させる
	if (input->TriggerKey(DIK_DOWN))
	{
		//項目最大値に行ったら最大で止める
		if (select >= MAX)
		{
			select = MAX;
		}
		else
		{
			select += 1;
		}
	}
	//上に選択させる
	if (input->TriggerKey(DIK_UP))
	{
		//項目最小値に行ったら最小で止める
		if (select <= MIN)
		{
			select = MIN;
		}
		else
		{
			select -= 1;
		}
	}

	//選択中の移動をスムージングする処理


	//決定させる
	if (input->TriggerKey(DIK_RETURN))
	{
		ok = select;
		OK = TRUE;
	}
	//ポーズ画面決定させたときの処理
	if (OK)
	{
		switch (ok)
		{
		case 1://ゲームへ戻る0
			OK = FALSE;
			break;

		case 2://リセット1
			OK = FALSE;
			Reset();
			break;

		case 3://設定2
			OK = FALSE;
			Reset();
			break;

		case 4://タイトルへ3
			OK = FALSE;
			Reset();
			break;

		case 5://ゲーム終了4
			OK = FALSE;
			Reset();
			break;
		} 
	}

	//Draw();
	
}

void PouseUi::Draw()
{
	//ポーズ画面項目すべて表示
	spriteUi_->Draw({640,360},{1,1,1,1});


	//選択中のものを表示


}

void PouseUi::Reset()
{
}
