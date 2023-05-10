#include "BossWarrier.h"

void BossWarrier::Initialize()
{
	//�{�X2�̃��f��������
	//Root�͑�{�œ������e�p�Ȃ̂ŋ�f�[�^��OK
	boss2Model[BossWarrierPart::Chest].reset(Model::CreateFromOBJ("Boss_Body", true));
	boss2Model[BossWarrierPart::Head].reset(Model::CreateFromOBJ("Boss_Head", true));
	boss2Model[BossWarrierPart::Waist].reset(Model::CreateFromOBJ("Boss_Waist", true));
	boss2Model[BossWarrierPart::ArmL].reset(Model::CreateFromOBJ("Boss_ShoulderL", true));
	boss2Model[BossWarrierPart::ArmR].reset(Model::CreateFromOBJ("Boss_ShoulderR", true));
	boss2Model[BossWarrierPart::HandL].reset(Model::CreateFromOBJ("Boss_ArmL", true));
	boss2Model[BossWarrierPart::HandR].reset(Model::CreateFromOBJ("Boss_ArmR", true));

	//�{�X���`�Ԃ̊e���ʏ�����
	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Transform[i].Initialize();
	}

	//���͑�{��e�Ɏ���
	boss2Transform[BossWarrierPart::Chest].parent_ = &boss2Transform[BossWarrierPart::Root];
	//���A���A���r�͋���e�Ɏ���
	boss2Transform[BossWarrierPart::Head].parent_ = &boss2Transform[BossWarrierPart::Chest];
	boss2Transform[BossWarrierPart::Waist].parent_ = &boss2Transform[BossWarrierPart::Chest];
	boss2Transform[BossWarrierPart::ArmL].parent_ = &boss2Transform[BossWarrierPart::Chest];
	boss2Transform[BossWarrierPart::ArmR].parent_ = &boss2Transform[BossWarrierPart::Chest];
	//���ꂼ��̎�͂��ꂼ��̘r��e�Ɏ���
	boss2Transform[BossWarrierPart::HandL].parent_ = &boss2Transform[BossWarrierPart::ArmL];
	boss2Transform[BossWarrierPart::HandR].parent_ = &boss2Transform[BossWarrierPart::ArmR];

	//�{�X�̃X�P�[����5�{��
	boss2Transform[BossWarrierPart::Root].scale_ = { 5,5,5 };

	for (int i = 0; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Transform[i].TransferMatrix();
	}
}

void BossWarrier::Draw(const ViewProjection& viewProMat)
{
	for (int i = BossWarrierPart::Chest; i < BossWarrierPart::Boss2PartMax; i++) {
		boss2Model[i]->Draw(boss2Transform[i], viewProMat);
	}
}
