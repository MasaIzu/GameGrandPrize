#include "PlayerAttackCollsion.h"
#include "CollisionManager.h"
#include <CollisionAttribute.h>

void PlayerAttackCollsion::AttackCollsion()
{

	if (input_->MouseInputing(0)) {
		//実行前にカウント値を取得
		//計測開始時間の初期化
		isAttack = true;
		startCount = 0;
		nowCount = 0;
		timeRate = 0;
		startIndex = 1;
	}

	if (isAttack == true) {
		if (nowCount < maxTime * 4) {
			nowCount++;
		}
		else {
			isAttack = false;
			for (int i = 0; i < SphereCount; i++) {
				AttackCollider[i]->SetAttribute(COLLISION_ATTR_NOTATTACK);
			}
		}

		//補間で使うデータ
		//start → end を知らん秒で完了させる
		Vector3 p0(worldTransform_.lookRight);									//スタート地点
		Vector3 p1((worldTransform_.look + worldTransform_.lookRight) / 2);		//制御点その1
		Vector3 p2(worldTransform_.look);										//制御点その2
		Vector3 p3((worldTransform_.look + worldTransform_.lookLeft) / 2);		//制御点その3
		Vector3 p4(worldTransform_.lookLeft);									//ゴール地点

		p0 = p0 + ((p0 - GetWorldPosition()) * attackDistanceX);
		p1 = p1 + ((p1 - GetWorldPosition()) * attackDistanceZ);
		p2 = p2 + ((p2 - GetWorldPosition()) * attackDistanceZ);
		p3 = p3 + ((p3 - GetWorldPosition()) * attackDistanceZ);
		p4 = p4 + ((p4 - GetWorldPosition()) * attackDistanceX);


		points = { p0,p0,p1,p2,p3,p4,p4 };

		// 落下
		// スタート地点        ：start
		// エンド地点        　：end
		// 経過時間            ：elapsedTime [s]
		// 移動完了の率(経過時間/全体時間) : timeRate (%)
		elapsedTime = nowCount - startCount;
		timeRate = elapsedTime / maxTime;

		if (timeRate >= 1.0f)
		{
			if (startIndex < points.size() - 3)
			{
				startIndex += 1.0f;
				timeRate -= 1.0f;
				startCount = nowCount;
			}
			else
			{
				timeRate = 1.0f;
			}
		}

		position = splinePosition(points, startIndex, timeRate);

		playerAttackTransform_.translation_ = position;

		float sphereX = position.x - GetWorldPosition().x;
		float sphereY = position.y - GetWorldPosition().y;
		float sphereZ = position.z - GetWorldPosition().z;

		Vector3 sphere(sphereX / SphereCount, sphereY / SphereCount, sphereZ / SphereCount);

		for (int i = 0; i < SphereCount; i++) {
			colliderPos[i] = GetWorldPosition() + sphere * i;
			worldSpherePos[i] = MyMath::Translation(colliderPos[i]);
			AttackCollider[i]->Update(worldSpherePos[i]);
			AttackCollider[i]->SetAttribute(COLLISION_ATTR_ATTACK);
			playerAttackTransformaaaa_[i].translation_ = colliderPos[i];
			playerAttackTransformaaaa_[i].TransferMatrix();
		}

	}
}

Vector3 PlayerAttackCollsion::splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t)
{
	// 補完すべき点の数
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n]; // Pnの値を返す
	if (startIndex < 1)return points[1]; // P1の値を返す

	// p0～p3の制御点を取得する　※　p1～p2を補完する
	Vector3 p0 = points[startIndex - 1];
	Vector3 p1 = points[startIndex];
	Vector3 p2 = points[startIndex + 1];
	Vector3 p3 = points[startIndex + 2];

	// Catmull - Romの式による補間
	Vector3 position = 0.5 * (2 * p1 + (-p0 + p2) * t
		+ (2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t
		+ (-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t);

	return position;
}
