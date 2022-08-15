
// TNLIB
#include"Debug.hpp"

// MyFile
#include"DirectXManager.h"
#include"DepthStencil.h"
#include"RasterizerContainer.h"
#include"BlendContainer.h"
#include"SamplerContainer.h"
#include"RenderingSystemModule/BackBuffer.h"
#include"Application.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

DirectXManager& DirectXManager::GetInstance() {
	static DirectXManager directxManager;
	return directxManager;
}

void DirectXManager::CreateDXGIFactoryInit() {
	auto& instance = DirectXManager::GetInstance();

	if (instance.m_factory) {
		instance.m_factory.Reset();
		Debug::Log("This IDxgiFactory_1 was previously created.");
	}

	auto hr = CreateDXGIFactory1(
		IID_PPV_ARGS(instance.m_factory.GetAddressOf())
	);

	IF_HRS(hr) {
		Debug::Log("DirectXManager::CreateDXGIFactoryInit Method Success.");
	}

	// 失敗した場合
	IF_HRF(hr) {
		Debug::LogWarning(
			"DirectXManager::CreateDXGIFactoryInit Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultMessage(
			"DirectXManager::CreateDXGIFactoryInit Method.\n",
			hr
		);
#endif
	}
}

void DirectXManager::CreateDXGIAdapterInit() {
	auto& instance = DirectXManager::GetInstance();
	ComObject::IDxgiAdapter_1 adapter;
	DxgiAdapterDesc_1 desc;

	for (uint_t i = 0; S_OK == instance.m_factory->EnumAdapters1(i, adapter.GetAddressOf()); ++i) {
		ZeroMemory(&desc, sizeof(DxgiAdapterDesc_1));
		adapter->GetDesc1(&desc);

		if (instance.m_adapter == nullptr && (desc.Flags ^= DXGI_ADAPTER_FLAG_SOFTWARE)) {
			instance.m_adapter = std::move(adapter);
			Debug::Log("Use adapter number : ", i);
			Debug::Log("DirectXManager::CreateDXGIAdapterInit Method Success.");
			return;
		}
	}
}

void DirectXManager::CreateDeviceInit() {
	auto& instance = DirectXManager::GetInstance();

	std::array<FeatureLevel, 7> featureLevels = { {
		D3D_FEATURE_LEVEL_11_1,					// DirectX11.1対応GPUレベル
		D3D_FEATURE_LEVEL_11_0,					// DirectX11対応GPUレベル
		D3D_FEATURE_LEVEL_10_1,					// DirectX10.1対応GPUレベル
		D3D_FEATURE_LEVEL_10_0,					// DirectX10対応GPUレベル
		D3D_FEATURE_LEVEL_9_3,					// DirectX9.3対応GPUレベル
		D3D_FEATURE_LEVEL_9_2,					// DirectX9.2対応GPUレベル
		D3D_FEATURE_LEVEL_9_1					// Direct9.1対応GPUレベル
	} };

	FeatureLevel usedFeatureLevel;

#if _DEBUG
	uint_t flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG;
#else
	uint_t flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif

	if (instance.m_device) {
		instance.m_device.Reset();
		Debug::Log("This DirectXDevice was previously created.");
	}
	if (instance.m_immediateContext) {
		instance.m_immediateContext.Reset();
		Debug::Log("This DirectXDeviceContext was previously created.");
	}

																	//=========================================================================================================================================================
																	// 引数の内容												：詳細
	auto hr = D3D11CreateDevice(							//=========================================================================================================================================================						
		instance.m_adapter.Get(),							// GPUアダプタ												：使用するGPUアダプタを選択　※nullptrを設定すると使用可能なGPUアダプタの中から自動的に選択してくる
		D3D_DRIVER_TYPE_UNKNOWN,							// ドライバタイプ											：今回は指定するのでUNKNOWN　※GPUアダプタを指定して作成するときは必ずD3D_DRIVER_TYPE_UNKNOWNを指定する必要がある
		nullptr,													// ソフトウェアによるラスタライザを実装したDLL	：DriverTypeにD3D_DRIVER_TYPE_SOFTWAREを指定した際に使用する
		flags,													// ランタイムレイヤーを表すフラグ					：D3D11_CREATE_DEVICE_FLAGで定義されている
		featureLevels.data(),								// 使用したい機能レベルの配列の先頭アドレス		：優先順位は要素の若いもの
		static_cast<UINT>(featureLevels.size()),		// 機能レベルの配列の要素数							：
		D3D11_SDK_VERSION,									// SDKのバージョン										：D3D11を使用するのでD3D11_SDK_VERSIONを使用する
		instance.m_device.GetAddressOf(),				// 初期化するデバイス									：ID3D11Device型
		&usedFeatureLevel,									// 機能レベルを受け取る変数							：D3D_FEATURE_LEVEL型
		instance.m_immediateContext.GetAddressOf()	// 初期化するデバイスコンテキスト					：ID3D11DeviceContext型
	);																//=========================================================================================================================================================

	IF_HRS(hr) {
		Debug::Log("DirectXManager::CreateDeviceInit Method Success.");
	}

	IF_HRF(hr) {
		MessageBoxUtility::HresultErrorMessage(
			"DirectXManager::CreateDeviceInit Method.\n",
			hr
		);
	}
}

