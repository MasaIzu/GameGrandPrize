#include "Boss.h"
#include<random>
#include"Quaternion.h"
#include"Affin.h"
#include"ImGuiManager.h"
#include"Easing.h"


Boss::~Boss()
{

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
	swordModel.reset(Model::CreateFromOBJ("dammySword", true));
	phase1 = BossFirstPhase::Idle;
	nextPhaseInterval = attackCooltime;

	swordTransform.Initialize();
	swordTransform.TransferMatrix();
}

void Boss::Update(const Vector3& targetPos)
{
	//第1形態の魚群の更新
	ImGui::Begin("sword");

	ImGui::Text("scale:%f", swordTransform.scale_.x);

	switch (phase1) {
	case BossFirstPhase::Idle:
		IdleUpdate();
		break;
	case BossFirstPhase::Atk_Sword:
		AtkSwordUpdate(targetPos);
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


	ImGui::End();

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
	newFish.pos.scale_ = { 0.5f,0.5f,0.5f };
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

	float plus = Random(-1.0f, 1.0f);
	float num = 1;
	if (plus < 0) {
		num = -1;
	}

	pos.y = (sqrt(fishParent.radius * fishParent.radius - newFish.radius * newFish.radius) * num);

	pos += newFish.displacement;

	newFish.pos.translation_ = pos;
	newFish.pos.TransferMatrix();
	//配列にい列
	fishes.push_back(newFish);
}

void Boss::Draw(ViewProjection viewProMat)
{
	if (phase1 == BossFirstPhase::Atk_Sword) {
		swordModel->Draw(swordTransform, viewProMat);
	}


	//swordModel->Draw(swordTransform, viewProMat);
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

		pos = fishes[i].pos.translation_ - fishes[i].displacement;

		//	pos.y = fishParent.radius - fishes[i].radius;
		pos.x = sin(PI / 180.0f * fishes[i].radian) * fishes[i].radius;
		pos.z = cos(PI / 180.0f * fishes[i].radian) * fishes[i].radius;

		float plus = Random(-1.0f, 1.0f);
		float num = 1;
		if (plus < 0) {
			num = -1;
		}

	//	pos.y = (sqrt(fishParent.radius * fishParent.radius - fishes[i].radius * fishes[i].radius) *num);

		pos += fishes[i].displacement;

		fishes[i].pos.translation_ = pos;
		//fishes[i].pos.rotation_.y =PI / fishes[i].radian * 180.0f;
		fishes[i].pos.TransferMatrix();
	}

	//攻撃のクールタイムを減らす
	nextPhaseInterval--;
	if (nextPhaseInterval == 0) {
		//0になったらクールタイムを攻撃開始モーションの時間に設定
		nextPhaseInterval = beginAttackDelay;
		//フェーズを移行
		phase1 = BossFirstPhase::BeginMotion;
		//ほんとは↑のフェーズは予備行動に移行だけどまだ完成しなさそうなのでいったん攻撃開始に即移る
		phase1 = BossFirstPhase::Atk_Sword;
		nextPhaseInterval = atkSwordMotionTime;
		swordTransform.scale_ = { 0,0,0 };
		swordTransform.SetRot({ 0,0,0 });
		swordTransform.TransferMatrix();

		//魚群の乱回転のためのランダムなベクトルを作成
		for (int i = 0; i < fishes.size(); i++) {
			fishes[i].randomVec = Vector3(Random(0.0f, 1.0f), Random(0.0f, 1.0f), Random(0.0f, 1.0f));
			fishes[i].randomVec.normalize();
		}

	}
}

