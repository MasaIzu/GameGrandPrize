#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "affin.h"

class TouchableObject;

struct ground
{
	WorldTransform pos;

};


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

	void CreateGround();

	static const int MaxCount = 5;
	//地面作成
	std::unique_ptr<Model>model_ = nullptr;
	std::vector<ground> objects;

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	//std::unique_ptr<Model> model_ = nullptr;

	/*std::unique_ptr<Model> groundModel = nullptr;
	TouchableObject* objGround = nullptr;*/

	//ステージサイズの変更
	CONST FLOAT XtZ = 40;
	//高さの変更
	CONST FLOAT Y = -1.25f;


	

	

	//std::unique_ptr<Model> modeltable[groundMaxCount];

};