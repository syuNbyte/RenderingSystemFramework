
// MyFile
#include"DepthStencil.h"
#include"DirectXManager.h"
#include"Application.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

void DXDepthStencil::CreateTexture2D(DirectXDevice* device) {
	if (m_tex) {
		m_tex.Reset();
		Debug::Log("This Texture2D was previously created.");
	}

	auto hr = DirectXManager::GetDevice()->CreateTexture2D(
		&m_texDesc,
		nullptr,
		m_tex.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXDepthStencil::CreateTextue2D Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXDepthStencil::CreateTextue2D Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXDepthStencil::CreateTextue2D Method.\n",
			hr
		);
#endif
	}
}

void DXDepthStencil::CreateDepthStencilState(DirectXDevice* device) {
	if (m_state) {
		m_state.Reset();
		Debug::Log("This DepthStencilState was previously created.");
	}

	auto hr = DirectXManager::GetDevice()->CreateDepthStencilState(
		&m_stateDesc,
		m_state.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXDepthStencil::CreateDepthStencilState Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXDepthStencil::CreateDepthStencilState Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXDepthStencil::CreateDepthStencilState Method.\n",
			hr
		);
#endif
	}
}

void DXDepthStencil::CreateDepthStencilView(DirectXDevice* device) {
	if (m_dsv) {
		m_dsv.Reset();
		Debug::Log("This DepthStencilView was previously created.");
	}

	auto hr = DirectXManager::GetDevice()->CreateDepthStencilView(
		m_tex.Get(),
		&m_dsvDesc,
		m_dsv.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXDepthStencil::CreateDepthStencilView Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXDepthStencil::CreateDepthStencilView Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXDepthStencil::CreateDepthStencilView Method.\n",
			hr
		);
#endif
	}
}

void DXDepthStencil::CreateShaderResourceView(DirectXDevice* device) {

	if (m_srv) {
		m_srv.Reset();
		Debug::Log("This ShaderResourceView was previously created.");
	}

	auto hr = DirectXManager::GetDevice()->CreateShaderResourceView(
		m_tex.Get(),
		&m_srvDesc,
		m_srv.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXDepthStencil::CreateShaderResourceView Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXDepthStencil::CreateShaderResourceView Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXDepthStencil::CreateShaderResourceView Method.\n",
			hr
		);
#endif
	}

}

void DXDepthStencil::Create(DirectXDevice* device) {
	Debug::BracketBegin("DXDepthStencil::Create");

	CreateTexture2D(device);
	CreateDepthStencilState(device);
	CreateDepthStencilView(device);
	CreateShaderResourceView(device);

	Debug::BracketEnd();
}

void DXDepthStencil::Clear(DirectXDeviceContext* context) {
	context->ClearDepthStencilView(
		m_dsv.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0U
	);
}

void DXDepthStencil::SetTexture2DDesc(const Texture2DDesc& desc) {
	m_texDesc = desc;
}

void DXDepthStencil::SetDepthStencilDesc(const DepthStencilDesc& desc) {
	m_stateDesc = desc;
}

void DXDepthStencil::SetDepthStencilViewDesc(const DepthStencilViewDesc& desc) {
	m_dsvDesc = desc;
}

void DXDepthStencil::SetShaderResourceViewDesc(const ShaderResourceViewDesc& desc) {
	m_srvDesc = desc;
}

ComObject::DepthStencilState DXDepthStencil::GetDepthStencilState() {
	return m_state;
}

ComObject::DepthStencilView DXDepthStencil::GetDSV() {
	return m_dsv;
}

ComObject::ShaderResourceView DXDepthStencil::GetSRV() {
	return m_srv;
}



DXDepthStencilDefault& DXDepthStencilDefault::GetInstance() {
	static DXDepthStencilDefault depthStencil;
	return depthStencil;
}

void DXDepthStencilDefault::CreateDepthStencil(DirectXDevice* device) {
	auto& instance = DXDepthStencilDefault::GetInstance();
	Texture2DDesc texDesc;
	DepthStencilDesc stateDesc;
	DepthStencilViewDesc dsvDesc;
	ShaderResourceViewDesc srvDesc;

	// Texture2DDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&texDesc, sizeof(DepthBufferDesc));
		texDesc.Width = Application::GetWindowSize().x;
		texDesc.Height = Application::GetWindowSize().y;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		//texDesc.Format					= DXGI_FORMAT_D24_UNORM_S8_UINT;
		texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
	}

	// DepthStencilDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&stateDesc, sizeof(DepthStencilDesc));
		stateDesc.DepthEnable = true;
		stateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		stateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		stateDesc.StencilEnable = true;
		stateDesc.StencilReadMask = 0xFF;
		stateDesc.StencilWriteMask = 0xFF;
		stateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		stateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		stateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		stateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		stateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		stateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	}

	// DepthStencilViewDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&dsvDesc, sizeof(DepthStencilViewDesc));
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
	}

	// ShaderResourceViewDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&srvDesc, sizeof(ShaderResourceViewDesc));
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
	}

	instance.m_depthStencil.SetTexture2DDesc(texDesc);
	instance.m_depthStencil.SetDepthStencilDesc(stateDesc);
	instance.m_depthStencil.SetDepthStencilViewDesc(dsvDesc);
	instance.m_depthStencil.SetShaderResourceViewDesc(srvDesc);
	instance.m_depthStencil.Create(device);
}

void DXDepthStencilDefault::Initialize(DirectXDevice* device) {
	Debug::BracketBegin("DXDepthStencilDefault::Initialize");

	DXDepthStencilDefault::CreateDepthStencil(device);

	Debug::BracketEnd();
}

void DXDepthStencilDefault::Clear(DirectXDeviceContext* context) {
	auto& instance = DXDepthStencilDefault::GetInstance();
	instance.m_depthStencil.Clear(context);
}

ComObject::DepthStencilState DXDepthStencilDefault::GetDepthStencilState() {
	auto& instance = DXDepthStencilDefault::GetInstance();
	return instance.m_depthStencil.GetDepthStencilState();
}

ComObject::DepthStencilView DXDepthStencilDefault::GetDSV() {
	auto& instance = DXDepthStencilDefault::GetInstance();
	return instance.m_depthStencil.GetDSV();
}

void DXDepthStencilDefault::DepthBufferON(DirectXDeviceContext* context) {
	auto& instance = DXDepthStencilDefault::GetInstance();

	//context->OMSetRenderTargets(
	//	rtvNum,
	//	rtvs,
	//	instance.m_dsv.Get()
	//);

	context->OMSetDepthStencilState(
		instance.GetDepthStencilState().Get(),
		NULL
	);
}

void DXDepthStencilDefault::DepthBufferOFF(DirectXDeviceContext* context) {
	auto& instance = DXDepthStencilDefault::GetInstance();

	//context->OMSetRenderTargets(
	//	rtvNum,
	//	rtvs,
	//	nullptr
	//);

	context->OMSetDepthStencilState(
		instance.GetDepthStencilState().Get(),
		NULL
	);
}