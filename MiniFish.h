#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include<memory>
#include"Boss.h"
#include"ViewProjection.h"

class MiniFish
{
private:
	std::unique_ptr<Model> bodyModel = nullptr;
	std::unique_ptr<Model> eyeModel = nullptr;

	WorldTransform world;	//���[���h�ϊ�
	float direction;	//����

	Vector3 positions[4];

	EasingData easeMove;
	

public:
	void Initialize(const Vector3& pos);

	void Update(const Vector3& stagePos,float stageRadius);

	void Draw(ViewProjection viewPro);

	Vector3 GetPosition()const { return world.translation_; }

	WorldTransform GetWorldTransform()const { return world; }
};

