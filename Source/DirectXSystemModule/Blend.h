#pragma once

// STL
#include<array>

// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"

// DirectXのブレンド方法を扱うクラス
class DXBlend {
protected:
	ComObject::BlendState m_state;
protected:
	BlendDesc m_desc;
protected:
	std::array<float_t, 4> m_factor;	// ブレンディング係数
	uint_t m_mask;							// サンプリングのマスク値
public:
	DXBlend();
	DXBlend(const DXBlend& blend);
	DXBlend(DXBlend&& blend);
public:
	// Blendを作成する
	void Create(DirectXDevice* device, const BlendDesc& desc);

	// ブレンディング係数を設定する
	void SetBlendFactor(float_t factor[4]);

	// サンプリングのマスク地を設定する
	void SetBlendMask(const uint_t& mask);
public:
	// OMSetBlendStateのラッパー
	void OMSetBlendState(DirectXDeviceContext* context);
};