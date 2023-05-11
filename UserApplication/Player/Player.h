#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "Sprite.h"
#include <list>
#include "BaseCollider.h"
#include <SphereCollider.h>
#include "Vector4.h"
#include <Easing.h>
#include"ParticleManager.h"
#include"recovery.h"
#include <FbxModel.h>
#include <FbxAnimation.h>

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

	void Attack();

	void SetKnockBackCount();
	void KnockBackUpdate();

	/// <summary>
	/// 描画
	/// <summary>
	void Draw(ViewProjection viewProjection_);

	void PlayerFbxDraw(ViewProjection viewProjection_);

	void ParticleDraw(ViewProjection view);

	void Collision(int damage);

	void DrawHealth();

	void Reset();

	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);
	Vector3 GetWorldPosition();
	void SetPosition(Vector3 pos);
	float GetRadius() { return radius; }
	unsigned short GetColliderAttribute() { return collider->GetAttribute(); }
	bool GetSpaceInput() { return isSpace; }
	Vector3 GetPlayerMoveMent() { return PlayerMoveMent; }


	void SetIsEnemyHit(bool isHit_) { isEnemyHit = isHit_; }
	void SetIsAttackHit(bool isHit_) { isAttackHit = isHit_; }
	void SetCameraRot(Matrix4 camera) { CameraRot = camera; }
	void SetCameraRot(Vector3 camera) { Rot = camera; }
	void SetCameraLook(Vector3 camera) { cameraLook = camera; }
	void SetAngle(float angle_) { angle = angle_; }
	void SetEnemyPos(Matrix4 enemyPos) { EnemyPos = enemyPos; }

	void SetParticlePos(Matrix4 ParticlePos_) { ParticlePos = ParticlePos_; }

	bool GetAlive() {return isAlive ; }

private:
	Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);

	// スプライトの初期化
	void SpriteInitialize();


	/// <summary>
	/// 攻撃が当たった時のプレイヤーの処理
	/// </summary>
	void AttackCollision();

