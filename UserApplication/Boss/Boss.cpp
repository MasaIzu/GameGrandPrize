#include "BossFish.h"
#include "Boss.h"
#include<random>
#include"Quaternion.h"
#include"Affin.h"
#include"ImGuiManager.h"
#include"Easing.h"
#include "CollisionManager.h"
#include <SphereCollider.h>
#include <CollisionAttribute.h>
#include"Sprite.h"
#include"TextureManager.h"

Boss::~Boss()
{

}

void Boss::Initialize()
{

	bossFish.Initialize();
}

void Boss::Update(const Vector3& targetPos, const Vector3 stagePos, float stageRadius)
{
	bossFish.Update(targetPos, stagePos, stageRadius);
}

void Boss::Draw(const ViewProjection& viewProMat)
{
	bossFish.Draw(viewProMat);
}

void Boss::DrawHealth()
{
	bossFish.DrawHealth();
}

void Boss::Reset()
{
	bossFish.Reset();
}
