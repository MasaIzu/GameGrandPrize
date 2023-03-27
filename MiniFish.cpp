#include "MiniFish.h"
#include"ImGuiManager.h"

void MiniFish::Initialize(const Vector3& pos)
{
	//���[���h�ϊ��̏�����
	world.Initialize();
	//���W�������Őݒ�
	positions[3] = pos;
	world.translation_ = pos;
	world.TransferMatrix();

	//���̃��f��������
	//bodyModel.reset(Model::CreateFromOBJ("FishBody", true));
	//eyeModel.reset(Model::CreateFromOBJ("FishMedama", true));
}

static int count = 0;
void MiniFish::Update(const Vector3& stagePos, float stageRadius)
{
	float spdPerSec = 120.0f;
	//�C�[�W���O�X�V
	easeMove.Update();


	//�C�[�W���O������
	if (!easeMove.GetActive()) {
		//3�_�������_���Ɍ���A�n�_�͑O��̏I�_
		positions[0] = positions[3];

		

		for (int i = 1; i < 4; i++) {

			//�x�W�G�Ȑ��̂��ߑ��x�𕪊�
			float spd = spdPerSec / 4.0f;

			//�X�e�[�W�̊O�ɍs���Ȃ����߂̍��W�ݒ�
			//	ImGui::Text("not active...");
				float rad = Random(0, 360);
				positions[i].x = sin(rad * PI / 180) * spd;
				positions[i].z = cos(rad * PI / 180) * spd;
				positions[i].y = stagePos.y;
				positions[i] += positions[i - 1];
				Vector3 vecPosToStage = stagePos - positions[i];
				if (stageRadius < vecPosToStage.length()) {
					//�X�e�[�W���S����ړ����W�ւ̋������X�e�[�W�̔��a��肨�������Ȃ�������for����
					i--;
				}
			
		}

		//�C�[�W���O�J�n
		easeMove.Start(60);
	}

	


	ImGui::Text("timeRate:%f", easeMove.GetTimeRate());

	if (easeMove.GetActive()) {
		ImGui::Text("active!");
		for (int i = 0; i < 4; i++) {
			ImGui::Text("pos[%d]:%f,%f,%f", i, positions[i].x, positions[i].y, positions[i].z);
		}

	}

	Vector3 pos;
	pos = LerpBezireCubic(positions[0], positions[1], positions[2], positions[3], easeMove.GetTimeRate());
	//���݂̍��W�Ǝ��ɑ���������W���琳�ʂ�������]�s����쐬
	Matrix4 matRot = CreateMatRot(world.translation_, pos);
	world.SetMatRot(matRot);
	world.translation_ = pos;
	world.TransferMatrix();

}

void MiniFish::Draw(ViewProjection viewPro)
{
	//bodyModel->Draw(world, viewPro);
	//eyeModel->Draw(world, viewPro);
}
