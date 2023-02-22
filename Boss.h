#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>

struct fish {
	WorldTransform pos;
	float radian;
	float radius;
	float spd;
};

class Boss
{
public:
	//魚が形成する球の中心のワールド座標
	fish fishParent;
	//魚群
	std::vector<fish> fishes;


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
