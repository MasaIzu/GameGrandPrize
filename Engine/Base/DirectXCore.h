#pragma once

#include <Windows.h>
#include <cstdlib>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>


class DirectXCore {

public://メンバ関数

	// シングルトンインスタンスの取得
	static DirectXCore* GetInstance();

	void DirectXCoreInitialize(HWND hwnd, int window_width, int window_height);


	/// <summary>
	/// 描画前後処理
	/// </summary>
	void PreDraw();
	void PostDraw();

	// レンダーターゲットのクリア
	void ClearRenderTarget();

	// 深度バッファのクリア
	void ClearDepthBuffer();

	void Destroy();

	// デバイスの取得
	ID3D12Device* GetDevice() { return device.Get(); }

	// 描画コマンドリストの取得
	ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }

	//バックバッファの数を取得
	size_t GetBackBufferCount()const { return backBuffers.size(); }

private://メンバ変数

	// Direct3D関連
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;

	int backBufferWidth_ = 0;
	int backBufferHeight_ = 0;

	//フェンスの生成
	Microsoft::WRL::ComPtr < ID3D12Fence> fence;
	UINT64 fenceVal = 0;

	HWND hwnd_;

	static DirectXCore* DirectXCore_;

private://メンバ変数
	DirectXCore() = default;
	~DirectXCore() = default;
	DirectXCore(const DirectXCore&) = delete;
	const DirectXCore& operator=(const DirectXCore&) = delete;

	// DXGIデバイス初期化
	void InitializeDevice();

	// スワップチェーンの生成
	void InitializeSwapChain();

	// コマンド関連初期化
	void InitializeCommand();

	// レンダーターゲット生成
	void InitializeRenderTargets();

	// 深度バッファ生成
	void InitializeDepthBuffer();

	// フェンス生成
	void InitializeFence();

};