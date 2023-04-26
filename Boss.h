#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>
#include"EasingData.h"
#include"ViewProjection.h"
#include <BaseCollider.h>
#include"Sprite.h"

struct fish {
	WorldTransform pos;	//ワールド座標
	float radian;	//周回軌道用の角度
	float radius;	//周回軌道用の半径
	float spd;	//移動速度
	Vector3 displacement;	//親座標からの微妙なずれ
	Vector3 randomVec;

	bool isUpper = false;

	Vector3 beforePos;
	Vector3 controll1;
	Vector3 controll2;
	Vector3 afterPos;
	EasingData easeMove;
};

enum class BossFirstPhase {
	Idle,//待機
	Atk_Sword,//剣変形攻撃
	Atk_Rush,//突進攻撃
	BeginMotion,//モーション開始動作

};

enum class BossSwordPhase {
	Start,
	Create,
	Cooltime_Create,
	Move,
	Attack,
	Destroy,
	Cooltime_Destroy,
};

class Boss
{
public:

	fish fishParent;			//魚の中心
	std::vector<fish> fishes;	//魚群配列
	std::unique_ptr<Model> fishBodyModel = nullptr;	//魚の体モデル
	std::unique_ptr<Model> fishEyeModel = nullptr;	//魚の目玉モデル
	std::unique_ptr<Model> swordModel = nullptr;	//剣のモデルデータ
	float randSpdParam = 0;							//ランダムで変化する速度の基本値
	BossFirstPhase phase1;							//ボス第一形態のフェーズ
	const int attackCooltime = 60 * 3;				//次の攻撃までのクールタイム
	const int beginAttackDelay = 60 * 1;			//攻撃の予備動作時間
	int nextPhaseInterval = 0;						//次のフェーズまでの時間
	const int rushMaxCount = 3;						//突進攻撃(片道)をする回数
	int rushCount = 0;								//突進攻撃の残り回数
	WorldTransform swordTransform;					//剣のワールド座標
	static const int fishMaxCount = 200;			//小魚の最大数
	int nextDamageInterval = 30;					//次にダメージを受けるまでの時間
	int damageTimer = 0;							//ボスの無敵時間

	// ボスのHP関連
	const float bossHpMax = 20;
	float bossHealth = 20;							//ボスのHP

	std::unique_ptr<Sprite> healthSprite;    // HPのスプライト
	std::unique_ptr<Sprite> healthAlfaSprite;// HPの下の部分のスプライト

	Vector2 hpSize;                       // Hpのスプライトのサイズ
	Vector2 hpAlfaSize = { 553.0f,25.0f };// Hpの下のスプライトのサイズ
	bool IsHpAlfa = false;                // Hpの下のものが現れるかどうか
	int hpAlfaTimer = 0;                  // Hpの下のものが動き出すまでのタイマー

	std::unique_ptr<Sprite> HP_barSprite; // Hpのバーのスプライト

	~Boss();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="targetPos">標的の座標</param>
	void Update(const Vector3& targetPos);

	/// <summary>
	/// 小魚の生成
	/// </summary>
	/// <param name="spawnPos">生成する座標</param>
	void CreateFish(Vector3 spawnPos);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProMat">ビュープロジェクション</param>
	void Draw(ViewProjection viewProMat);

	/// <summary>
	/// 体力の描画
	/// </summary>
	void DrawHealth();

	/// <summary>
	/// ダメージを受けた時のコールバック関数
	/// </summary>
	/// <param name="atk">ダメージ量</param>
	void Damage(int atk);

	//魚の数のゲッター
	int GetFishCount() { return fishes.size(); }

	//当たり判定用座標のゲッター1
	Vector3 GetSwordCollisionCube1()const { return posSwordColCube1; }
	//当たり判定用座標のゲッター2
	Vector3 GetSwordCollisionCube2()const { return posSwordColCube2; }
	//剣の座標のゲッター
	Matrix4 GetSwordWorldPos() { return swordTransform.matWorld_; }



	/// <summary>
	/// メンバ関数(プライベート)
	/// </summary>
private:

	/// <summary>
	/// 非攻撃状態の更新
	/// </summary>
	void UpdateIdle();

	/// <summary>
	/// 剣攻撃の更新
	/// </summary>
	void UpdateAtkSword();

	/// <summary>
	/// 突進攻撃の更新
	/// </summary>
	void UpdateAtkRush();

	/// <summary>
	/// 攻撃前予備動作の更新
	/// </summary>
	void UpdateBeginMotion();

	/// <summary>
	/// 剣の当たり判定の更新
	/// </summary>
	void SwordColCubeUpdate();

	/// <summary>
	/// 魚の配列を小さい順に並べる
	/// </summary>
	void SortFishMin();

	/// <summary>
	/// 魚の向きの更新
	/// </summary>
	void FishDirectionUpdate();

	// スプライトの初期化
	void SpriteInitialize();

	/// <summary>
	/// メンバ変数(プライベート)
	/// </summary>
private:

	Vector3 targetPos;				//標的(プレイヤー)の座標
	BossSwordPhase bossSwordPhase;	//剣の行動フェーズ
	WorldTransform Transform;		//剣のワールド座標
	Vector3 swordPos = { 0,0,0 };	//剣の座標
	EasingData easeSwordPos;		//剣の座標イージング用データ
	EasingData easeSwordScale;		//剣のスケールイージング用データ
	float swordRotAngle = 0;		//剣の回転角
	const int moveFishMax = 120;	//魚の最大数
	EasingData easePFishToSword[fishMaxCount];	//魚の移動用イージングタイマー
	std::vector<int> choiceFishIndex;			//配列から何番目の魚が選ばれているか(重複対策)
	Vector3 parentBeforePos;					//親座標の移動前座標(補完用)
	Vector3 parentAfterPos;						//親座標の移動後座標(補完用)
	Vector3 fishesBeforePos[fishMaxCount];		//小魚の移動前座標(補完用)
	Vector3 fishesControllP1[fishMaxCount];		//小魚の移動制御用座標1(補完用)
	Vector3 fishesControllP2[fishMaxCount];		//小魚の移動制御用座標2(補完用)
	Vector3 fishesAfterPos[fishMaxCount];		//小魚の移動後座標(補完用)
	Vector3 beforeScale;						//変化前のスケール
	Vector3 afterScale;							//変化後のスケール
	float lenTargetToFishes[fishMaxCount];		//小魚と標的の距離(スカラー)


	// コライダー
	BaseCollider* collider = nullptr;
	float radius = 30.0f;//当たり判定半径

	//当たり判定用の剣の大きさ用変数
	WorldTransform testTrans;
	WorldTransform testTrans2;
	Vector3 posSwordColCube1;
	Vector3 posSwordColCube2;
	//剣のオブジェクトとしての大きさ(当たり判定用)
	const float swordSizeX1 = -0.3f - 0.2f;
	const float swordSizeX2 = 0.6f  + 0.3f;
	const float swordSizeY1 = -1.0f - 2.0f;
	const float swordSizeY2 = 1.0f;
	const float swordSizeZ1 = -1.0f - 2.0f;
	const float swordSizeZ2 = 12.4f;

	EasingData easeParentPos;

	Vector3 angleVec{ 0,0,0 };

	int moveFlag = 0;

	uint32_t healthPicture = 0;
	//std::unique_ptr<Sprite> healthSprite;
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

/// <summary>
/// 引数％の確率でtrueを返す関数
/// </summary>
/// <param name="param"></param>
/// <returns>引数の確率でtrue</returns>
bool IsPercent(float param = 100.0f);

Matrix4 CreateMatRot(const Vector3& pos, const Vector3& target);