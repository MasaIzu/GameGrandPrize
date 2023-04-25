#pragma once

#include"Vector3.h"
#include"Vector2.h"

/// <summary>
/// スポットライト
/// </summary>
class SpotLight {
private://エイリアス
	//Microsoft::WRL::を省略
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;


public://サブクラス
	struct ConstBufferData
	{
		Vector3 lightv;
		Vector3 lightpos;
		float pad1;
		Vector3 lightcolor;
		float pad2;
		Vector3 lightatten;
		float pad3;
		Vector2 lightfactoranglecos;
		unsigned int active;
		float pad4;
		
	};

public:
	//メンバ関数

	inline void SetLightDir(const Vector3& lightdir) { this->lightdir = lightdir; }
	inline const Vector3& GetLightDir() { return lightdir; }
	inline void SetLightPos(const Vector3& lightpos) { this->lightpos = lightpos; }
	inline const Vector3& GetLightPos() { return lightpos; }
	inline void SetLightColor(const Vector3& lightcolor) { this->lightcolor = lightcolor; }
	inline const Vector3& GetLightColor() { return lightcolor; }
	inline void SetLightAtten(const Vector3& lightAtten) { this->lightAtten = lightAtten; }
	inline const Vector3& GetLightAtten() { return lightAtten; }
	inline void SetLightFactorAngle(const Vector2& lightFactorAngle) {
		this->lightFactorAngleCos.x = cosf(DirectX::XMConvertToRadians(lightFactorAngle.x));
		this->lightFactorAngleCos.y = cosf(DirectX::XMConvertToRadians(lightFactorAngle.y));
	}
	inline const Vector2& GetLightFactorAngleCos() { return lightFactorAngleCos; }
	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }


private:

	//ライトの方向
	Vector3 lightdir = { 1,0,0 };
	//ライト座標
	Vector3 lightpos = { 0,0,0 };
	//ライトの色
	Vector3 lightcolor = { 1,1,1 };
	//ライトの距離減衰係数
	Vector3 lightAtten = { 1.0f,1.0f,1.0f };
	//ライト減衰角度
	Vector2 lightFactorAngleCos = { 0.5f,0.2f };
	//有効フラグ
	bool active = false;


};