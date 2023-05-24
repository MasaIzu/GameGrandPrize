#include "recovery.h"
#include"ImGuiManager.h"

void Recovery::Initialize()
{

	pointModel_.reset(Model::CreateFromOBJ("recoveryPoint", true));
	{
		LightGroup light = pointModel_->GetLigit();

		light.SetDirLightActive(0, true);
		light.SetDirLightActive(1, false);
		light.SetDirLightActive(2, false);

		light.SetDirLightColor(0, { 0,5,0 });
		light.SetDirLightColor(1, { 0,5,0 });
		light.SetDirLightColor(2, { 0,5,0 });

		pointModel_->SetLight(light);
	}

	pointWorldTransform_.Initialize();

	pointWorldTransform_.scale_ = { 3,3,3 };

	pointWorldTransform_.translation_ = { 25,-1.5,25 };

	pointWorldTransform_.TransferMatrix();

	tex1 = TextureManager::GetInstance()->Load("recovery/tex1.png");

	tex2 = TextureManager::GetInstance()->Load("recovery/tex2.png");

	model_.reset(Model::CreateFromOBJ("recovery", true));

	for (int i = 0; i < 20; i++)
	{
		worldTransform_[i].Initialize();

		int randVertex = rand() % 32;

		float x = pointWorldTransform_.translation_.x + sin(11.25 * randVertex * 3.141592 / 180) * scale;

		float z = pointWorldTransform_.translation_.z + cos(11.25 * randVertex * 3.141592 / 180) * scale;

		worldTransform_[i].translation_.z = z;

		worldTransform_[i].translation_.x = x;

		worldTransform_[i].translation_.y = (rand() % 500) / 100;

		worldTransform_[i].scale_ = { 0.3,0.3,0.3 };

		worldTransform_[i].alpha = 1 - worldTransform_[i].translation_.y * 0.2;
	}
	{
		LightGroup light = model_->GetLigit();

		light.SetDirLightActive(2, true);
		light.SetDirLightActive(1, true);
		light.SetDirLightActive(0, true);

		light.SetDirLightColor(0, { 0,5,0 });
		light.SetDirLightColor(1, { 0,5,0 });
		light.SetDirLightColor(2, { 0,5,0 });

		model_->SetLight(light);
	}
}

void Recovery::Update()
{
	//for (int i = 0; i < 3; i++)
	//{
	//	flame = 0;
	//	int randVertex = rand() % 32;

	//	float x = pointWorldTransform_.translation_.x + sin(11.25 * randVertex * 3.141592 / 180) * scale;

	//	float z = pointWorldTransform_.translation_.z + cos(11.25 * randVertex * 3.141592 / 180) * scale;
	//	//追加
	//	ParticleMan->Add(ParticleManager::Type::Normal, 100, { x,0,z }, { x,5,z }, 0.5, 0.5, { 0,1,0,0.5 }, { 0,1,0,0.1 });
	//}
	if (isActive == true)
	{
		for (int i = 0; i < 20; i++)
		{
			if (worldTransform_[i].translation_.y >= 5)
			{
				worldTransform_[i].translation_.y = 0;

				int randVertex = rand() % 32;

				float x = pointWorldTransform_.translation_.x + sin(11.25 * randVertex * 3.141592 / 180) * scale;

				float z = pointWorldTransform_.translation_.z + cos(11.25 * randVertex * 3.141592 / 180) * scale;

				worldTransform_[i].translation_.z = z;

				worldTransform_[i].translation_.x = x;

				worldTransform_[i].alpha = 1;
			}

			worldTransform_[i].translation_.y += 0.05;

			if (worldTransform_[i].alpha > 0.1f)
			{
				//worldTransform_[i].alpha -= 0.01;
			}

			worldTransform_[i].TransferMatrix();
		}
	}
	else
	{
		IntervalFlame++;
		if (IntervalFlame >= MaxIntervalFlame)
		{
			isActive = true;
			LightGroup light = pointModel_->GetLigit();

			light.SetDirLightActive(0, true);
			light.SetDirLightActive(1, true);
			light.SetDirLightActive(2, true);

			pointModel_->SetLight(light);
			//pointModel_->SetTextureHandle(tex1);
		}
	}

	pointWorldTransform_.TransferMatrix();

	ImGui::Begin("recovery");

	ImGui::Text("HP:%f", IntervalFlame);

	ImGui::End();
}

void Recovery::Draw(ViewProjection view)
{

	if (isActive == true)
	{
		for (int i = 0; i < 20; i++)
		{
			model_->Draw(worldTransform_[i], view);
		}
		pointModel_->Draw(pointWorldTransform_, view);
	}
	else {
		pointModel_->Draw(pointWorldTransform_, view, tex2);
	}
}

void Recovery::Collision()
{
	isActive = false;
	LightGroup light = pointModel_->GetLigit();

	light.SetDirLightActive(0, false);
	light.SetDirLightActive(1, false);
	light.SetDirLightActive(2, false);

	pointModel_->SetLight(light);
	IntervalFlame = 0;
	//pointModel_->SetTextureHandle(tex2);
}

void Recovery::Reset()
{

	for (int i = 0; i < 20; i++)
	{

		int randVertex = rand() % 32;

		float x = pointWorldTransform_.translation_.x + sin(11.25 * randVertex * 3.141592 / 180) * scale;

		float z = pointWorldTransform_.translation_.z + cos(11.25 * randVertex * 3.141592 / 180) * scale;

		worldTransform_[i].translation_.z = z;

		worldTransform_[i].translation_.x = x;

		worldTransform_[i].translation_.y = (rand() % 500) / 100;

		worldTransform_[i].alpha = 1 - worldTransform_[i].translation_.y * 0.2;
	}
	IntervalFlame = 0;

	isActive = true;
}

Vector3 Recovery::GetWorldPosition() {

	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列移動成分を取得(ワールド座標)
	worldPos.x = pointWorldTransform_.matWorld_.m[3][0];
	worldPos.y = pointWorldTransform_.matWorld_.m[3][1];
	worldPos.z = pointWorldTransform_.matWorld_.m[3][2];

	return worldPos;
}
