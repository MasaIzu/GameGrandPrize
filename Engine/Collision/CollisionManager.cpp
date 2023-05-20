#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"
#include "MeshCollider.h"
#include <CollisionAttribute.h>

using namespace DirectX;

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::CheckAllCollisions()
{
	isEnemyHit = false;
	isAttackHit = false;
	isWakeEnemyHit = false;
	hitNumber = 0;
	isWakeEnemyAttackHit = false;
	isEnemySwordHit = false;
	isEnemyReception = false;
	playerPos = { 0,0,0 };

	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	// �S�Ă̑g�ݍ��킹�ɂ��đ�����`�F�b�N
	itA = colliders.begin();
	for (; itA != colliders.end(); ++itA) {
		itB = itA;
		++itB;
		for (; itB != colliders.end(); ++itB) {
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			// �Ƃ��ɋ�
			if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
				colB->GetShapeType() == COLLISIONSHAPE_SPHERE) {
				Sphere* SphereA = dynamic_cast<Sphere*>(colA);
				Sphere* SphereB = dynamic_cast<Sphere*>(colB);
				Vector4 inter;
				if ((colA->attribute == COLLISION_ATTR_ALLIES && colB->attribute == COLLISION_ATTR_ENEMYS) ||
					(colA->attribute == COLLISION_ATTR_ENEMYS && colB->attribute == COLLISION_ATTR_ALLIES)) {

					if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter)) {
						EnemyWorldPos = colA->GetWorldPos();
						isEnemyHit = true;
					}
				}
				else if (colA->attribute == COLLISION_ATTR_ENEMYS && colB->attribute == COLLISION_ATTR_ATTACK) {
					if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter)) {
						HitWorldPos = colB->GetWorldPos();
						isAttackHit = true;
					}
				}
				else if (colA->attributeWakeEnemy == COLLISION_ATTR_WEAKENEMYS && colB->attribute == COLLISION_ATTR_ALLIES) {
					for (int i = 0; i < 10; i++) {
						if (colA->attribute == COLLISION_ATTR_WEAKENEMYS1 + i && colB->attribute == COLLISION_ATTR_ALLIES) {
							if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter)) {
								EnemyWorldPos = colA->GetWorldPos();
								isWakeEnemyHit = true;
							}
						}
					}
				}
				else if (colA->attributeWakeEnemy == COLLISION_ATTR_WEAKENEMYS && colB->attribute == COLLISION_ATTR_ATTACK) {
					int a = 1;
					for (int i = 0; i < 10; i++) {
						if (colA->attribute == COLLISION_ATTR_WEAKENEMYS1 + i && colB->attribute == COLLISION_ATTR_ATTACK) {
							if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter)) {
								hitNumber = i + 1;
								HitWorldPos = colB->GetWorldPos();
								isWakeEnemyAttackHit = true;
							}
						}
					}
				}
				else if (colA->attribute == COLLISION_ATTR_ENEMYBIGSOWRD && colB->attribute == COLLISION_ATTR_ALLIES) {
					if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter)) {
						EnemyWorldPos = colA->GetWorldPos();
						isEnemySwordHit = true;
					}
				}
				else if (colA->attribute == COLLISION_ATTR_ENEMYRECEPTION && colB->attribute == COLLISION_ATTR_ALLIES) {
					playerPos = ResolveCollision(*SphereA, *SphereB);
				}
				else if (colA->attribute == COLLISION_ATTR_ENEMYRECEPTION && colB->attribute == COLLISION_ATTR_ATTACK) {
					if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter)) {
						
						isAttackHit = true;
					}
				}
				else if (colA->attribute == COLLISION_ATTR_ENEMYSOWRDATTACK && colB->attribute == COLLISION_ATTR_ALLIES) {
					if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter)) {

						/*EnemyWorldPos = colA->GetWorldPos();
						isEnemyHit = true;*/
					}
				}
				//if (Collision::CheckSphere2Sphere(*SphereA, *SphereB, &inter)) {
				//	//isEnemyHit = true;
				//}
			}
			else if (colA->GetShapeType() == COLLISIONSHAPE_MESH &&
				colB->GetShapeType() == COLLISIONSHAPE_SPHERE) {
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
				Sphere* sphere = dynamic_cast<Sphere*>(colB);
				Vector4 inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter, nullptr)) {

				}
			}
			else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
				colB->GetShapeType() == COLLISIONSHAPE_MESH) {
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				Vector4 inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter, nullptr)) {

				}
			}
		}
	}
}

