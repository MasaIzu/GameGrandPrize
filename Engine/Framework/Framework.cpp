#include "Framework.h"
#include"ParticleManager.h"
#include"PostEffectCommon.h"
#include"PostEffectBlurH.h"
#include"PostEffectBlurW.h"
#include"PostEffectLuminance.h"
#include"PostEffectMixed.h"

void Framework::Initialize()
{

	// �Q�[���E�B���h�E�̍쐬
	winApp_ = WinApp::GetInstance();
	winApp_->MakeWindow(L"Atoms");

	// DirectX����������
	directXCore_ = DirectXCore::GetInstance();
	directXCore_->DirectXCoreInitialize(winApp_->Gethwnd(), winApp_->window_width, winApp_->window_height);

#pragma region �ėp�@�\������
	// ���͂̏�����
	input_ = Input::GetInstance();
	input_->Initialize();


	// �e�N�X�`���}�l�[�W���̏�����
	TextureManager_ = TextureManager::GetInstance();
	TextureManager_->Initialize(directXCore_->GetDevice());
	TextureManager::Load("white1x1.png");

	// FBX�֘A�ÓI������
	fbxLoader_ = FbxLoader::GetInstance();
	fbxLoader_->Initialize(directXCore_->GetDevice());

	// �X�v���C�g�ÓI������
	Sprite::StaticInitialize(directXCore_->GetDevice());


	// 3D���f���ÓI������
	Model::StaticInitialize();
	FbxModel::StaticInitialize();

	//Imgui������
	imGui = std::make_unique <ImGuiManager>();
	imGui->Initialize(winApp_, directXCore_);

	fps = std::make_unique<FPS>();

	ParticleManager::StaticInitialize(DirectXCore::GetInstance()->GetDevice());

	PostEffectCommon::StaticInitialize(DirectXCore::GetInstance());

	PostEffectBlurH::Initialize();

	PostEffectBlurW::Initialize();

	PostEffectLuminance::Initialize();

	PostEffectMixed::Initialize();

	// �T�E���h�̐ÓI�̏�����
	Sound::StaticInitialize();

#pragma endregion

	//�V�[���}�l�[�W���[�̐���
	sceneManager_ = SceneManager::GetInstance();


}

void Framework::Update()
{

	//fps����
	fps->FpsControlBegin();

	// ���b�Z�[�W����
	if (winApp_->ProcessMessage()) {
		isEndRequst = true;
	}

	// ���͊֘A�̖��t���[������
	input_->Update();

	//Imgui�̍X�V
	imGui->Bigin();

	//�f���E�B���h�E�̕\���I��
	//ImGui::ShowDemoWindow();

	//�V�[���}�l�[�W���[�̍X�V
	sceneManager_->Update();

	//Imgui�̃R�}���h��t�I��
	imGui->End();
}



void Framework::Finalize()
{
	// �e����
	sceneManager_->Finalize();

	ParticleManager::StaticFinalize();

	imGui->Finalize();
	sceneFactory_.reset();

	FbxModel::StaticFainalize();
	Model::StaticFinalize();

	Sprite::StaticFinalize();

	fbxLoader_->Finalize();

	TextureManager_->Delete();

	input_->Destroy();

	directXCore_->Destroy();

	// �Q�[���E�B���h�E�̔j��
	winApp_->DeleteGameWindow();
	winApp_->Destroy();


}

bool Framework::isPlayMyGame()
{
	return isEndRequst;
}

void Framework::Run()
{

	//�Q�[���̏�����
	Initialize();

	while (true) {//�Q�[�����[�v

		//���t���[���X�V
		Update();
		//�I�����N�G�X�g�������甲����
		if (isPlayMyGame()) {
			break;
		}
		PostEffectLuminance::PreDrawScene(directXCore_->GetCommandList());

		PostEffectDraw();

		PostEffectLuminance::PostDrawScene();

		PostEffectBlurW::PreDrawScene(directXCore_->GetCommandList());

		PostEffectLuminance::Draw();

		PostEffectBlurW::PostDrawScene();

		PostEffectBlurH::PreDrawScene(directXCore_->GetCommandList());

		PostEffectBlurW::Draw();

		PostEffectBlurH::PostDrawScene();

		PostEffectMixed::PreDrawScene(directXCore_->GetCommandList());

		PostEffectBlurH::Draw();

		PostEffectMixed::PostDrawScene();
		// �`��J�n
		directXCore_->PreDraw();

		PostEffectMixed::Draw(PostEffectLuminance::GettextureHandle());

		Draw();


		//ImGui�`��
		imGui->Draw();

		

		// �`��I��
		directXCore_->PostDraw();

		//FPS�Œ�
		fps->FpsControlEnd();

		if (input_->TriggerKey(DIK_ESCAPE)) {
			break;
		}

	}

	//�Q�[���̏I��
	Finalize();


}
