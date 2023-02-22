#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "Input.h"
#include "affin.h"
#include <memory>
#include "Sprite.h"
#include <list>


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

	/// <summary>
	/// �`��
	/// <summary>
	void Draw(ViewProjection viewProjection_);


	Vector3 bVelocity(Vector3 velocity, WorldTransform& worldTransform);
	Vector3 GetWorldPosition();
	

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	std::unique_ptr<Model> model_ = nullptr;
	//�C���v�b�g
	Input* input_ = nullptr;

	//���f��
	std::unique_ptr<Model> playerModel_;
	

	float Window_Width;
	float Window_Height;
};