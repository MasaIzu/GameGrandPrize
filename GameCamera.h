#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include <Input.h>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Easing.h"

class GameCamera {

public:

	GameCamera(int window_width, int window_height);

	~GameCamera();

	void Initialize();

	void Update(ViewProjection* viewProjection_);

	void PlaySceneCamera(ViewProjection* viewProjection_);

	void MultiplyMatrix(Matrix4& matrix);

public://ゲッターセッター

	Vector3 GetEye() { return vTargetEye; }
	Vector3 GetUp() { return vUp; }
	Vector3 GetTarget() { return target; }
	Matrix4 GetCameraRot() { return this->CameraRot; }
	Vector3 GetCameraRotVec3() { return this->rot; }

	void SetCameraPosition(Vector3 pos) { playerPos = pos; }
	void SetSpaceInput(bool isSpaceInput) { spaceInput = isSpaceInput; }

private:

	Easing* easing_;

	// カメラ注視点までの距離
	float distance_ = 10;
	Vector2 mousePos = { 0.0f,0.0f };
	Vector2 oldMousePos = { 0.0f,0.0f };
	Input* input_ = nullptr;

	// 回転行列
	Matrix4 matRot;
	Vector3 rot;
	float fTheta = 4.57f;//カメラ横方向角度
	float fDelta = 0.43f;//カメラ縦方向角度

	float mousepoint_a;//マウス位置
	float mousepoint_b;//マウス位置
	bool dirty = false;

	bool spaceInput = false;

	// スケーリング
	float scaleX_ = 1.0f;
	float scaleY_ = 1.0f;

	Vector3 vTargetEye;
	Vector3 vUp;
	Vector3 playerPos;
	Vector3 target;

	bool cameraMode = false;

	int cameraType = 0;

	int winWidth = 0;
	int winHeight = 0;
	Vector2 MouseMove;
	Vector2 mouseMoved;
	WorldTransform worldTransform_;
	WorldTransform EnemyWorld_;

	Matrix4 CameraRot;
	float playerCameraDistance = 5.0f;

	int cameraTime = 0;
	int MaxCameraTime = 0;

};