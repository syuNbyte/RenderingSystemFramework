#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"

// DirectXのラスタライザを扱うクラス
class DXRasterizer {
protected:
	ComObject::RasterizerState m_state;
protected:
	RasterizerDesc m_desc;
public:
	DXRasterizer();
	DXRasterizer(const DXRasterizer& rasterizer);
	DXRasterizer(DXRasterizer&& rasterizer);
public:
	// ラスタライザを作成する
	void Create(DirectXDevice* device, const RasterizerDesc& desc);

	// ラスタライザを設定する
	void RSSetState(DirectXDeviceContext* context);
};