bool CollisionManager::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance)
{
	return Raycast(ray, 0xffff, hitInfo, maxDistance);
}

bool CollisionManager::Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo, float maxDistance)
{
	bool result = false;
	std::forward_list<BaseCollider*>::iterator it;
	std::forward_list<BaseCollider*>::iterator it_hit;
	float distance = maxDistance;
	Vector4 inter;

	// �S�ẴR���C�_�[�Ƒ�����`�F�b�N
	it = colliders.begin();
	for (; it != colliders.end(); ++it) {
		BaseCollider* colA = *it;

		// ����������Ȃ���΃X�L�b�v
		if (!(colA->attribute & attribute)) {
			continue;
		}

		if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE) {
			Sphere* sphere = dynamic_cast<Sphere*>(colA);

			float tempDistance;
			Vector4 tempInter;

			if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDistance, &tempInter)) continue;
			if (tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
		else if (colA->GetShapeType() == COLLISIONSHAPE_MESH) {
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);

			float tempDistance;
			Vector4 tempInter;
			/// <summary>
			/// //���Ԃ񂱂�
			/// </summary>
			/// <param name="ray"></param>
			/// <param name="attribute"></param>
			/// <param name="hitInfo"></param>
			/// <param name="maxDistance"></param>
			/// <param name="MatWorldPos"></param>
			/// <returns></returns>
			if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
			if (tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
	}

	if (result && hitInfo) {
		hitInfo->distance = distance;
		hitInfo->inter = inter;
		hitInfo->collider = *it_hit;
	}

	return result;
}

void CollisionManager::QuerySphere(const Sphere& sphere, QueryCallback* callback, unsigned short attribute, Matrix4* worldPos)
{
	assert(callback);

	std::forward_list<BaseCollider*>::iterator it;

	// �S�ẴR���C�_�[�Ƒ�����`�F�b�N
	it = colliders.begin();
	for (; it != colliders.end(); ++it) {
		BaseCollider* col = *it;

		// ����������Ȃ���΃X�L�b�v
		if (!(col->attribute & attribute)) {
			continue;
		}

		// ��
		if (col->GetShapeType() == COLLISIONSHAPE_SPHERE) {
			Sphere* sphereB = dynamic_cast<Sphere*>(col);

			Vector4 tempInter;
			Vector4 tempReject;
			if (!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

			// ���������Z�b�g
			QueryHit info;
			info.collider = col;
			info.inter = tempInter;
			info.reject = tempReject;

			// �N�G���[�R�[���o�b�N�Ăяo��
			if (!callback->OnQueryHit(info)) {
				// �߂�l��false�̏ꍇ�A�p�������I��
				return;
			}
		}
		// ���b�V��
		else if (col->GetShapeType() == COLLISIONSHAPE_MESH) {
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

			Vector4 tempInter;
			Vector4 tempReject;
			if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

			// ���������Z�b�g
			QueryHit info;
			info.collider = col;
			info.inter = tempInter;
			info.reject = tempReject;

			// �N�G���[�R�[���o�b�N�Ăяo��
			if (!callback->OnQueryHit(info)) {
				// �߂�l��false�̏ꍇ�A�p�������I��
				return;
			}
		}
	}
}

bool CollisionManager::DetectCollision(const Sphere& sphereA, const Sphere& sphereB, Vector3& out_collision_depth_direction)
{
	Vector3 a_center = { sphereA.center.x,sphereA.center.y,sphereA.center.z };
	Vector3 b_center = { sphereB.center.x,sphereB.center.y,sphereB.center.z };

	Vector3 diff = a_center - b_center;
	float distance = diff.length();
	float total_radius = sphereA.radius + sphereB.radius;

	// If the distance between the centers of the spheres is less than the sum of their radii, then they are colliding
	if (distance < total_radius) {
		// Calculate the collision depth and direction
		float collision_depth = total_radius - distance;
		Vector3 collision_direction = diff.norm();

		// Output the collision depth multiplied by the collision direction
		out_collision_depth_direction = collision_direction * collision_depth;

		out_collision_depth_direction *= -1;

		out_collision_depth_direction.y = 0;

		return true;
	}

	return false;
}

Vector3 CollisionManager::ResolveCollision(Sphere& sphereA, const Sphere& sphereB) {
	Vector3 collision_depth_direction;
	if (DetectCollision(sphereA, sphereB, collision_depth_direction)) {
		
		isEnemyReception = true;

	}

	return collision_depth_direction;
}
