
// TNLIB
#include"Debug.hpp"

// MyFile
#include"SamplerStateContainer.h"
#include"SamplerFilterType.h"
#include"DirectXManager.h"
#include"HresultUtility.h"
#include"MessageBoxUtility.h"

SamplerStateContainer& SamplerStateContainer::GetInstance() {
	static SamplerStateContainer samplerStateContainer;
	return samplerStateContainer;
}

void SamplerStateContainer::CreatePointSampler() {
	auto& instance = SamplerStateContainer::GetInstance();
	SamplerDesc desc;

	// SamplerDesc�̍쐬
	{
		ZeroMemory(&desc, sizeof(SamplerDesc));
		desc.Filter = static_cast<D3D11_FILTER>(SamplerFilterType::Point);	//�e�N�X�`���̐F��ԕ��@
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;									//�e�N�X�`����U���W��0.0f�`1.0f�𒴂����ꍇ�̏���
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;									//�e�N�X�`����V���W��0.0f�`1.0f�𒴂����ꍇ�̏���
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;									//�e�N�X�`����W���W��0.0f�`1.0f�𒴂����ꍇ�̏���
	}

	// SmaplerState�̍쐬
	{
		auto hr = DirectXManager::GetDevice()->CreateSamplerState(
			&desc,
			instance.m_point.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_pointDesc = desc;
			Debug::Log("DepthStencil::CreatePointSampler Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"SamplerStateContainer::CreatePointSampler Method.\n",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"SamplerStateContainer::CreatePointSampler Method.\n",
				hr
			);
#endif
		}
	}
}

void SamplerStateContainer::CreateLinearSampler() {
	auto& instance = SamplerStateContainer::GetInstance();
	SamplerDesc desc;

	// SamplerDesc�̍쐬
	{
		ZeroMemory(&desc, sizeof(SamplerDesc));
		desc.Filter = static_cast<D3D11_FILTER>(SamplerFilterType::Linear);	//�e�N�X�`���̐F��ԕ��@
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;									//�e�N�X�`����U���W��0.0f�`1.0f�𒴂����ꍇ�̏���
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;									//�e�N�X�`����V���W��0.0f�`1.0f�𒴂����ꍇ�̏���
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;									//�e�N�X�`����W���W��0.0f�`1.0f�𒴂����ꍇ�̏���
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[3] = 1.0f;
	}

	// SmaplerState�̍쐬
	{
		auto hr = DirectXManager::GetDevice()->CreateSamplerState(
			&desc,
			instance.m_linear.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_linearDesc = desc;
			Debug::Log("SamplerStateContainer::CreateLinearSampler Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"SamplerStateContainer::CreateLinearSampler Method.\n",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"SamplerStateContainer::CreateLinearSampler Method.\n",
				hr
			);
#endif
		}
	}
}

void SamplerStateContainer::Initialize() {
	Debug::BracketBegin("SamplerStateContainer::Initialize Method.");

	SamplerStateContainer::CreatePointSampler();
	SamplerStateContainer::CreateLinearSampler();

	Debug::BracketEnd();
}

ComObject::SamplerState SamplerStateContainer::GetPoint() {
	auto& instance = SamplerStateContainer::GetInstance();
	return instance.m_point;
}

ComObject::SamplerState SamplerStateContainer::GetLinear() {
	auto& instance = SamplerStateContainer::GetInstance();
	return instance.m_linear;
}
