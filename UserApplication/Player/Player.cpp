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
	//NULL�|�C���^�`�F�b�N
	assert(model);
	playerModel_ = model;
	oldPlayerModel_.reset(Model::CreateFromOBJ("UFO", true));

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();

	Window_Width = WindowWidth;
	Window_Height = WindowHeight;

	// �R���W�����}�l�[�W���ɒǉ�
	collider = new SphereCollider(Vector4(0, radius, 0, 0), radius);
	CollisionManager::GetInstance()->AddCollider(collider);

	playerAvoidance = 6.0f;

	//worldTransform_.translation_ = { 0,0,-100 };

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	oldWorldTransform_.Initialize();

	collider->Update(worldTransform_.matWorld_);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);



	worldTransform_.TransferMatrix();
	oldWorldTransform_.TransferMatrix();
}


void Player::Update(const ViewProjection& viewProjection) {

	Move();


	Attack(Vector3(0, 0, 0), Vector3(5, 5, 5));


	worldTransform_.TransferMatrix();
	oldWorldTransform_.TransferMatrix();

	collider->Update(worldTransform_.matWorld_);
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
		alpha -= 0.02f;
	}
	else {
		spaceInput = false;
		collider->SetAttribute(COLLISION_ATTR_ALLIES);
	}

	Matrix4 cameraLookmat = MyMath::Rotation(Vector3(0, 90 * (MyMath::PI / 180), 0), 2);

	Vector3 moveRot = cameraLook;



	if (input_->PushKey(DIK_W)) {
		playerMovement.z = -playerSpeed;
		//cameraLook *= playerSpeed;

		worldTransform_.translation_ += cameraLook * playerSpeed;
	}
	if (input_->PushKey(DIK_A)) {
		playerMovement.x = playerSpeed;
		moveRot = MyMath::MatVector(cameraLookmat, moveRot);
		moveRot.y = 0;
		moveRot.norm();
		worldTransform_.translation_ -= moveRot * playerSpeed;
		isPushLeft = true;
	}
	if (input_->PushKey(DIK_S)) {
		playerMovement.z = playerSpeed;
		//cameraLook *= playerSpeed;

		worldTransform_.translation_ -= cameraLook * playerSpeed;
		isPushBack = true;
	}
	if (input_->PushKey(DIK_D)) {
		playerMovement.x = -playerSpeed;
		moveRot = MyMath::MatVector(cameraLookmat, moveRot);
		moveRot.y = 0;
		moveRot.norm();
		worldTransform_.translation_ += moveRot * playerSpeed;
		isPushRight = true;
	}

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

	playerMovement = MyMath::MatVector(CameraRot, playerMovement);
	playerMovement.y = 0;
	playerMovement.normalize();
	playerMovement *= playerSpeed;

	Avoidance = MyMath::MatVector(CameraRot, Avoidance);
	Avoidance.y = 0;
	Avoidance.normalize();
	Avoidance *= playerAvoidance;

	//worldTransform_.translation_ += playerMovement;
	worldTransform_.translation_ += Avoidance;

	if (input_->MouseInputTrigger(2)) {
		oldWorldTransform_.translation_ = worldTransform_.look;
	}


}

void Player::Attack(Vector3 start, Vector3 Finish) {

	if (input_->MouseInputTrigger(1)) {
		//���s�O�ɃJ�E���g�l���擾
		//�v���J�n���Ԃ̏�����
		QueryPerformanceCounter(&startTime);
		startIndex = 1;
	}

	//��ԂŎg���f�[�^
	//start �� end ��5�b�Ŋ���������
	Vector3 p0(-10.0f, 0, 0);			//�X�^�[�g�n�_
	Vector3 p1(-5.0f, 5.0f, 5.0f);		//����_����1
	Vector3 p2(5.0f, -3.0f, -5.0f);		//����_����2
	Vector3 p3(10.0f, 0.0f, 0.0f);		//�S�[���n�_

	std::vector<Vector3>points{ p0,p0,p1,p2,p3,p3 };



	nowCount++;
	// ����
	// �X�^�[�g�n�_        �Fstart
	// �G���h�n�_        �Fend
	// �o�ߎ���            �FelapsedTime [s]
	// �ړ������̗�(�o�ߎ���/�S�̎���) : timeRate (%)
	elapsedTime = nowCount - startCount;
	timeRate = elapsedTime / maxTime;

	if (timeRate >= 1.0f)
	{
		if (startIndex < points.size() - 3)
		{
			startIndex += 1.0f;
			timeRate -= 1.0f;
			startCount = nowCount;
		}
		else
		{
			timeRate = 1.0f;
		}
	}

	position = splinePosition(points, startIndex, timeRate);

	oldWorldTransform_.translation_ = position;

	//if (makeColliders == false) {
	//	float sphereX = Finish.x - start.x;
	//	float sphereY = Finish.y - start.y;
	//	float sphereZ = Finish.z - start.z;

	//	Vector3 sphere(sphereX / SphereCount, sphereY / SphereCount, sphereZ / SphereCount);


	//	for (int i = 0; i < SphereCount; i++) {
	//		// �R���W�����}�l�[�W���ɒǉ�
	//		AttackCollider[i] = new SphereCollider(Vector4(0, radius, 0, 0), radius);
	//		CollisionManager::GetInstance()->AddCollider(AttackCollider[i]);

	//		colliderPos[i] = start + sphere * i;

	//		worldSpherePos[i] = MyMath::Translation(colliderPos[i]);

	//		AttackCollider[i]->Update(worldSpherePos[i]);
	//		AttackCollider[i]->SetAttribute(COLLISION_ATTR_ALLIES);
	//	}
	//}
	//else {
	//	for (int i = 0; i < SphereCount; i++) {

	//		AttackCollider[i]->Update(worldSpherePos[i]);
	//	}
	//}

}

void Player::Draw(ViewProjection viewProjection_) {

	if (timer == 0) {
		playerModel_->Draw(worldTransform_, viewProjection_);
	}
	if (timer > 0) {
		oldPlayerModel_->SetAlpha(alpha);
		oldPlayerModel_->Draw(oldWorldTransform_, viewProjection_);
	}
	oldPlayerModel_->Draw(oldWorldTransform_, viewProjection_);
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

Vector3 Player::splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t) {

	//��Ԃ��ׂ��_�̐�
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n];
	if (startIndex < 1)return points[1];

	Vector3 p0 = points[startIndex - 1];
	Vector3 p1 = points[startIndex];
	Vector3 p2 = points[startIndex + 1];
	Vector3 p3 = points[startIndex + 2];

	Vector3 position = 0.5 * (2 * p1 + (-p0 + p2) * t + (2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) + (-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t));

	return position;
}