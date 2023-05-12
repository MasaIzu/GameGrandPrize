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
	//(骨)は描画する
	//(関節)は描画しない
	//骨は動かさず関節の回転で動かす
	//enumの順番は子になるにつれて下に行くように決めた
	Root,	//大本(関節)
	Chest,	//胸(骨)
	Neck,	//首(関節)
	Head,	//頭(骨)
	ShoulderL,//左肩(関節)
	ArmL,	//左腕(骨)
	elbowL,//左肘(関節)
	HandL,	//左手(骨)
	ShoulderR,//右肩(関節)
	ArmR,	//右腕(骨)
	elbowR,  //右肘(関節)
	HandR,	//右手(骨)
	Crotch, //股(関節)
	Waist,	//腰(骨)
	Boss2PartMax,	//ボスの部位数
};

struct BossWarrierModel{
	std::unique_ptr<Model> model;
	WorldTransform Transform;
	bool isDraw;
};

class BossWarrier
{

public:
	void Initialize();

	void Update();

	void Draw(const ViewProjection& viewProMat);

private:
	BossWarrierModel boss2Model[BossWarrierPart::Boss2PartMax];	//ボス第二形態のモデル
	std::unique_ptr<Model> boss2TornadeModel;

	WorldTransform boss2TornadoTransform[2];
	float TornadoRotY[2];
	float TornadoSpeedRotY = 5;
	bool isTornado = false;
	int TornadoFlame = 0;

};

