#pragma once
#include "Input.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Easing.h"
enum class Scene {
	Title,
	Game,
	GameOver,
	Result
};

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
	//�摜�̃C�[�W���O����
	void DrawEasing();
	//�`��
	void Draw();

	void Load();

	void Select();

	void Process();

	//���Z�b�g�֐�
	void Reset();
	

	void Destroy();

	void Back();

	void GameReset();

	void Title();
	
	void Setting();

	void Exit();

	bool GetisPouse() { return isPouse; }

	//�Q�[���V�[���ڍs�v
	Scene scene = Scene::Title;

	Scene oldScene = Scene::Title;


private:

	//static PouseUi* PouseUi_;
	//�V�[���I��p
	Input* input_ = nullptr;

	const int MIN = 1;
	const int MAX = 5;
	const int SMAX = 3;
	int select = MIN;
	//�摜�̍ŏI���W�ʒu
	Vector2 printXY = { spriteSizeX,spriteSizeY };//�摜�ŏ��n�_
	Vector2 printXY2 = { spriteSizeX,spriteSizeY * 2 + size };
	Vector2 printXY3 = { spriteSizeX,spriteSizeY * 3 + size * 2 };
	Vector2 printXY4 = { spriteSizeX,spriteSizeY * 4 + size * 3 };
	Vector2 printXY5 = { spriteSizeX,spriteSizeY * 5 + size * 4 };

	//�X�v���C�g�̍��W�����锠
	Vector2 sp;
	Vector2 sp2;
	Vector2 sp3;
	Vector2 sp4;
	Vector2 sp5;
	//���肳���������
	int decided = MIN;
	
	//�t���O
	bool OK = false;
	bool isPouse = false;
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

	float spriteSizeX = 640.0f;
	float spriteSizeY = 64.0f;

	float size = 32.0f;
};

