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

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataWorldTransform) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

}

void WorldTransform::Map() {

	//定数バッファのマッピング
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

}

void WorldTransform::TransferMatrix() {

	Matrix4 matScale, matRot, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale = MyMath::Scale(scale_);
	matRot = MyMath::Initialize();
	//matRot *= MyMath::Rotation(rotation_, 6);
	matRot = quaternion.Rotate();
	matTrans = MyMath::Translation(translation_);

	//ワールド行列の合成
	matWorld_ = MyMath::Initialize();//変形をリセット
	matWorld_ *= matScale;//ワールド行列にスケーリングを反映
	matWorld_ *= matRot;//ワールド行列に回転を反映
	matWorld_ *= matTrans;//ワールド行列に平行移動を反映

	//親オブジェクトがあれば
	if (parent_) {
		//親オブジェクトのワールド行列を掛ける
		matWorld_ *= parent_->matWorld_;
	}
	//定数バッファへのデータ転送
	constMap->matWorld = matWorld_;

}


void WorldTransform::SetRot(const Vector3& rot)
{
	rotation_ = rot;

	quaternion.SeTEuler(rotation_);
}

void WorldTransform::MoveRot(const Vector3& move)
{
	rotation_ += move;

	quaternion.SeTEuler(rotation_);
}

void WorldTransform::SetQuater(const Quaternion& quater)
{
	quaternion = quater;

	rotation_ = quaternion.GetEulerAngles();
}

void WorldTransform::MoveQuater(const Quaternion& move)
{
	quaternion += move;

	rotation_ = quaternion.GetEulerAngles();
}

Quaternion& WorldTransform::GetQuaternion()
{
	return quaternion;
}

