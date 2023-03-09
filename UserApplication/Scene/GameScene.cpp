#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <fstream>
#include "FbxLoader.h"
#include"ImGuiManager.h"


GameScene::GameScene() {}

GameScene::~GameScene() {
	model_.reset();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCore::GetInstance();
	winApp_ = WinApp::GetInstance();
	input_ = Input::GetInstance();

	model_.reset(Model::CreateFromOBJ("UFO", true));

	viewProjection_.eye = { 0,0,5 };

	sceneManager_ = SceneManager::GetInstance();

	viewProjection_.Initialize();
	viewProjection_.UpdateMatrix();

	worldTransform_.Initialize();
	//worldTransform_.translation_ = { 0,0,100 };
	//worldTransform_.rotation_ = { 0,0,0 };
	//worldTransform_.scale_ = { 0.1f,0.1f,0.1f };
	//worldTransform_.TransferMatrix();

	/*fbxmodel = std::make_unique<FbxModel>();
	
	fbxmodel.reset(FbxLoader::GetInstance()->LoadModelFromFile("lowpoliHitokunBoss"));
	fbxmodel->Initialize();*/

	boss.Initialize();

	for (int i = 0; i < 200; i++) {
		boss.CreateFish(Random(-boss.fishParent.radius, boss.fishParent.radius));
	}

	model_->SetPolygonExplosion({0.0f,1.0f,0.0f,0.0f});
	ParticleMan = std::make_unique<ParticleManager>();
	ParticleMan->Initialize();

	UINT tex = TextureManager::GetInstance()->Load("effect4.png");
	ParticleMan->SetTextureHandle(tex);
}

void GameScene::Update() {
	
	if (input_->TriggerKey(DIK_SPACE))
	{
		sceneManager_->ChangeScene("TITLE");
	}

	ImGui::Begin("Phase");

	ImGui::Text("ParticleListSize:%d", ParticleMan->GetParticlesListSize());


	ImGui::End();

	boss.Update();
	viewProjection_.UpdateMatrix();

	if (input_->TriggerKey(DIK_Z)) {
		//�X�y�[�X�L�[�������Ă�����
		for (int i = 0; i < 30; i++)
		{
			const float rnd_life = 290.0f;
			float life = (float)rand() / RAND_MAX * rnd_life - rnd_life / 2.0f + 10;

			//X,Y,Z�S��[-5.0,+5.0f]�Ń����_���ɕ��z
			const float rnd_pos = 10.0f;
			Vector3 pos{};
			pos.x = (float)rand() / RAND_MAX * rnd_pos  - rnd_pos / 2.0f;
			pos.y = abs((float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f) + 1;
			pos.z = (float)rand() / RAND_MAX * rnd_pos  - rnd_pos / 2.0f;
			//�ǉ�
			ParticleMan->InAdd(life, { 0,-3,0 }, pos, 0.1f, 0.1f, { 1,1,1,1 }, { 1,1,1,1 });
		}
		////�X�y�[�X�L�[�������Ă�����
		//for (int i = 0; i < 50; i++)
		//{

		//	//X,Y,Z�S��[-5.0,+5.0f]�Ń����_���ɕ��z
		//	const float rnd_pos = 10.0f;
		//	Vector3 pos{};
		//	pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//	pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//	pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//	//�ǉ�
		//	ParticleMan->InAdd(60, pos, {0,0,0}, 1.0f, 1.0f, { 1,1,0,0.5 }, { 1,1,1,1 });
		//}
	}

	ParticleMan->Update();

}

void GameScene::Draw() {

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	
	// �[�x�o�b�t�@�N���A
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	//// 3D�I�u�W�F�N�g�`��O����
	Model::PreDraw(commandList);

	//model_->Draw(worldTransform_, viewProjection_);

	/*for (int i = 0; i < boss.fishes.size(); i++) {
		model_->Draw(boss.fishes[i].pos, viewProjection_);
	}

	boss.Draw(viewProjection_);*/

	// 3D�I�u�W�F�N�g�`��㏈��
	Model::PostDraw();

	FbxModel::PreDraw(commandList);

	//fbxmodel->Draw(worldTransform_, viewProjection_);

	FbxModel::PostDraw();

	ParticleManager::PreDraw(commandList);

	ParticleMan->Draw(viewProjection_);

	ParticleManager::PostDraw();


#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	


#pragma endregion
}

void GameScene::Finalize()
{
}
