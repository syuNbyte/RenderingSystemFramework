
// TNLIB
#include"Debug.hpp"

// MyFile
#include"GraphicBuffer.h"
#include"Application.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

DXGraphicBuffer& DXGraphicBuffer::GetInstance() {
	static DXGraphicBuffer graphicBuffer;
	return graphicBuffer;
}

void DXGraphicBuffer::CreateAlbedoRenderTarget(DirectXDevice* device) {
	auto& instance = DXGraphicBuffer::GetInstance();
	Texture2DDesc texDesc;
	RenderTargetViewDesc rtvDesc;
	ShaderResourceViewDesc srvDesc;

	// Texture2DDescの初期化
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

	// RenderTargetViewDescの初期化
	{
		ZeroMemory(&rtvDesc, sizeof(RenderTargetViewDesc));
		rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;// Texture2DDesc.Formatと同じ
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
	}

	// ShaderResourceViewDescの初期化
	{
		ZeroMemory(&srvDesc, sizeof(ShaderResourceViewDesc));
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
	}

	instance.m_albedo.SetTexture2DDesc(texDesc);
	instance.m_albedo.SetRenderTargetViewDesc(rtvDesc);
	instance.m_albedo.SetShaderResourceViewDesc(srvDesc);
	instance.m_albedo.Create(device);
}

void DXGraphicBuffer::CreateNormalRenderTarget(DirectXDevice* device) {
	auto& instance = DXGraphicBuffer::GetInstance();
	Texture2DDesc texDesc;
	RenderTargetViewDesc rtvDesc;
	ShaderResourceViewDesc srvDesc;

	// Texture2DDescの初期化
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

	// RenderTargetViewDescの初期化
	{
		ZeroMemory(&rtvDesc, sizeof(RenderTargetViewDesc));
		rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // Texture2DDesc.Formatと同じ
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
	}

	// ShaderResourceViewDescの初期化
	{
		ZeroMemory(&srvDesc, sizeof(ShaderResourceViewDesc));
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
	}

	instance.m_normal.SetTexture2DDesc(texDesc);
	instance.m_normal.SetRenderTargetViewDesc(rtvDesc);
	instance.m_normal.SetShaderResourceViewDesc(srvDesc);
	instance.m_normal.Create(device);
}

void DXGraphicBuffer::CreatePositionRenderTarget(DirectXDevice* device) {
	auto& instance = DXGraphicBuffer::GetInstance();
	Texture2DDesc texDesc;
	RenderTargetViewDesc rtvDesc;
	ShaderResourceViewDesc srvDesc;

	// Texture2DDescの初期化
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

	// RenderTargetViewDescの初期化
	{
		ZeroMemory(&rtvDesc, sizeof(RenderTargetViewDesc));
		rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;// Texture2DDesc.Formatと同じ
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
	}

	// ShaderResourceViewDescの初期化
	{
		ZeroMemory(&srvDesc, sizeof(ShaderResourceViewDesc));
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
	}

	instance.m_position.SetTexture2DDesc(texDesc);
	instance.m_position.SetRenderTargetViewDesc(rtvDesc);
	instance.m_position.SetShaderResourceViewDesc(srvDesc);
	instance.m_position.Create(device);
}

void DXGraphicBuffer::CreateDepthRenderTarget(DirectXDevice* device) {
	auto& instance = DXGraphicBuffer::GetInstance();
	Texture2DDesc texDesc;
	RenderTargetViewDesc rtvDesc;
	ShaderResourceViewDesc srvDesc;

	// Texture2DDescの初期化
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

	// RenderTargetViewDescの初期化
	{
		ZeroMemory(&rtvDesc, sizeof(RenderTargetViewDesc));
		rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;// Texture2DDesc.Formatと同じ
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
	}

	// ShaderResourceViewDescの初期化
	{
		ZeroMemory(&srvDesc, sizeof(ShaderResourceViewDesc));
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
	}

	instance.m_depth.SetTexture2DDesc(texDesc);
	instance.m_depth.SetRenderTargetViewDesc(rtvDesc);
	instance.m_depth.SetShaderResourceViewDesc(srvDesc);
	instance.m_depth.Create(device);
}

void DXGraphicBuffer::CreateMaterialRenderTarget(DirectXDevice* device) {
	auto& instance = DXGraphicBuffer::GetInstance();
	Texture2DDesc texDesc;
	RenderTargetViewDesc rtvDesc;
	ShaderResourceViewDesc srvDesc;

	// Texture2DDescの初期化
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

	// RenderTargetViewDescの初期化
	{
		ZeroMemory(&rtvDesc, sizeof(RenderTargetViewDesc));
		rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;// Texture2DDesc.Formatと同じ
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
	}

	// ShaderResourceViewDescの初期化
	{
		ZeroMemory(&srvDesc, sizeof(ShaderResourceViewDesc));
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
	}

	instance.m_material.SetTexture2DDesc(texDesc);
	instance.m_material.SetRenderTargetViewDesc(rtvDesc);
	instance.m_material.SetShaderResourceViewDesc(srvDesc);
	instance.m_material.Create(device);
}

