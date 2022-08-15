
// TNLIB
#include"Debug.hpp"

// MyFile
#include"Camera.h"

Camera::Camera() {
	m_perspectiveProjection = DXMatrix4x4::Zero();
	m_lookToView = DXMatrix4x4::Zero();
	m_screenSize.Set(0.0f);
	m_screenAspect = 0.0f;
	m_fov = 0.0f;
	m_nearClip = 0.0f;
	m_farClip = 1.0f;
	m_zoom = 1.0f;
	m_position.Set(0.0f);
	m_direction.Set(0.0f, 0.0f, 1.0f);
	m_upVector.Set(0.0f, 1.0f, 0.0f);
}

Camera::~Camera() {}

void Camera::CreateLookToViewMatrix() {
	m_lookToView = DirectX::XMMatrixLookToLH(
		m_position,
		m_direction,
		m_upVector
	);
}

void Camera::CreateLookAtViewMatrix() {
	m_lookAtView = DirectX::XMMatrixLookAtLH(
		m_position,
		m_lookAt,
		m_upVector
	);
}

void Camera::CreatePerspectiveProjectionMatrix() {
	m_perspectiveProjection = DirectX::XMMatrixPerspectiveFovLH(
		m_fov / m_zoom,
		m_screenAspect,
		m_nearClip,
		m_farClip
	);
}

void Camera::CreateOrthoProjectionMatrix() {
	m_orthoProjection = DirectX::XMMatrixOrthographicLH(
		m_screenSize.x,	// ‚È‚ñ‚©2”{‚·‚é‚Æ‘å‚«‚³‚ª’š“x‚É‚È‚é
		m_screenSize.y,	// ‚È‚ñ‚©2”{‚·‚é‚Æ‘å‚«‚³‚ª’š“x‚É‚È‚é
		m_nearClip,
		m_farClip
	);
	//m_orthoProjection = DirectX::XMMatrixOrthographicOffCenterLH(
	//	0.0f,
	//	1.0f,
	//	1.0f,
	//	0.0f,
	//	m_nearClip,
	//	m_farClip
	//);
}

void Camera::Initialize() {
	Debug::BracketBegin("Camera::Initialize");

	CreateLookToViewMatrix();
	CreateLookAtViewMatrix();
	CreatePerspectiveProjectionMatrix();
	CreateOrthoProjectionMatrix();

	Debug::BracketEnd();
}

void Camera::UpdateViewMatrix() {
	CreateLookToViewMatrix();
	CreateLookAtViewMatrix();
}

void Camera::UpdateProjectionMatrix() {
	CreatePerspectiveProjectionMatrix();
	CreateOrthoProjectionMatrix();
}

void Camera::SetScreenSize(DXVector2 screenSize) {
	m_screenSize = screenSize;
	m_screenAspect = screenSize.x / screenSize.y;
}

void Camera::SetFov(float_t fov) {
	m_fov = fov;
}

void Camera::SetNearClip(float_t nearClip) {
	m_nearClip = nearClip;
}

void Camera::SetFarClip(float_t farClip) {
	m_farClip = farClip;
}

void Camera::SetZoom(float_t zoom) {
	m_zoom = zoom;
}

void Camera::SetPosition(DXVector3 position) {
	m_position = position;
}

void Camera::SetLookAtPosition(DXVector3 position) {
	m_lookAt = position;
}

void Camera::SetViewDirection(DXVector3 direction) {
	m_direction = direction;
	m_direction.Normalize();
}

void Camera::SetUpVector(DXVector3 upVector) {
	m_upVector = upVector;
}

DXVector3 Camera::GetPosition() {
	return m_position;
}

DXVector3 Camera::GetViewDirection() {
	return m_direction;
}

DXVector3 Camera::GetUpVector() {
	return m_upVector;
}

void Camera::SetRelativeZoom(float_t zoom) {
	m_zoom += zoom;
}

void Camera::SetRelativePosition(DXVector3 position) {
	m_position += position;
}

void Camera::SetRelativeViewDirection(DXVector3 direction) {
	m_direction += direction;
	m_direction.Normalize();
}

DXVector2 Camera::GetScreenSize() {
	return m_screenSize;
}

float_t Camera::GetNearClip() {
	return m_nearClip;
}

float_t Camera::GetFarClip() {
	return m_farClip;
}

float_t Camera::GetZoom() {
	return m_zoom;
}

DXMatrix4x4 Camera::GetLookAtViewMatrix() {
	return m_lookAtView;
}

DXMatrix4x4 Camera::GetLookToViewMatrix() {
	return m_lookToView;
}

DXMatrix4x4 Camera::GetPerspectiveProjectionMatrix() {
	return m_perspectiveProjection;
}

DXMatrix4x4 Camera::GetOrthoProjectionMatrix() {
	return m_orthoProjection;
}
