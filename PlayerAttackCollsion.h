#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Vector4.h"
#include "BaseCollider.h"
#include <SphereCollider.h>

class PlayerAttackCollsion
{
public:// エイリアス

	// 何コンボ目かを表す
	enum Combo
	{
		Combo1,
		Combo2,
		Combo3,
		Combo4,
		Combo5,
	};

public:// メンバ関数

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 一つの当たり判定の処理
	void AttackCollsion();



private:// プライベート関数

	Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);



private:// メンバ変数

	// 当たり判定のわーとら
	WorldTransform playerAttackTransform_;

	//インプット
	Input* input_ = nullptr;

	// コンボ
	Combo combo = Combo::Combo1;

	// 何コンボになっているを判断するフラグ
	bool IsCombo1 = false;
	bool IsCombo2 = false;
	bool IsCombo3 = false;
	bool IsCombo4 = false;
	bool IsCombo5 = false;

	// 一つのアニメーションがどれぐらい進んでいるかを測るタイマー
	int animetionTimer = 0;

	// 次のコンボに移行する時に使うタイマー
	int nextComboTimer = 0;

	// 次のコンボに移行する時に受付するインターバル
	int nextComboInterval = 30;

	// 
	static const int SphereCount = 10;

	BaseCollider* AttackCollider[SphereCount];
	Vector3 colliderPos[SphereCount];
	Matrix4 worldSpherePos[SphereCount];
	bool makeColliders = false;

	WorldTransform playerAttackTransformaaaa_[SphereCount];

	///攻撃に使う変数

//時間計算に必要なデータ
	int startCount = 0;
	int nowCount = 0;
	int elapsedCount_ = 0;

	int elapsedTime = 0;

	//補間で使うデータ
	//start → end を5秒で完了させる
	Vector3 p0;			//スタート地点
	Vector3 p1;	//制御点その1
	Vector3 p2;	//制御点その2
	Vector3 p3;		//ゴール地点

	std::vector<Vector3>points;

	float maxTime = 0.1f * 60;				//全体時間[s]
	float timeRate;						//何％時間が進んだか
	//球の位置
	Vector3 position;
	size_t startIndex = 1;

	bool isAttack = false;

	float attackDistanceX = 4.0f;
	float attackDistanceZ = 10.0f;
};

