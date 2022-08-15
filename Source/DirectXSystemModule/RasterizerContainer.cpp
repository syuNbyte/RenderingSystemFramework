
// TNLIB
#include"Debug.hpp"

// MyFile
#include"RasterizerContainer.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

DXRasterizerContainer& DXRasterizerContainer::GetInstance() {
	static DXRasterizerContainer container;
	return container;
}

void DXRasterizerContainer::CreateNoneSolid(DirectXDevice* device) {
	auto& instance = DXRasterizerContainer::GetInstance();
	RasterizerDesc desc;
	
	// RasterizerDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(RasterizerDesc));
		desc.AntialiasedLineEnable = true;
		desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = true;
		desc.FrontCounterClockwise = false;
		desc.MultisampleEnable = false;
		desc.ScissorEnable = false;
		desc.SlopeScaledDepthBias = 0.0f;
	}

	// RasterizerÇÃçÏê¨
	{
		instance.m_noneSolid.Create(device, desc);
	}
}

void DXRasterizerContainer::CreateFrontSolid(DirectXDevice* device) {
	auto& instance = DXRasterizerContainer::GetInstance();
	RasterizerDesc desc;

	// RasterizerDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(RasterizerDesc));
		desc.AntialiasedLineEnable = true;
		desc.CullMode = D3D11_CULL_FRONT;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = true;
		desc.FrontCounterClockwise = false;
		desc.MultisampleEnable = false;
		desc.ScissorEnable = false;
		desc.SlopeScaledDepthBias = 0.0f;
	}

	// RasterizerÇÃçÏê¨
	{
		instance.m_frontSolid.Create(device, desc);
	}
}

void DXRasterizerContainer::CreateBackSolid(DirectXDevice* device) {
	auto& instance = DXRasterizerContainer::GetInstance();
	RasterizerDesc desc;

	// RasterizerDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(RasterizerDesc));
		desc.AntialiasedLineEnable = true;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = true;
		desc.FrontCounterClockwise = false;
		desc.MultisampleEnable = false;
		desc.ScissorEnable = false;
		desc.SlopeScaledDepthBias = 0.0f;
	}

	// RasterizerÇÃçÏê¨
	{
		instance.m_backSolid.Create(device, desc);
	}
}

void DXRasterizerContainer::CreateNoneWire(DirectXDevice* device) {
	auto& instance = DXRasterizerContainer::GetInstance();
	RasterizerDesc desc;

	// RasterizerDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(RasterizerDesc));
		desc.AntialiasedLineEnable = true;
		desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = true;
		desc.FrontCounterClockwise = false;
		desc.MultisampleEnable = false;
		desc.ScissorEnable = false;
		desc.SlopeScaledDepthBias = 0.0f;
	}

	// RasterizerÇÃçÏê¨
	{
		instance.m_noneWire.Create(device, desc);
	}
}

void DXRasterizerContainer::CreateFrontWire(DirectXDevice* device) {
	auto& instance = DXRasterizerContainer::GetInstance();
	RasterizerDesc desc;

	// RasterizerDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(RasterizerDesc));
		desc.AntialiasedLineEnable = true;
		desc.CullMode = D3D11_CULL_FRONT;
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = true;
		desc.FrontCounterClockwise = false;
		desc.MultisampleEnable = false;
		desc.ScissorEnable = false;
		desc.SlopeScaledDepthBias = 0.0f;
	}

	// RasterizerÇÃçÏê¨
	{
		instance.m_frontWire.Create(device, desc);
	}
}

void DXRasterizerContainer::CreateBackWire(DirectXDevice* device) {
	auto& instance = DXRasterizerContainer::GetInstance();
	RasterizerDesc desc;

	// RasterizerDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(RasterizerDesc));
		desc.AntialiasedLineEnable = true;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = true;
		desc.FrontCounterClockwise = false;
		desc.MultisampleEnable = false;
		desc.ScissorEnable = false;
		desc.SlopeScaledDepthBias = 0.0f;
	}

	// RasterizerStateÇÃçÏê¨
	{
		instance.m_backWire.Create(device, desc);
	}
}

void DXRasterizerContainer::Initialize(DirectXDevice* device) {
	Debug::BracketBegin("RasterizerContainer::Initialize");

	DXRasterizerContainer::CreateNoneSolid(device);
	DXRasterizerContainer::CreateFrontSolid(device);
	DXRasterizerContainer::CreateBackSolid(device);
	DXRasterizerContainer::CreateNoneWire(device);
	DXRasterizerContainer::CreateFrontWire(device);
	DXRasterizerContainer::CreateBackWire(device);

	Debug::BracketEnd();
}

void DXRasterizerContainer::SetNoneSolid(DirectXDeviceContext* context) {
	auto& instance = DXRasterizerContainer::GetInstance();
	instance.m_noneSolid.RSSetState(context);
}

void DXRasterizerContainer::SetFrontSolid(DirectXDeviceContext* context) {
	auto& instance = DXRasterizerContainer::GetInstance();
	instance.m_frontSolid.RSSetState(context);
}

void DXRasterizerContainer::SetBackSolid(DirectXDeviceContext* context) {
	auto& instance = DXRasterizerContainer::GetInstance();
	instance.m_backSolid.RSSetState(context);
}

void DXRasterizerContainer::SetNoneWire(DirectXDeviceContext* context) {
	auto& instance = DXRasterizerContainer::GetInstance();
	instance.m_noneWire.RSSetState(context);
}

void DXRasterizerContainer::SetFrontWire(DirectXDeviceContext* context) {
	auto& instance = DXRasterizerContainer::GetInstance();
	instance.m_frontWire.RSSetState(context);
}

void DXRasterizerContainer::SetBackWire(DirectXDeviceContext* context) {
	auto& instance = DXRasterizerContainer::GetInstance();
	instance.m_backWire.RSSetState(context);
}
