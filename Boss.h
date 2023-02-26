#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>

struct fish {
	WorldTransform pos;	//ワールド座標
	float radian;	//周回軌道用の角度
	float radius;	//周回軌道用の半径
	float spd;	//移動速度
	Vector3 displacement;	//親座標からの微妙なずれ
};

class Boss
{
public:

	~Boss();

	//魚が形成する球の中心のワールド座標
	fish fishParent;
	//魚群
	std::vector<fish> fishes;

	//剣のモデルデータ　
	Model* swordModel = nullptr;

	//ランダムで変化する速度の基本値
	float randSpdParam = 0;

	void Initialize();

	void Update();

	void CreateFish(float posY = 0);

};

/// <summary>
/// 任意の数値の範囲からランダムな値を返す
/// </summary>
/// <param name="num1">値1</param>
/// <param name="num2">値2</param>
/// <returns>値1と値2の間のランダムな値</returns>
float Random(float num1, float num2);
