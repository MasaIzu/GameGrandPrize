#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>
#include"EasingData.h"
#include"ViewProjection.h"
#include <BaseCollider.h>
#include"Sprite.h"
#include"EasingData.h"

enum BossWarrierPart {
	//(��)�͕`�悷��
	//(�֐�)�͕`�悵�Ȃ�
	//���͓��������֐߂̉�]�œ�����
	//enum�̏��Ԃ͎q�ɂȂ�ɂ�ĉ��ɍs���悤�Ɍ��߂�
	Root,		//��{(�֐�)
	Chest,		//��(��)
	Neck,		//��(�֐�)
	Head,		//��(��)
	ShoulderL,	//����(�֐�)
	ArmL,		//���r(��)
	elbowL,		//���I(�֐�)
	HandL,		//����(��)
	ShoulderR,	//�E��(�֐�)
	ArmR,		//�E�r(��)
	elbowR,		//�E�I(�֐�)
	HandR,		//�E��(��)
	Crotch,		//��(�֐�)
	Waist,		//��(��)
	Boss2PartMax,	//�{�X�̕��ʐ�
};

struct BossWarrierModel {
	std::unique_ptr<Model> model;
	WorldTransform Transform;
	bool isDraw;
};

class BossWarrier
{

public:
	void Initialize();

	void Update(const Vector3& targetPos);

	void Draw(const ViewProjection& viewProMat);

private:
	BossWarrierModel boss2Model[BossWarrierPart::Boss2PartMax];	//�{�X���`�Ԃ̃��f��
	std::unique_ptr<Model> boss2TornadeModel;

	WorldTransform boss2TornadoTransform[2];
	float TornadoRotY[2];
	float TornadoSpeedRotY = 5;
	bool isTornado = false;
	bool isAtkArmSwing = false;
	float rootRotRad = 0;
	int TornadoFlame = 0;
	bool isLastAtkStart = false;
	int lastAtkCount = 0;
	EasingData easeRotArm;//�r�U�萧��̃C�[�W���O�f�[�^
	Vector3 dataRotArm[2];	//�C�[�W���O�p��]�p�f�[�^
	Vector3 dataRotElbow[2];
	Vector3 dataRotShoulder[2];
	Vector3 dummyTargetPos;

	//�����Ȃǂł�����Ă���ϐ�
	Vector3 targetPos = { 0,0,0 };

private:
	//�r�U��U���̏�����
	void InitAtkArmSwing();

	//�r�U��U���̍X�V
	void UpdateAtkArmSwing();

};

