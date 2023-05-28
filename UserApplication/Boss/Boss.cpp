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
	bossFish = std::make_unique<BossFish>();
	bossFish->Initialize();

	bossWarrier= std::make_unique<BossWarrier>();
	bossWarrier->Initialize();
}

void Boss::Update(const Vector3& targetPos, const Vector3 stagePos, float stageRadius)
{
	bossFish->Update(targetPos, stagePos, stageRadius);

	//���`�Ԃ����񂾂���`�Ԃ̃X�|�[��������
	if (bossFish->GetIsDeathEnd() && !isBossSpawn) {
		bossWarrier->Spawn();
		isBossSpawn = true;
	}

	//�e�X�g�p
	if (Input::GetInstance()->TriggerKey(DIK_P)) {
		//bossWarrier->Spawn();
	}

	if (bossWarrier->GetAlive()) {
		bossWarrier->Update(targetPos);
	}
}

void Boss::Draw(const ViewProjection& viewProMat)
{
	if (!bossFish->GetIsDeathEnd()) {
		bossFish->Draw(viewProMat);
	}

	if (bossWarrier->GetAlive()) {
		bossWarrier->Draw(viewProMat);
	}
}

void Boss::DrawHealth()
{
	bossFish->DrawHealth();
}

void Boss::Reset()
{
	bossFish->Reset();
}