void DXGraphicBuffer::CreateBrightnessRenderTarget(DirectXDevice* device) {
	auto& instance = DXGraphicBuffer::GetInstance();
	Texture2DDesc texDesc;
	RenderTargetViewDesc rtvDesc;
	ShaderResourceViewDesc srvDesc;

	// Texture2DDescの初期化
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

	// RenderTargetViewDescの初期化
	{
		ZeroMemory(&rtvDesc, sizeof(RenderTargetViewDesc));
		rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;// Texture2DDesc.Formatと同じ
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
	}

	// ShaderResourceViewDescの初期化
	{
		ZeroMemory(&srvDesc, sizeof(ShaderResourceViewDesc));
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
	}

	instance.m_brightness.SetTexture2DDesc(texDesc);
	instance.m_brightness.SetRenderTargetViewDesc(rtvDesc);
	instance.m_brightness.SetShaderResourceViewDesc(srvDesc);
	instance.m_brightness.Create(device);
}

void DXGraphicBuffer::Initialize(DirectXDevice* device) {
	Debug::BracketBegin("DXGraphicBuffer::Initialize");

	DXGraphicBuffer::CreateAlbedoRenderTarget(device);
	DXGraphicBuffer::CreateNormalRenderTarget(device);
	DXGraphicBuffer::CreatePositionRenderTarget(device);
	DXGraphicBuffer::CreateDepthRenderTarget(device);
	DXGraphicBuffer::CreateMaterialRenderTarget(device);
	DXGraphicBuffer::CreateBrightnessRenderTarget(device);

	Debug::BracketEnd();
}

void DXGraphicBuffer::ClearRenderTargetView(DirectXDeviceContext* context) {
	auto& instance = DXGraphicBuffer::GetInstance();

	// アルベドテクスチャの初期化
	instance.m_albedo.Clear(context, DXColor::Clear());

	// 法線テクスチャの初期化
	instance.m_normal.Clear(context, DXColor::Clear());

	// 座標テクスチャの初期化
	instance.m_position.Clear(context, DXColor::Clear());

	// 深度テクスチャの初期化
	instance.m_depth.Clear(context, DXColor::Clear());

	// 質感テクスチャの初期化
	instance.m_material.Clear(context, DXColor::Clear());

	// 輝度テクスチャの初期化
	instance.m_brightness.Clear(context, DXColor::Clear());
}

ComObject::RenderTargetView DXGraphicBuffer::GetAlbedoRTV() {
	auto& instance = DXGraphicBuffer::GetInstance();
	return instance.m_albedo.GetRTV();
}

ComObject::RenderTargetView DXGraphicBuffer::GetNormalRTV() {
	auto& instance = DXGraphicBuffer::GetInstance();
	return instance.m_normal.GetRTV();
}

ComObject::RenderTargetView DXGraphicBuffer::GetPositionRTV() {
	auto& instance = DXGraphicBuffer::GetInstance();
	return instance.m_position.GetRTV();
}

ComObject::RenderTargetView DXGraphicBuffer::GetDepthRTV() {
	auto& instance = DXGraphicBuffer::GetInstance();
	return instance.m_depth.GetRTV();
}

ComObject::RenderTargetView DXGraphicBuffer::GetMaterialRTV() {
	auto& instance = DXGraphicBuffer::GetInstance();
	return instance.m_material.GetRTV();
}

ComObject::RenderTargetView DXGraphicBuffer::GetBrightnessRTV() {
	auto& instance = DXGraphicBuffer::GetInstance();
	return instance.m_brightness.GetRTV();
}

ComObject::ShaderResourceView DXGraphicBuffer::GetAlbedoSRV() {
	auto& instance = DXGraphicBuffer::GetInstance();
	return instance.m_albedo.GetSRV();
}

ComObject::ShaderResourceView DXGraphicBuffer::GetNormalSRV() {
	auto& instance = DXGraphicBuffer::GetInstance();
	return instance.m_normal.GetSRV();
}

ComObject::ShaderResourceView DXGraphicBuffer::GetPositionSRV() {
	auto& instance = DXGraphicBuffer::GetInstance();
	return instance.m_position.GetSRV();
}

ComObject::ShaderResourceView DXGraphicBuffer::GetDepthSRV() {
	auto& instance = DXGraphicBuffer::GetInstance();
	return instance.m_depth.GetSRV();
}

