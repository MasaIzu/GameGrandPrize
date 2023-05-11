#pragma once
#include"WorldTransform.h"
#include"Model.h"
#include<vector>
#include<memory>
#include"EasingData.h"
#include"ViewProjection.h"
#include <BaseCollider.h>
#include"Sprite.h"
#include"BossFish.h"
#include"BossWarrier.h"

class Boss
{

public:

	std::unique_ptr<BossFish> bossFish;

	std::unique_ptr<BossWarrier> bossWarrier;

	~Boss();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="targetPos">�W�I�̍��W</param>
	void Update(const Vector3& targetPos, const Vector3 stagePos, float stageRadius);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProMat">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProMat);

	/// <summary>
	/// �̗͂̕`��
	/// </summary>
	void DrawHealth();

	void Reset();


	/// <summary>
	/// �����o�֐�(�v���C�x�[�g)
	/// </summary>
private:


	/// <summary>
	/// �����o�ϐ�(�v���C�x�[�g)
	/// </summary>
private:

	

};
