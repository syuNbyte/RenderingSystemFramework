
// TNLIB
#include"Debug.hpp"

// MyFile
#include"PostProcessBuffer.h"
#include"Application.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

DXPostProcessBuffer& DXPostProcessBuffer::GetInstance() {
	static DXPostProcessBuffer postProcessBuffer;
	return postProcessBuffer;
}

void DXPostProcessBuffer::CreateRimRenderTarget(DirectXDevice* device) {
}

void DXPostProcessBuffer::CreateBloomRenderTarget(DirectXDevice* device) {
	auto& instance = DXPostProcessBuffer::GetInstance();
	Texture2DDesc texDesc;
	RenderTargetViewDesc rtvDesc;
	ShaderResourceViewDesc srvDesc;

	// Texture2DDescÇÃèâä˙âª
	{
		ZeroMemory(&texDesc, sizeof(Texture2DDesc));
		texDesc.Width = Application::GetWindowSize().x;
		texDesc.Height = Application::GetWindowSize().y;
		texDesc.MipLevels = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.ArraySize = 1;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = false;
	}

	// RenderTargetViewDescÇÃèâä˙âª
	{
		ZeroMemory(&rtvDesc, sizeof(RenderTargetViewDesc));
		rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;// Texture2DDesc.FormatÇ∆ìØÇ∂
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
	}

	// ShaderResourceViewDescÇÃèâä˙âª
	{
		ZeroMemory(&srvDesc, sizeof(ShaderResourceViewDesc));
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
	}

	instance.m_bloom.SetTexture2DDesc(texDesc);
	instance.m_bloom.SetRenderTargetViewDesc(rtvDesc);
	instance.m_bloom.SetShaderResourceViewDesc(srvDesc);
	instance.m_bloom.Create(device);
}

void DXPostProcessBuffer::CreateVignetteRenderTarget(DirectXDevice* device) {
	auto& instance = DXPostProcessBuffer::GetInstance();
	Texture2DDesc texDesc;
	RenderTargetViewDesc rtvDesc;
	ShaderResourceViewDesc srvDesc;

	// Texture2DDescÇÃèâä˙âª
	{
		ZeroMemory(&texDesc, sizeof(Texture2DDesc));
		texDesc.Width = Application::GetWindowSize().x;
		texDesc.Height = Application::GetWindowSize().y;
		texDesc.MipLevels = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.ArraySize = 1;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = false;
	}

	// RenderTargetViewDescÇÃèâä˙âª
	{
		ZeroMemory(&rtvDesc, sizeof(RenderTargetViewDesc));
		rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;// Texture2DDesc.FormatÇ∆ìØÇ∂
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
	}

	// ShaderResourceViewDescÇÃèâä˙âª
	{
		ZeroMemory(&srvDesc, sizeof(ShaderResourceViewDesc));
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
	}

	instance.m_vignette.SetTexture2DDesc(texDesc);
	instance.m_vignette.SetRenderTargetViewDesc(rtvDesc);
	instance.m_vignette.SetShaderResourceViewDesc(srvDesc);
	instance.m_vignette.Create(device);
}

void DXPostProcessBuffer::CreateSSAORenderTarget(DirectXDevice* device) {
	auto& instance = DXPostProcessBuffer::GetInstance();
	Texture2DDesc texDesc;
	RenderTargetViewDesc rtvDesc;
	ShaderResourceViewDesc srvDesc;

	// Texture2DDescÇÃèâä˙âª
	{
		ZeroMemory(&texDesc, sizeof(Texture2DDesc));
		texDesc.Width = Application::GetWindowSize().x;
		texDesc.Height = Application::GetWindowSize().y;
		texDesc.MipLevels = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.ArraySize = 1;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = false;
	}

	// RenderTargetViewDescÇÃèâä˙âª
	{
		ZeroMemory(&rtvDesc, sizeof(RenderTargetViewDesc));
		rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;// Texture2DDesc.FormatÇ∆ìØÇ∂
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
	}

	// ShaderResourceViewDescÇÃèâä˙âª
	{
		ZeroMemory(&srvDesc, sizeof(ShaderResourceViewDesc));
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
	}

	instance.m_vignette.SetTexture2DDesc(texDesc);
	instance.m_vignette.SetRenderTargetViewDesc(rtvDesc);
	instance.m_vignette.SetShaderResourceViewDesc(srvDesc);
	instance.m_vignette.Create(device);
}

void DXPostProcessBuffer::CreateSSRRenderTarget(DirectXDevice* device) {
}

void DXPostProcessBuffer::Initialize(DirectXDevice* device) {
	Debug::BracketBegin("DXPostProcessBuffer::Initialize");

	DXPostProcessBuffer::CreateBloomRenderTarget(device);
	DXPostProcessBuffer::CreateVignetteRenderTarget(device);

	Debug::BracketEnd();
}

ComObject::RenderTargetView DXPostProcessBuffer::GetBloomRTV() {
	auto& instance = DXPostProcessBuffer::GetInstance();
	return instance.m_bloom.GetRTV();
}

ComObject::RenderTargetView DXPostProcessBuffer::GetVignetteRTV() {
	auto& instance = DXPostProcessBuffer::GetInstance();
	return instance.m_vignette.GetRTV();
}

ComObject::ShaderResourceView DXPostProcessBuffer::GetBloomSRV() {
	auto& instance = DXPostProcessBuffer::GetInstance();
	return instance.m_bloom.GetSRV();
}

ComObject::ShaderResourceView DXPostProcessBuffer::GetVignetteSRV() {
	auto& instance = DXPostProcessBuffer::GetInstance();
	return instance.m_vignette.GetSRV();
}
