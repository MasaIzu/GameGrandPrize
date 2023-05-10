#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>
#include"EasingData.h"
#include"ViewProjection.h"
#include <BaseCollider.h>
#include"Sprite.h"

enum BossWarrierPart {
	Root,	//大本
	Chest,	//胸
	Head,	//頭
	Waist,	//腰
	ArmL,	//左腕
	ArmR,	//右腕
	HandL,	//左手
	HandR,	//右手
	Boss2PartMax,	//ボスの部位数
};

class BossWarrier
{

public:
	void Initialize();

	void Update();

	void Draw(const ViewProjection& viewProMat);

private:
	std::unique_ptr<Model> boss2Model[BossWarrierPart::Boss2PartMax];	//ボス第二形態のモデル
	WorldTransform boss2Transform[BossWarrierPart::Boss2PartMax];	//ボス第二形態の各部位のワールド行列

};

