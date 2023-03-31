#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <assert.h>
#include "affin.h"

class TouchableObject;

struct ground
{
	WorldTransform pos;

};


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

	void CreateGround();

	static const int MaxCount = 5;
	//�n�ʍ쐬
	std::unique_ptr<Model>model_ = nullptr;
	std::vector<ground> objects;

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	//std::unique_ptr<Model> model_ = nullptr;

	/*std::unique_ptr<Model> groundModel = nullptr;
	TouchableObject* objGround = nullptr;*/

	//�X�e�[�W�T�C�Y�̕ύX
	CONST FLOAT XtZ = 40;
	//�����̕ύX
	CONST FLOAT Y = -1.25f;


	

	

	//std::unique_ptr<Model> modeltable[groundMaxCount];

};