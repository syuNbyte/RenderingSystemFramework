
// TNLIB
#include"Debug.hpp"

// MyFile
#include"BlendStateContainer.h"
#include"DirectXManager.h"
#include"HresultUtility.h"
#include"MessageBoxUtility.h"

BlendStateContainer& BlendStateContainer::GetInstance() {
	static BlendStateContainer blendStateContainer;
	return blendStateContainer;
}

void BlendStateContainer::CreateNone() {
	auto& instance = BlendStateContainer::GetInstance();
	BlendDesc desc;

	// BlendDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= false;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendState‚Ìì¬
	{
		auto hr = DirectXManager::GetDevice()->CreateBlendState(
			&desc,
			instance.m_none.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_noneDesc = desc;
			Debug::Log("BlendStateContainer::CreateNone Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"BlendStateContainer::CreateNone Method.\n",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"BlendStateContainer::CreateNone Method.\n",
				hr
			);
#endif
		}
	}
}

void BlendStateContainer::CreateDefault() {
	auto& instance = BlendStateContainer::GetInstance();
	BlendDesc desc;

	// BlendDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendState‚Ìì¬
	{
		auto hr = DirectXManager::GetDevice()->CreateBlendState(
			&desc,
			instance.m_default.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_defaultDesc = desc;
			Debug::Log("BlendStateContainer::CreateDefault Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"BlendStateContainer::CreateDefault Method.\n",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"BlendStateContainer::CreateDefault Method.\n",
				hr
			);
#endif
		}
	}
}

void BlendStateContainer::CreateSrcAlpha() {
	auto& instance = BlendStateContainer::GetInstance();
	BlendDesc desc;

	// BlendDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendState‚Ìì¬
	{
		auto hr = DirectXManager::GetDevice()->CreateBlendState(
			&desc,
			instance.m_srcAlpha.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_srcAlphaDesc = desc;
			Debug::Log("BlendStateContainer::CreateSrcAlpha Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"BlendStateContainer::CreateSrcAlpha Method.\n",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"BlendStateContainer::CreateSrcAlpha Method.\n",
				hr
			);
#endif
		}
	}
}

void BlendStateContainer::CreateAlpha() {
	auto& instance = BlendStateContainer::GetInstance();
	BlendDesc desc;

	// BlendDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendState‚Ìì¬
	{
		auto hr = DirectXManager::GetDevice()->CreateBlendState(
			&desc,
			instance.m_alpha.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_alphaDesc = desc;
			Debug::Log("BlendStateContainer::CreateAlpha Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"BlendStateContainer::CreateAlpha Method.\n",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"BlendStateContainer::CreateAlpha Method.\n",
				hr
			);
#endif
		}
	}
}

void BlendStateContainer::CreateAlphaToCoverage() {
	auto& instance = BlendStateContainer::GetInstance();
	BlendDesc desc;

	// BlendDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.AlphaToCoverageEnable						= true;
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendState‚Ìì¬
	{
		auto hr = DirectXManager::GetDevice()->CreateBlendState(
			&desc,
			instance.m_alphaToCoverage.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_alphaToCoverageDesc = desc;
			Debug::Log("BlendStateContainer::CreateAlphaToCoverage Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"BlendStateContainer::CreateAlphaToCoverage Method.\n",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"BlendStateContainer::CreateAlphaToCoverage Method.\n",
				hr
			);
#endif
		}
	}
}

void BlendStateContainer::CreateAdd() {
	auto& instance = BlendStateContainer::GetInstance();
	BlendDesc desc;

	// BlendDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendState‚Ìì¬
	{
		auto hr = DirectXManager::GetDevice()->CreateBlendState(
			&desc,
			instance.m_add.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_addDesc = desc;
			Debug::Log("BlendStateContainer::CreateAdd Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"BlendStateContainer::CreateAdd Method.\n",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"BlendStateContainer::CreateAdd Method.\n",
				hr
			);
#endif
		}
	}
}

void BlendStateContainer::CreateAddAlpha() {
	auto& instance = BlendStateContainer::GetInstance();
	BlendDesc desc;

	// BlendDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_ONE;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendState‚Ìì¬
	{
		auto hr = DirectXManager::GetDevice()->CreateBlendState(
			&desc,
			instance.m_addAlpha.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_addAlphaDesc = desc;
			Debug::Log("BlendStateContainer::CreateAddAlpha Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"BlendStateContainer::CreateAddAlpha Method.\n",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"BlendStateContainer::CreateAddAlpha Method.\n",
				hr
			);
#endif
		}
	}
}

void BlendStateContainer::CreateSub() {
	auto& instance = BlendStateContainer::GetInstance();
	BlendDesc desc;

	// BlendDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_ONE;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_REV_SUBTRACT;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendState‚Ìì¬
	{
		auto hr = DirectXManager::GetDevice()->CreateBlendState(
			&desc,
			instance.m_sub.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_subDesc = desc;
			Debug::Log("BlendStateContainer::CreateSub Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"BlendStateContainer::CreateSub Method.\n",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"BlendStateContainer::CreateSub Method.\n",
				hr
			);
#endif
		}
	}
}

void BlendStateContainer::CreateMul() {
	auto& instance = BlendStateContainer::GetInstance();
	BlendDesc desc;

	// BlendDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_ZERO;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_SRC_COLOR;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendState‚Ìì¬
	{
		auto hr = DirectXManager::GetDevice()->CreateBlendState(
			&desc,
			instance.m_mul.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_mulDesc = desc;
			Debug::Log("BlendStateContainer::CreateMul Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"BlendStateContainer::CreateMul Method.\n",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"BlendStateContainer::CreateMul Method.\n",
				hr
			);
#endif
		}
	}
}

