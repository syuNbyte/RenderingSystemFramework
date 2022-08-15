#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"

// DirectX�̃R���e�L�X�g�Ȃǂ��܂Ƃ߂Ă����V���O���g���N���X
class DirectXManager {
private:
	ComObject::DirectXDevice m_device;					// 
	ComObject::ImmediateContext m_immediateContext;	// 
	ComObject::IDxgiAdapter_1 m_adapter;				// 
	ComObject::IDxgiFactory_1 m_factory;				// 
	ComObject::SwapChain m_swapChain;					// 
	ComObject::RasterizerState m_rasterizer;			// 
	ComObject::DirectXDebug m_debug;						// 
	ComObject::IDxgiDebug_1 m_idxgiDebug;				// 
private:
	SwapChainDesc m_swapChainDesc;						// 
	RasterizerDesc m_rasterizerDesc;						// 
	ViewPort m_viewPort;										// Application�̕ێ������ʃT�C�Y�Ɠ����̃T�C�Y�̃r���[�|�[�g
private:
	bool m_initializeFlag;
	bool m_finalizeFlag;
private:
	static DirectXManager& GetInstance();
	static void CreateDXGIFactoryInit();
	static void CreateDXGIAdapterInit();
	static void CreateDeviceInit();
	static void CreateSwapChainInit();
	static void CreateRasterizerInit();
	static void CreateViewPortInit();
public:
	static SwapChainDesc GetSwapChainDesc();
	static RasterizerDesc GetRasterizerDesc();
public:
	static void CreateAndSetSwapChain(SwapChainDesc desc);
	static void CreateAndSetRasterizer(RasterizerDesc desc);
public:
	static bool IsInitialized();
	static bool IsFinalized();
	static void Initialize();
	static void Finalize();
	static void CreateDebugReport();
	static void CreateDebugReportIDXGI();
	static ComObject::DirectXDevice GetDevice();
	static ComObject::ImmediateContext GetImmediateContext();
	static ComObject::SwapChain GetSwapChain();
	static ComObject::RasterizerState GetRasterizerState();
};