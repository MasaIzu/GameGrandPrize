#include "SphereCollider.h"


void SphereCollider::Update(const Matrix4& worldPos)
{
	// ワールド行列から座標を抽出
	const Matrix4& matWorld = worldPos;

	// 球のメンバ変数を更新
	Sphere::center = Vector4(matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2], matWorld.m[3][3]) + offset;
	Sphere::radius = radius;
}
