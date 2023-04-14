#pragma once
#include "Sprite.h"
#include"PostEffectCommon.h"
class PostEffectBlurH
{
public:

    static void Initialize();
    
    /// <summary>
    /// パイプライン生成
    /// </summary>
    static void CreatGraphicsPipelineState();

    /// <summary>
    /// シーン描画前処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    static void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    static void Draw();

    /// <summary>
    /// シーン描画後処理
    /// </summary>
    /// <param name="cmdList">コマンド処理</param>
    static void PostDrawScene();

private://静的メンバ変数
    static const float clearColor[4];

private:

    static ID3D12GraphicsCommandList* commandList;

    static VertexPosUv vertices[4];

    static VertexPosUv* vertMap;

    static Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;	//頂点バッファ

    //頂点バッファビューの作成
    static D3D12_VERTEX_BUFFER_VIEW vbView;

    static Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
    //深度バッファ
    static Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    //RTV用のデスクリプタヒープ
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV用のデスクリプタヒープ
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

    static uint32_t textureHandle;
    
    static PostEffectCommon* PECommon;
};

