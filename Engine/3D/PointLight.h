#pragma once

#include"Vector3.h"
class PointLight
{
private://エイリアス
	//Microsoft::WRL::を省略
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public://サブクラス
	struct ConstBufferData
	{
		Vector3 lightpos;
		float pad1;

		Vector3 lightcolor;
		float pad2;

		Vector3 lightatten;
		unsigned int active;
	};

public:

	inline void SetLightPos(const Vector3& lightpos) {
		this->lightpos = lightpos;
	}
	inline const Vector3& GetLightPos() { return lightpos; }
	inline void SetLightColor(const Vector3& lightcolor) {
		this->lightcolor = lightcolor;
	}
	inline const Vector3& GetLightColor() { return lightcolor; }
	inline void SetLightAtten(const Vector3& lightAtten) {
		this->lightAtten = lightAtten;
	}
	inline const Vector3& GetLightAtten() { return lightAtten; }
	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }



private:

	Vector3 lightpos = { 0,0,0 };
	Vector3 lightcolor = { 1,1,1 };
	Vector3 lightAtten = { 1.0f,1.0f,1.0f };
	bool active = false;

};