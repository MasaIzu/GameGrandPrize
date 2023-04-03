#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <array>
#include <vector>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "WinApp.h"
#include "Mouse.h"



// ����
class Input
{

public: // �����o�֐�

	Input();
	~Input();
	

	// ������
	void Initialize(WinApp* winApp_);

	// �X�V
	void Update();

	// �L�[�̉����������`�F�b�N(������)
	bool PushKey(BYTE keyNumber);

	// �L�[�̃g���K�[���`�F�b�N(�������u��)
	bool TriggerKey(BYTE keyNumber);

	// �L�[�̃g���K�[���`�F�b�N(�������u��)
	bool ReleasedKey(BYTE keyNumber);

	// �}�E�X�{�^���̃g���K�[����
	bool MouseInputTrigger(int button);


	// �}�E�X�{�^���̓���
	bool MouseInputing(int button);


	// �}�E�X�{�^���̗������u��
	bool MouseOffTrigger(int button);

	// �}�E�X�̈ʒu
	const Vector2 GetMousePos()const;

	// �}�E�X�̈ʒu
	const Vector3 GetMouseMove();

	void Destroy();

private:

private:// �ÓI�����o�ϐ�

private:
	// DirectInput�̃C���X�^���X
	Microsoft::WRL::ComPtr<IDirectInput8> dInput_ = nullptr;

	// �L�[�{�[�h�̃f�o�C�X
	Microsoft::WRL::ComPtr<IDirectInputDevice8> devKeyboard_ = nullptr;

	// �S�L�[�̏��
	BYTE key[256] = {};

	// �O��̑S�L�[�̏��
	BYTE keyPre[256] = {};

	// WindowsAPI
	WinApp* winApp_ = nullptr;

	HWND hwnd_;

	//�}�E�X
	std::unique_ptr<Mouse> mouse;
};
