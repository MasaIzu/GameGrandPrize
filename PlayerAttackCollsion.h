#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Vector4.h"
#include "BaseCollider.h"
#include <SphereCollider.h>

class PlayerAttackCollsion
{
public:// �G�C���A�X

	// ���R���{�ڂ���\��
	enum Combo
	{
		Combo1,
		Combo2,
		Combo3,
		Combo4,
		Combo5,
	};

public:// �����o�֐�

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// ��̓����蔻��̏���
	void AttackCollsion();



private:// �v���C�x�[�g�֐�

	Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);



private:// �����o�ϐ�

	// �����蔻��̂�[�Ƃ�
	WorldTransform playerAttackTransform_;

	//�C���v�b�g
	Input* input_ = nullptr;

	// �R���{
	Combo combo = Combo::Combo1;

	// ���R���{�ɂȂ��Ă���𔻒f����t���O
	bool IsCombo1 = false;
	bool IsCombo2 = false;
	bool IsCombo3 = false;
	bool IsCombo4 = false;
	bool IsCombo5 = false;

	// ��̃A�j���[�V�������ǂꂮ�炢�i��ł��邩�𑪂�^�C�}�[
	int animetionTimer = 0;

	// ���̃R���{�Ɉڍs���鎞�Ɏg���^�C�}�[
	int nextComboTimer = 0;

	// ���̃R���{�Ɉڍs���鎞�Ɏ�t����C���^�[�o��
	int nextComboInterval = 30;

	// 
	static const int SphereCount = 10;

	BaseCollider* AttackCollider[SphereCount];
	Vector3 colliderPos[SphereCount];
	Matrix4 worldSpherePos[SphereCount];
	bool makeColliders = false;

	WorldTransform playerAttackTransformaaaa_[SphereCount];

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

