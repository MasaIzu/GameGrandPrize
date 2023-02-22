#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "Sprite.h"
#include <list>


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


	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);
	Vector3 GetWorldPosition();
	

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	std::unique_ptr<Model> model_ = nullptr;
	//インプット
	Input* input_ = nullptr;

	//モデル
	std::unique_ptr<Model> playerModel_;
	

	float Window_Width;
	float Window_Height;
};