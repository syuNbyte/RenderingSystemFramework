#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXVector.h"
#include"DirectXUtilityModule/DirectXColor.h"

// DirectX‚ÌRenderTarget‚ðˆµ‚¤ƒNƒ‰ƒX
class DXRenderTarget {
protected:
	ComObject::Texture2D m_tex;
	ComObject::RenderTargetView m_rtv;
	ComObject::ShaderResourceView m_srv;
protected:
	Texture2DDesc m_texDesc;
	RenderTargetViewDesc m_rtvDesc;
	ShaderResourceViewDesc m_srvDesc;
protected:
	void CreateTexture2D(DirectXDevice* device);
	void CreateRenderTargetView(DirectXDevice* device);
	void CreateShaderResourceView(DirectXDevice* device);
public:
	void Create(DirectXDevice* device);
	void Clear(DirectXDeviceContext* context, const DXColor& color);
	void SetTexture2DDesc(const Texture2DDesc& desc);
	void SetRenderTargetViewDesc(const RenderTargetViewDesc& desc);
	void SetShaderResourceViewDesc(const ShaderResourceViewDesc& desc);
public:
	ComObject::RenderTargetView GetRTV();
	ComObject::ShaderResourceView GetSRV();
public:
	DXVector2Int GetRenderTargetSize();
};