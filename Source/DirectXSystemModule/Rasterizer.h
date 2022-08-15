#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"

// DirectX�̃��X�^���C�U�������N���X
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
	// ���X�^���C�U���쐬����
	void Create(DirectXDevice* device, const RasterizerDesc& desc);

	// ���X�^���C�U��ݒ肷��
	void RSSetState(DirectXDeviceContext* context);
};