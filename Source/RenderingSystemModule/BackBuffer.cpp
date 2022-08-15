
// MyFile
#include"BackBuffer.h"
#include"Application.h"
#include"DirectXSystemModule/DirectXManager.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

DXBackBuffer& DXBackBuffer::GetInstance() {
	static DXBackBuffer backBuffer;
	return backBuffer;
}

void DXBackBuffer::CreateDesc() {
	auto& instance = DXBackBuffer::GetInstance();
	Texture2DDesc texDesc;
	RenderTargetViewDesc rtvDesc;
	ShaderResourceViewDesc srvDesc;

	// Texture2DDescの初期化
	{
		texDesc.Width = Application::GetWindowSize().x;
		texDesc.Height = Application::GetWindowSize().y;
		texDesc.MipLevels = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.ArraySize = 1;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = false;
	}

	// RenderTargetViewDescの初期化
	{
		ZeroMemory(&rtvDesc, sizeof(RenderTargetViewDesc));
		rtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;// Texture2DDesc.Formatと同じ
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
	}

	// ShaderResourceViewDescの初期化
	{
		ZeroMemory(&srvDesc, sizeof(ShaderResourceViewDesc));
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
	}

	instance.m_texDesc = texDesc;
	instance.m_rtvDesc = rtvDesc;
	instance.m_srvDesc = srvDesc;
}

void DXBackBuffer::GetBackBuffer(SwapChain* swapChain) {
	auto& instance = DXBackBuffer::GetInstance();

	// バックバッファを取得
	auto hr = swapChain->GetBuffer(
		0,
		__uuidof(Texture2D),
		static_cast<void**>(&instance.m_buf)
	);

	IF_HRS(hr) {
		Debug::Log("BackBuffer::GetBackBuffer Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"BackBuffer::GetBackBuffer Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"BackBuffer::GetBackBuffer Method.\n",
			hr
		);
#endif
	}
}

void DXBackBuffer::CreateRenderTargetView(DirectXDevice* device) {
	auto& instance = DXBackBuffer::GetInstance();

	// バックバッファのレンダーターゲットビューを作成
	auto hr = device->CreateRenderTargetView(
		instance.m_buf.Get(),
		nullptr,
		instance.m_rtv.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("BackBuffer::CreateRenderTargetView Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"BackBuffer::CreateRenderTargetView Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"BackBuffer::CreateRenderTargetView Method.\n",
			hr
		);
#endif
	}
}

void DXBackBuffer::CreateShaderResourceView(DirectXDevice* device) {
	auto& instance = DXBackBuffer::GetInstance();

	// バックバッファのシェーダーリソースビューを作成
	auto hr = device->CreateShaderResourceView(
		instance.m_buf.Get(),
		nullptr,
		instance.m_srv.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("BackBuffer::CreateShaderResourceView Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"BackBuffer::CreateShaderResourceView Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"BackBuffer::CreateShaderResourceView Method.\n",
			hr
		);
#endif
	}
}

void DXBackBuffer::Initialize(DirectXDevice* device, SwapChain* swapChain) {
	Debug::BracketBegin("BackBuffer::Initialize Method");

	DXBackBuffer::CreateDesc();
	DXBackBuffer::GetBackBuffer(swapChain);
	DXBackBuffer::CreateRenderTargetView(device);
	//DXBackBuffer::CreateShaderResourceView(device);

	Debug::BracketEnd();
}

void DXBackBuffer::Clear(DirectXDeviceContext* context, const DXColor& color) {
	auto& instance = DXBackBuffer::GetInstance();
	context->ClearRenderTargetView(instance.m_rtv.Get(), color.rgba.data());
}

ComObject::RenderTargetView DXBackBuffer::GetRTV() {
	auto& instance = DXBackBuffer::GetInstance();
	return instance.m_rtv;
}

// TODO
ComObject::ShaderResourceView DXBackBuffer::GetSRV() {
	auto& instance = DXBackBuffer::GetInstance();
	return ComObject::ShaderResourceView();
}

