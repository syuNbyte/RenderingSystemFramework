
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

	// ���s�����ꍇ
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
		D3D_FEATURE_LEVEL_11_1,					// DirectX11.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_11_0,					// DirectX11�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_1,					// DirectX10.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_0,					// DirectX10�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_3,					// DirectX9.3�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_2,					// DirectX9.2�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_1					// Direct9.1�Ή�GPU���x��
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
																	// �����̓��e												�F�ڍ�
	auto hr = D3D11CreateDevice(							//=========================================================================================================================================================						
		instance.m_adapter.Get(),							// GPU�A�_�v�^												�F�g�p����GPU�A�_�v�^��I���@��nullptr��ݒ肷��Ǝg�p�\��GPU�A�_�v�^�̒����玩���I�ɑI�����Ă���
		D3D_DRIVER_TYPE_UNKNOWN,							// �h���C�o�^�C�v											�F����͎w�肷��̂�UNKNOWN�@��GPU�A�_�v�^���w�肵�č쐬����Ƃ��͕K��D3D_DRIVER_TYPE_UNKNOWN���w�肷��K�v������
		nullptr,													// �\�t�g�E�F�A�ɂ�郉�X�^���C�U����������DLL	�FDriverType��D3D_DRIVER_TYPE_SOFTWARE���w�肵���ۂɎg�p����
		flags,													// �����^�C�����C���[��\���t���O					�FD3D11_CREATE_DEVICE_FLAG�Œ�`����Ă���
		featureLevels.data(),								// �g�p�������@�\���x���̔z��̐擪�A�h���X		�F�D�揇�ʂ͗v�f�̎Ⴂ����
		static_cast<UINT>(featureLevels.size()),		// �@�\���x���̔z��̗v�f��							�F
		D3D11_SDK_VERSION,									// SDK�̃o�[�W����										�FD3D11���g�p����̂�D3D11_SDK_VERSION���g�p����
		instance.m_device.GetAddressOf(),				// ����������f�o�C�X									�FID3D11Device�^
		&usedFeatureLevel,									// �@�\���x�����󂯎��ϐ�							�FD3D_FEATURE_LEVEL�^
		instance.m_immediateContext.GetAddressOf()	// ����������f�o�C�X�R���e�L�X�g					�FID3D11DeviceContext�^
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

	// SwapChainDesc�̏�����
	{
		// BufferDesc�̏��Fhttps://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/bb173064(v=vs.85)

		ZeroMemory(&desc, sizeof(SwapChainDesc));
		desc.OutputWindow = Application::GetWindowHandle();			// �֘A�t����E�B���h�E�̃E�B���h�E�n���h��
		desc.BufferCount = 1;													// �o�b�N�o�b�t�@�̐��@���_�u���o�b�t�@�̂��߃t�����g�o�b�t�@1���ƃo�b�N�o�b�t�@1���̑g�ݍ��킹
		desc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;					// �t�����g�o�b�t�@�̐؂�ւ����s�������̃o�b�t�@�̓��e���ǂ����邩���w�肷�����
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			// �o�b�t�@�̎g�p�@�����߂�B�o�b�t�@�̓V�F�[�_���\�[�X�⃌���_�[�^�[�Q�b�g�A�A���I�[�_�[�h�A�N�Z�X�Ƃ��Đݒ肷�邱�Ƃ��ł���
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;			// IDXGISwapChain�̓�����w�肷��t���O
		desc.SampleDesc.Count = 1;												// �}���`�T���v���̐�
		desc.SampleDesc.Quality = 0;											// �}���`�T���v���̃N�I���e�B
		desc.Windowed = true;													// true : windowMode, false : fullscreenMode
		desc.BufferDesc.Width = Application::GetWindowSize().x;		// �o�b�N�o�b�t�@�̉���
		desc.BufferDesc.Height = Application::GetWindowSize().y;		// �o�b�N�o�b�t�@�̍���
		desc.BufferDesc.RefreshRate.Numerator = 60;						// ���t���b�V�����[�g����
		desc.BufferDesc.RefreshRate.Denominator = 1;						// ���t���b�V�����[�g���q
		desc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;		// �t�H�[�}�b�g	// �@�\���x��9.1�ȏ�
		//desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// �t�H�[�}�b�g
		//desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// �t�H�[�}�b�g
	}

	// SwapChain�̍쐬
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

	// RasterizerDesc�̏�����
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

	// RasterizerState�̍쐬
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

	// ViewPort�̏�����
	{
		viewPort.Width = Application::GetWindowSize().x;	// �r���[�|�[�g�̑傫��
		viewPort.Height = Application::GetWindowSize().y;	// �r���[�|�[�g�̑傫��
		viewPort.MinDepth = 0.0f;									// �[�x�l�̈�����͈͂̉���
		viewPort.MaxDepth = 1.0f;									// �[�x�l�̈�����͈͂̏��
		viewPort.TopLeftX = 0.0f;									// �r���[�|�[�g�̍�����W�@���X�N���[�����W�n�ō���
		viewPort.TopLeftY = 0.0f;									// �r���[�|�[�g�̍�����W�@���X�N���[�����W�n�ō���
	}

	// ViewPort��ݒ�
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

		// DirectXManager�̏������֐������ׂĎ��s
		DirectXManager::CreateDXGIFactoryInit();
		DirectXManager::CreateDXGIAdapterInit();
		DirectXManager::CreateDeviceInit();
		DirectXManager::CreateSwapChainInit();
		DirectXManager::CreateRasterizerInit();
		DirectXManager::CreateViewPortInit();

		// DirectX�֌W�̑��̃N���X�̏�����
		DXDepthStencilDefault::Initialize(instance.m_device.Get());
		DXRasterizerContainer::Initialize(instance.m_device.Get());
		DXSamplerContainer::Initialize(instance.m_device.Get());
		DXBlendContainer::Initialize(instance.m_device.Get());
		DXBackBuffer::Initialize(instance.m_device.Get(), instance.m_swapChain.Get());

		// �������t���O�̒l��ύX
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

		// �I�������̃t���O�̒l��ύX
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

	// TODO�F�����ɈꉞWindows8�ȏォ�𔻒肷�鏈������ꂽ�ق����ǂ�

	typedef HRESULT(__stdcall* FuncPtr)(const IID&, void**);
	auto hModule = GetModuleHandleW(L"dxgidebug.dll");
	auto debugInterface = reinterpret_cast<FuncPtr>(GetProcAddress(hModule, "DXGIGetDebugInterface"));
	auto hr = debugInterface(__uuidof(IDxgiDebug), reinterpret_cast<void**>(instance.m_idxgiDebug.GetAddressOf()));

	IF_HRS(hr) {
		// �\���������e�Ɋւ��鎑��
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
