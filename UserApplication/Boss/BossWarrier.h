#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>
#include"EasingData.h"
#include"ViewProjection.h"
#include <BaseCollider.h>
#include"Sprite.h"

enum BossWarrierPart {
	//(��)�͕`�悷��
	//(�֐�)�͕`�悵�Ȃ�
	//���͓��������֐߂̉�]�œ�����
	//enum�̏��Ԃ͎q�ɂȂ�ɂ�ĉ��ɍs���悤�Ɍ��߂�
	Root,	//��{(�֐�)
	Chest,	//��(��)
	Neck,	//��(�֐�)
	Head,	//��(��)
	ShoulderL,//����(�֐�)
	ArmL,	//���r(��)
	elbowL,//���I(�֐�)
	HandL,	//����(��)
	ShoulderR,//�E��(�֐�)
	ArmR,	//�E�r(��)
	elbowR,  //�E�I(�֐�)
	HandR,	//�E��(��)
	Crotch, //��(�֐�)
	Waist,	//��(��)
	Boss2PartMax,	//�{�X�̕��ʐ�
};

struct BossWarrierModel{
	std::unique_ptr<Model> model;
	WorldTransform Transform;
	bool isDraw;
};

class BossWarrier
{

public:
	void Initialize();

	void Update();

	void Draw(const ViewProjection& viewProMat);

private:
	BossWarrierModel boss2Model[BossWarrierPart::Boss2PartMax];	//�{�X���`�Ԃ̃��f��
	std::unique_ptr<Model> boss2TornadeModel;

	WorldTransform boss2TornadoTransform[2];
	float TornadoRotY[2];
	float TornadoSpeedRotY = 5;
	bool isTornado = false;
	int TornadoFlame = 0;

};

