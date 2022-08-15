
// TNLIB
#include"Debug.hpp"

// MyFile
#include"RenderTarget.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

void DXRenderTarget::CreateTexture2D(DirectXDevice* device) {
	if (m_tex) {
		m_tex.Reset();
		Debug::Log("This Texture2D was previously created.");
	}

	auto hr = device->CreateTexture2D(
		&m_texDesc,
		nullptr,
		m_tex.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXRenderTarget::CreateTexture2D Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXRenderTarget::CreateTexture2D Method Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXRenderTarget::CreateTexture2D Method Method.\n",
			hr
		);
#endif
	}
}

void DXRenderTarget::CreateRenderTargetView(DirectXDevice* device) {
	if (m_rtv) {
		m_rtv.Reset();
		Debug::Log("This RenderTargetView was previously created.");
	}

	auto hr = device->CreateRenderTargetView(
		m_tex.Get(),
		&m_rtvDesc,
		m_rtv.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXRenderTarget::CreateRenderTargetView Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXRenderTarget::CreateRenderTargetView Method Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXRenderTarget::CreateRenderTargetView MettexDeshod Method.\n",
			hr
		);
#endif
	}
}

void DXRenderTarget::CreateShaderResourceView(DirectXDevice* device) {
	if (m_srv) {
		m_srv.Reset();
		Debug::Log("This ShaderResourceView was previously created.");
	}

	auto hr = device->CreateShaderResourceView(
		m_tex.Get(),
		&m_srvDesc,
		m_srv.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXRenderTarget::CreateShaderResourceView Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXRenderTarget::CreateShaderResourceView Method Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXRenderTarget::CreateShaderResourceView Method Method.\n",
			hr
		);
#endif
	}
}

void DXRenderTarget::Create(DirectXDevice* device) {
	Debug::BracketBegin("DXRenderTarget::Create");

	CreateTexture2D(device);
	CreateRenderTargetView(device);
	CreateShaderResourceView(device);

	Debug::BracketEnd();
}

void DXRenderTarget::Clear(DirectXDeviceContext* context, const DXColor& color) {
	context->ClearRenderTargetView(
		m_rtv.Get(),
		color.rgba.data()
	);
}

void DXRenderTarget::SetTexture2DDesc(const Texture2DDesc& desc) {
	m_texDesc = desc;
}

void DXRenderTarget::SetRenderTargetViewDesc(const RenderTargetViewDesc& desc) {
	m_rtvDesc = desc;
}

void DXRenderTarget::SetShaderResourceViewDesc(const ShaderResourceViewDesc& desc) {
	m_srvDesc = desc;
}

ComObject::RenderTargetView DXRenderTarget::GetRTV() {
	return m_rtv;
}

ComObject::ShaderResourceView DXRenderTarget::GetSRV() {
	return m_srv;
}

DXVector2Int DXRenderTarget::GetRenderTargetSize() {
	return DXVector2Int(m_texDesc.Width, m_texDesc.Height);
}
