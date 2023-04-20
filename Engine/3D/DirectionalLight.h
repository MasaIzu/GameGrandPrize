#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>
#include"Vector3.h"

class DirectionalLight {

private://�G�C���A�X
		//Microsoft::WRL::���ȗ�
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public://�T�u�N���X�@
	//�萔�o�b�t�@�p�f�[�^�\��
	struct ConstBufferData
	{
		Vector3 lightv; //���C�g�ւ̕�����\���x�N�g��
		float pad=0;//�萔�o�b�t�@�p�̃���������
		Vector3 lightcolor; //���C�g�̐F
		unsigned int active;
	};

	HRESULT result;

public://�ÓI���C�o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �萔�o�b�t�@�]��
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// ���C�g�������Z�b�g
	/// </summary>
	void SetLightDir(const Vector3& lightdir);

	/// <summary>
	/// ���C�g�F���Z�b�g
	/// </summary>
	void SetLightColor(const Vector3& lightcolor);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	/// <summary>
	/// �L���t���O���Z�b�g
	/// </summary>
	/// <param name="active">�L���t���O</param>
	inline void SetActive(bool active) { this->active = active; }

	
	/// <summary>
	/// �L���`�F�b�N
	/// </summary>
	/// <returns>�L���t���O</returns>
	inline bool IsActive() { return active; }

	Vector3 GetLightDir() { return lightdir; }
	Vector3 GetLightColor() { return lightcolor; }

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	static DirectionalLight* Create();
private://�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//���C�g��������
	Vector3 lightdir = { 1,0,0 };
	//���C�g�F
	Vector3 lightcolor = { 1,1,1, };
	//�_�[�e�B�t���O
	bool dirty = false;

	//�L���t���O
	bool active = false;
};