void DirectXManager::CreateSwapChainInit() {
	auto& instance = DirectXManager::GetInstance();
	SwapChainDesc desc;

	// SwapChainDescの初期化
	{
		// BufferDescの情報：https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/bb173064(v=vs.85)

		ZeroMemory(&desc, sizeof(SwapChainDesc));
		desc.OutputWindow = Application::GetWindowHandle();			// 関連付けるウィンドウのウィンドウハンドル
		desc.BufferCount = 1;													// バックバッファの数　※ダブルバッファのためフロントバッファ1枚とバックバッファ1枚の組み合わせ
		desc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;					// フロントバッファの切り替えを行った時のバッファの内容をどうするかを指定するもの
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			// バッファの使用法を決める。バッファはシェーダリソースやレンダーターゲット、アンオーダードアクセスとして設定することができる
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;			// IDXGISwapChainの動作を指定するフラグ
		desc.SampleDesc.Count = 1;												// マルチサンプルの数
		desc.SampleDesc.Quality = 0;											// マルチサンプルのクオリティ
		desc.Windowed = true;													// true : windowMode, false : fullscreenMode
		desc.BufferDesc.Width = Application::GetWindowSize().x;		// バックバッファの横幅
		desc.BufferDesc.Height = Application::GetWindowSize().y;		// バックバッファの高さ
		desc.BufferDesc.RefreshRate.Numerator = 60;						// リフレッシュレート分母
		desc.BufferDesc.RefreshRate.Denominator = 1;						// リフレッシュレート分子
		desc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;		// フォーマット	// 機能レベル9.1以上
		//desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// フォーマット
		//desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// フォーマット
	}

	// SwapChainの作成
	{
		if (instance.m_swapChain) {
			instance.m_swapChain.Reset();
			Debug::Log("This SwapChain was previously created.");
		}

		auto hr = instance.m_factory->CreateSwapChain(
			instance.m_device.Get(),
			&desc,
			instance.m_swapChain.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_swapChainDesc = desc;
			Debug::Log("DirectXManager::CreateSwapChainInit Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DirectXManager::CreateSwapChainInit Method.\n",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DirectXManager::CreateSwapChainInit Method.\n",
				hr
			);
#endif
		}
	}
}

void DirectXManager::CreateRasterizerInit() {
	auto& instance = DirectXManager::GetInstance();
	RasterizerDesc desc;

	// RasterizerDescの初期化
	{
		ZeroMemory(&desc, sizeof(RasterizerDesc));
		desc.AntialiasedLineEnable = true;
		desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = true;
		desc.FrontCounterClockwise = false;
		desc.MultisampleEnable = false;
		desc.ScissorEnable = false;
		desc.SlopeScaledDepthBias = 0.0f;
	}

	// RasterizerStateの作成
	{
		if (instance.m_rasterizer) {
			instance.m_rasterizer.Reset();
			Debug::Log("This RasterizerState was previously created.");
		}

		auto hr = instance.m_device->CreateRasterizerState(
			&desc,
			instance.m_rasterizer.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_rasterizerDesc = desc;
			Debug::Log("DirectXManager::CreateRasterizerInit Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DirectXManager::CreateRasterizerInit Method.\n",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DirectXManager::CreateRasterizerInit Method.\n",
				hr
			);
#endif
		}
	}
}

