#pragma once

// MyFile
#include"DirectXDataTypes.h"

// 一般的によく使われるサンプリング方法の状態を保持するクラス
class SamplerStateContainer {
private:
	ComObject::SamplerState m_point;
	ComObject::SamplerState m_linear;
private:
	SamplerDesc m_pointDesc;
	SamplerDesc m_linearDesc;
private:
	static SamplerStateContainer& GetInstance();
	static void CreatePointSampler();
	static void CreateLinearSampler();
public:
	static void Initialize();
	static ComObject::SamplerState GetPoint();
	static ComObject::SamplerState GetLinear();
};

