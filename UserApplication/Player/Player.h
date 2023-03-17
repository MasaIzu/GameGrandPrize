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

	/// <summary>
	/// 描画
	/// <summary>
	void Draw(ViewProjection viewProjection_);

	void Attack(Vector3 start,Vector3 Finish);

	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);
	Vector3 GetWorldPosition();
	float GetRadius() { return radius; }
	unsigned short GetColliderAttribute() { return collider->GetAttribute(); }
	bool GetSpaceInput() { return spaceInput; }

	void SetCameraRot(Matrix4 camera) { CameraRot = camera; }
	void SetCameraRot(Vector3 camera) { Rot = camera; }
	void SetCameraLook(Vector3 camera) { cameraLook = camera; }
	void SetAngle(float angle_) { angle = angle_; }

private:
	Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform oldWorldTransform_;
	WorldTransform worldTransformaaaa_;

	//インプット
	Input* input_ = nullptr;

	// コライダー
	BaseCollider* collider = nullptr;

	static const int SphereCount = 24;

	BaseCollider* AttackCollider[SphereCount];
	Vector3 colliderPos[SphereCount];
	Matrix4 worldSpherePos[SphereCount];
	bool makeColliders = false;

	//モデル
	Model* playerModel_ = nullptr;
	std::unique_ptr<Model> oldPlayerModel_;

	Matrix4 CameraRot;
	Vector3 Rot;
	Vector3 Avoidance;

	Vector3 cameraLook;

	int timer = 0;
	float alpha = 0.0f;

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

	float angle = 0.0f;


	///攻撃に使う変数

	//時間計算に必要なデータ
	int startCount = 0;
	int nowCount = 0;
	int elapsedCount_ = 0;

	int elapsedTime = 0;

	//計測開始時間
	LARGE_INTEGER startTime{};
	//今の時間
	LARGE_INTEGER nowTime{};
	LARGE_INTEGER elapsedCount{};

	//補間で使うデータ
	//start → end を5秒で完了させる
	Vector3 p0;			//スタート地点
	Vector3 p1;	//制御点その1
	Vector3 p2;	//制御点その2
	Vector3 p3;		//ゴール地点

	std::vector<Vector3>points;

	float maxTime = 1.2f * 60;				//全体時間[s]
	float timeRate;						//何％時間が進んだか
	//球の位置
	Vector3 position;
	size_t startIndex = 1;

	bool isAttack = false;

};