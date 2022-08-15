
// TNLIB
#include"Debug.hpp"

// MyFile
#include"DeferredRenderer.h"
#include"Application.h"
#include"BackBuffer.h"
#include"GraphicBuffer.h"
#include"DirectXSystemModule/DepthStencil.h"
#include"ConstantBuffer/CBWorld.h"
#include"DeferredRenderingCamera.h"
#include"DirectXUtilityModule/DirectXColor.h"

DeferredRenderer& DeferredRenderer::GetInstance() {
	static DeferredRenderer renderer;
	return renderer;
}

void DeferredRenderer::Initialize(DirectXDevice* device, SwapChain* swapChain) {
	DXBackBuffer::Initialize(device, swapChain);
	DXGraphicBuffer::Initialize(device);
}

void DeferredRenderer::Rendering(DirectXDeviceContext* context) {

	// 書き込み先のバックバッファのワールド座標
	CBWorld::SetWorldMatrix(
		Math::Matrix::Affine::Scaling(
			DXVector3(
				Application::GetWindowSize().x / 2.0f,
				Application::GetWindowSize().y / 2.0f,
				1)
		)
	);

	CBWorld::UpdateConstantBuffer(context);
	CBWorld::VSSetConstantBuffer(context);
	CBWorld::PSSetConstantBuffer(context);
	DeferredRenderingCamera::SetViewMatrixConstantBuffer(context);
	DeferredRenderingCamera::SetOrthoProjectionMatrixConstantBuffer(context);
	DeferredRenderingCamera::VSSetConstantBuffer(context);
	DeferredRenderingCamera::PSSetConstantBuffer(context);
}

void DeferredRenderer::BackBufferClear(DirectXDeviceContext* context) {
	DXBackBuffer::Clear(context, DXColor::Green());
}

void DeferredRenderer::GraphicBufferClear(DirectXDeviceContext* context) {
	DXGraphicBuffer::ClearRenderTargetView(context);
}

void DeferredRenderer::SetDepthFlag(bool flag) {
	auto& instance = DeferredRenderer::GetInstance();
	instance.m_depthEnable = flag;
}

bool DeferredRenderer::GetDepthFlag() {
	auto& instance = DeferredRenderer::GetInstance();
	return instance.m_depthEnable;
}

void DeferredRenderer::OMSetBackBuffer(DirectXDeviceContext* context) {
	auto rtv = DXBackBuffer::GetRTV();
	auto dsv = DXDepthStencilDefault::GetDSV();
	context->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());
}

void DeferredRenderer::OMSetGraphicBuffer(DirectXDeviceContext* context) {
	auto rtvs = DXGraphicBuffer::GetRTVs();
	auto dsv = DXDepthStencilDefault::GetDSV();
	context->OMSetRenderTargets(rtvs.size(), rtvs.data(), dsv.Get());
}

ComObject::ShaderResourceView DeferredRenderer::GetAlbedoSRV() {
	return DXGraphicBuffer::GetAlbedoSRV();
}

ComObject::ShaderResourceView DeferredRenderer::GetNormalSRV() {
	return DXGraphicBuffer::GetNormalSRV();
}

ComObject::ShaderResourceView DeferredRenderer::GetPositionSRV() {
	return DXGraphicBuffer::GetPositionSRV();
}

ComObject::ShaderResourceView DeferredRenderer::GetDepthSRV() {
	return DXGraphicBuffer::GetDepthSRV();
}
