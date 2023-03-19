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

#include "Vector4.h"
#include <SphereCollider.h>
#include <Easing.h>

class Player {

public:
	Player();
	~Player();

	/// <summary>
	/// ������
	/// <summary>
	void Initialize(Model* model, float WindowWidth, float WindowHeight);

	/// <summary>
	/// �X�V
	/// <summary>
	void Move();

	/// <summary>
	/// �X�V
	/// <summary>
	void Update(const ViewProjection& viewProjection);

	void SetKnockBackCount();
	void KnockBackUpdata();

	/// <summary>
	/// �`��
	/// <summary>
	void Draw(ViewProjection viewProjection_);

	void Attack();

	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);
	Vector3 GetWorldPosition();
	float GetRadius() { return radius; }
	unsigned short GetColliderAttribute() { return collider->GetAttribute(); }
	bool GetSpaceInput() { return spaceInput; }

	void SetIsHit(bool isHit_) { isHit = isHit_; }
	void SetCameraRot(Matrix4 camera) { CameraRot = camera; }
	void SetCameraRot(Vector3 camera) { Rot = camera; }
	void SetCameraLook(Vector3 camera) { cameraLook = camera; }
	void SetAngle(float angle_) { angle = angle_; }
	void SetEnemyPos(Matrix4 enemyPos) { EnemyPos = enemyPos; }

private:
	Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);

private:

	Easing* easing_;
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	WorldTransform oldWorldTransform_;
	WorldTransform playerAttackTransform_;

	bool isHit = false;

	//�C���v�b�g
	Input* input_ = nullptr;

	// �R���C�_�[
	BaseCollider* collider = nullptr;

	static const int SphereCount = 20;

	BaseCollider* AttackCollider[SphereCount];
	Vector3 colliderPos[SphereCount];
	Matrix4 worldSpherePos[SphereCount];
	bool makeColliders = false;


	WorldTransform playerAttackTransformaaaa_[SphereCount];

	//���f��
	Model* playerModel_ = nullptr;
	std::unique_ptr<Model> oldPlayerModel_;

	Matrix4 CameraRot;
	Matrix4 EnemyPos;
	Vector3 Rot;
	Vector3 Avoidance;
	Vector3 PlayerMoveMent;
	Vector3 cameraLook;

	int timer = 0;
	float alpha = 0.0f;

	int moveTime = 0;
	int MaxMoveTime = 60;

	float x = 0;
	float radius = 2.0f;//�����蔻�蔼�a
	float Window_Width;
	float Window_Height;
	float playerSpeed = 0.01f;
	float playerAvoidance = 0.0f;


	bool isPushLeft = false;
	bool isPushRight = false;
	bool isPushBack = false;
	bool spaceInput = false;

	float angle = 0.0f;
	Matrix4 cameraLookmat;
	Vector3 KnockBack;
	float KnockBackDistance = 10.0f;
	///�U���Ɏg���ϐ�

	//���Ԍv�Z�ɕK�v�ȃf�[�^
	int startCount = 0;
	int nowCount = 0;
	int elapsedCount_ = 0;

	int elapsedTime = 0;

	//��ԂŎg���f�[�^
	//start �� end ��5�b�Ŋ���������
	Vector3 p0;			//�X�^�[�g�n�_
	Vector3 p1;	//����_����1
	Vector3 p2;	//����_����2
	Vector3 p3;		//�S�[���n�_

	std::vector<Vector3>points;

	float maxTime = 0.1f * 60;				//�S�̎���[s]
	float timeRate;						//�������Ԃ��i�񂾂�
	//���̈ʒu
	Vector3 position;
	size_t startIndex = 1;

	bool isAttack = false;

	float attackDistanceX = 4.0f;
	float attackDistanceZ = 10.0f;
};