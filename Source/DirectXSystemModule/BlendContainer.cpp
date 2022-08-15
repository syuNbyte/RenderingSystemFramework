
// TNLIB
#include"Debug.hpp"

// MyFile
#include "BlendContainer.h"

DXBlendContainer& DXBlendContainer::GetInstance() {
	static DXBlendContainer container;
	return container;
}

void DXBlendContainer::CreateNone(DirectXDevice* device) {
	auto& instance = DXBlendContainer::GetInstance();
	BlendDesc desc;

	// BlendDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= false;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendStateÇÃçÏê¨
	{
		instance.m_none.Create(
			device,
			desc
		);
	}
}

void DXBlendContainer::CreateDefault(DirectXDevice* device) {
	auto& instance = DXBlendContainer::GetInstance();
	BlendDesc desc;

	// BlendDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendStateÇÃçÏê¨
	{
		instance.m_default.Create(
			device,
			desc
		);
	}
}

void DXBlendContainer::CreateSrcAlpha(DirectXDevice* device) {
	auto& instance = DXBlendContainer::GetInstance();
	BlendDesc desc;

	// BlendDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendStateÇÃçÏê¨
	{
		instance.m_srcAlpha.Create(
			device,
			desc
		);
	}
}

void DXBlendContainer::CreateAlpha(DirectXDevice* device) {
	auto& instance = DXBlendContainer::GetInstance();
	BlendDesc desc;

	// BlendDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendStateÇÃçÏê¨
	{
		instance.m_alpha.Create(
			device,
			desc
		);
	}
}

void DXBlendContainer::CreateAlphaToCoverage(DirectXDevice* device) {
	auto& instance = DXBlendContainer::GetInstance();
	BlendDesc desc;

	// BlendDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.AlphaToCoverageEnable						= true;
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendStateÇÃçÏê¨
	{
		instance.m_alphaToCoverage.Create(
			device,
			desc
		);
	}
}

void DXBlendContainer::CreateAdd(DirectXDevice* device) {
	auto& instance = DXBlendContainer::GetInstance();
	BlendDesc desc;

	// BlendDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendStateÇÃçÏê¨
	{
		instance.m_add.Create(
			device,
			desc
		);
	}
}

void DXBlendContainer::CreateAddAlpha(DirectXDevice* device) {
	auto& instance = DXBlendContainer::GetInstance();
	BlendDesc desc;

	// BlendDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_ONE;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendStateÇÃçÏê¨
	{
		instance.m_addAlpha.Create(
			device,
			desc
		);
	}
}

void DXBlendContainer::CreateSub(DirectXDevice* device) {
	auto& instance = DXBlendContainer::GetInstance();
	BlendDesc desc;

	// BlendDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_ONE;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_REV_SUBTRACT;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendStateÇÃçÏê¨
	{
		instance.m_sub.Create(
			device,
			desc
		);
	}
}

void DXBlendContainer::CreateMul(DirectXDevice* device) {
	auto& instance = DXBlendContainer::GetInstance();
	BlendDesc desc;

	// BlendDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_ZERO;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_SRC_COLOR;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendStateÇÃçÏê¨
	{
		instance.m_mul.Create(
			device,
			desc
		);
	}
}

void DXBlendContainer::CreateAlign(DirectXDevice* device) {
	auto& instance = DXBlendContainer::GetInstance();
	BlendDesc desc;

	// BlendDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendStateÇÃçÏê¨
	{
		instance.m_align.Create(
			device,
			desc
		);
	}
}

void DXBlendContainer::CreateScreen(DirectXDevice* device) {
	auto& instance = DXBlendContainer::GetInstance();
	BlendDesc desc;

	// BlendDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_INV_DEST_COLOR;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_ONE;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendStateÇÃçÏê¨
	{
		instance.m_screen.Create(
			device,
			desc
		);
	}
}

void DXBlendContainer::CreateReverse(DirectXDevice* device) {
	auto& instance = DXBlendContainer::GetInstance();
	BlendDesc desc;

	// BlendDescÇÃèâä˙âª
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_INV_DEST_COLOR;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_INV_SRC_COLOR;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendStateÇÃçÏê¨
	{
		instance.m_reverse.Create(
			device,
			desc
		);
	}
}

void DXBlendContainer::Initialize(DirectXDevice* device) {
	Debug::BracketBegin("DXBlendContainer::Initialize");

	DXBlendContainer::CreateNone(device);
	DXBlendContainer::CreateDefault(device);
	DXBlendContainer::CreateSrcAlpha(device);
	DXBlendContainer::CreateAlpha(device);
	DXBlendContainer::CreateAlphaToCoverage(device);
	DXBlendContainer::CreateAdd(device);
	DXBlendContainer::CreateAddAlpha(device);
	DXBlendContainer::CreateSub(device);
	DXBlendContainer::CreateMul(device);
	DXBlendContainer::CreateAlign(device);
	DXBlendContainer::CreateScreen(device);
	DXBlendContainer::CreateReverse(device);

	Debug::BracketEnd();
}

DXBlend DXBlendContainer::GetNoneBlend() {
	auto& instance = DXBlendContainer::GetInstance();
	return instance.m_none;
}

DXBlend DXBlendContainer::GetDefaultBlend() {
	auto& instance = DXBlendContainer::GetInstance();
	return instance.m_default;
}

DXBlend DXBlendContainer::GetSrcAlphaBlend() {
	auto& instance = DXBlendContainer::GetInstance();
	return instance.m_srcAlpha;
}

DXBlend DXBlendContainer::GetAlphaBlend() {
	auto& instance = DXBlendContainer::GetInstance();
	return instance.m_alpha;
}

DXBlend DXBlendContainer::GetAlphaToCoverageBlend() {
	auto& instance = DXBlendContainer::GetInstance();
	return instance.m_alphaToCoverage;
}

DXBlend DXBlendContainer::GetAddBlend() {
	auto& instance = DXBlendContainer::GetInstance();
	return instance.m_add;
}

DXBlend DXBlendContainer::GetAddAlphaBlend() {
	auto& instance = DXBlendContainer::GetInstance();
	return instance.m_addAlpha;
}

DXBlend DXBlendContainer::GetSubBlend() {
	auto& instance = DXBlendContainer::GetInstance();
	return instance.m_sub;
}

DXBlend DXBlendContainer::GetMulBlend() {
	auto& instance = DXBlendContainer::GetInstance();
	return instance.m_mul;
}

DXBlend DXBlendContainer::GetAlignBlend() {
	auto& instance = DXBlendContainer::GetInstance();
	return instance.m_align;
}

DXBlend DXBlendContainer::GetScreenBlend() {
	auto& instance = DXBlendContainer::GetInstance();
	return instance.m_screen;
}

DXBlend DXBlendContainer::GetReverseBlend() {
	auto& instance = DXBlendContainer::GetInstance();
	return instance.m_reverse;
}
