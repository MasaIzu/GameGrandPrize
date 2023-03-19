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
	delete easing_;
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

	easing_ = new Easing();

	// コリジョンマネージャに追加
	collider = new SphereCollider(Vector4(0, radius, 0, 0), radius);
	CollisionManager::GetInstance()->AddCollider(collider);

	playerAvoidance = 6.0f;

	for (int i = 0; i < SphereCount; i++) {
		// コリジョンマネージャに追加
		AttackCollider[i] = new SphereCollider(Vector4(0, radius, 0, 0), radius);
		CollisionManager::GetInstance()->AddCollider(AttackCollider[i]);
	}

	//worldTransform_.translation_ = { 0,0,-100 };

	//ワールド変換の初期化
	worldTransform_.Initialize();
	oldWorldTransform_.Initialize();
	playerAttackTransform_.Initialize();

	collider->Update(worldTransform_.matWorld_);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	for (int i = 0; i < SphereCount; i++) {
		playerAttackTransformaaaa_[i].Initialize();
		playerAttackTransformaaaa_[i].TransferMatrix();
	}

	worldTransform_.TransferMatrix();
	oldWorldTransform_.TransferMatrix();
	playerAttackTransform_.TransferMatrix();
}


void Player::Update(const ViewProjection& viewProjection) {

	Move();

	Attack();

	if (isHit) {
		SetKnockBackCount();
	}

	KnockBackUpdata();

	worldTransform_.TransferMatrix();
	oldWorldTransform_.TransferMatrix();

}

void Player::Move() {

	PlayerMoveMent = { 0,0,0 };
	Avoidance = { 0,0,0 };
	isPushLeft = false;
	isPushRight = false;
	isPushBack = false;
	spaceInput = false;

	if (timer > 0) {
		timer--;
		alpha -= 0.02f;
	}
	else {
		spaceInput = false;
		collider->SetAttribute(COLLISION_ATTR_ALLIES);
	}

	cameraLookmat = MyMath::Rotation(Vector3(0, 90 * (MyMath::PI / 180), 0), 2);

	Vector3 moveRot = cameraLook;


	}
	if (input_->PushKey(DIK_A)) {
		moveRot = MyMath::MatVector(cameraLookmat, moveRot);
		moveRot.y = 0;
		moveRot.norm();

		isPushBack = true;
	}
	if (input_->PushKey(DIK_D)) {
		moveRot = MyMath::MatVector(cameraLookmat, moveRot);
		moveRot.y = 0;
		moveRot.norm();

		isPushRight = true;
	}

	worldTransform_.translation_ += PlayerMoveMent;

	if (spaceInput == false) {
		if (input_->TriggerKey(DIK_SPACE)) {
			spaceInput = true;
			timer = 20;
			alpha = 0.3f;
			collider->SetAttribute(COLLISION_ATTR_INVINCIBLE);
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
	}

	worldTransform_.SetRot({ 0,-MyMath::GetAngle(angle),0 });

	CameraRot = MyMath::Rotation(Vector3(Rot.x, Rot.y, Rot.z), 6);

	Avoidance = MyMath::MatVector(CameraRot, Avoidance);
	Avoidance.y = 0;
	Avoidance.normalize();
	Avoidance *= playerAvoidance;

	//worldTransform_.translation_ += playerMovement;
	move += Avoidance;

	float AR;
	float BR;

	AR = pow((worldTransform_.translation_.x + move.x) - 0, 2) + pow((0 + worldTransform_.translation_.z + move.z) - 0, 2);
	BR = pow((50 - worldTransform_.scale_.x * 2), 2);

	if (AR <= BR)
	{
		worldTransform_.translation_ += move;
	}

	if (input_->MouseInputing(2)) {
		oldWorldTransform_.translation_ = worldTransform_.look;
	}


}

void Player::Attack() {

	Vector3 moveRot = cameraLook;

	if (input_->MouseInputing(0)) {
		//実行前にカウント値を取得
		//計測開始時間の初期化
		isAttack = true;
		startCount = 0;
		nowCount = 0;
		timeRate = 0;
		startIndex = 1;
	}

	if (isAttack == true) {
		if (nowCount < maxTime * 4) {
			nowCount++;
		}
		else {
			isAttack = false;
			for (int i = 0; i < SphereCount; i++) {
				AttackCollider[i]->SetAttribute(COLLISION_ATTR_NOTATTACK);
			}
		}



		position = splinePosition(points, startIndex, timeRate);

		playerAttackTransform_.translation_ = position;

		float sphereX = position.x - GetWorldPosition().x;
		float sphereY = position.y - GetWorldPosition().y;
		float sphereZ = position.z - GetWorldPosition().z;


		for (int i = 0; i < SphereCount; i++) {
			colliderPos[i] = GetWorldPosition() + sphere * i;
			worldSpherePos[i] = MyMath::Translation(colliderPos[i]);
			AttackCollider[i]->Update(worldSpherePos[i]);
			AttackCollider[i]->SetAttribute(COLLISION_ATTR_ATTACK);
			playerAttackTransformaaaa_[i].translation_ = colliderPos[i];
			playerAttackTransformaaaa_[i].TransferMatrix();
		}

	}

}

void Player::SetKnockBackCount()
{
	KnockBack = MyMath::GetWorldTransform(worldTransform_.matWorld_) - MyMath::GetWorldTransform(EnemyPos);
	KnockBack.normalize();
	KnockBack.y = 0;
	KnockBack = KnockBack * KnockBackDistance;

	moveTime = 0;
	KnockBack = MyMath::GetWorldTransform(worldTransform_.matWorld_) + KnockBack;
}

void Player::KnockBackUpdata()
{
	if (moveTime < MaxMoveTime) {
		moveTime++;
		KnockBack += PlayerMoveMent;
		worldTransform_.translation_ = easing_->InOutVec3(MyMath::GetWorldTransform(worldTransform_.matWorld_), KnockBack, moveTime, MaxMoveTime);
	}

}

void Player::Draw(ViewProjection viewProjection_) {

	if (timer == 0) {
		playerModel_->Draw(worldTransform_, viewProjection_);
	}
	if (timer > 0) {
		oldPlayerModel_->SetAlpha(alpha);
		oldPlayerModel_->Draw(oldWorldTransform_, viewProjection_);
	}
	if (isAttack) {
		for (int i = 0; i < SphereCount; i++) {
			playerModel_->Draw(playerAttackTransformaaaa_[i], viewProjection_);
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

<
