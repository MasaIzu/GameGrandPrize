#pragma once
#include<Windows.h>
#include <cstdint>
#include <string>

class WinApp
{
public:
	// ウィンドウサイズ
	static const int window_width = 1280; // 横幅
	static const int window_height = 720; // 縦幅

public: // 静的メンバ関数

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public://メンバ関数

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


private://メンバ変数
	

	WNDCLASSEX w{};
	HWND hwnd;

private:


};

