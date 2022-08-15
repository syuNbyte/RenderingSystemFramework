
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

	// SamplerDescの作成
	{
		ZeroMemory(&desc, sizeof(SamplerDesc));
		desc.Filter = static_cast<D3D11_FILTER>(SamplerFilterType::Point);	//テクスチャの色補間方法
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;									//テクスチャのU座標が0.0f～1.0fを超えた場合の処理
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;									//テクスチャのV座標が0.0f～1.0fを超えた場合の処理
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;									//テクスチャのW座標が0.0f～1.0fを超えた場合の処理
	}

	// SmaplerStateの作成
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

	// SamplerDescの作成
	{
		ZeroMemory(&desc, sizeof(SamplerDesc));
		desc.Filter = static_cast<D3D11_FILTER>(SamplerFilterType::Linear);	//テクスチャの色補間方法
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;									//テクスチャのU座標が0.0f～1.0fを超えた場合の処理
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;									//テクスチャのV座標が0.0f～1.0fを超えた場合の処理
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;									//テクスチャのW座標が0.0f～1.0fを超えた場合の処理
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[3] = 1.0f;
	}

	// SmaplerStateの作成
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
