
// MyFile
#include"MainCamera.h"
#include"ConstantBuffer/CBCameraParam.h"
#include"Application.h"

MainCamera& MainCamera::GetInstance() {
	static MainCamera mainCamera;
	return mainCamera;
}

void MainCamera::CreateCamera() {
	auto& instance = MainCamera::GetInstance();

	instance.m_camera.SetScreenSize(Application::GetWindowSize());
	instance.m_camera.SetNearClip(0.01f);
	instance.m_camera.SetFarClip(100.0f);
	instance.m_camera.SetFov(Math::PiDiv2<float_t>);
	instance.m_camera.SetZoom(1.0f);
	instance.m_camera.SetPosition(DXVector3(0.0f, -0.0f, -10.0f));
	instance.m_camera.SetViewDirection(DXVector3(0.0f, 0.0f, +1.0f));
	instance.m_camera.SetUpVector(DXVector3(0.0f, 1.0f, 0.0f));
	instance.m_camera.Initialize();
}

void MainCamera::Initialize() {
	MainCamera::CreateCamera();
}

void MainCamera::SetCameraPositionConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = MainCamera::GetInstance();
	CBCameraParam::SetCameraPosition(instance.m_camera.GetPosition());
	CBCameraParam::UpdateConstantBuffer(context);
}

void MainCamera::SetCameraViewDirectionConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = MainCamera::GetInstance();
	CBCameraParam::SetCameraViewDirection(instance.m_camera.GetViewDirection());
	CBCameraParam::UpdateConstantBuffer(context);
}

void MainCamera::SetCameraViewPortSize(DirectXDeviceContext* context) {
	auto& instance = MainCamera::GetInstance();
	CBCameraParam::SetViewPortSize(Application::GetWindowSize());
	CBCameraParam::UpdateConstantBuffer(context);
}

void MainCamera::SetViewMatrixConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = MainCamera::GetInstance();
	CBCameraParam::SetViewMatrix(instance.m_camera.GetLookToViewMatrix());
	CBCameraParam::UpdateConstantBuffer(context);
}

void MainCamera::SetPerspectiveProjectionMatrixConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = MainCamera::GetInstance();
	CBCameraParam::SetProjectionMatrix(instance.m_camera.GetPerspectiveProjectionMatrix());
	CBCameraParam::UpdateConstantBuffer(context);
}

void MainCamera::SetOrthoProjectionMatrixConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = MainCamera::GetInstance();
	CBCameraParam::SetProjectionMatrix(instance.m_camera.GetOrthoProjectionMatrix());
	CBCameraParam::UpdateConstantBuffer(context);
}

void MainCamera::UpdateConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = MainCamera::GetInstance();
	CBCameraParam::UpdateConstantBuffer(context);
}

void MainCamera::VSSetConstantBuffer(DirectXDeviceContext* context) {
	CBCameraParam::VSSetConstantBuffer(context);
}

void MainCamera::PSSetConstantBuffer(DirectXDeviceContext* context) {
	CBCameraParam::PSSetConstantBuffer(context);
}

Camera& MainCamera::GetCamera() {
	auto& instance = MainCamera::GetInstance();
	return instance.m_camera;
}
