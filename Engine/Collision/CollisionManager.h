#pragma once

#include"CollisionPrimitive.h"
#include "RaycastHit.h"

#include "QueryCallback.h"

#include"Matrix4.h"
#include <d3d12.h>
#include <forward_list>

class BaseCollider;

class CollisionManager {
public:
	static CollisionManager* GetInstance();

public://メンバ関数

	inline void AddCollider(BaseCollider* collider) {
		colliders.push_front(collider);
	}

	inline void RemoveCollider(BaseCollider* collide) {
		colliders.remove(collide);
	}

	void CheckAllCollisions();

	bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);


	//レイキャストオーバーロード
	bool Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

	void QuerySphere(const Sphere& sphere, QueryCallback* callback, unsigned short attribute = (unsigned short)0xffffffff, Matrix4* worldPos = nullptr);


	bool GetIsEnemyHit() { return isEnemyHit; }
	bool GetIsAttackHit() { return isAttackHit; }
	bool GetIsWakeEnemyAttackHit(int x) { return isWakeEnemyAttackHit[x]; }
	Matrix4 GetEnemyWorldPos() { return EnemyWorldPos; }
	Matrix4 GetAttackHitWorldPos() { return HitWorldPos; }

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator = (const CollisionManager&) = delete;

	//コライダーのリスト
	std::forward_list<BaseCollider*> colliders;

	bool isEnemyHit = false;
	bool isAttackHit = false;
	bool isWakeEnemyAttackHit[10];
	Matrix4 EnemyWorldPos;
	Matrix4 HitWorldPos;
};