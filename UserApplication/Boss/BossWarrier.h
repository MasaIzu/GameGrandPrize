#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>
#include"EasingData.h"
#include"ViewProjection.h"
#include <BaseCollider.h>
#include"Sprite.h"
#include "Player.h"
#include "Input.h"

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
	void SetPlayer(Player* player_) { pl = player_; }

	void phase2Attack();
	void phase2AttackP2();

	void phase2AttackDraw(ViewProjection viewProMat);

	void Rota();
private:

	std::unique_ptr<Model> swordModel = nullptr;	//���̃��f���f�[�^

	Input* input_ = nullptr;
	Player* pl = nullptr;
	BossWarrierModel boss2Model[BossWarrierPart::Boss2PartMax];	//�{�X���`�Ԃ̃��f��
	std::unique_ptr<Model> boss2TornadeModel;

	WorldTransform boss2TornadoTransform[2];
	float TornadoRotY[2];
	float TornadoSpeedRotY = 5;
	bool isTornado = false;
	bool isAtkArmSwing = false;
	float rootRotRad = 0;
	int TornadoFlame = 0;

	//

	//���phase�̌��̓���Attack�̌��̍ő吔
	const int MAXSWROD = 5;
	WorldTransform w[5];
	//�������Ă��猕���΂��܂ł̎���
	int phase2AttackCoolTime = 70;
	bool t;
	bool t2;
	WorldTransform pPos[5];
	WorldTransform num[5];
	int interval = 10;
	float moveSpeed = 0.2f;
	bool isSat = false;
	bool isSat2 = false;
	bool isOn = false;
	bool isShot[5];
	const int MAXSHOTTIME = 40;
	int shotTime = MAXSHOTTIME;

	bool kenrot[5];

private:
	//�r�U��U���̏�����
	void InitAtkArmSwing();

	//�r�U��U���̍X�V
	void UpdateAtkArmSwing();

};

Matrix4 CreateMatRot(const Vector3& pos, const Vector3& target);