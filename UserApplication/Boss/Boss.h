#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>
#include"EasingData.h"
#include"ViewProjection.h"
#include <BaseCollider.h>
#include"Sprite.h"
#include"BossFish.h"
#include"BossWarrier.h"

class Boss
{

public:

	std::unique_ptr<BossFish> bossFish;

	std::unique_ptr<BossWarrier> bossWarrier;

	~Boss();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="targetPos">標的の座標</param>
	void Update(const Vector3& targetPos, const Vector3 stagePos, float stageRadius);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProMat">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProMat);

	/// <summary>
	/// 体力の描画
	/// </summary>
	void DrawHealth();

	void Reset();


	/// <summary>
	/// メンバ関数(プライベート)
	/// </summary>
private:


	/// <summary>
	/// メンバ変数(プライベート)
	/// </summary>
private:

	

};
