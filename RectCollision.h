#pragma once
#include "BaseCollider.h"
#include "Vector3.h"

class Rect{

public:
	Rect(Vector3 WidthHeightDepth_);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(const Matrix4& worldPos,Vector3 WidthHeightDepth);


};