
// MyFile
#include"CBMaterial.h"

CBMaterial& CBMaterial::GetInstance() {
	static CBMaterial cbMaterial;
	return cbMaterial;
}

void CBMaterial::CreateConstantBuffer(DirectXDevice* device) {
	auto& instance = CBMaterial::GetInstance();

	instance.m_cbuf.SetBufferSize(sizeof(CBMaterialData));
	instance.m_cbuf.SetRegisterNumber(2);
	instance.m_cbuf.CreateUpdateSubResource(device);
}

void CBMaterial::Initialize(DirectXDevice* device) {
	CBMaterial::CreateConstantBuffer(device);
}

void CBMaterial::SetAlbedo(const DXColor& albedo) {
	auto& instance = CBMaterial::GetInstance();
	instance.m_data.albedo = albedo;
}

void CBMaterial::SetRoughness(const float_t& roughness) {
	auto& instance = CBMaterial::GetInstance();
	instance.m_data.material.r = roughness;
}

void CBMaterial::SetMetalness(const float_t& metalness) {
	auto& instance = CBMaterial::GetInstance();
	instance.m_data.material.g = metalness;
}

void CBMaterial::SetSpecular(const float_t& specular) {
	auto& instance = CBMaterial::GetInstance();
	instance.m_data.material.b = specular;
}

void CBMaterial::SetEmissive(const float_t& emissive) {
	auto& instance = CBMaterial::GetInstance();
	instance.m_data.material.a = emissive;
}

void CBMaterial::UpdateConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBMaterial::GetInstance();
	instance.m_cbuf.Update(context, &instance.m_data);
}

void CBMaterial::VSSetConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBMaterial::GetInstance();
	instance.m_cbuf.VSSetConstantBuffers(context);
}

void CBMaterial::PSSetConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBMaterial::GetInstance();
	instance.m_cbuf.PSSetConstantBuffers(context);
}
