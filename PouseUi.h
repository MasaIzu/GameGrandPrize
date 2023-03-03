#pragma once
#include "Input.h"
#include "Sprite.h"
#include "TextureManager.h"


class PouseUi
{
public:
	//�R���X�g���N�^
	PouseUi();
	//�f�X�g���N�^
	~PouseUi();

	//������
	void Initialize();
	//����
	void Update();
	//�`��
	void Draw();
	//���Z�b�g�֐�
	void Reset();

	void Destroy();

	//static PouseUi* GetInstance();


private:

	//static PouseUi* PouseUi_;

	Input* input = nullptr;

	const int MIN = 0;
	const int MAX = 4;
	const int SMAX = 3;
	int select = MIN;
	//���肳���������
	int ok = MIN;
	
	//�t���O
	bool OK = false;

	//�e�N�X�`��
	//�|�[�Y��ʂ̔w�i
	std::unique_ptr<Sprite> spriteUi_ = nullptr;
	uint32_t loserUi_ = 0;
	//�߂�
	std::unique_ptr<Sprite> spriteBack_ = nullptr;
	uint32_t loserBack_ = 0;
	//���Z�b�g
	std::unique_ptr<Sprite> spriteReset_ = nullptr;
	uint32_t loserReset_ = 0;
	//�^�C�g��
	std::unique_ptr<Sprite> spriteTitle_ = nullptr;
	uint32_t loserTitle_ = 0;
	//�ݒ�
	std::unique_ptr<Sprite> spriteSetting_ = nullptr;
	uint32_t loserSetting_ = 0;
	//�I���
	std::unique_ptr<Sprite> spriteExit_ = nullptr;
	uint32_t loserExit_ = 0;
	//�Z���N�g
	std::unique_ptr<Sprite> spriteSelect_ = nullptr;
	uint32_t loserSelect_ = 0;
};

