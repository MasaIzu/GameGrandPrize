#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "affin.h"

class TouchableObject;


class Ground {

public:

	Ground();
	~Ground();

	/// <summary>
	/// ������
	/// <summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// <summary>
	void Update();

	/// <summary>
	/// �`��
	/// <summary>
	void Draw(ViewProjection viewProjection_);


private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	std::unique_ptr<Model> model_ = nullptr;

	Model* modelGround = nullptr;
	TouchableObject* objGround = nullptr;

};