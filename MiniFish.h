#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include<memory>
#include"Boss.h"
#include"ViewProjection.h"

#include"BaseCollider.h"
#include <SphereCollider.h>

class MiniFish
{
private:
	std::unique_ptr<Model> bodyModel = nullptr;
	std::unique_ptr<Model> eyeModel = nullptr;

	WorldTransform world;	//ÉèÅ[ÉãÉhïœä∑
	float direction;	//å¸Ç´

	Vector3 positions[4];

	EasingData easeMove;
	float spdPerSec = 120.0f;
	float moveRad = 0;

	int spdResetInterval = 2;
	int speedResetCount = 0;
	Vector3 move;
	float spdtemp = 1.0f;

	float fishRadius = 3;
	BaseCollider* FishCollider;

public:
	void Initialize(const Vector3& pos);

	void Update(const Vector3& stagePos,float stageRadius);

	void Draw(ViewProjection viewPro);

	void SetMovePos(const Vector3& stagePos, float stageRadius);

	Vector3 GetPosition()const { return world.translation_; }

	WorldTransform GetWorldTransform()const { return world; }
};

