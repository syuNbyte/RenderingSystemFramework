
// TNLIB
#include"Debug.hpp"

// MyFile
#include"Sampler.h"
#include"DirectXUtilityModule//HresultUtility.h"
#include"DirectXUtilityModule//MessageBoxUtility.h"

DXSampler::DXSampler() {
	m_registNumber = -1;
}

DXSampler::DXSampler(const DXSampler& sampler) {
	m_state = sampler.m_state;
	m_desc = sampler.m_desc;
	m_registNumber = sampler.m_registNumber;
}

DXSampler::DXSampler(DXSampler&& sampler) {
	m_state = std::move(sampler.m_state);
	m_desc = std::move(sampler.m_desc);
	m_registNumber = std::move(sampler.m_registNumber);
}

void DXSampler::Create(DirectXDevice* device, const SamplerDesc& desc) {
	if (m_state) {
		m_state.Reset();
		Debug::Log("This SamplerState was previously created.");
	}

	auto hr = device->CreateSamplerState(
		&desc,
		m_state.GetAddressOf()
	);

	IF_HRS(hr) {
		m_desc = desc;
		Debug::Log("DXSampler::Create Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXSampler::Create Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXSampler::Create Method.\n",
			hr
		);
#endif
	}
}

void DXSampler::SetRegistNumber(const int_t& registNumber) {
	m_registNumber = registNumber;
}

ComObject::SamplerState DXSampler::GetSamplerState() {
	return m_state;
}

void DXSampler::VSSetSampler(DirectXDeviceContext* context) {
	context->VSSetSamplers(
		m_registNumber,
		1,
		m_state.GetAddressOf()
	);
}

void DXSampler::PSSetSampler(DirectXDeviceContext* context) {
	context->PSSetSamplers(
		m_registNumber,
		1,
		m_state.GetAddressOf()
	);
}


