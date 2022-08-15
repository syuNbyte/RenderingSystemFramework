
// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"MainDirectionalLight.h"
#include"ConstantBuffer/CBDirectionalLight.h"

MainDirectionalLight& MainDirectionalLight::GetInstance() {
	static MainDirectionalLight mainLight;
	return mainLight;
}

void MainDirectionalLight::CreateDirectionalLight() {
	auto& instance = MainDirectionalLight::GetInstance();

	instance.m_light.SetScreenSize(DXVector2(1024.0f, 1024.0f)); // シャドウマップの解像度
	instance.m_light.SetNearClip(0.01f);
	instance.m_light.SetFarClip(100.0f);
	instance.m_light.SetFov(Math::PiDiv2<float_t>);
	instance.m_light.SetViewDirection(DXVector3::Left() + DXVector3::Down() + DXVector3::Forward());
	instance.m_light.SetLightColor(DXColor::White());
	instance.m_light.Initialize();
}

void MainDirectionalLight::CreateDepthRenderTarget(DirectXDevice* device) {
	auto& instance = MainDirectionalLight::GetInstance();
	Texture2DDesc texDesc;
	RenderTargetViewDesc rtvDesc;
	ShaderResourceViewDesc srvDesc;

	// Texture2DDescの初期化
	{
		ZeroMemory(&texDesc, sizeof(Texture2DDesc));
		texDesc.Width = instance.m_light.GetScreenSize().x;
		texDesc.Height = instance.m_light.GetScreenSize().y;
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

	instance.m_rt.SetTexture2DDesc(texDesc);
	instance.m_rt.SetRenderTargetViewDesc(rtvDesc);
	instance.m_rt.SetShaderResourceViewDesc(srvDesc);
	instance.m_rt.Create(device);
}

void MainDirectionalLight::CreateDepthStencil(DirectXDevice* device) {
	auto& instance = MainDirectionalLight::GetInstance();

	Texture2DDesc texDesc;
	DepthStencilDesc stateDesc;
	DepthStencilViewDesc dsvDesc;
	ShaderResourceViewDesc srvDesc;

	// Texture2DDescの初期化
	{
		ZeroMemory(&texDesc, sizeof(DepthBufferDesc));
		texDesc.Width = instance.m_light.GetScreenSize().x;
		texDesc.Height = instance.m_light.GetScreenSize().y;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		//texDesc.Format					= DXGI_FORMAT_D24_UNORM_S8_UINT;
		texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
	}

	// DepthStencilDescの初期化
	{
		ZeroMemory(&stateDesc, sizeof(DepthStencilDesc));
		stateDesc.DepthEnable = true;
		stateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		stateDesc.StencilEnable = true;
		stateDesc.StencilReadMask = 0xFF;
		stateDesc.StencilWriteMask = 0xFF;
		stateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		stateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		stateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		stateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	}

	// DepthStencilViewDescの初期化
	{
		ZeroMemory(&dsvDesc, sizeof(DepthStencilViewDesc));
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
	}

	// ShaderResourceViewDescの初期化
	{
		ZeroMemory(&srvDesc, sizeof(ShaderResourceViewDesc));
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
	}

	instance.m_ds.SetTexture2DDesc(texDesc);
	instance.m_ds.SetDepthStencilDesc(stateDesc);
	instance.m_ds.SetDepthStencilViewDesc(dsvDesc);
	instance.m_ds.SetShaderResourceViewDesc(srvDesc);
	instance.m_ds.Create(device);
}

void MainDirectionalLight::Initialize(DirectXDevice* device) {
	MainDirectionalLight::CreateDirectionalLight();
	MainDirectionalLight::CreateDepthRenderTarget(device);
	MainDirectionalLight::CreateDepthStencil(device);
}

void MainDirectionalLight::SetLightDirectionConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = MainDirectionalLight::GetInstance();
	CBDirectionalLight::SetDirection(instance.m_light.GetViewDirection());
	CBDirectionalLight::UpdateConstantBuffer(context);
}

void MainDirectionalLight::SetLightColorConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = MainDirectionalLight::GetInstance();
	CBDirectionalLight::SetDirectionalLightColor(instance.m_light.GetLightColor());
	CBDirectionalLight::UpdateConstantBuffer(context);
}

void MainDirectionalLight::SetAmbientColorConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = MainDirectionalLight::GetInstance();
	CBDirectionalLight::SetAmbientLightColor(instance.m_light.GetLightColor());	// DirectionalLightColorと同じにする
	CBDirectionalLight::UpdateConstantBuffer(context);
}

void MainDirectionalLight::SetViewMatrixConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = MainDirectionalLight::GetInstance();
	CBDirectionalLight::SetViewMatrix(instance.m_light.GetViewMatrix());
	CBDirectionalLight::UpdateConstantBuffer(context);
}

void MainDirectionalLight::SetProjectionMatrixConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = MainDirectionalLight::GetInstance();
	CBDirectionalLight::SetProjectionMatrix(instance.m_light.GetProjectionMatrix());
	CBDirectionalLight::UpdateConstantBuffer(context);
}

void MainDirectionalLight::UpdateConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = MainDirectionalLight::GetInstance();
	CBDirectionalLight::UpdateConstantBuffer(context);
}

void MainDirectionalLight::VSSetConstantBuffer(DirectXDeviceContext* context) {
	CBDirectionalLight::VSSetConstantBuffer(context);
}

void MainDirectionalLight::PSSetConstantBuffer(DirectXDeviceContext* context) {
	CBDirectionalLight::PSSetConstantBuffer(context);
}

DirectionalLight& MainDirectionalLight::GetLight() {
	auto& instance = MainDirectionalLight::GetInstance();
	return instance.m_light;
}

DXRenderTarget& MainDirectionalLight::GetRenderTarget() {
	auto& instance = MainDirectionalLight::GetInstance();
	return instance.m_rt;
}

DXDepthStencil& MainDirectionalLight::GetDepthStencil() {
	auto& instance = MainDirectionalLight::GetInstance();
	return instance.m_ds;
}
