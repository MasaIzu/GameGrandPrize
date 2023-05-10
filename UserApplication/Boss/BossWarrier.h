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
	Root,	//��{
	Chest,	//��
	Head,	//��
	Waist,	//��
	ArmL,	//���r
	ArmR,	//�E�r
	HandL,	//����
	HandR,	//�E��
	Boss2PartMax,	//�{�X�̕��ʐ�
};

class BossWarrier
{

public:
	void Initialize();

	void Update();

	void Draw(const ViewProjection& viewProMat);

private:
	std::unique_ptr<Model> boss2Model[BossWarrierPart::Boss2PartMax];	//�{�X���`�Ԃ̃��f��
	WorldTransform boss2Transform[BossWarrierPart::Boss2PartMax];	//�{�X���`�Ԃ̊e���ʂ̃��[���h�s��

};

