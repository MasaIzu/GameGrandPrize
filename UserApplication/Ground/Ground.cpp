#include "Ground.h"

using namespace DirectX;

Ground::Ground()
{
}

Ground::~Ground()
{
	//delete objGround;
	//delete modelGround;
	
}

void Ground::Initialize() {
	////NULL�|�C���^�`�F�b�N
	//assert(model);
	//model_.reset(model);
	////���[���h�ϊ��̏�����
	//worldTransform_.Initialize();

	//worldTransform_.translation_ = Vector3(0, -20.0f, 0);
	//worldTransform_.scale_ = Vector3(600, 100, 600);
	//worldTransform_.matWorld_ *= AffinTrans::Translation(worldTransform_.translation_);
	//worldTransform_.matWorld_ *= AffinTrans::Scale(worldTransform_.scale_);
	////�s��̓]��
	//worldTransform_.TransferMatrix();

	model_ = std::make_unique<Model>();

	//�n�ʂ̕`��
	model_.reset(Model::CreateFromOBJ("Ground", true));

	for (auto newGround : objects)
	{
		Ground::CreateGround();
	}

}

void Ground::Update() {

}

void Ground::Draw(ViewProjection viewProjection_) {
	//3D���f���`��
	//model_->Draw(worldTransform_, viewProjection_);

	for (int i = 0; i < 64; i++)
	{
		model_->Draw(objects[i].pos, viewProjection_);

	}
}

void Ground::CreateGround()
{
	//ground newGround;

	Vector3 SCALE = { XtZ,Y,XtZ};
	//newGround.pos.Initialize();
	//newGround.pos.TransferMatrix();

	//z
	for (int i = 0; i < MaxCount; i++)
	{ //x
		for (int j = 0; j < MaxCount; j++)
		{
			ground newGround;
			newGround.pos.Initialize();
			newGround.pos.TransferMatrix();

			float x = (j * SCALE.x) + (SCALE.x * j) - SCALE.x * (MaxCount);
			float y = SCALE.y;
			float z = (i * SCALE.z) + (SCALE.z * i) - SCALE.z * (MaxCount);

			//int modelIndex = rand() % 10;
			//newGround = Model::Create(modeltable[i]);

			newGround.pos.translation_ = { x,y,z };
			newGround.pos.scale_ = SCALE;
			newGround.pos.TransferMatrix();


			objects.push_back(newGround);

		}
	}
}
