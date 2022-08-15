
// TNLIB
#include"Debug.hpp"

// MyFile
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() {
	m_viewMatrix = DXMatrix4x4::Zero();
	m_projectionMatrix = DXMatrix4x4::Zero();
	m_screenSize.Set(1024.0f);
	m_screenAspect = 1.0f;	// 1024 / 1024
	m_fov = 0.0f;
	m_nearClip = 0.0f;
	m_farClip = 1.0f;
	m_position.Set((DXVector3::Right() + DXVector3::Back()).NormalizedVector().InverseVector());
	m_lookAt.Set(0.0f, 0.0f, 0.0f);
	m_upVector.Set(0.0f, 1.0f, 0.0f);
	m_direction.Set((DXVector3::Right() + DXVector3::Back()).NormalizedVector());
	m_color.Set(DXColor::White());
}

DirectionalLight::~DirectionalLight() {
}

void DirectionalLight::CreateViewMatrix() {
	m_viewMatrix = DirectX::XMMatrixLookAtLH(
		m_position,
		m_lookAt,
		m_upVector
	);
}

void DirectionalLight::CreateProjectionMatrix() {
	// TODO
	// ラスタライザのViewPortSizeの大きさと同等である必要がある
	// ライトの場合はシャドウマップ用に使用されるので
	// シャドウマップ用のレンダーターゲットと同等のサイズにする必要がある
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(
		m_fov,
		m_screenAspect,
		m_nearClip,
		m_farClip
	);
}

void DirectionalLight::Initialize() {
	Debug::BracketBegin("DirectionalLight::Initialize");

	CreateViewMatrix();
	CreateProjectionMatrix();

	Debug::BracketEnd();
}

void DirectionalLight::UpdateViewMatrix() {
	CreateViewMatrix();
}

void DirectionalLight::UpdateProjectionMatrix() {
	CreateProjectionMatrix();
}

void DirectionalLight::SetScreenSize(DXVector2 screenSize) {
	m_screenSize = screenSize;
	m_screenAspect = screenSize.x / screenSize.y;
}

void DirectionalLight::SetFov(float_t fov) {
	m_fov = fov;
}

void DirectionalLight::SetNearClip(float_t nearClip) {
	m_nearClip = nearClip;
}

void DirectionalLight::SetFarClip(float_t farClip) {
	m_farClip = farClip;
}

void DirectionalLight::SetViewDirection(const DXVector3& direction) {
	m_position = direction.NormalizedVector().InverseVector() * 20.0f;	// 10.0fは適当
	m_direction = direction.NormalizedVector();
}

void DirectionalLight::SetUpVector(const DXVector3& direction) {
	m_upVector = direction;
}

void DirectionalLight::SetLightColor(const DXColor& color) {
	m_color = color;
}

DXVector2 DirectionalLight::GetScreenSize() {
	return m_screenSize;
}

DXVector3 DirectionalLight::GetViewDirection() {
	return m_direction;
}

DXColor DirectionalLight::GetLightColor() {
	return m_color;
}

DXMatrix4x4 DirectionalLight::GetViewMatrix() {
	return m_viewMatrix;
}

DXMatrix4x4 DirectionalLight::GetProjectionMatrix() {
	return m_projectionMatrix;
}