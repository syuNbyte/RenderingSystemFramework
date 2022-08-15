
// TNLIB
#include"Debug.hpp"

// MyFile
#include"SamplerContainer.h"
#include"DirectXSystemModule/SamplerFilterType.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

DXSamplerContainer& DXSamplerContainer::GetInstance() {
	static DXSamplerContainer container;
	return container;
}

void DXSamplerContainer::CreatePointSampler(DirectXDevice* device) {
	auto& instance = DXSamplerContainer::GetInstance();
	SamplerDesc desc;

	// SamplerDesc�̍쐬
	{
		ZeroMemory(&desc, sizeof(SamplerDesc));
		desc.Filter = static_cast<D3D11_FILTER>(SamplerFilterType::Point);	//�e�N�X�`���̐F��ԕ��@
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;									//�e�N�X�`����U���W��0.0f�`1.0f�𒴂����ꍇ�̏���
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;									//�e�N�X�`����V���W��0.0f�`1.0f�𒴂����ꍇ�̏���
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;									//�e�N�X�`����W���W��0.0f�`1.0f�𒴂����ꍇ�̏���
	}

	// SamplerState�̍쐬
	{
		instance.m_point.Create(device, desc);
		instance.m_point.SetRegistNumber(0);
	}
}

void DXSamplerContainer::CreateLinearSampler(DirectXDevice* device) {
	auto& instance = DXSamplerContainer::GetInstance();
	SamplerDesc desc;

	// SamplerDesc�̍쐬
	{
		ZeroMemory(&desc, sizeof(SamplerDesc));
		desc.Filter = static_cast<D3D11_FILTER>(SamplerFilterType::Linear);	//�e�N�X�`���̐F��ԕ��@
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;									//�e�N�X�`����U���W��0.0f�`1.0f�𒴂����ꍇ�̏���
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;									//�e�N�X�`����V���W��0.0f�`1.0f�𒴂����ꍇ�̏���
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;									//�e�N�X�`����W���W��0.0f�`1.0f�𒴂����ꍇ�̏���
	}

	// SamplerState�̍쐬
	{
		instance.m_linear.Create(device, desc);
		instance.m_linear.SetRegistNumber(0);
	}
}

void DXSamplerContainer::Initialize(DirectXDevice* device) {
	Debug::BracketBegin("DXSamplerContainer::Initialize");

	DXSamplerContainer::CreatePointSampler(device);
	DXSamplerContainer::CreateLinearSampler(device);

	Debug::BracketEnd();
}

DXSampler DXSamplerContainer::GetPointSampler() {
	auto& instance = DXSamplerContainer::GetInstance();
	return instance.m_point;
}

DXSampler DXSamplerContainer::GetLinearSampler() {
	auto& instance = DXSamplerContainer::GetInstance();
	return instance.m_linear;
}