void DirectXManager::CreateViewPortInit() {
	auto& instance = DirectXManager::GetInstance();
	ViewPort viewPort;

	// ViewPortの初期化
	{
		viewPort.Width = Application::GetWindowSize().x;	// ビューポートの大きさ
		viewPort.Height = Application::GetWindowSize().y;	// ビューポートの大きさ
		viewPort.MinDepth = 0.0f;									// 深度値の扱える範囲の下限
		viewPort.MaxDepth = 1.0f;									// 深度値の扱える範囲の上限
		viewPort.TopLeftX = 0.0f;									// ビューポートの左上座標　※スクリーン座標系で左上
		viewPort.TopLeftY = 0.0f;									// ビューポートの左上座標　※スクリーン座標系で左上
	}

	// ViewPortを設定
	{
		instance.m_immediateContext->RSSetViewports(1, &viewPort);
		instance.m_viewPort = viewPort;
	}

	Debug::Log("DirectXManager::CreateViewPortInit Method Success.");
}

void DirectXManager::CreateAndSetSwapChain(SwapChainDesc desc) {
	auto& instance = DirectXManager::GetInstance();

	if (instance.m_swapChain) {
		instance.m_swapChain.Reset();
		Debug::Log("This SwapChain was previously created.");
	}

	auto hr = instance.m_factory->CreateSwapChain(
		instance.m_device.Get(),
		&desc,
		instance.m_swapChain.GetAddressOf()
	);

	IF_HRS(hr) {
		instance.m_swapChainDesc = desc;
		Debug::Log("DirectXManager::CreateAndSetSwapChain Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DirectXManager::CreateAndSetSwapChain Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DirectXManager::CreateAndSetSwapChain Method.\n",
			hr
		);
#endif
	}
}

void DirectXManager::CreateAndSetRasterizer(RasterizerDesc desc) {
	auto& instance = DirectXManager::GetInstance();

	if (instance.m_rasterizer) {
		instance.m_rasterizer.Reset();
		Debug::Log("This RasterizerState was previously created.");
	}

	auto hr = instance.m_device->CreateRasterizerState(
		&desc,
		instance.m_rasterizer.GetAddressOf()
	);

	IF_HRS(hr) {
		instance.m_rasterizerDesc = desc;
		Debug::Log("DirectXManager::CreateAndSetRasterizer Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DirectXManager::CreateAndSetRasterizer Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DirectXManager::CreateAndSetRasterizer Method.\n",
			hr
		);
#endif
	}
}

bool DirectXManager::IsInitialized() {
	auto& instance = DirectXManager::GetInstance();
	return (instance.m_initializeFlag == true);
}

bool DirectXManager::IsFinalized() {
	auto& instance = DirectXManager::GetInstance();
	return (instance.m_finalizeFlag == true);
}

void DirectXManager::Initialize() {
	auto& instance = DirectXManager::GetInstance();

	if (instance.m_initializeFlag == false) {
		Debug::BracketBegin("DirectXManager::Initialize Method");

		// DirectXManagerの初期化関数をすべて実行
		DirectXManager::CreateDXGIFactoryInit();
		DirectXManager::CreateDXGIAdapterInit();
		DirectXManager::CreateDeviceInit();
		DirectXManager::CreateSwapChainInit();
		DirectXManager::CreateRasterizerInit();
		DirectXManager::CreateViewPortInit();

		// DirectX関係の他のクラスの初期化
		DXDepthStencilDefault::Initialize(instance.m_device.Get());
		DXRasterizerContainer::Initialize(instance.m_device.Get());
		DXSamplerContainer::Initialize(instance.m_device.Get());
		DXBlendContainer::Initialize(instance.m_device.Get());
		DXBackBuffer::Initialize(instance.m_device.Get(), instance.m_swapChain.Get());

		// 初期化フラグの値を変更
		instance.m_initializeFlag = true;

		Debug::BracketEnd();
	}
	else {
		Debug::LogWarning(
			"DirectXManager::Initialize Method.\n"
			"This function has been executed multiple times."
		);
	}
}

void DirectXManager::Finalize() {
	auto& instance = DirectXManager::GetInstance();

	if (instance.m_finalizeFlag == false) {
		Debug::BracketBegin("DirectXManager::Finalize Method");

		// 終了処理のフラグの値を変更
		instance.m_finalizeFlag = true;

		Debug::BracketEnd();
	}
	else {
		Debug::LogWarning(
			"DirectXManager::Finalize Method.\n"
			"This function has been executed multiple times."
		);
	}
}

