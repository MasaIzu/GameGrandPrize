#pragma once
#include "Sprite.h"
#include"PostEffectCommon.h"
class PostEffectBlurH
{
public:

    static void Initialize();
    
    /// <summary>
    /// �p�C�v���C������
    /// </summary>
    static void CreatGraphicsPipelineState();

    /// <summary>
    /// �V�[���`��O����
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    static void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    static void Draw();

    /// <summary>
    /// �V�[���`��㏈��
    /// </summary>
    /// <param name="cmdList">�R�}���h����</param>
    static void PostDrawScene();

private://�ÓI�����o�ϐ�
    static const float clearColor[4];

private:

    static ID3D12GraphicsCommandList* commandList;

    static VertexPosUv vertices[4];

    static VertexPosUv* vertMap;

    static Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;	//���_�o�b�t�@

    //���_�o�b�t�@�r���[�̍쐬
    static D3D12_VERTEX_BUFFER_VIEW vbView;

    static Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
    //�[�x�o�b�t�@
    static Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    //RTV�p�̃f�X�N���v�^�q�[�v
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV�p�̃f�X�N���v�^�q�[�v
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
    static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

    static uint32_t textureHandle;
    
    static PostEffectCommon* PECommon;
};

