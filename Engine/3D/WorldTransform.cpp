#include "WorldTransform.h"
#include "DirectXCore.h"
#include <cassert>
#include <Quaternion.h>

void WorldTransform::Initialize() {
	CreateConstBuffer();
	Map();
	TransferMatrix();
}

void WorldTransform::CreateConstBuffer() {

	HRESULT result;

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataWorldTransform) + 0xff) & ~0xff);

	// �萔�o�b�t�@�̐���
	result = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

}

void WorldTransform::Map() {

	//�萔�o�b�t�@�̃}�b�s���O
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

}

void WorldTransform::TransferMatrix() {

	Matrix4 matScale, matRot, matTrans;
	Quaternion QuaternionMatRot = { rotation_.x,rotation_.y,rotation_.z,0 };

	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = MyMath::Scale(scale_);
	matRot = MyMath::Initialize();
	matRot *= QuaternionMatRot.Rotate();
	matTrans = MyMath::Translation(translation_);

	//���[���h�s��̍���
	matWorld_ = MyMath::Initialize();//�ό`�����Z�b�g
	matWorld_ *= matScale;//���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld_ *= matRot;//���[���h�s��ɉ�]�𔽉f
	matWorld_ *= matTrans;//���[���h�s��ɕ��s�ړ��𔽉f

	//�e�I�u�W�F�N�g�������
	if (parent_) {
		//�e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld_ *= parent_->matWorld_;
	}
	//�萔�o�b�t�@�ւ̃f�[�^�]��
	constMap->matWorld = matWorld_;

}