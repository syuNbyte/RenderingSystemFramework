
// MyFile
#include "ViewPort.h"
#include"Application.h"

DXViewPort::DXViewPort() {
	// デフォルトで画面サイズと同等のビューポートを作成する
	m_viewPort.Width = Application::GetWindowSize().x;
	m_viewPort.Height = Application::GetWindowSize().y;
	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.TopLeftY = 0.0f;
}

DXViewPort::DXViewPort(const DXViewPort& viewPort) {
	m_viewPort = viewPort.m_viewPort;
}

DXViewPort::DXViewPort(DXViewPort&& viewPort) {
	m_viewPort = viewPort.m_viewPort;
}

DXViewPort::~DXViewPort() {
}

void DXViewPort::SetViewPortTopLeftPos(const DXVector2& pos) {
	m_viewPort.TopLeftX = pos.x;
	m_viewPort.TopLeftY = pos.y;
}

void DXViewPort::SetViewPortBottomRightPos(const DXVector2& pos) {
	m_viewPort.Width = pos.x - m_viewPort.TopLeftX;
	m_viewPort.Height = pos.y - m_viewPort.TopLeftY;
}

void DXViewPort::SetViewPortDepthRange(float_t minDepth, float_t maxDepth) {
	m_viewPort.MinDepth = minDepth;
	m_viewPort.MaxDepth = maxDepth;
}

DXVector2 DXViewPort::GetViewPortSize() {
	return DXVector2(
		m_viewPort.Width,
		m_viewPort.Height
	);
}

DXVector2 DXViewPort::GetViewPortTopLeftPos() {
	return DXVector2(
		m_viewPort.TopLeftX,
		m_viewPort.TopLeftY
	);
}

DXVector2 DXViewPort::GetViewPortBottomRightPos() {
	return DXVector2(
		m_viewPort.Width - m_viewPort.TopLeftX,
		m_viewPort.Height - m_viewPort.TopLeftY
	);
}

float_t DXViewPort::GetViewPortMinDepth() {
	return m_viewPort.MinDepth;
}

float_t DXViewPort::GetViewPortMaxDepth() {
	return m_viewPort.MaxDepth;
}

void DXViewPort::RSSetViewPort(DirectXDeviceContext* context) {
	context->RSSetViewports(
		1,
		&m_viewPort
	);
}
