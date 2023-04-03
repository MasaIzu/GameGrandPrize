#pragma once
#include<Windows.h>
#include <cstdint>
#include <string>

class WinApp
{
public:
	// �E�B���h�E�T�C�Y
	static const int window_width = 1280; // ����
	static const int window_height = 720; // �c��

public: // �ÓI�����o�֐�

	/// <summary>
	/// �E�B���h�E�v���V�[�W��
	/// </summary>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public://�����o�֐�

	WinApp();
	~WinApp();

	void MakeWindow(std::wstring title);
	bool ProcessMessage();
	void DeleteGameWindow();

	void Destroy();

	HINSTANCE GetHInstance() const { return w.hInstance; }


public://GetterSetter

	HWND Gethwnd() { return hwnd; }

	int GetWindowWidth() { return window_width; }
	int GetWindowHeight() { return window_height; }


private://�����o�ϐ�
	

	WNDCLASSEX w{};
	HWND hwnd;

private:


};

