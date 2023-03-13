#pragma once

#include "DirectXCore.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin.h"
#include"ParticleManager.h"

#include "BaseScene.h"
#include "SceneManager.h"

#include "FbxModel.h"

#include<memory>
#include<vector>

#include "BoxCollision.h"

#include "Player.h"
#include "GameCamera.h"
#include"Boss.h"
#include <CollisionManager.h>

/// <summary>
/// �Q�[���V�[��
/// </summary>
class GameScene : public BaseScene {

public: // �����o�֐�
	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	GameScene();

public:


	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameScene();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	// �I������
	void Finalize() override;


private: // �����o�ϐ�
	WinApp* winApp_ = nullptr;
	DirectXCore* dxCommon_ = nullptr;
	Input* input_ = nullptr;


	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0;
	// 3D���f��
	std::unique_ptr<Model> model_;

	std::unique_ptr<Player> player;
	std::unique_ptr<GameCamera> gameCamera;

	//�����蔻��
	CollisionManager* collisionManager = nullptr;

	//�{�b�N�X�����蔻��
	std::unique_ptr <BoxCollision> boxCollision;

	//�V�[���}�l�[�W���[
	SceneManager* sceneManager_ = nullptr;

	//Fbx���f��
	//std::unique_ptr<FbxModel> fbxmodel;

	Boss boss;
	//�f�o�b�O�ɂ�鐶���p
	float newFishPosY = 0;

	std::unique_ptr< ParticleManager> ParticleMan;
};