#include "Boss.h"
#include<random>
#include"Quaternion.h"
#include"Affin.h"

Boss::~Boss()
{
	delete swordModel;
}

void Boss::Initialize()
{
	fishParent.pos.Initialize();
	fishParent.radius = 20.0f;

	if (!fishes.empty()) {
		fishes.clear();
	}

	randSpdParam = 3.75f;

	//剣のモデル初期化
	swordModel = Model::CreateFromOBJ("dammySword");
	phase1 = BossFirstPhase::Idle;
	nextPhaseInterval = attackCooltime;
}

void Boss::Update()
{
	//第1形態の魚群の更新
	 
	switch (phase1) {
	case BossFirstPhase::Idle:
		IdleUpdate();
		break;
	case BossFirstPhase::Atk_Sword:
		AtkSwordUpdate();
		break;
	case BossFirstPhase::Atk_Rush:
		AtkRushUpdate();
		break;
	case BossFirstPhase::BeginMotion:
		BeginMotionUpdate();
		break;
	default:
		break;

	}
	
}

void Boss::CreateFish(float posY)
{
	//y座標が親の半径を超えないようにする
	if (fabs(posY) > fishParent.radius) {
		if (posY > 0)posY = fishParent.radius;
		else posY = -fishParent.radius;
	}

	//新しい魚を作成
	fish newFish;
	//角度をランダムに指定
	newFish.radian = Random(0.0f, 360.0f);
	//与えられた引数から半径を設定
	newFish.radius = sqrt(fishParent.radius * fishParent.radius - posY * posY);
	//ワールド行列初期化
	newFish.pos.Initialize();
	newFish.pos.scale_ = { 5.0f,5.0f,5.0f };
	//Y座標は引数で
	newFish.pos.translation_.y = posY;
	//newFish.pos.rotation_.y = 0.78f;
	newFish.pos.TransferMatrix();

	//速度をランダムに決定
	newFish.spd = Random(0.0f, randSpdParam);

	//親のポインタ貰う
	newFish.pos.parent_ = &fishParent.pos;

	//微妙なずれの大きさは自分の半径の1/10からランダムに
	float displacementParam = newFish.radius / 10.0f * 1.5f;
	newFish.displacement = Vector3(Random(0.0f, displacementParam), Random(0.0f, displacementParam), Random(0.0f, displacementParam));

	Vector3 pos;

	//	pos.y = fishParent.radius - fishes[i].radius;
	pos.x = sin(PI / 180.0f * newFish.radian) * newFish.radius;
	pos.z = cos(PI / 180.0f * newFish.radian) * newFish.radius;
	pos.y = (sqrt(fishParent.radius * fishParent.radius - newFish.radius * newFish.radius) * (newFish.pos.translation_.y / fabs(newFish.pos.translation_.y)));

	pos += newFish.displacement;

	newFish.pos.translation_ = pos;
	newFish.pos.TransferMatrix();
	//配列にい列
	fishes.push_back(newFish);
}

void Boss::IdleUpdate()
{
	//魚群の中心(真ん中)の座標更新
	fishParent.pos.TransferMatrix();

	//魚1匹ずつの更新
	for (int i = 0; i < fishes.size(); i++) {

		//魚のラジアン(球の周回軌道)を加算
		fishes[i].radian += fishes[i].spd;
		if (fishes[i].radian > 360.0f) {
			fishes[i].radian -= 360.0f;
			fishes[i].spd = Random(0.0f, randSpdParam);
		}

		//座標を計算
		Vector3 pos;

		//	pos.y = fishParent.radius - fishes[i].radius;
		pos.x = sin(PI / 180.0f * fishes[i].radian) * fishes[i].radius;
		pos.z = cos(PI / 180.0f * fishes[i].radian) * fishes[i].radius;
		pos.y = (sqrt(fishParent.radius * fishParent.radius - fishes[i].radius * fishes[i].radius) * (fishes[i].pos.translation_.y / fabs(fishes[i].pos.translation_.y)));

		pos += fishes[i].displacement;

		fishes[i].pos.translation_ = pos;
		//fishes[i].pos.rotation_.y =PI / fishes[i].radian * 180.0f;
		fishes[i].pos.TransferMatrix();
	}

	//攻撃のクールタイムを減らす
	nextPhaseInterval--;
	if (nextPhaseInterval == 0) {
		//0になったらクールタイムを攻撃開始モーションの時間に設定
		nextPhaseInterval =  beginAttackDelay;
		//フェーズを移行
		phase1 = BossFirstPhase::BeginMotion;
		//魚群の乱回転のためのランダムなベクトルを作成
		for (int i = 0; i < fishes.size(); i++) {
			fishes[i].randomVec = Vector3(Random(0.0f,1.0f), Random(0.0f, 1.0f), Random(0.0f, 1.0f));
			fishes[i].randomVec.normalize();
		}

	}
}

void Boss::AtkSwordUpdate()
{
}

void Boss::AtkRushUpdate()
{
}

void Boss::BeginMotionUpdate()
{
	//魚群の中心(真ん中)の座標更新
	fishParent.pos.TransferMatrix();

	//魚1匹ずつの更新
	for (int i = 0; i < fishes.size(); i++) {

		//魚のラジアン(球の周回軌道)を加算
		fishes[i].radian += fishes[i].spd;
		if (fishes[i].radian > 360.0f) {
			fishes[i].radian -= 360.0f;
			fishes[i].spd = Random(0.0f, randSpdParam);
		}


		//ランダムに取得したベクトルと自座標から原点(魚群の中心)のベクトルの外積をとり、乱回転の軸を作成
		Vector3 vec = fishes[i].pos.translation_ - fishParent.pos.translation_;
		Vector3 cross;
		cross = fishes[i].randomVec.cross(vec);

		Vector3 pos;
		Quaternion randomRotate =(cross,fishes[i].radian);
		pos = randomRotate.RotateVector(fishes[i].pos.translation_);
		fishes[i].pos.translation_ = pos;
		fishes[i].pos.TransferMatrix();
	
	}

	//攻撃のクールタイムを減らす
	nextPhaseInterval--;
	if (nextPhaseInterval == 0) {
		//0になったらクールタイムを攻撃開始モーションの時間に設定
		nextPhaseInterval = beginAttackDelay;
		//フェーズを移行
		phase1 = BossFirstPhase::BeginMotion;
	}
}

float Random(float num1, float num2)
{
	//引数から小さい方とおおきい方を分ける
	float min, max;
	min = num2;
	max = num1;
	if (num1 < num2) {
		min = num1;
		max = num2;
	}
	//乱数シード生成器
	std::random_device seedGem;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seedGem());
	//乱数範囲の指定
	std::uniform_real_distribution<float> dist(min, max);

	//ランダムな値を生成して返す
	return dist(engine);
}
