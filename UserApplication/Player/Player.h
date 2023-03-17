#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "Sprite.h"
#include <list>
#include "BaseCollider.h"

#include "Vector4.h"
#include <SphereCollider.h>
#include"ParticleManager.h"

class Player {

public:
	Player();
	~Player();

	/// <summary>
	/// ������
	/// <summary>
	void Initialize(Model* model, float WindowWidth, float WindowHeight);

	/// <summary>
	/// �X�V
	/// <summary>
	void Move();

	/// <summary>
	/// �X�V
	/// <summary>
	void Update(const ViewProjection& viewProjection);

	/// <summary>
	/// �`��
	/// <summary>
	void Draw(ViewProjection viewProjection_);

	void ParticleDraw(ViewProjection viewProjection_);

	void Attack(Vector3 start, Vector3 Finish);

	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);
	Vector3 GetWorldPosition();
	float GetRadius() { return radius; }
	unsigned short GetColliderAttribute() { return collider->GetAttribute(); }
	bool GetSpaceInput() { return spaceInput; }

	void SetCameraRot(Matrix4 camera) { CameraRot = camera; }
	void SetCameraRot(Vector3 camera) { Rot = camera; }
	void SetCameraLook(Vector3 camera) { cameraLook = camera; }

	void SetIsHit(bool isHit) { this->isHit = isHit; }

	void Collision();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	WorldTransform oldWorldTransform_;

	//�C���v�b�g
	Input* input_ = nullptr;

	// �R���C�_�[
	BaseCollider* collider = nullptr;

	static const int SphereCount = 24;

	BaseCollider* AttackCollider[SphereCount];
	Vector3 colliderPos[SphereCount];
	Matrix4 worldSpherePos[SphereCount];
	bool makeColliders = false;

	//���f��
	Model* playerModel_ = nullptr;
	std::unique_ptr<Model> oldPlayerModel_;

	Matrix4 CameraRot;
	Vector3 Rot;
	Vector3 Avoidance;

	Vector3 cameraLook;

	int timer = 0;
	float alpha = 0.0f;

	float x = 0;
	float radius = 2.0f;//�����蔻�蔼�a
	float Window_Width;
	float Window_Height;
	float playerSpeed = 0.01f;
	float playerAvoidance = 0.0f;


	bool isPushLeft = false;
	bool isPushRight = false;
	bool isPushBack = false;
	bool spaceInput = false;

	bool isHit;

	std::unique_ptr<ParticleManager> ParticleMan;
};