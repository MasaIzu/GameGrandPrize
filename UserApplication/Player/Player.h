#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "Sprite.h"
#include <list>

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


	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);
	Vector3 GetWorldPosition();
	
	bool GetSpaceInput() { return spaceInput; }

	void SetCameraRot(Matrix4 camera) { CameraRot = camera; }
	void SetCameraRot(Vector3 camera) { Rot = camera; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	WorldTransform oldWorldTransform_;

	//�C���v�b�g
	Input* input_ = nullptr;

	//���f��
	Model* playerModel_ = nullptr;
	std::unique_ptr<Model> oldPlayerModel_;

	Matrix4 CameraRot;
	Vector3 Rot;
	Vector3 Avoidance;

	float playerSpeed = 0.3f;
	float playerAvoidance = 0.0f;


	bool isPushLeft = false;
	bool isPushRight = false;
	bool isPushBack = false;

	bool spaceInput = false;

	int timer = 0;

	float Window_Width;
	float Window_Height;
};