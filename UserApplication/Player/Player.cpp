#include "Player.h"
#include "WinApp.h"
#include"MyMath.h"
#include "CollisionManager.h"
#include <CollisionAttribute.h>


Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(Model* model, float WindowWidth, float WindowHeight) {
	//NULLポインタチェック
	assert(model);
	playerModel_ = model;
	oldPlayerModel_.reset(Model::CreateFromOBJ("UFO", true));

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();

	Window_Width = WindowWidth;
	Window_Height = WindowHeight;

	// コリジョンマネージャに追加
	collider = new SphereCollider(Vector4(0, radius, 0, 0), radius);
	CollisionManager::GetInstance()->AddCollider(collider);

	playerAvoidance = 6.0f;

	//ワールド変換の初期化
	worldTransform_.Initialize();
	oldWorldTransform_.Initialize();

	collider->Update(worldTransform_.matWorld_);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);



	worldTransform_.TransferMatrix();
	oldWorldTransform_.TransferMatrix();
}

void Player::Move() {

	Vector3 playerMovement = { 0,0,0 };
	Avoidance = { 0,0,0 };
	isPushLeft = false;
	isPushRight = false;
	isPushBack = false;
	spaceInput = false;

	if (timer > 0) {
		timer--;
	}

	if (input_->PushKey(DIK_W)) {
		playerMovement.z = -playerSpeed;
	}
	if (input_->PushKey(DIK_A)) {
		playerMovement.x = playerSpeed;
		isPushLeft = true;
	}
	if (input_->PushKey(DIK_S)) {
		playerMovement.z = playerSpeed;
		isPushBack = true;
	}
	if (input_->PushKey(DIK_D)) {
		playerMovement.x = -playerSpeed;
		isPushRight = true;
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		spaceInput = true;
		timer = 8;
		oldWorldTransform_.translation_ = worldTransform_.translation_;

		if (isPushLeft == true) {
			Avoidance.x = playerAvoidance;
		}
		else if (isPushRight == true) {
			Avoidance.x = -playerAvoidance;
		}
		else if (isPushBack == true) {
			Avoidance.z = playerAvoidance;
		}
		else {
			Avoidance.z = -playerAvoidance;
		}
	}



	CameraRot = MyMath::Rotation(Vector3(Rot.x, Rot.y, Rot.z), 6);

	playerMovement = MyMath::MatVector(CameraRot, playerMovement);
	playerMovement.y = 0;
	playerMovement.normalize();
	playerMovement *= playerSpeed;

	Avoidance = MyMath::MatVector(CameraRot, Avoidance);
	Avoidance.y = 0;
	Avoidance.normalize();
	Avoidance *= playerAvoidance;

	worldTransform_.translation_ += playerMovement;
	worldTransform_.translation_ += Avoidance;

	worldTransform_.matWorld_ *= CameraRot;
}


void Player::Update(const ViewProjection& viewProjection) {

	Move();

	if (input_->MouseInputTrigger(0)) {
		Attack(Vector3(0,0,0),Vector3(5,5,5));
	}

	worldTransform_.TransferMatrix();
	oldWorldTransform_.TransferMatrix();

	collider->Update(worldTransform_.matWorld_);
}

void Player::Draw(ViewProjection viewProjection_) {

	if (timer == 0) {
		playerModel_->Draw(worldTransform_, viewProjection_);
	}
	if (timer > 0) {
		oldPlayerModel_->SetAlpha(0.3f);
		oldPlayerModel_->Draw(oldWorldTransform_, viewProjection_);
	}

}

void Player::Attack(Vector3 start, Vector3 Finish) {

	if (makeColliders == false) {
		float sphereX = Finish.x - start.x;
		float sphereY = Finish.y - start.y;
		float sphereZ = Finish.z - start.z;

		Vector3 sphere(sphereX / SphereCount, sphereY / SphereCount, sphereZ / SphereCount);


		for (int i = 0; i < SphereCount; i++) {
			// コリジョンマネージャに追加
			AttackCollider[i] = new SphereCollider(Vector4(0, radius, 0, 0), radius);
			CollisionManager::GetInstance()->AddCollider(AttackCollider[i]);

			colliderPos[i] = start + sphere * i;

			worldSpherePos[i] = MyMath::Translation(colliderPos[i]);

			AttackCollider[i]->Update(worldSpherePos[i]);
			AttackCollider[i]->SetAttribute(COLLISION_ATTR_ALLIES);
		}
	}
	else {
		for (int i = 0; i < SphereCount; i++) {
			
			AttackCollider[i]->Update(worldSpherePos[i]);
		}
	}

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

	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}