#pragma once

// MyFile
#include"DirectXDataTypes.h"

// ��ʓI�ɂ悭�g����T���v�����O���@�̏�Ԃ�ێ�����N���X
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

