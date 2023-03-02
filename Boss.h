#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>
#include"EasingData.h"
#include"ViewProjection.h"

struct fish {
	WorldTransform pos;	//ワールド座標
	float radian;	//周回軌道用の角度
	float radius;	//周回軌道用の半径
	float spd;	//移動速度
	Vector3 displacement;	//親座標からの微妙なずれ
	Vector3 randomVec;
};

enum class BossFirstPhase {
	Idle,//待機
	Atk_Sword,//剣変形攻撃
	Atk_Rush,//突進攻撃
	BeginMotion,//モーション開始動作

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
	std::unique_ptr<Model> swordModel = nullptr;

	//ランダムで変化する速度の基本値
	float randSpdParam = 0;

	void Initialize();

	void Update();

	void CreateFish(float posY = 0);

	void Draw(ViewProjection viewProMat);

	BossFirstPhase phase1;
	const int attackCooltime = 60 * 3;
	const int beginAttackDelay = 60 * 1;

	//攻撃のモーション制御タイマー	　　生成　移動  攻撃　霧散   ﾓｰｼｮﾝ補間
	const int atkSwordMotionTime = 120 + 45 + 60 + 60 + (10 * 3);
	int nextPhaseInterval = 0;

	int GetFishCount() { return fishes.size(); }
private:
	//フェーズごとの更新処理
	void IdleUpdate();

	void AtkSwordUpdate();

	void AtkRushUpdate();

	void BeginMotionUpdate();

	EasingData easeData;

	WorldTransform swordTransform;

	WorldTransform Transform;
	Vector3 swordPos = { 30,-30,20 };
	EasingData easeSwordScale;

	EasingData easePFishToSword[120];	//魚の移動用イージングタイマー
	Vector3 spd[120];	//移動する魚の移動速度
	std::vector<int> choiceFishIndex;	//配列から何番目の魚が選ばれているか(重複対策)
	Vector3 fishesBeforePos[120], fishesControllP1[120], fishesControllP2[120];
};

/// <summary>
/// 任意の数値の範囲からランダムな値を返す
/// </summary>
/// <param name="num1">値1</param>
/// <param name="num2">値2</param>
/// <returns>値1と値2の間のランダムな値</returns>
float Random(float num1, float num2);

/// <summary>
/// 線形補間
/// </summary>
/// <param name="start">始点</param>
/// <param name="end">終点</param>
/// <param name="t">時間</param>
/// <returns>座標</returns>
Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

/// <summary>
/// ベジエ曲線補間
/// </summary>
/// <param name="start">始点</param>
/// <param name="contRollP1">制御点1</param>
/// <param name="contRollP2">制御点2</param>
/// <param name="end">終点</param>
/// <param name="t">時間</param>
/// <returns>座標</returns>
Vector3 LerpBezire(const Vector3& start, const Vector3& contRollP1, const Vector3& contRollP2, const Vector3& end, float t);