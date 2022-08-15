
// MyFile
#include"CBDirectionalLight.h"

CBDirectionalLight& CBDirectionalLight::GetInstance() {
	static CBDirectionalLight cbDirectionalLight;
	return cbDirectionalLight;
}

void CBDirectionalLight::CreateConstantBuffer(DirectXDevice* device) {
	auto& instance = CBDirectionalLight::GetInstance();

	instance.m_cbuf.SetBufferSize(sizeof(CBDirectionalLightData));
	instance.m_cbuf.SetRegisterNumber(3);
	instance.m_cbuf.CreateUpdateSubResource(device);
}

void CBDirectionalLight::Initialize(DirectXDevice* device) {
	CBDirectionalLight::CreateConstantBuffer(device);
}

void CBDirectionalLight::SetViewMatrix(const DXMatrix4x4& matrix) {
	auto& instance = CBDirectionalLight::GetInstance();
	instance.m_data.viewMatrix = matrix.TransposeMatrix();
}

void CBDirectionalLight::SetProjectionMatrix(const DXMatrix4x4& matrix) {
	auto& instance = CBDirectionalLight::GetInstance();
	instance.m_data.projectionMatrix = matrix.TransposeMatrix();
}

void CBDirectionalLight::SetDirection(const DXVector3& lightDirection) {
	auto& instance = CBDirectionalLight::GetInstance();
	instance.m_data.direction = lightDirection;
}

void CBDirectionalLight::SetDirectionalLightColor(const DXColor& color) {
	auto& instance = CBDirectionalLight::GetInstance();
	instance.m_data.lightColor = color;
}

void CBDirectionalLight::SetAmbientLightColor(const DXColor& color) {
	auto& instance = CBDirectionalLight::GetInstance();
	instance.m_data.ambientLightColor = color;
}

void CBDirectionalLight::UpdateConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBDirectionalLight::GetInstance();
	instance.m_cbuf.Update(context, &instance.m_data);
}

void CBDirectionalLight::VSSetConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBDirectionalLight::GetInstance();
	instance.m_cbuf.VSSetConstantBuffers(context);
}

void CBDirectionalLight::PSSetConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBDirectionalLight::GetInstance();
	instance.m_cbuf.PSSetConstantBuffers(context);
}
