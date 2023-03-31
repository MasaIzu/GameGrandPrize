#pragma once

#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <wrl.h>
#include "Vector3.h"

/// <summary>
/// マテリアル
/// </summary>
class Material {
private: // エイリアス
	// Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferData {
		XMFLOAT3 ambient;  // アンビエント係数
		float pad1;        // パディング
		XMFLOAT3 diffuse;  // ディフューズ係数
		float pad2;        // パディング
		XMFLOAT3 specular; // スペキュラー係数
	};

public: // 静的メンバ関数
	/// <summary>
	/// マテリアル生成
	/// </summary>
	/// <returns>生成されたマテリアル</returns>
	static Material* Create();

public:
	std::string name_;            // マテリアル名
	XMFLOAT3 ambient_;            // アンビエント影響度
	XMFLOAT3 diffuse_;            // ディフューズ影響度
	XMFLOAT3 specular_;           // スペキュラー影響度
	float alpha_;                 // アルファ
	std::string textureFilename_; // テクスチャファイル名

public:
	/// <summary>
	/// 定数バッファの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetConstantBuffer() { return constBuff_.Get(); }

	/// テクスチャ読み込み
	/// </summary>
	/// <param name="directoryPath">読み込みディレクトリパス</param>
	void LoadTexture(const std::string& directoryPath);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// グラフィックスコマンドのセット
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rooParameterIndexMaterial">マテリアルのルートパラメータ番号</param>
	/// <param name="rooParameterIndexTexture">テクスチャのルートパラメータ番号</param>
	void SetGraphicsCommand(
		ID3D12GraphicsCommandList* commandList, UINT rooParameterIndexMaterial,
		UINT rooParameterIndexTexture);

	/// <summary>
	/// グラフィックスコマンドのセット（テクスチャ差し替え版）
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rooParameterIndexMaterial">マテリアルのルートパラメータ番号</param>
	/// <param name="rooParameterIndexTexture">テクスチャのルートパラメータ番号</param>
	/// <param name="textureHandle">差し替えるテクスチャハンドル</param>
	void SetGraphicsCommand(
		ID3D12GraphicsCommandList* commandList, UINT rooParameterIndexMaterial,
		UINT rooParameterIndexTexture, uint32_t textureHandle);

	// セットテクスチャハンドル
	void SetTextureHadle(uint32_t& textureHandles) { textureHandle_ = textureHandles; }

	//// テクスチャハンドル
	//uint32_t GetTextureHadle(size_t index) { return textureHandle_[index]; }

	void SetLight(float alpha);

	void SetLight(Vector3 ambient, Vector3 diffuse, Vector3 specular, float alpha);

private:
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff_;
	// 定数バッファのマップ
	ConstBufferData* constMap_;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

private:
	// コンストラクタ
	Material() {
		ambient_ = { 0.3f, 0.3f, 0.3f };
		diffuse_ = { 0.0f, 0.0f, 0.0f };
		specular_ = { 0.0f, 0.0f, 0.0f };
		alpha_ = 1.0f;
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファの生成
	/// </summary>
	void CreateConstantBuffer();
};