private:

	enum PlayerMotion {

		soukenCombo1,//0
		soukenCombo2,//1
		soukenCombo3,//2
		soukenCombo4,//3
		soukenCombo5,//4
		aruki,//5
		taiki,//6

	};

	PlayerMotion playerNowMotion = PlayerMotion::taiki;
	float MaxFrem = 2.0f;
	float MinimumFrem = 0.5f;
	bool isWalk = false;
	bool isWalking = false;


	Easing* easing_;
	//ワールド変換データ
	WorldTransform worldTransform_;
	WorldTransform oldWorldTransform_;
	WorldTransform playerAttackTransform_;

	bool isEnemyHit = false;
	bool isAttackHit = false;
	//インプット
	Input* input_ = nullptr;

	// コライダー
	BaseCollider* collider = nullptr;

	static const int SphereCount = 10;

	BaseCollider* AttackCollider[SphereCount];
	Vector3 colliderPos[SphereCount];
	Matrix4 worldSpherePos[SphereCount];
	bool makeColliders = false;

	Vector3 AvoidanceMove;

	WorldTransform playerAttackTransformaaaa_[SphereCount];

	//モデル
	Model* playerModel_ = nullptr;
	std::unique_ptr<Model> oldPlayerModel_;

	Matrix4 CameraRot;
	Matrix4 EnemyPos;
	Vector3 Rot;
	Vector3 Avoidance;
	Vector3 PlayerMoveMent;
	bool PlayerMoveZ=false;
	bool PlayerMoveX=false;
	Vector3 cameraLook;

	Matrix4 ParticlePos;

	int timer = 0;
	float alpha = 0.0f;

	int moveTime = 0;
	int MaxMoveTime = 60;

	float x = 0;
	float radius = 4.0f;//当たり判定半径
	float Window_Width;
	float Window_Height;
	float playerSpeed = 0.5f;
	float playerAvoidance = 20.0f;

	bool isPushSenter = false;
	bool isPushLeft = false;
	bool isPushRight = false;
	bool isPushBack = false;
	bool spaceInput = false;

	float angle = 0.0f;
	Matrix4 cameraLookmat;
	Vector3 KnockBack;
	float KnockBackDistance = 20.0f;
	bool isKnockBack = false;
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

	float maxTime = 0.1f * 40;				//全体時間[s]
	float timeRate;						//何％時間が進んだか
	//球の位置
	Vector3 position;
	size_t startIndex = 1;

	bool isAttack = false;

	float attackDistanceX = 8.0f;
	float attackDistanceZ = 20.0f;

	const int satgeSize = 200;

	std::unique_ptr<ParticleManager> ParticleMan;

	std::unique_ptr<Recovery> recovery;

	std::unique_ptr<Model> startPointModel;

	WorldTransform startPointTrans;

	std::unique_ptr<Model> startModel;

	WorldTransform startTrans;

	const int maxHP = 100;

	float HP = maxHP;
	std::unique_ptr<Sprite> healthSprite;
	std::unique_ptr<Sprite> healthAlfaSprite;

	Vector2 hpSize;
	Vector2 hpAlfaSize={ 553.0f,25.0f };
	bool IsHpAlfa = false;
	int hpAlfaTimer = 0;

	std::unique_ptr<Sprite> HP_barSprite;

	// プレイヤーの操作のスプライト
	std::unique_ptr<Sprite> AttackFontSprite[2];
	std::unique_ptr<Sprite> MoveFontSprite;
	std::unique_ptr<Sprite> W_FontSprite[2];
	std::unique_ptr<Sprite> A_FontSprite[2];
	std::unique_ptr<Sprite> S_FontSprite[2];
	std::unique_ptr<Sprite> D_FontSprite[2];
	std::unique_ptr<Sprite> AvoidFontSprite[2];

	std::unique_ptr<Sprite> avoidGauge1;
	std::unique_ptr<Sprite> avoidGauge2;
	std::unique_ptr<Sprite> avoidGauge3;
	std::unique_ptr<Sprite> avoidGauge_under;

	Vector2 avoidGaugeUnderPos = { 1070,412 };
	Vector2 avoidGaugeUnderSize = { 128,128 };
	//Fbxモデル
	std::unique_ptr<FbxModel> fbxmodel;
	FbxModel* fbxmodel2;
	std::unique_ptr<FbxAnimation> modelAnim;
	float frem = 0;

	float fremX = 1.0f;

	bool taikiFlag = false;

	Vector3 root;

	int attackConbo = 0;
	bool isPlayMotion = false;

	float receptionTime = 0.0f;
	bool conboFlag = false;

	bool isSpace = false;

	Vector3 rot;

	bool isAdmission = true;

	bool isAlive=true;

	int playerEvasionTimes = 0;
	float playerEvasionCoolTime = 0;
	int playerEvasionMaxTimes = 3;
	float CoolTime = 180;

	float flame = 0;
	float playerAttackMovement = 0.0f;

	float spriteAlpha1 = 1.0f;
	float spriteAlpha2 = 1.0f;
	float spriteAlpha3 = 1.0f;

	Matrix4 matL;
	Matrix4 matR;

	Vector3 PlayerRot;
	bool isInput = false;

	Vector3 LookingMove;
	Vector3 AttackedPos;
	Vector3 AttackNowPos;
	double attackMoveTimer = 0;
	double MaxAttackMoveTimer = 50;
	bool IsCombo = false;
	bool IsCombo2 = false;
	bool IsCombo3 = false;
	bool IsCombo4 = false;
	bool IsCombo5 = false;

	WorldTransform LBoneTrans;
	WorldTransform RBoneTrans;
	std::unique_ptr<Model> LSowrdModel;
	std::unique_ptr<Model> RSowrdModel;

	int sowrdFlame=0;
};