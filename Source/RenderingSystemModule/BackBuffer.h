#pragma once

// MyFile
#include"DirectXUtilityModule//DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXColor.h"

class DXBackBuffer {
private:
	ComObject::Texture2D m_buf;
	ComObject::RenderTargetView m_rtv;
	ComObject::ShaderResourceView m_srv;
private:
	Texture2DDesc m_texDesc;
	RenderTargetViewDesc m_rtvDesc;
	ShaderResourceViewDesc m_srvDesc;
private:
	static DXBackBuffer& GetInstance();
	static void CreateDesc();
	static void GetBackBuffer(SwapChain* swapChain);
	static void CreateRenderTargetView(DirectXDevice* device);
	static void CreateShaderResourceView(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device, SwapChain* swapChain);
	static void Clear(DirectXDeviceContext* context, const DXColor& color);
	static ComObject::RenderTargetView GetRTV();
	static ComObject::ShaderResourceView GetSRV();
};