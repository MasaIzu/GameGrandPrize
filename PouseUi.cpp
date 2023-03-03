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

void PouseUi::Update()
{
	//���I��������
	if (input->TriggerKey(DIK_DOWN))
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
	if (input->TriggerKey(DIK_UP))
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
	if (input->TriggerKey(DIK_RETURN))
	{
		ok = select;
		OK = TRUE;
	}
	//�|�[�Y��ʌ��肳�����Ƃ��̏���
	if (OK)
	{
		switch (ok)
		{
		case 1://�Q�[���֖߂�0
			OK = FALSE;
			break;

		case 2://���Z�b�g1
			OK = FALSE;
			Reset();
			break;

		case 3://�ݒ�2
			OK = FALSE;
			Reset();
			break;

		case 4://�^�C�g����3
			OK = FALSE;
			Reset();
			break;

		case 5://�Q�[���I��4
			OK = FALSE;
			Reset();
			break;
		} 
	}

	//Draw();
	
}

void PouseUi::Draw()
{
	//�|�[�Y��ʍ��ڂ��ׂĕ\��
	spriteUi_->Draw({640,360},{1,1,1,1});


	//�I�𒆂̂��̂�\��


}

void PouseUi::Reset()
{
}