void BlendStateContainer::CreateAlign() {
	auto& instance = BlendStateContainer::GetInstance();
	BlendDesc desc;

	// BlendDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendState‚Ìì¬
	{
		auto hr = DirectXManager::GetDevice()->CreateBlendState(
			&desc,
			instance.m_align.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_alignDesc = desc;
			Debug::Log("BlendStateContainer::CreateAlign Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"BlendStateContainer::CreateAlign Method.\n",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"BlendStateContainer::CreateAlign Method.\n",
				hr
			);
#endif
		}
	}
}

void BlendStateContainer::CreateScreen() {
	auto& instance = BlendStateContainer::GetInstance();
	BlendDesc desc;

	// BlendDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_INV_DEST_COLOR;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_ONE;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendState‚Ìì¬
	{
		auto hr = DirectXManager::GetDevice()->CreateBlendState(
			&desc,
			instance.m_screen.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_screenDesc = desc;
			Debug::Log("BlendStateContainer::CreateScreen Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"BlendStateContainer::CreateScreen Method.\n",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"BlendStateContainer::CreateScreen Method.\n",
				hr
			);
#endif
		}
	}
}

void BlendStateContainer::CreateReverse() {
	auto& instance = BlendStateContainer::GetInstance();
	BlendDesc desc;

	// BlendDesc‚Ì‰Šú‰»
	{
		ZeroMemory(&desc, sizeof(BlendDesc));
		desc.RenderTarget->BlendEnable				= true;
		desc.RenderTarget->SrcBlend					= D3D11_BLEND_INV_DEST_COLOR;
		desc.RenderTarget->DestBlend					= D3D11_BLEND_INV_SRC_COLOR;
		desc.RenderTarget->BlendOp						= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->SrcBlendAlpha				= D3D11_BLEND_ONE;
		desc.RenderTarget->DestBlendAlpha			= D3D11_BLEND_ZERO;
		desc.RenderTarget->BlendOpAlpha				= D3D11_BLEND_OP_ADD;
		desc.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	// BlendState‚Ìì¬
	{
		auto hr = DirectXManager::GetDevice()->CreateBlendState(
			&desc,
			instance.m_reverse.GetAddressOf()
		);

		IF_HRS(hr) {
			instance.m_reverseDesc = desc;
			Debug::Log("BlendStateContainer::CreateReverse Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"BlendStateContainer::CreateReverse Method.\n",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"BlendStateContainer::CreateReverse Method.\n",
				hr
			);
#endif
		}
	}
}

void BlendStateContainer::Initialize() {
	Debug::BracketBegin("BlendStateContainer::Initialize Method.");

	BlendStateContainer::CreateNone();
	BlendStateContainer::CreateDefault();
	BlendStateContainer::CreateSrcAlpha();
	BlendStateContainer::CreateAlpha();
	BlendStateContainer::CreateAlphaToCoverage();
	BlendStateContainer::CreateAdd();
	BlendStateContainer::CreateAddAlpha();
	BlendStateContainer::CreateSub();
	BlendStateContainer::CreateMul();
	BlendStateContainer::CreateAlign();
	BlendStateContainer::CreateScreen();
	BlendStateContainer::CreateReverse();

	Debug::BracketEnd();
}

ComObject::BlendState BlendStateContainer::GetNone() {
	auto& instance = BlendStateContainer::GetInstance();
	return instance.m_none;
}

ComObject::BlendState BlendStateContainer::GetDefault() {
	auto& instance = BlendStateContainer::GetInstance();
	return instance.m_default;
}

ComObject::BlendState BlendStateContainer::GetSrcAlpha() {
	auto& instance = BlendStateContainer::GetInstance();
	return instance.m_srcAlpha;
}

ComObject::BlendState BlendStateContainer::GetAlpha() {
	auto& instance = BlendStateContainer::GetInstance();
	return instance.m_alpha;
}

ComObject::BlendState BlendStateContainer::GetAlphaToCoverage() {
	auto& instance = BlendStateContainer::GetInstance();
	return instance.m_alphaToCoverage;
}

ComObject::BlendState BlendStateContainer::GetAdd() {
	auto& instance = BlendStateContainer::GetInstance();
	return instance.m_add;
}

ComObject::BlendState BlendStateContainer::GetAddAlpha() {
	auto& instance = BlendStateContainer::GetInstance();
	return instance.m_addAlpha;
}

ComObject::BlendState BlendStateContainer::GetSub() {
	auto& instance = BlendStateContainer::GetInstance();
	return instance.m_sub;
}

ComObject::BlendState BlendStateContainer::GetMul() {
	auto& instance = BlendStateContainer::GetInstance();
	return instance.m_mul;
}

ComObject::BlendState BlendStateContainer::GetAlign() {
	auto& instance = BlendStateContainer::GetInstance();
	return instance.m_align;
}

ComObject::BlendState BlendStateContainer::GetScreen() {
	auto& instance = BlendStateContainer::GetInstance();
	return instance.m_screen;
}

ComObject::BlendState BlendStateContainer::GetReverse() {
	auto& instance = BlendStateContainer::GetInstance();
	return instance.m_reverse;
}