void DirectXManager::CreateDebugReport() {
	Debug::BracketBegin("DebugReport");

	auto& instance = DirectXManager::GetInstance();

	auto hr = instance.m_device->QueryInterface(
		__uuidof(ID3D11Debug),
		reinterpret_cast<void**>(instance.m_debug.GetAddressOf())
	);

	IF_HRS(hr) {
		{
			Debug::BracketBegin("Summary Report");
			auto reportHr = instance.m_debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
			IF_HRS(reportHr) Debug::Log("ReportSuccess");
			IF_HRF(reportHr) Debug::Log("ReportFailed");
			Debug::BracketEnd();
		}
		{
			Debug::BracketBegin("Detail Report");
			auto reportHr = instance.m_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			IF_HRS(reportHr) Debug::Log("ReportSuccess");
			IF_HRF(reportHr) Debug::Log("ReportFailed");
			Debug::BracketEnd();
		}
		{
			Debug::BracketBegin("Ignore Internal Report");
			auto reportHr = instance.m_debug->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL);
			IF_HRS(reportHr) Debug::Log("ReportSuccess");
			IF_HRF(reportHr) Debug::Log("ReportFailed");
			Debug::BracketEnd();
		}
		Debug::Log("DirectXManager::CreateDebugReport Mehod Success.");
	}

	IF_HRF(hr) {
		Debug::Log("DirectXManager::CreateDebugReport Mehod Failed.");
	}

	Debug::BracketEnd();
}

