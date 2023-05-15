#include "Skydome.h"

void Skydome::Initialize(Model* model) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.scale_ = Vector3(800, 800, 800);
	worldTransform_.SetRot({ 0,1,0 });
	worldTransform_.matWorld_ *= AffinTrans::Scale(worldTransform_.scale_);
	worldTransform_.translation_.y = 50;
	//行列の転送
	worldTransform_.TransferMatrix();
}

void Skydome::Update() {

}

void Skydome::Draw(ViewProjection viewProjection_) {
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_);
}

void Skydome::SetModel(Model* model)
{
	model_ = model;
}
