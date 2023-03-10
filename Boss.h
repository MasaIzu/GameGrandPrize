#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>
#include"EasingData.h"
#include"ViewProjection.h"
#include <BaseCollider.h>

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

	//魚のモデルデータ
	std::unique_ptr<Model> fishBodyModel = nullptr;
	std::unique_ptr<Model> fishEyeModel = nullptr;

	//ランダムで変化する速度の基本値
	float randSpdParam = 0;

	void Initialize();

	void Update(const Vector3& targetPos);

	void CreateFish(float posY = 0);

	void Draw(ViewProjection viewProMat);

	BossFirstPhase phase1;
	const int attackCooltime = 60 * 3;	//次の攻撃までのクールタイム
	const int beginAttackDelay = 60 * 1;	//攻撃の予備動作時間


	//攻撃のモーション制御タイマー	　　生成　移動  攻撃　  霧散
	const int atkSwordMotionTime = 120 + 45 + 150 + 120;
	int nextPhaseInterval = 0;
	const int rushMaxCount = 3;	//突進攻撃(片道)をする回数
	int rushCount = 0;	//突進攻撃の残り回数


	int GetFishCount() { return fishes.size(); }

	WorldTransform swordTransform;

	static const int fishMaxCount = 200;

private:
	//フェーズごとの更新処理
	void IdleUpdate();

	void AtkSwordUpdate(const Vector3& targetPos);

	void AtkRushUpdate(const Vector3& targetPos);

	void BeginMotionUpdate();

	void FishLookFront(Vector3 pos,Vector3 dirVec,int fishNum);
	
	WorldTransform Transform;
	Vector3 swordPos = {0,0,0 };
	EasingData easeSwordPos;
	EasingData easeSwordScale;

	const int moveFishMax = 120;


	EasingData easePFishToSword[120];	//魚の移動用イージングタイマー
	std::vector<int> choiceFishIndex;	//配列から何番目の魚が選ばれているか(重複対策)
	Vector3 parentBeforePos, parentAfterPos;
	Vector3 fishesBeforePos[fishMaxCount], fishesControllP1[fishMaxCount], fishesControllP2[fishMaxCount],fishesAfterPos[fishMaxCount];
	Vector3 beforeScale, afterScale;

	// コライダー
	BaseCollider* collider = nullptr;
	float radius = 10.0f;//当たり判定半径

	EasingData easeParentPos;

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
/// 3次ベジエ曲線補間
/// </summary>
/// <param name="start">始点</param>
/// <param name="contRollP1">制御点1</param>
/// <param name="contRollP2">制御点2</param>
/// <param name="end">終点</param>
/// <param name="t">時間</param>
/// <returns>座標</returns>
Vector3 LerpBezireCubic(const Vector3& start, const Vector3& contRollP1, const Vector3& contRollP2, const Vector3& end, float t);

Vector3 LerpBezireQuadratic(const Vector3& start, const Vector3& contRollP, const Vector3& end, float t);


/// <summary>
/// 0~1への線形補間をInBackの補間に変換する
///</summary>
/// <param name="t">時間</param>
/// <returns></returns>
float LerpConbertInback(float t);

float LerpConbertOut(float t);