ComObject::ShaderResourceView DXGraphicBuffer::GetMaterialSRV() {
	auto& instance = DXGraphicBuffer::GetInstance();
	return instance.m_material.GetSRV();
}

ComObject::ShaderResourceView DXGraphicBuffer::GetBrightnessSRV() {
	auto& instance = DXGraphicBuffer::GetInstance();
	return instance.m_brightness.GetSRV();
}

std::vector<RenderTargetView*> DXGraphicBuffer::GetRTVs() {
	auto& instance = DXGraphicBuffer::GetInstance();
	return { {
		{instance.m_albedo.GetRTV().Get()},
		{instance.m_normal.GetRTV().Get()},
		{instance.m_position.GetRTV().Get()},
		{instance.m_depth.GetRTV().Get()},
		{instance.m_material.GetRTV().Get()},
		//{instance.m_brightness.GetRTV().Get()}
	} };
}

DXGraphicBufferResult& DXGraphicBufferResult::GetInstance() {
	static DXGraphicBufferResult graphicBufferResult;
	return graphicBufferResult;
}

void DXGraphicBufferResult::CreateResultRenderTarget(DirectXDevice* device) {
	auto& instance = DXGraphicBufferResult::GetInstance();
	Texture2DDesc texDesc;
	RenderTargetViewDesc rtvDesc;
	ShaderResourceViewDesc srvDesc;

	// Texture2DDescの初期化
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

	// RenderTargetViewDescの初期化
	{
		ZeroMemory(&rtvDesc, sizeof(RenderTargetViewDesc));
		rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;// Texture2DDesc.Formatと同じ
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
	}

	// ShaderResourceViewDescの初期化
	{
		ZeroMemory(&srvDesc, sizeof(ShaderResourceViewDesc));
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
	}

	instance.m_result[0].SetTexture2DDesc(texDesc);
	instance.m_result[0].SetRenderTargetViewDesc(rtvDesc);
	instance.m_result[0].SetShaderResourceViewDesc(srvDesc);
	instance.m_result[0].Create(device);
	instance.m_result[1].SetTexture2DDesc(texDesc);
	instance.m_result[1].SetRenderTargetViewDesc(rtvDesc);
	instance.m_result[1].SetShaderResourceViewDesc(srvDesc);
	instance.m_result[1].Create(device);
}

void DXGraphicBufferResult::Initailize(DirectXDevice* device) {
	Debug::BracketBegin("DXGraphicBufferResult::Initialize");

	auto& instance = DXGraphicBufferResult::GetInstance();
	instance.m_write = 0;
	instance.m_read = 1;
	DXGraphicBufferResult::CreateResultRenderTarget(device);

	Debug::BracketEnd();
}

void DXGraphicBufferResult::ClearRenderTargetView(DirectXDeviceContext* context) {
	auto& instance = DXGraphicBufferResult::GetInstance();

	// 一時バッファを初期化
	context->ClearRenderTargetView(
		instance.m_result[0].GetRTV().Get(),
		DXColor::Clear().rgba.data()
	);

	context->ClearRenderTargetView(
		instance.m_result[1].GetRTV().Get(),
		DXColor::Clear().rgba.data()
	);
}

ComObject::RenderTargetView DXGraphicBufferResult::GetResultRTV1() {
	auto& instance = DXGraphicBufferResult::GetInstance();
	return instance.m_result[0].GetRTV();
}

ComObject::ShaderResourceView DXGraphicBufferResult::GetResultSRV1() {
	auto& instance = DXGraphicBufferResult::GetInstance();
	return instance.m_result[0].GetSRV();
}

ComObject::RenderTargetView DXGraphicBufferResult::GetResultRTV2() {
	auto& instance = DXGraphicBufferResult::GetInstance();
	return instance.m_result[1].GetRTV();
}

ComObject::ShaderResourceView DXGraphicBufferResult::GetResultSRV2() {
	auto& instance = DXGraphicBufferResult::GetInstance();
	return instance.m_result[1].GetSRV();
}

void DXGraphicBufferResult::SwitchRenderTarget() {
	auto& instance = DXGraphicBufferResult::GetInstance();
	instance.m_write	= (instance.m_write == 0)	? 1 : 0;
	instance.m_read	= (instance.m_read == 0)	? 1 : 0;
}

DXRenderTarget& DXGraphicBufferResult::GetWriteRenderTarget() {
	auto& instance = DXGraphicBufferResult::GetInstance();
	return instance.m_result[instance.m_write];
}

DXRenderTarget& DXGraphicBufferResult::GetReadRenderTarget() {
	auto& instance = DXGraphicBufferResult::GetInstance();
	return instance.m_result[instance.m_read];
}
