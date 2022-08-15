
// MyFile
#include"CBWorld.h"

CBWorld& CBWorld::GetInstance() {
	static CBWorld cbWorld;
	return cbWorld;
}

void CBWorld::CreateConstantBuffer(DirectXDevice* device) {
	auto& instance = CBWorld::GetInstance();

	instance.m_cbuf.SetBufferSize(sizeof(CBWorldData));
	instance.m_cbuf.SetRegisterNumber(1);
	instance.m_cbuf.CreateUpdateSubResource(device);
}

void CBWorld::Initialize(DirectXDevice* device) {
	CBWorld::CreateConstantBuffer(device);
}

void CBWorld::SetWorldMatrix(const DXMatrix4x4& worldMatrix) {
	auto& instance = CBWorld::GetInstance();
	instance.m_data.world = worldMatrix.TransposeMatrix();
}

void CBWorld::UpdateConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBWorld::GetInstance();
	instance.m_cbuf.Update(context, &instance.m_data);
}

void CBWorld::VSSetConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBWorld::GetInstance();
	instance.m_cbuf.VSSetConstantBuffers(context);
}

void CBWorld::PSSetConstantBuffer(DirectXDeviceContext* context) {
	auto& instance = CBWorld::GetInstance();
	instance.m_cbuf.PSSetConstantBuffers(context);
}
