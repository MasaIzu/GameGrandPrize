#include "Ground.h"

using namespace DirectX;

Ground::Ground()
{
}

Ground::~Ground()
{
	delete objGround;
	delete modelGround;
}

void Ground::Initialize() {
	////NULL�|�C���^�`�F�b�N
	//assert(model);
	//model_.reset(model);
	////���[���h�ϊ��̏�����
	//worldTransform_.Initialize();

	//worldTransform_.translation_ = Vector3(0, -20.0f, 0);
	//worldTransform_.scale_ = Vector3(600, 100, 600);
	//worldTransform_.matWorld_ *= AffinTrans::Translation(worldTransform_.translation_);
	//worldTransform_.matWorld_ *= AffinTrans::Scale(worldTransform_.scale_);
	////�s��̓]��
	//worldTransform_.TransferMatrix();


}

void Ground::Update() {

}

void Ground::Draw(ViewProjection viewProjection_) {
	//3D���f���`��
	model_->Draw(worldTransform_, viewProjection_);
}