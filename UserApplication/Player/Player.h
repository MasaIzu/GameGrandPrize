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


class Player {

public:
	Player();
	~Player();

	/// <summary>
	/// 初期化
	/// <summary>
	void Initialize(Model* model, float WindowWidth, float WindowHeight);

	/// <summary>
	/// 更新
	/// <summary>
	void Move();

	/// <summary>
	/// 更新
	/// <summary>
	void Update(const ViewProjection& viewProjection);

	void SetKnockBackCount();
	void KnockBackUpdata();

	/// <summary>
	/// 描画
	/// <summary>
	void Draw(ViewProjection viewProjection_);



	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);
	Vector3 GetWorldPosition();
	float GetRadius() { return radius; }
	unsigned short GetColliderAttribute() { return collider->GetAttribute(); }
	bool GetSpaceInput() { return spaceInput; }

	void SetIsHit(bool isHit_) { isHit = isHit_; }
	void SetCameraRot(Matrix4 camera) { CameraRot = camera; }
	void SetCameraRot(Vector3 camera) { Rot = camera; }
	void SetCameraLook(Vector3 camera) { cameraLook = camera; }
	void SetAngle(float angle_) { angle = angle_; }
	void SetEnemyPos(Matrix4 enemyPos) { EnemyPos = enemyPos; }

private:
	Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);

	void SetIsHit(bool isHit) { this->isHit = isHit; }

	void Collision();

private:

	Easing* easing_;
	//ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform oldWorldTransform_;
	WorldTransform playerAttackTransform_;

	bool isHit = false;

	//インプット
	Input* input_ = nullptr;

	// コライダー
	BaseCollider* collider = nullptr;

	static const int SphereCount = 20;

	BaseCollider* AttackCollider[SphereCount];
	Vector3 colliderPos[SphereCount];
	Matrix4 worldSpherePos[SphereCount];
	bool makeColliders = false;


	WorldTransform playerAttackTransformaaaa_[SphereCount];

	//モデル
	Model* playerModel_ = nullptr;
	std::unique_ptr<Model> oldPlayerModel_;

	Matrix4 CameraRot;
	Matrix4 EnemyPos;
	Vector3 Rot;
	Vector3 Avoidance;
	Vector3 PlayerMoveMent;
	Vector3 cameraLook;

	int timer = 0;
	float alpha = 0.0f;

	int moveTime = 0;
	int MaxMoveTime = 60;

	float x = 0;
	float radius = 2.0f;//当たり判定半径
	float Window_Width;
	float Window_Height;
	float playerSpeed = 0.01f;
	float playerAvoidance = 0.0f;


	bool isPushLeft = false;
	bool isPushRight = false;
	bool isPushBack = false;
	bool spaceInput = false;

};