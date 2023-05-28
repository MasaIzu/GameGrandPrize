#include "PouseUi.h"


PouseUi::PouseUi()
{
}

PouseUi::~PouseUi()
{
	
}

void PouseUi::Destroy()
{
	//delete PouseUi_;
}

void PouseUi::Initialize()
{
	input_ = Input::GetInstance();

	Load();
}

void PouseUi::Update()
{
	if (input_->TriggerKey(DIK_P) && isPouse == FALSE)
	{
		isPouse = TRUE;
	}
	else if (input_->TriggerKey(DIK_P) && isPouse == TRUE)
	{
		isPouse = FALSE;
	}

	if (isPouse)
	{
		Select();
		
		//ポーズ画面決定させたときの処理
		Process();
	}
	
}

void PouseUi::DrawEasing()
{
	

}

void PouseUi::Select()
{

	//下選択させる
	if (input_->TriggerKey(DIK_DOWN))
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
	if (input_->TriggerKey(DIK_UP))
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
	if (input_->TriggerKey(DIK_RETURN))
	{
		decided = select;
		OK = TRUE;
	}

}

void PouseUi::Process()
{
	if (OK)
	{
		switch (decided)
		{
		case 1://ゲームへ戻る0
			Back();
			OK = FALSE;
			break;

		case 2://リセット1
			GameReset();
			OK = FALSE;
			break;

		case 3://設定2
			Setting();

			break;

		case 4://タイトルへ3
			Title();
			Reset();
			OK = FALSE;
			break;

		case 5://ゲーム終了4
			Exit();
			Reset();
			OK = FALSE;
			break;
		}
	}

}

void PouseUi::Draw()
{
	//ポーズ画面項目すべて表示
	//spriteUi_->		Draw({spriteSizeX,spriteSizeY+296},{1,1,1,1});
	spriteBack_->	Draw(printXY, { 1,1,1,1 });
	spriteReset_->	Draw(printXY2, { 1,1,1,1 });
	spriteTitle_->	Draw(printXY3, { 1,1,1,1 });
	spriteSetting_->Draw(printXY4, { 1,1,1,1 });
	spriteExit_->	Draw(printXY5, { 1,1,1,1 });
	



	//選択中のものを表示
	spriteSelect_->Draw({ spriteSizeX,spriteSizeY * select + size * (select-1) }, { 1,1,1,1 });

}

void PouseUi::Load()
{
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

void PouseUi::Reset()
{

}

void PouseUi::Back()
{
	isPouse = FALSE;
}

void PouseUi::GameReset()
{

}

void PouseUi::Title()
{

}

void PouseUi::Setting()
{

}

void PouseUi::Exit()
{

}

