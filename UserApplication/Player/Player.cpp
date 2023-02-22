#include "Player.h"
#include "WinApp.h"
#include"MyMath.h"


Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(Model* model,float WindowWidth,float WindowHeight) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	playerModel_.reset(model);


	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();

	Window_Width = WindowWidth;
	Window_Height = WindowHeight;

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	


	worldTransform_.TransferMatrix();
}

void Player::Move() {

	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.z += 0.1;
	}
	if (input_->PushKey(DIK_A)) {
		worldTransform_.translation_.x += 0.1;
	}
	if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.z -= 0.1;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x -= 0.1;
	}

}


void Player::Update(const ViewProjection& viewProjection) {

	Move();

	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection viewProjection_) {

	playerModel_->Draw(worldTransform_, viewProjection_);
	
}




Vector3 Player::bVelocity(Vector3 velocity, WorldTransform& worldTransform) {

	Vector3 result = { 0, 0, 0 };


	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
		velocity.y * worldTransform.matWorld_.m[1][0] +
		velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
		velocity.y * worldTransform.matWorld_.m[1][1] +
		velocity.z * worldTransform.matWorld_.m[2][1];

	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
		velocity.y * worldTransform.matWorld_.m[1][2] +
		velocity.z * worldTransform.matWorld_.m[2][2];

	return result;
}

Vector3 Player::GetWorldPosition() {

	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
