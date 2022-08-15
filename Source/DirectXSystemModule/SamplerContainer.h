#pragma once

// MyFile
#include"Sampler.h"

// ��ʓI�ɂ悭�g����T���v�����O���@��ێ�����N���X
class DXSamplerContainer {
private:
	DXSampler m_point;
	DXSampler m_linear;
private:
	static DXSamplerContainer& GetInstance();
	static void CreatePointSampler(DirectXDevice* device);
	static void CreateLinearSampler(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static DXSampler GetPointSampler();
	static DXSampler GetLinearSampler();
};