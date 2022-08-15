
// MyFile
#include"CBCameraParam.h"

#include"Debug.hpp"

CBCameraParam& CBCameraParam::GetInstance() {
	static CBCameraParam CBCameraParam;
	return CBCameraParam;
}

void CBCameraParam::CreateConstantBuffer(DirectXDevice* device) {
	auto& instance = CBCameraParam::GetInstance();

	instance.m_cbuf.SetBufferSize(sizeof(CBCameraParamData));
	instance.m_cbuf.SetRegisterNumber(0);
	instance.m_cbuf.CreateUpdateSubResource(device);
}

void CBCameraParam::Initialize(DirectXDevice* device) {
	CBCameraParam::CreateConstantBuffer(device);
}

void CBCameraParam::SetCameraPosition(const DXVector3& cameraPosition) {
	auto& instance = CBCameraParam::GetInstance();
	instance.m_data.cameraPosition = cameraPosition;
}

void CBCameraParam::SetCameraViewDirection(const DXVector3& direction) {
	auto& instance = CBCameraParam::GetInstance();
	instance.m_data.cameraViewDirection = direction;
}

void CBCameraParam::SetViewPortSize(const DXVector2Int& size) {
	auto& instance = CBCameraParam::GetInstance();
	instance.m_data.viewPortSize = size;
}

void CBCameraParam::SetViewMatrix(const DXMatrix4x4& matrix) {
	auto& instance = CBCameraParam::GetInstance();
	instance.m_data.viewMatrix = matrix.TransposeMatrix();
}

void CBCameraParam::SetProjectionMatrix(const DXMatrix4x4& matrix) {
	auto& instance = CBCameraParam::GetInstance();
	instance.m_data.projectionMatrix = matrix.TransposeMatrix();
}

void CBCameraParam::UpdateConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBCameraParam::GetInstance();
	instance.m_cbuf.Update(context, &instance.m_data);
}

void CBCameraParam::VSSetConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBCameraParam::GetInstance();
	instance.m_cbuf.VSSetConstantBuffers(context);
}

void CBCameraParam::PSSetConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBCameraParam::GetInstance();
	instance.m_cbuf.PSSetConstantBuffers(context);
}