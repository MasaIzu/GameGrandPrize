#pragma once
#include"WorldTransform.h"
#include"ParticleManager.h"
#include"Model.h"
class Recovery
{
public:/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize();

	/// <summary>
	/// –ˆƒtƒŒ[ƒ€ˆ—
	/// </summary>
	void Update();

	void PostEffectDraw(ViewProjection view);

	void particleDraw(ViewProjection view);

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw(ViewProjection view);
private:
	std::unique_ptr<ParticleManager> ParticleMan;

	std::unique_ptr<Model> pointModel;

	WorldTransform pointWorldTransform;

	uint32_t tex1;

	uint32_t tex2;

	const float scale = 3.0f;

	const int intervalFlame=10;

	int flame;

	const float maxVartex = 32;
};

