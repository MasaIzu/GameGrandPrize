#include "PlayerAttackCollsion.h"
#include "CollisionManager.h"
#include <CollisionAttribute.h>

void PlayerAttackCollsion::AttackCollsion()
{

	if (input_->MouseInputing(0)) {
		//���s�O�ɃJ�E���g�l���擾
		//�v���J�n���Ԃ̏�����
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

		//��ԂŎg���f�[�^
		//start �� end ��m���b�Ŋ���������
		Vector3 p0(worldTransform_.lookRight);									//�X�^�[�g�n�_
		Vector3 p1((worldTransform_.look + worldTransform_.lookRight) / 2);		//����_����1
		Vector3 p2(worldTransform_.look);										//����_����2
		Vector3 p3((worldTransform_.look + worldTransform_.lookLeft) / 2);		//����_����3
		Vector3 p4(worldTransform_.lookLeft);									//�S�[���n�_

		p0 = p0 + ((p0 - GetWorldPosition()) * attackDistanceX);
		p1 = p1 + ((p1 - GetWorldPosition()) * attackDistanceZ);
		p2 = p2 + ((p2 - GetWorldPosition()) * attackDistanceZ);
		p3 = p3 + ((p3 - GetWorldPosition()) * attackDistanceZ);
		p4 = p4 + ((p4 - GetWorldPosition()) * attackDistanceX);


		points = { p0,p0,p1,p2,p3,p4,p4 };

		// ����
		// �X�^�[�g�n�_        �Fstart
		// �G���h�n�_        �@�Fend
		// �o�ߎ���            �FelapsedTime [s]
		// �ړ������̗�(�o�ߎ���/�S�̎���) : timeRate (%)
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
	// �⊮���ׂ��_�̐�
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n]; // Pn�̒l��Ԃ�
	if (startIndex < 1)return points[1]; // P1�̒l��Ԃ�

	// p0�`p3�̐���_���擾����@���@p1�`p2��⊮����
	Vector3 p0 = points[startIndex - 1];
	Vector3 p1 = points[startIndex];
	Vector3 p2 = points[startIndex + 1];
	Vector3 p3 = points[startIndex + 2];

	// Catmull - Rom�̎��ɂ����
	Vector3 position = 0.5 * (2 * p1 + (-p0 + p2) * t
		+ (2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t
		+ (-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t);

	return position;
}
