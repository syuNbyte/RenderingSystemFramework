
// MyFile
#include"DeferredRenderingCamera.h"
#include"ConstantBuffer/CBCameraParam.h"
#include"Application.h"

DeferredRenderingCamera& DeferredRenderingCamera::GetInstance() {
	static DeferredRenderingCamera DeferredRenderingCamera;
	return DeferredRenderingCamera;
}

void DeferredRenderingCamera::Initialize() {
	auto& instance = DeferredRenderingCamera::GetInstance();

	instance.m_camera.SetScreenSize(Application::GetWindowSize());
	instance.m_camera.SetNearClip(0.01f);
	instance.m_camera.SetFarClip(100.0f);
	instance.m_camera.SetFov(Math::PiDiv2<float_t>);
	instance.m_camera.SetZoom(1.0f);
	instance.m_camera.SetPosition(DXVector3(0.0f, 0.0f, -1.0f));
	instance.m_camera.SetViewDirection(DXVector3(0.0f, 0.0f, +1.0f));
	instance.m_camera.SetUpVector(DXVector3(0.0f, 1.0f, 0.0f));
	instance.m_camera.Initialize();
}

void DeferredRenderingCamera::SetViewMatrixConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = DeferredRenderingCamera::GetInstance();
	CBCameraParam::SetViewMatrix(instance.m_camera.GetLookToViewMatrix());
	CBCameraParam::UpdateConstantBuffer(context);
}

void DeferredRenderingCamera::SetPerspectiveProjectionMatrixConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = DeferredRenderingCamera::GetInstance();
	CBCameraParam::SetProjectionMatrix(instance.m_camera.GetPerspectiveProjectionMatrix());
	CBCameraParam::UpdateConstantBuffer(context);
}

void DeferredRenderingCamera::SetOrthoProjectionMatrixConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = DeferredRenderingCamera::GetInstance();
	CBCameraParam::SetProjectionMatrix(instance.m_camera.GetOrthoProjectionMatrix());
	CBCameraParam::UpdateConstantBuffer(context);
}

void DeferredRenderingCamera::VSSetConstantBuffer(DirectXDeviceContext* context) {
	CBCameraParam::VSSetConstantBuffer(context);
}

void DeferredRenderingCamera::PSSetConstantBuffer(DirectXDeviceContext* context) {
	CBCameraParam::PSSetConstantBuffer(context);
}