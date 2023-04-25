#pragma once
#include "Sprite.h"
#include"DirectXCore.h"
struct VertexPosUv {
    Vector3 pos; // xyzç¿ïW
    Vector2 uv;  // uvç¿ïW
};
class PostEffectCommon
{
public:

    static void StaticInitialize(DirectXCore* dxCommon);

    static uint32_t CreateDescHeapSRV(ID3D12Resource* texBuff);

    static ID3D12Device* device;

    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;

    static uint32_t texIndex;
};

