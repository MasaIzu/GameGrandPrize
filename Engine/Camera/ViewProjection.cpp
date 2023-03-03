#include "DirectXCore.h"
#include "ViewProjection.h"
#include "WinApp.h"
#include <cassert>
#include <d3dx12.h>
#include "MyMath.h"
#include<DirectXMath.h>

void ViewProjection::Initialize() {
	CreateConstBuffer();
	Map();
	UpdateMatrix();
}

void ViewProjection::CreateConstBuffer() {
	HRESULT result;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataViewProjection) + 0xff) & ~0xff);

	// 定数バッファの生成
	result = DirectXCore::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));
}

void ViewProjection::Map() {
	// 定数バッファとのデータリンク
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
}

void ViewProjection::UpdateMatrix() {

	// ビュー行列の生成
	Matrix4 tmp = MyMath::LookAtLH(eye, target, up);
	matView = MyMath::MatrixInverse(tmp);

	// 透視投影による射影行列の生成
	matProjection = MyMath::PerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);

	// 定数バッファに書き込み
	constMap->view = matView;
	constMap->projection = matProjection;
	constMap->cameraPos = eye;

	//視点座標
	Vector3 eyePosition = eye;
	//注視点座標X
	Vector3 targetPosition = target;
	//(仮の)上方向
	Vector3 upVector = up;

	//カメラZ軸(視点方向)
	Vector3 cameraAxisZ = targetPosition - eyePosition;

	//ベクトルを正規化
	cameraAxisZ.normalize();

	//カメラのX軸(右方向)
	Vector3 cameraAxisX;
	//X軸は上方向→Z軸の外積で求まる
	cameraAxisX = upVector.cross(cameraAxisZ);
	//ベクトルを正規化
	cameraAxisX.normalize();

	//カメラのY軸(上方向)
	Vector3 cameraAxisY;
	//Y軸は上方向→Z軸の外積で求まる
	cameraAxisY = cameraAxisZ.cross(cameraAxisX);
	//ベクトルを正規化
	cameraAxisY.normalize();

	//視点座標に-1を掛けた座標
	Vector3 reverseEyePosition = eyePosition * -1;
	//カメラの位置からワールド原点へのベクトル(カメラ座標系)
	float tX = cameraAxisX.dot(reverseEyePosition);
	float tY = cameraAxisY.dot(reverseEyePosition);
	float tZ = cameraAxisZ.dot(reverseEyePosition);
	//一つのベクトルにまとめる
	Vector3 translation = { tX, tY, tZ };

#pragma region 全方向ビルボード行列の計算
	//ビルボード行列
	matBillboard.m[0][0] = cameraAxisX.x;
	matBillboard.m[0][1] = cameraAxisX.y;
	matBillboard.m[0][2] = cameraAxisX.z;
	matBillboard.m[0][3] = 0;
	matBillboard.m[1][0] = cameraAxisY.x;
	matBillboard.m[1][1] = cameraAxisY.y;
	matBillboard.m[1][2] = cameraAxisY.z;
	matBillboard.m[1][3] = 0;
	matBillboard.m[2][0] = cameraAxisZ.x;
	matBillboard.m[2][1] = cameraAxisZ.y;
	matBillboard.m[2][2] = cameraAxisZ.z;
	matBillboard.m[2][3] = 0;
	matBillboard.m[3][0] = 0;
	matBillboard.m[3][1] = 0;
	matBillboard.m[3][2] = 0;
	matBillboard.m[3][3] = 1;
}
