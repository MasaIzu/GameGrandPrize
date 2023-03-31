#include "recovery.h"

void Recovery::Initialize()
{

	pointModel.reset(Model::CreateFromOBJ("recoveryPoint", true));

	pointWorldTransform.Initialize();

	pointWorldTransform.scale_ = { 3,3,3 };

	pointWorldTransform.translation_ = { 0,-1,0 };

	tex1 = TextureManager::GetInstance()->Load("recovery/tex1.png");

	tex2 = TextureManager::GetInstance()->Load("recovery/tex2.png");

	ParticleMan = std::make_unique<ParticleManager>();

	ParticleMan->Initialize();

	//pointModel->SetTextureHandle(tex2);
}

void Recovery::Update()
{
	flame++;
	if (flame > intervalFlame)
	{
		for (int i = 0; i < 3; i++)
		{
			flame = 0;
			int randVertex = rand() % 32;

			float x = pointWorldTransform.translation_.x + sin(11.25 * randVertex * 3.141592 / 180) * scale;

			float z = pointWorldTransform.translation_.z + cos(11.25 * randVertex * 3.141592 / 180) * scale;
			//’Ç‰Á
			ParticleMan->Add(ParticleManager::Type::Normal, 100, { x,0,z }, { x,5,z }, 0.5, 0.5, { 0,1,0,0.5 }, { 0,1,0,0.1 });
		}
	}
	ParticleMan->Update();

	pointWorldTransform.TransferMatrix();
}

void Recovery::PostEffectDraw(ViewProjection view)
{
	pointModel->Draw(pointWorldTransform, view);
}

void Recovery::particleDraw(ViewProjection view)
{
	ParticleMan->Draw(view);
}

void Recovery::Draw(ViewProjection view)
{
	pointModel->Draw(pointWorldTransform, view);
}