void DirectXManager::CreateDebugReportIDXGI() {
	Debug::BracketBegin("DebugReportIDXGI");

	auto& instance = DirectXManager::GetInstance();

	// TODO：ここに一応Windows8以上かを判定する処理を入れたほうが良い

	typedef HRESULT(__stdcall* FuncPtr)(const IID&, void**);
	auto hModule = GetModuleHandleW(L"dxgidebug.dll");
	auto debugInterface = reinterpret_cast<FuncPtr>(GetProcAddress(hModule, "DXGIGetDebugInterface"));
	auto hr = debugInterface(__uuidof(IDxgiDebug), reinterpret_cast<void**>(instance.m_idxgiDebug.GetAddressOf()));

	IF_HRS(hr) {
		// 表示される内容に関する資料
		// https://docs.microsoft.com/ja-jp/windows/win32/direct3ddxgi/dxgi-debug-id?msclkid=d1364917b5f311ecae75542f26856abe
		{
			Debug::BracketBegin("DXGI_DEBUG_D3D11");
			{
				Debug::BracketBegin("Summary Report");
				auto reportHr = instance.m_idxgiDebug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_FLAGS::DXGI_DEBUG_RLO_SUMMARY);
				IF_HRS(reportHr) Debug::Log("ReportSuccess");
				IF_HRF(reportHr) Debug::Log("ReportFailed");
				Debug::BracketEnd();
			}
			{
				Debug::BracketBegin("Detail Report");
				auto reportHr = instance.m_idxgiDebug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_FLAGS::DXGI_DEBUG_RLO_DETAIL);
				IF_HRS(reportHr) Debug::Log("ReportSuccess");
				IF_HRF(reportHr) Debug::Log("ReportFailed");
				Debug::BracketEnd();
			}
			{
				Debug::BracketBegin("Ignore Internal Report");
				auto reportHr = instance.m_idxgiDebug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_FLAGS::DXGI_DEBUG_RLO_IGNORE_INTERNAL);
				IF_HRS(reportHr) Debug::Log("ReportSuccess");
				IF_HRF(reportHr) Debug::Log("ReportFailed");
				Debug::BracketEnd();
			}
			{
				Debug::BracketBegin("All Report");
				auto reportHr = instance.m_idxgiDebug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_FLAGS::DXGI_DEBUG_RLO_ALL);
				IF_HRS(reportHr) Debug::Log("ReportSuccess");
				IF_HRF(reportHr) Debug::Log("ReportFailed");
				Debug::BracketEnd();
			}
			Debug::BracketEnd();
		}
		{
			Debug::BracketBegin("DXGI_DEBUG_DXGI");
			{
				Debug::BracketBegin("Summary Report");
				auto reportHr = instance.m_idxgiDebug->ReportLiveObjects(DXGI_DEBUG_DXGI, DXGI_DEBUG_RLO_FLAGS::DXGI_DEBUG_RLO_SUMMARY);
				IF_HRS(reportHr) Debug::Log("ReportSuccess");
				IF_HRF(reportHr) Debug::Log("ReportFailed");
				Debug::BracketEnd();
			}
			{
				Debug::BracketBegin("Detail Report");
				auto reportHr = instance.m_idxgiDebug->ReportLiveObjects(DXGI_DEBUG_DXGI, DXGI_DEBUG_RLO_FLAGS::DXGI_DEBUG_RLO_DETAIL);
				IF_HRS(reportHr) Debug::Log("ReportSuccess");
				IF_HRF(reportHr) Debug::Log("ReportFailed");
				Debug::BracketEnd();
			}
			{
				Debug::BracketBegin("Ignore Internal Report");
				auto reportHr = instance.m_idxgiDebug->ReportLiveObjects(DXGI_DEBUG_DXGI, DXGI_DEBUG_RLO_FLAGS::DXGI_DEBUG_RLO_IGNORE_INTERNAL);
				IF_HRS(reportHr) Debug::Log("ReportSuccess");
				IF_HRF(reportHr) Debug::Log("ReportFailed");
				Debug::BracketEnd();
			}
			{
				Debug::BracketBegin("All Report");
				auto reportHr = instance.m_idxgiDebug->ReportLiveObjects(DXGI_DEBUG_DXGI, DXGI_DEBUG_RLO_FLAGS::DXGI_DEBUG_RLO_ALL);
				IF_HRS(reportHr) Debug::Log("ReportSuccess");
				IF_HRF(reportHr) Debug::Log("ReportFailed");
				Debug::BracketEnd();
			}
			Debug::BracketEnd();
		}
		{
			Debug::BracketBegin("DXGI_DEBUG_APP");
			{
				Debug::BracketBegin("Summary Report");
				auto reportHr = instance.m_idxgiDebug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_FLAGS::DXGI_DEBUG_RLO_SUMMARY);
				IF_HRS(reportHr) Debug::Log("ReportSuccess");
				IF_HRF(reportHr) Debug::Log("ReportFailed");
				Debug::BracketEnd();
			}
			{
				Debug::BracketBegin("Detail Report");
				auto reportHr = instance.m_idxgiDebug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_FLAGS::DXGI_DEBUG_RLO_DETAIL);
				IF_HRS(reportHr) Debug::Log("ReportSuccess");
				IF_HRF(reportHr) Debug::Log("ReportFailed");
				Debug::BracketEnd();
			}
			{
				Debug::BracketBegin("Ignore Internal Report");
				auto reportHr = instance.m_idxgiDebug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_FLAGS::DXGI_DEBUG_RLO_IGNORE_INTERNAL);
				IF_HRS(reportHr) Debug::Log("ReportSuccess");
				IF_HRF(reportHr) Debug::Log("ReportFailed");
				Debug::BracketEnd();
			}
			{
				Debug::BracketBegin("All Report");
				auto reportHr = instance.m_idxgiDebug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_FLAGS::DXGI_DEBUG_RLO_ALL);
				IF_HRS(reportHr) Debug::Log("ReportSuccess");
				IF_HRF(reportHr) Debug::Log("ReportFailed");
				Debug::BracketEnd();
			}
			Debug::BracketEnd();
		}
		Debug::Log("DirectXManager::CreateDebugReportIDXGI Success.");
	}

	IF_HRF(hr) {
		Debug::Log("DirectXManager::CreateDebugReportIDXGI Failed.");
	}

	Debug::BracketEnd();
}

ComObject::DirectXDevice DirectXManager::GetDevice() {
	auto& instance = DirectXManager::GetInstance();
	return instance.m_device;
}

ComObject::ImmediateContext DirectXManager::GetImmediateContext() {
	auto& instance = DirectXManager::GetInstance();
	return instance.m_immediateContext;
}

ComObject::SwapChain DirectXManager::GetSwapChain() {
	auto& instance = DirectXManager::GetInstance();
	return instance.m_swapChain;
}

ComObject::RasterizerState DirectXManager::GetRasterizerState() {
	auto& instance = DirectXManager::GetInstance();
	return instance.m_rasterizer;
}

SwapChainDesc DirectXManager::GetSwapChainDesc() {
	auto& instance = DirectXManager::GetInstance();
	return instance.m_swapChainDesc;
}

RasterizerDesc DirectXManager::GetRasterizerDesc() {
	auto& instance = DirectXManager::GetInstance();
	return instance.m_rasterizerDesc;
}
