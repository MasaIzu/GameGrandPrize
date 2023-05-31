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

	Reset();
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
		if (easingOkX == TRUE)
		{
			
			if (easingYTimer >= EASINGMAXTIME_Y)
			{
				easingYTimer = EASINGMAXTIME_Y;
			}
			else
			{
				easingYTimer++;
			}
		}
		
		
		if (easingXTimer >= EASINGMAXTIME_X)
		{
			easingXTimer = EASINGMAXTIME_X;
			easingOkX = TRUE;
		}
		else
		{
			easingXTimer++;
		}

		DrawEasing();
		Select();
		
		//ポーズ画面決定させたときの処理
		Process();
	}
	else
	{
		EasingReset();
	}
	
}

void PouseUi::DrawEasing()
{//                     最初のところ　　　　　　　　　　止めたいところ　　タイマー　　　最大タイマー
	sp.x = Easing::In(spriteSizeX - spriteSizeX*2, spriteSizeX, easingXTimer, EASINGMAXTIME_X);
	sp2.x = Easing::In(spriteSizeX - spriteSizeX*2, spriteSizeX, easingXTimer, EASINGMAXTIME_X);
	sp3.x = Easing::In(spriteSizeX - spriteSizeX*2, spriteSizeX, easingXTimer, EASINGMAXTIME_X);
	sp4.x = Easing::In(spriteSizeX - spriteSizeX*2, spriteSizeX, easingXTimer, EASINGMAXTIME_X);
	sp5.x = Easing::In(spriteSizeX - spriteSizeX*2, spriteSizeX, easingXTimer, EASINGMAXTIME_X);

	sp2.y = Easing::In(spriteSizeY , printXY2.y, easingYTimer, EASINGMAXTIME_Y);
	sp3.y = Easing::In(spriteSizeY , printXY3.y, easingYTimer, EASINGMAXTIME_Y);
	sp4.y = Easing::In(spriteSizeY , printXY4.y, easingYTimer, EASINGMAXTIME_Y);
	sp5.y = Easing::In(spriteSizeY , printXY5.y, easingYTimer, EASINGMAXTIME_Y);
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
			isPouse = FALSE;
			break;

		case 2://リセット1
			GameReset();
			OK = FALSE;
			isPouse = FALSE;
			break;

		case 3://タイトルへ2
			
			Title();
			Reset();
			OK = FALSE;
			isPouse = FALSE;
			break;

		case 4://ゲーム終了3
			Exit();
			Reset();

			break;

		case 5://設定4
			Setting();
			break;

		
		}
	}

}

void PouseUi::Draw()
{
	

	if (isCheck)
	{
		//本当に終わってもいいのかを確認する用の画像表示
		spriteCheck_->Draw(mdl, { 1,1,1,1 });

		if (input_->TriggerKey(DIK_Y))
		{
			spriteCheckY_->Draw(mdl, { 1,1,1,1 });
		}

		if (input_->TriggerKey(DIK_N))
		{
			spriteCheckN_->Draw(mdl, { 1,1,1,1 });
		}

	}
	else
	{
		//ポーズ画面項目すべて表示
		spriteUi_->Draw(mdl, { 1,1,1,1 });
		spriteBack_->Draw(sp, { 1,1,1,1 });
		spriteReset_->Draw(sp2, { 1,1,1,1 });
		spriteTitle_->Draw(sp3, { 1,1,1,1 });
		//spriteSetting_->Draw(sp4, { 1,1,1,1 });
		spriteExit_->Draw(sp4, { 1,1,1,1 });
	}
	



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

	//CHECK
	loserCheck_ = TextureManager::Load("UI/Check01.png");
	spriteCheck_ = Sprite::Create(loserCheck_);

	loserCheckY_ = TextureManager::Load("UI/Check02.png");
	spriteCheckY_ = Sprite::Create(loserCheckY_);

	loserCheckN_ = TextureManager::Load("UI/Check03.png");
	spriteCheckN_ = Sprite::Create(loserCheckN_);

}

void PouseUi::Reset()//ポーズのリセット

{
	printXY = { spriteSizeX,spriteSizeY };//画像最初地点
	printXY2 = { spriteSizeX,spriteSizeY * 2 + size };
	printXY3 = { spriteSizeX,spriteSizeY * 3 + size * 2 };
	printXY4 = { spriteSizeX,spriteSizeY * 4 + size * 3 };
	printXY5 = { spriteSizeX,spriteSizeY * 5 + size * 4 };

	sp = { spriteSizeX,spriteSizeY };
	sp2 = { spriteSizeX,spriteSizeY };
	sp3 = { spriteSizeX,spriteSizeY };
	sp4 = { spriteSizeX,spriteSizeY };
	sp5 = { spriteSizeX,spriteSizeY };

	easingXTimer = 0;

}

void PouseUi::Back()
{
	isPouse = FALSE;
}

void PouseUi::GameReset()
{
	gameReset = TRUE;
}

void PouseUi::Title()
{
	isTitle = TRUE;
	//scene = Scene::Title;
}

void PouseUi::Setting()
{

}

void PouseUi::Exit()
{
	isCheck = TRUE;
	//YES
	if (input_->TriggerKey(DIK_Y))
	{
		isEnd = TRUE;
		OK = FALSE;
		isCheck = FALSE;
		isPouse = FALSE;
		
	}
	//NO
	if (input_->TriggerKey(DIK_N))
	{
		isCheck = FALSE;
		OK = FALSE;
		
	}

	

}

void PouseUi::EasingReset()
{
	printXY = { spriteSizeX,spriteSizeY };//画像最初地点
	printXY2 = { spriteSizeX,spriteSizeY * 2 + size };
	printXY3 = { spriteSizeX,spriteSizeY * 3 + size * 2 };
	printXY4 = { spriteSizeX,spriteSizeY * 4 + size * 3 };
	printXY5 = { spriteSizeX,spriteSizeY * 5 + size * 4 };

	sp = { spriteSizeX,spriteSizeY };
	sp2 = { spriteSizeX,spriteSizeY };
	sp3 = { spriteSizeX,spriteSizeY };
	sp4 = { spriteSizeX,spriteSizeY };
	sp5 = { spriteSizeX,spriteSizeY };

	easingXTimer = 0;
	easingYTimer = 0;
	easingOkX = false;
}

