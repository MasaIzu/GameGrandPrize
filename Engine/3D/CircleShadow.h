#pragma once

#include<DirectXMath.h>
#include"Vector3.h"

/// <summary>
/// 丸影
/// </summary>
class CircleShadow {

private://エイリアス
	//Microsoft::WRL::を省略
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//サブクラス
	struct ConstBufferData {
		Vector3 dir;
		Vector3 casterPos;
		float distanceCasterLight;
		Vector3 atten;
		float pad3;
		Vector2 factorAngleCos;
		unsigned int active;
		float pad4;
	};
	
public:
	//メンバ関数

	inline void SetDir(const Vector3& dir) { this->dir =dir; }
	inline const Vector3& GetDir() { return dir; }

	inline void SetCasterPos(const Vector3& casterPos) { this->casterPos = casterPos; }
	inline const Vector3& GetCasterPos() { return casterPos; }

	inline void SetDistanceCasterLight(float distanceCasterLight) { this->distanceCasterLight = distanceCasterLight; }
	inline float GetDistanceCasterLight() { return distanceCasterLight; }

	inline void SetAtten(const Vector3& atten) { this->atten = atten; }
	inline const Vector3& GetAtten() { return atten; }

	inline void SetFactorAngle(const Vector2& factorAngle) {
		this->factorAngleCos.x = cosf(DirectX::XMConvertToRadians(factorAngle.x));
		this->factorAngleCos.y = cosf(DirectX::XMConvertToRadians(factorAngle.y));
	}

	inline const Vector2& GetFactorAngleCos() { return factorAngleCos; }
	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }

private:
	//方向
	Vector3 dir = { 1,0,0 };

	float distanceCasterLight = 100.0f;

	Vector3 casterPos = { 0,0,0 };

	Vector3 atten = { 0.5f,0.6f,0.0f };

	Vector2 factorAngleCos = { 0.2f,0.5f };

	bool active = false;

};