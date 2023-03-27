#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "affin.h"

class TouchableObject;


class Ground {

public:

	Ground();
	~Ground();

	/// <summary>
	/// 初期化
	/// <summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// <summary>
	void Update();

	/// <summary>
	/// 描画
	/// <summary>
	void Draw(ViewProjection viewProjection_);


private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	std::unique_ptr<Model> model_ = nullptr;

	Model* modelGround = nullptr;
	TouchableObject* objGround = nullptr;

};