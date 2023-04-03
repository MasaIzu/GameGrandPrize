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

	WorldTransform world;	//ƒ[ƒ‹ƒh•ÏŠ·
	float direction;	//Œü‚«

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

	/// <summary>
/// ŠÔŒ‡ò‚É–ß‚éXV(ƒ`ƒ…[ƒgƒŠƒAƒ‹I—¹‚Ì‡})
/// </summary>
	void LeaveGayser(Vector3 gayserPos);

	void Initialize(const Vector3& pos, const unsigned short fishNumber);

	void Update(const Vector3& stagePos,float stageRadius);


	void Draw(ViewProjection viewPro);

	void SetMovePos(const Vector3& stagePos, float stageRadius);

	Vector3 GetPosition()const { return world.translation_; }

	WorldTransform GetWorldTransform()const { return world; }
};

