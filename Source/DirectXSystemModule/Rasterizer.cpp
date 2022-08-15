
// TNLIB
#include"Debug.hpp"

// MyFile
#include"Rasterizer.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

DXRasterizer::DXRasterizer() {
}

DXRasterizer::DXRasterizer(const DXRasterizer& rasterizer) {
	m_state = rasterizer.m_state;
	m_desc = rasterizer.m_desc;
}

DXRasterizer::DXRasterizer(DXRasterizer&& rasterizer) {
	m_state = std::move(rasterizer.m_state);
	m_desc = std::move(rasterizer.m_desc);
}

void DXRasterizer::Create(DirectXDevice* device, const RasterizerDesc& desc) {
	if (m_state) {
		m_state.Reset();
		Debug::Log("This RasterizerState was previously created.");
	}

	auto hr = device->CreateRasterizerState(
		&desc,
		m_state.GetAddressOf()
	);

	IF_HRS(hr) {
		m_desc = desc;
		Debug::Log("DXRasterizer::Create Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXRasterizer::Create Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXRasterizer::Create Method.\n",
			hr
		);
#endif
	}
}

void DXRasterizer::RSSetState(DirectXDeviceContext* context) {
	context->RSSetState(
		m_state.Get()
	);
}
