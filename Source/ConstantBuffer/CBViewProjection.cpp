
// TNLIB
#include"Debug.hpp"

// MyFile
#include"CBViewProjection.h"

CBViewProjection& CBViewProjection::GetInstance() {
	static CBViewProjection cbViewProjection;
	return cbViewProjection;
}

void CBViewProjection::CreateConstantBuffer(DirectXDevice* device) {
	auto& instance = CBViewProjection::GetInstance();
	
	instance.m_cbuf.SetBufferSize(sizeof(ViewProjection));
	instance.m_cbuf.SetRegisterNumber(0);
	instance.m_cbuf.CreateUpdateSubResource(device);
}

void CBViewProjection::Initialize(DirectXDevice* device) {
	CBViewProjection::CreateConstantBuffer(device);
}

void CBViewProjection::SetViewMatrix(const DXMatrix4x4& viewMatrix) {
	auto& instance = CBViewProjection::GetInstance();
	instance.m_data.view = viewMatrix.TransposeMatrix();
}

void CBViewProjection::SetProjectionMatrix(const DXMatrix4x4& projectionMatrix) {
	auto& instance = CBViewProjection::GetInstance();
	instance.m_data.projection = projectionMatrix.TransposeMatrix();
}

void CBViewProjection::UpdateConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBViewProjection::GetInstance();
	instance.m_cbuf.Update(context, &instance.m_data);
}

void CBViewProjection::VSSetConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBViewProjection::GetInstance();
	instance.m_cbuf.VSSetConstantBuffers(context);
}

void CBViewProjection::PSSetConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBViewProjection::GetInstance();
	instance.m_cbuf.PSSetConstantBuffers(context);
}
