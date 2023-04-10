#pragma once
#include"WorldTransform.h"
#include"ParticleManager.h"
#include"Model.h"
class Recovery
{
public:/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection view);

	void Collision();

	Vector3 GetWorldPosition();

	Vector3 GetScale() { return pointWorldTransform_.scale_; }

	bool GetActive(){return isActive;}
private:

	std::unique_ptr<Model> pointModel_;

	WorldTransform pointWorldTransform_;

	std::unique_ptr<Model> model_;

	WorldTransform worldTransform_[20];

	uint32_t tex1;

	uint32_t tex2;

	const float scale = 3;

	const float MaxIntervalFlame = 6000;

	float IntervalFlame = 0;

	bool isActive = true;
};

