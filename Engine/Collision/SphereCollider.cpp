﻿#include "SphereCollider.h"


void SphereCollider::Update(const Matrix4& worldPos)
{
	// ワールド行列から座標を抽出
	worldPos_ = worldPos;

	// 球のメンバ変数を更新
	Vector4 Vector(worldPos_.m[3][0], worldPos_.m[3][1], worldPos_.m[3][2], worldPos_.m[3][3]);
	Sphere::center = Vector;
	Sphere::radius = radius;
}

void SphereCollider::Update(const Matrix4& worldPos, const float& radius)
{
	// ワールド行列から座標を抽出
	worldPos_ = worldPos;

	// 球のメンバ変数を更新
	Vector4 Vector(worldPos_.m[3][0], worldPos_.m[3][1], worldPos_.m[3][2], worldPos_.m[3][3]);
	Sphere::center = Vector;
	Sphere::radius = radius;
}

void SphereCollider::Update(const Matrix4& worldPos, const uint32_t* Cooltime)
{

	// ワールド行列から座標を抽出
	worldPos_ = worldPos;

	// 球のメンバ変数を更新
	Vector4 Vector(worldPos_.m[3][0], worldPos_.m[3][1], worldPos_.m[3][2], worldPos_.m[3][3]);
	Sphere::center = Vector;
	Sphere::radius = radius;
	Sphere::coolTime = *Cooltime;

}

void SphereCollider::Update(const Matrix4& worldPos, const float& radius, const uint32_t* Cooltime)
{
	// ワールド行列から座標を抽出
	worldPos_ = worldPos;

	// 球のメンバ変数を更新
	Vector4 Vector(worldPos_.m[3][0], worldPos_.m[3][1], worldPos_.m[3][2], worldPos_.m[3][3]);
	Sphere::center = Vector;
	Sphere::radius = radius;
	Sphere::coolTime = *Cooltime;
}
