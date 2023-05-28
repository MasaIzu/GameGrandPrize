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

	printXY = { spriteSizeX,spriteSizeY };//�摜�ŏ��n�_
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
		
		//�|�[�Y��ʌ��肳�����Ƃ��̏���
		Process();
	}
	else
	{
		EasingReset();
	}
	
}

void PouseUi::DrawEasing()
{//                     �ŏ��̂Ƃ���@�@�@�@�@�@�@�@�@�@�~�߂����Ƃ���@�@�^�C�}�[�@�@�@�ő�^�C�}�[
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

	//���I��������
	if (input_->TriggerKey(DIK_DOWN))
	{
		//���ڍő�l�ɍs������ő�Ŏ~�߂�
		if (select >= MAX)
		{
			select = MAX;
		}
		else
		{
			select += 1;
		}
	}
	//��ɑI��������
	if (input_->TriggerKey(DIK_UP))
	{
		//���ڍŏ��l�ɍs������ŏ��Ŏ~�߂�
		if (select <= MIN)
		{
			select = MIN;
		}
		else
		{
			select -= 1;
		}
	}

	//�I�𒆂̈ړ����X���[�W���O���鏈��


	//���肳����
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
		case 1://�Q�[���֖߂�0
			Back();
			OK = FALSE;
			break;

		case 2://���Z�b�g1
			GameReset();
			OK = FALSE;
			break;

		case 3://�ݒ�2
			Setting();

			break;

		case 4://�^�C�g����3
			Title();
			Reset();
			OK = FALSE;
			break;

		case 5://�Q�[���I��4
			Exit();
			Reset();
			OK = FALSE;
			break;
		}
	}

}

void PouseUi::Draw()
{
	//�|�[�Y��ʍ��ڂ��ׂĕ\��
	//spriteUi_->Draw({spriteSizeX,spriteSizeY},{1,1,1,1});
	spriteBack_->Draw(sp, { 1,1,1,1 });
	spriteReset_->Draw(sp2, { 1,1,1,1 });
	spriteTitle_->Draw(sp3, { 1,1,1,1 });
	spriteSetting_->Draw(sp4, { 1,1,1,1 });
	spriteExit_->Draw(sp5, { 1,1,1,1 });

	
	



	//�I�𒆂̂��̂�\��
	spriteSelect_->Draw({ spriteSizeX,spriteSizeY * select + size * (select-1) }, { 1,1,1,1 });

}

void PouseUi::Load()
{
	//UI�w�i
	loserUi_ = TextureManager::Load("UI/UIPouse.png");
	spriteUi_ = Sprite::Create(loserUi_);
	//�߂�
	loserBack_ = TextureManager::Load("UI/Back.png");
	spriteBack_ = Sprite::Create(loserBack_);

	//���Z�b�g
	loserReset_ = TextureManager::Load("UI/Restart.png");
	spriteReset_ = Sprite::Create(loserReset_);

	//�^�C�g��
	loserTitle_ = TextureManager::Load("UI/Title.png");
	spriteTitle_ = Sprite::Create(loserTitle_);

	//�ݒ�
	loserSetting_ = TextureManager::Load("UI/Setting.png");
	spriteSetting_ = Sprite::Create(loserSetting_);

	//�I���
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
	scene = Scene::Title;
}

void PouseUi::Setting()
{

}

void PouseUi::Exit()
{
	
}

void PouseUi::EasingReset()
{
	printXY = { spriteSizeX,spriteSizeY };//�摜�ŏ��n�_
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