void Boss::AtkSwordUpdate(const Vector3& targetPos)
{
	//剣の生成開始
	//行動の切り替え(開始)タイミングで各行動のフレーム数でイージングタイマーを動かす

	const int swordCreateTime = 120;	//剣の生成時間
	const int swordMoveTime = 45;		//剣の移動時間
	const int swordAtkTime = 150;		//剣の攻撃時間
	const int swordBreakTime = 120;		//剣の崩壊時間

	//モーション開始の瞬間
	if (nextPhaseInterval == atkSwordMotionTime) {
		swordTransform.scale_ = { 0,0,0 };
		easeSwordScale.Start(swordCreateTime);
		//剣の座標
		swordPos.x = fishParent.pos.translation_.x + 30;
		swordPos.y = fishParent.pos.translation_.y - 30;
		swordPos.z = fishParent.pos.translation_.z;
		//swordPos.x = fishParent.pos.translation_.x;
		beforeScale = { 0.5f,0.5f,0.5f };
		afterScale = { 0,0,0 };

		if (!choiceFishIndex.empty()) {
			choiceFishIndex.clear();
		}

	}//移動開始の瞬間
	else if (nextPhaseInterval == atkSwordMotionTime - swordCreateTime - 60) {
		easeSwordMove.Start(swordMoveTime);
		swordTransform.translation_ = swordPos;
		swordTransform.SetRot({ 0,0,0 });

	}//攻撃開始の瞬間
	else if (nextPhaseInterval == atkSwordMotionTime - swordCreateTime - swordMoveTime - 60)
	{
		easeSwordMove.Start(swordAtkTime);
	}//霧散の瞬間
	else if (nextPhaseInterval == atkSwordMotionTime - swordCreateTime - swordMoveTime - swordAtkTime - 60) {
		easeSwordScale.Start(swordBreakTime);
		afterScale = { 0.5f,0.5f,0.5f };
		beforeScale = { 0,0,0 };
	}

	float distancePtoSword = 90.0f;

	if (nextPhaseInterval > atkSwordMotionTime - swordCreateTime) {
		//敵中心から剣の位置の中心まで移動する(120f)
		//毎フレームランダムに魚群から魚を選び、選ばれた魚は10fで剣の中心まで移動する

		//最初にどの魚を剣まで移動させるか決める
		int goFishToSwordIndex = 0;
		goFishToSwordIndex = static_cast<int>(Random(0, fishes.size()));
		//重複対策で配列内を探索
		if (!choiceFishIndex.empty()) {
			for (int i = 0; i < choiceFishIndex.size(); i++) {
				//重複していたらもう一度ランダムに振り分ける
				if (choiceFishIndex[i] == goFishToSwordIndex) {
					goFishToSwordIndex = static_cast<int>(Random(0, fishes.size()));
					//for文最初からやり直し
					i = -1;
				}
			}
		}
		//配列に挿入
		choiceFishIndex.push_back(goFishToSwordIndex);
		//動的配列末尾の要素のイージングを時間10fで開始させる(順番に移動してもらうため)

		size_t sizeA = choiceFishIndex.size();
		size_t indexA = choiceFishIndex[choiceFishIndex.size() - 1];

		Vector3 pos;
		float randomParam = 10.0f;
		//移動を開始する魚の元の座標を取っておき、制御点もイイ感じに決める
		fishesBeforePos[choiceFishIndex.size() - 1] = fishes[choiceFishIndex[choiceFishIndex.size() - 1]].pos.translation_;

		//選択された魚は親子関係をいったん切る
		fishes[choiceFishIndex[choiceFishIndex.size() - 1]].pos.parent_ = nullptr;
		fishesBeforePos[choiceFishIndex.size() - 1] = fishParent.pos.matWorld_.transform(fishesBeforePos[choiceFishIndex.size() - 1], fishParent.pos.matWorld_);

		//制御点1は始点から誤差x(今は5)のランダムな地点。制御点2は終点から誤差xにランダム
		pos = fishesBeforePos[choiceFishIndex.size() - 1];
		fishesControllP1[choiceFishIndex.size() - 1] = Vector3(Random(pos.x - randomParam, pos.x + randomParam), Random(pos.y - randomParam, pos.y + randomParam), Random(pos.z - randomParam, pos.z + randomParam));
		pos = swordPos;
		fishesAfterPos[choiceFishIndex.size() - 1] = pos;
		fishesControllP2[choiceFishIndex.size() - 1] = Vector3(Random(pos.x - randomParam, pos.x + randomParam), Random(pos.y - randomParam, pos.y + randomParam), Random(pos.z - randomParam, pos.z + randomParam));
		easePFishToSword[choiceFishIndex.size() - 1].Start(60);

		//剣を徐々におおきくする
		Vector3 swordScale;
		easeSwordScale.Update();
		swordScale = Lerp({ 0,0,0 }, { 4,4,4 }, easeSwordScale.GetTimeRate());

		swordTransform.scale_ = swordScale;
		swordTransform.translation_ = swordPos;

		//剣移動のイージングを開始する(実際の座標移動や更新は後ろのif文でやってるので影響なし)
	//	easeSwordMove.Start(swordMoveTime);


	}
	else if (nextPhaseInterval > atkSwordMotionTime - swordCreateTime - 60) {
		//今のとこ何もする予定なし
		static int a = 0;
		a++;

	}
	//アニメーション時間が移動した(魚の数+最後に移動した魚の移動時間)より小さくなったなら次のモーション(攻撃開始座標への移動)を開始
	else if (nextPhaseInterval > atkSwordMotionTime - swordCreateTime - swordMoveTime - 60) {


		Vector3 rotaVec;
		rotaVec.x = sin(PI / 3.0f);
		rotaVec.z = cos(PI / 3.0f);
		rotaVec.normalize();
		rotaVec *= distancePtoSword;

		//標的の座標と掛け算
		Vector3 aftetVec;
		aftetVec = targetPos + rotaVec;

		ImGui::Text("afterVec:%f,%f,%f", aftetVec.x, aftetVec.y, aftetVec.z);

		easeSwordMove.Update();
		Vector3 pos;
		pos = Lerp(swordPos, aftetVec, LerpConbertOut(easeSwordMove.GetTimeRate()));
		/*swordTransform.rotation_.z = easeSwordMove.GetTimeRate() * -(PI / 2.0f);
		swordTransform.rotation_.x = easeSwordMove.GetTimeRate() * -(PI / 2.0f);*/

		Vector3 rot = { easeSwordMove.GetTimeRate() * -(PI / 2.0f),0,easeSwordMove.GetTimeRate() * -(PI / 2.0f) };
		swordTransform.SetRot(rot);

		swordTransform.translation_ = pos;

	}
	
	else if (nextPhaseInterval > atkSwordMotionTime - swordCreateTime - swordMoveTime - swordAtkTime -60) {
		Vector3 rotaVec;
		rotaVec.x = sin(PI / 3.0f);
		rotaVec.z = cos(PI / 3.0f);
		rotaVec.normalize();
		rotaVec *= distancePtoSword;

		//標的の座標と掛け算
		Vector3 beforePos, afterPos;
		beforePos = targetPos + rotaVec;
		afterPos = beforePos;
		afterPos.x = -afterPos.x;
		//afterPos.z = -afterPos.z;

		//イージング更新
		easeSwordMove.Update();
		Vector3 pos, rot;
		//ベジエ曲線で座標を補完
		pos = LerpBezireQuadratic(beforePos, targetPos, afterPos, LerpConbertInback(easeSwordMove.GetTimeRate()));
		//ワールド行列から回転を借りてくる
		rot = swordTransform.rotation_;
		//回転
		rot.x = -(PI / 2.0f) - (LerpConbertInback(easeSwordMove.GetTimeRate()) * PI / 3.0f);
		swordTransform.SetRot(rot);
		swordTransform.translation_ = pos;
	}
	//崩壊モーション
	else if (nextPhaseInterval > atkSwordMotionTime - swordCreateTime - swordMoveTime - swordAtkTime - swordBreakTime - 60) {
		//崩壊モーションのための座標設定
		int fishIndex = swordBreakTime - nextPhaseInterval;
		if (fishIndex >= moveFishMax)fishIndex = moveFishMax - 1;
		ImGui::Text("fishIndex:%d", fishIndex);

		//毎フレーム別の魚の移動を開始する
		Vector3 pos;
		float randomParam = 10.0f;
		//前座標は自座標
		fishesBeforePos[fishIndex] = fishes[choiceFishIndex[fishIndex]].pos.translation_;
		fishesBeforePos[fishIndex] = swordTransform.translation_;//制御点1は自座標から誤差x

		pos = fishesBeforePos[fishIndex];
		fishesControllP1[fishIndex] = Vector3(Random(pos.x - randomParam, pos.x + randomParam), Random(pos.y - randomParam, pos.y + randomParam), Random(pos.z - randomParam, pos.z + randomParam));
		//移動後の座標は魚群の球につくようにする(魚の生成関数と同じようなもんを使う)
		float posY = Random(-fishParent.radius, fishParent.radius);
		fishes[choiceFishIndex[fishIndex]].radian = Random(0.0f, 360.0f);
		fishes[choiceFishIndex[fishIndex]].radius = sqrt(fishParent.radius * fishParent.radius - posY * posY);
		fish newFish = fishes[choiceFishIndex[fishIndex]];
		pos.x = sin(PI / 180.0f * fishes[choiceFishIndex[fishIndex]].radian) * fishes[choiceFishIndex[fishIndex]].radius;
		pos.z = cos(PI / 180.0f * fishes[choiceFishIndex[fishIndex]].radian) * fishes[choiceFishIndex[fishIndex]].radius;
		pos.y = (sqrt(fishParent.radius * fishParent.radius - newFish.radius * newFish.radius) * (posY / fabs(posY)));
		pos += newFish.displacement;
		fishesAfterPos[fishIndex] = pos;

		fishesAfterPos[fishIndex] = fishParent.pos.matWorld_.transform(fishesAfterPos[fishIndex], fishParent.pos.matWorld_);
		fishesControllP2[fishIndex] = Vector3(Random(pos.x - randomParam, pos.x + randomParam), Random(pos.y - randomParam, pos.y + randomParam), Random(pos.z - randomParam, pos.z + randomParam));
		easePFishToSword[fishIndex].Start(60);

		//剣を徐々におおきくする
		Vector3 swordScale;
		swordScale = Lerp({ 4,4,4 }, { 0,0,0 }, easeSwordScale.GetTimeRate());
		easeSwordScale.Update();
		swordTransform.scale_ = swordScale;

	}
	//モーション終了
	else if(nextPhaseInterval > atkSwordMotionTime - swordCreateTime - swordMoveTime - swordAtkTime - swordBreakTime - 120) {

		//親子関係を戻す
		for (int i = 0; i < fishes.size(); i++) {
			if (fishes[i].pos.parent_ == nullptr) {
				fishes[i].pos.parent_ = &fishParent.pos;
			}
		}

		//モーション終了
		nextPhaseInterval = attackCooltime;
		//フェーズを移行
		phase1 = BossFirstPhase::Idle;
		return;
	}



	//イージングによるスケールと座標の制御6
	for (int i = 0; i < choiceFishIndex.size(); i++) {
		easePFishToSword[i].Update();

		fishes[choiceFishIndex[i]].pos.translation_ = LerpBezireCubic(fishesBeforePos[i], fishesControllP1[i], fishesControllP2[i], fishesAfterPos[i], easePFishToSword[i].GetTimeRate());
		ImGui::Text("afterPosZ[%d]:%1.5f", i, fishesAfterPos[i].z);
		if (easePFishToSword[i].GetActive()) {
			fishes[choiceFishIndex[i]].pos.scale_ = Lerp(beforeScale, afterScale, easePFishToSword[i].GetTimeRate());
		}
		fishes[choiceFishIndex[i]].pos.TransferMatrix();
	}

	ImGui::SliderFloat("rotaX", &swordTransform.rotation_.x, 0.0f, 360.0f);
	ImGui::SliderFloat("rotaY", &swordTransform.rotation_.y, 0.0f, 360.0f);
	ImGui::SliderFloat("rotaZ", &swordTransform.rotation_.z, 0.0f, 360.0f);
	ImGui::Text("ParentPosZ:%1.5f", fishParent.pos.translation_.z);
	ImGui::Text("swordPosZ:%1.5f", swordTransform.translation_.z);

	swordTransform.TransferMatrix();

	//タイマー制御
	nextPhaseInterval--;
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
		Quaternion randomRotate = (cross, fishes[i].radian);
		//	pos = randomRotate.RotateVector(fishes[i].pos.translation_);
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

Vector3 Lerp(const Vector3& start, const Vector3& end, float t)
{
	return start * (1.0f - t) + end * t;
}

Vector3 LerpBezireCubic(const Vector3& start, const Vector3& contRollP1, const Vector3& contRollP2, const Vector3& end, float t)
{
	Vector3 p1, p2, p3, p4, p5, result;
	p1 = Lerp(start, contRollP1, t);
	p2 = Lerp(contRollP1, contRollP2, t);
	p3 = Lerp(contRollP2, end, t);
	p4 = Lerp(p1, p2, t);
	p5 = Lerp(p2, p3, t);
	result = Lerp(p4, p5, t);

	return result;
}

Vector3 LerpBezireQuadratic(const Vector3& start, const Vector3& contRollP, const Vector3& end, float t)
{
	Vector3 p1, p2, p3;
	p1 = Lerp(start, contRollP, t);
	p2 = Lerp(contRollP, end, t);
	p3 = Lerp(p1, p2, t);

	return p3;
}

float LerpConbertInback(float t)
{
	const float c1 = 1.70158;
	const float c3 = c1 + 1;

	return c3 * t * t * t - c1 * t * t;
}

float LerpConbertOut(float t)
{
	return 1 - pow(1 - t, 5);
}




