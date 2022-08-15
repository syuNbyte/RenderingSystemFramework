
// TNLIB
#include"Debug.hpp"

// MyFile
#include"Blend.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

DXBlend::DXBlend() {
	m_factor = { 0,0,0,0 };
	m_mask = 0xffffffff;
}

DXBlend::DXBlend(const DXBlend& blend) {
	m_state = blend.m_state;
	m_desc = blend.m_desc;
	m_factor = blend.m_factor;
	m_mask = blend.m_mask;
}

DXBlend::DXBlend(DXBlend&& blend) {
	m_state = std::move(blend.m_state);
	m_desc = std::move(blend.m_desc);
	m_factor = std::move(blend.m_factor);
	m_mask = std::move(blend.m_mask);
}

void DXBlend::Create(DirectXDevice* device, const BlendDesc& desc) {
	if (m_state) {
		m_state.Reset();
		Debug::Log("This BlendState was previously created.");
	}

	auto hr = device->CreateBlendState(
		&desc,
		m_state.GetAddressOf()
	);

	IF_HRS(hr) {
		m_desc = desc;
		Debug::Log("DXBlend::Create Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXBlend::Create Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXBlend::Create Method.\n",
			hr
		);
#endif
	}
}

void DXBlend::SetBlendFactor(float_t factor[4]) {
	for (int_t i = 0; i < 4; i++) m_factor[i] = factor[i];
}

void DXBlend::SetBlendMask(const uint_t& mask) {
	m_mask = mask;
}

void DXBlend::OMSetBlendState(DirectXDeviceContext* context) {
	context->OMSetBlendState(
		m_state.Get(),
		m_factor.data(),
		m_mask
	);
}
