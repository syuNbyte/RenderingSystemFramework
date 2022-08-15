
// STL
#include<memory>

// DirectXTex
#include"DirectXTex.h"
#include"WICTextureLoader.h"
#include"DDSTextureLoader.h"

// TNLIB
#include"Debug.hpp"
#include"Utility.hpp"
#include"FileUtility.hpp"

// MyFile
#include"ShaderResourceView.h"
#include"TextureMetaData.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

void DXShaderResourceView::CreateWICTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context) {
	auto image = std::make_unique<DirectX::ScratchImage>();
	auto resource = ComObject::Resource();

	if (m_srv) {
		m_srv.Reset();
		Debug::Log("This ShaderResourceView was previously created.");
	}

	auto hr = DirectX::CreateWICTextureFromFile(
		device,
		context,
		m_wpath.c_str(),
		resource.GetAddressOf(),
		m_srv.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXShaderResourceView::CreateWICTextureFromFile Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXShaderResourceView::CreateWICTextureFromFile Method.\n",
			HresultUtility::GetHresultMessage(hr),
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXShaderResourceView::CreateWICTextureFromFile Method.\n",
			hr
		);
#endif
	}
}

void DXShaderResourceView::CreateDDSTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context) {
	auto image = std::make_unique<DirectX::ScratchImage>();
	auto resource = ComObject::Resource();

	if (m_srv) {
		m_srv.Reset();
		Debug::Log("This ShaderResourceView was previously created.");
	}

	auto hr = DirectX::CreateDDSTextureFromFile(
		device,
		context,
		m_wpath.c_str(),
		resource.GetAddressOf(),
		m_srv.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXShaderResourceView::CreateDDSTextureFromFile Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXShaderResourceView::CreateDDSTextureFromFile Method.\n",
			HresultUtility::GetHresultMessage(hr),
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXShaderResourceView::CreateDDSTextureFromFile Method.\n",
			hr
		);
#endif
	}
}

void DXShaderResourceView::CreateTGATextureFromFile(DirectXDevice* device, DirectXDeviceContext* context) {
	auto image = std::make_unique<DirectX::ScratchImage>();
	auto resource = ComObject::Resource();

	// TGAファイルのロード
	{
		auto hr = DirectX::LoadFromTGAFile(
			m_wpath.c_str(),
			&m_metaData,
			*image
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceView::CreateTGATextureFromFile Method.\n",
				"DirectX::LoadFromTGAFile Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceView::CreateTGATextureFromFile Method.\n",
				"DirectX::LoadFromTGAFile Method.",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceView::CreateTGATextureFromFile Method.\n\
				DirectX::LoadFromTGAFile Method.",
				hr
			);
#endif
		}
	}

	// ロードしたデータからテクスチャを作成する
	{
		auto hr = DirectX::CreateTexture(
			device,
			image->GetImages(),
			image->GetImageCount(),
			m_metaData,
			resource.GetAddressOf()
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceView::CreateTGATextureFromFile Method.\n",
				"DirectX::CreateTexture Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceView::CreateTGATextureFromFile Method.\n",
				"DirectX::CreateTexture Method.",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceView::CreateTGATextureFromFile Method.\n\
				DirectX::CreateTexture Method.",
				hr
			);
#endif
		}
	}

	// シェーダーリソースビューを作成する
	{
		if (m_srv) {
			m_srv.Reset();
			Debug::Log("This ShaderResourceView was previously created.");
		}

		auto hr = DirectX::CreateShaderResourceView(
			device,
			image->GetImages(),
			image->GetImageCount(),
			m_metaData,
			m_srv.GetAddressOf()
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceView::CreateTGATextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceView::CreateTGATextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method.",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceView::CreateTGATextureFromFile Method.\n\
				DirectX::CreateShaderResourceView Method.",
				hr
			);
#endif
		}
	}
}

void DXShaderResourceView::CreateMipMapWICTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context) {
	auto image = std::make_unique<DirectX::ScratchImage>();

	// WICファイルのロード
	{
		auto hr = DirectX::LoadFromWICFile(
			m_wpath.c_str(),
			DirectX::WIC_FLAGS_NONE,
			&m_metaData,
			*image
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceView::CreateMipMapWICTextureFromFile Method.\n",
				"DirectX::LoadFromWICFile Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceView::CreateMipMapWICTextureFromFile Method.\n",
				"DirectX::LoadFromWICFile Method.",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceView::CreateMipMapWICTextureFromFile Method.\n\
				DirectX::LoadFromWICFile Method.",
				hr
			);
#endif
		}
	}

	// ロードしたデータからテクスチャを作成する
	{
		auto mipChain = std::make_unique<DirectX::ScratchImage>();

		auto hr = DirectX::GenerateMipMaps(
			image->GetImages(),
			image->GetImageCount(),
			image->GetMetadata(),
			DirectX::TEX_FILTER_DEFAULT,
			0,
			*mipChain
		);

		IF_HRS(hr) {
			image = std::move(mipChain);
			Debug::Log(
				"DXShaderResourceView::CreateMipMapWICTextureFromFile Method.\n",
				"DirectX::GenerateMpMaps Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceView::CreateMipMapWICTextureFromFile Method.\n",
				"DirectX::GenerateMpMaps Method.",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceView::CreateMipMapWICTextureFromFile Method.\n\
				DirectX::GenerateMpMaps Method.",
				hr
			);
#endif
		}
	}

	// シェーダーリソースビューを作成する
	{
		if (m_srv) {
			m_srv.Reset();
			Debug::Log("This ShaderResourceView was previously created.");
		}

		auto hr = DirectX::CreateShaderResourceView(
			device,
			image->GetImages(),
			image->GetImageCount(),
			m_metaData,
			m_srv.GetAddressOf()
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceView::CreateMipMapWICTextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceView::CreateMipMapWICTextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method.",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceView::CreateMipMapWICTextureFromFile Method.\n\
				DirectX::CreateShaderResourceView Method.",
				hr
			);
#endif
		}
	}
}

void DXShaderResourceView::CreateMipMapDDSTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context) {
	auto image = std::make_unique<DirectX::ScratchImage>();

	// DDSファイルのロード
	{
		auto hr = DirectX::LoadFromDDSFile(
			m_wpath.c_str(),
			DirectX::DDS_FLAGS_NONE,
			&m_metaData,
			*image
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceView::CreateMipMapDDSTextureFromFile Method.\n",
				"DirectX::LoadFromDDSFile Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceView::CreateMipMapDDSTextureFromFile Method.\n",
				"DirectX::LoadFromDDSFile Method.",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceView::CreateMipMapDDSTextureFromFile Method.\n\
				DirectX::LoadFromDDSFile Method.",
				hr
			);
#endif
		}
	}

	// ロードしたデータからテクスチャを作成する
	{
		auto mipChain = std::make_unique<DirectX::ScratchImage>();

		auto hr = DirectX::GenerateMipMaps(
			image->GetImages(),
			image->GetImageCount(),
			image->GetMetadata(),
			DirectX::TEX_FILTER_DEFAULT,
			0,
			*mipChain
		);

		IF_HRS(hr) {
			image = std::move(mipChain);
			Debug::Log(
				"DXShaderResourceView::CreateMipMapDDSTextureFromFile Method.\n",
				"DirectX::GenerateMpMaps Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceView::CreateMipMapDDSTextureFromFile Method.\n",
				"DirectX::GenerateMpMaps Method.",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceView::CreateMipMapDDSTextureFromFile Method.\n\
				DirectX::GenerateMpMaps Method.",
				hr
			);
#endif
		}
	}

	// シェーダーリソースビューを作成する
	{
		if (m_srv) {
			m_srv.Reset();
			Debug::Log("This ShaderResourceView was previously created.");
		}

		auto hr = DirectX::CreateShaderResourceView(
			device,
			image->GetImages(),
			image->GetImageCount(),
			m_metaData,
			m_srv.GetAddressOf()
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceView::CreateMipMapDDSTextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceView::CreateMipMapDDSTextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method.",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceView::CreateMipMapDDSTextureFromFile Method.\n\
				DirectX::CreateShaderResourceView Method.",
				hr
			);
#endif
		}
	}
}

void DXShaderResourceView::CreateMipMapTGATextureFromFile(DirectXDevice* device, DirectXDeviceContext* context) {
	auto image = std::make_unique<DirectX::ScratchImage>();

	// TGAファイルのロード
	{
		auto hr = DirectX::LoadFromTGAFile(
			m_wpath.c_str(),
			&m_metaData,
			*image
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceView::CreateMipMapTGATextureFromFile Method.\n",
				"DirectX::LoadFromTGAFile Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceView::CreateMipMapTGATextureFromFile Method.\n",
				"DirectX::LoadFromTGAFile Method.",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceView::CreateMipMapTGATextureFromFile Method.\n\
				DirectX::LoadFromTGAFile Method.",
				hr
			);
#endif
		}
	}

	// ロードしたデータからテクスチャを作成する
	{
		auto mipChain = std::make_unique<DirectX::ScratchImage>();

		auto hr = DirectX::GenerateMipMaps(
			image->GetImages(),
			image->GetImageCount(),
			image->GetMetadata(),
			DirectX::TEX_FILTER_DEFAULT,
			0,
			*mipChain
		);

		IF_HRS(hr) {
			image = std::move(mipChain);
			Debug::Log(
				"DXShaderResourceView::CreateMipMapTGATextureFromFile Method.\n",
				"DirectX::GenerateMpMaps Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceView::CreateMipMapTGATextureFromFile Method.\n",
				"DirectX::GenerateMpMaps Method.",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceView::CreateMipMapTGATextureFromFile Method.\n\
				DirectX::GenerateMpMaps Method.",
				hr
			);
#endif
		}
	}

	// シェーダーリソースビューを作成する
	{
		auto hr = DirectX::CreateShaderResourceView(
			device,
			image->GetImages(),
			image->GetImageCount(),
			m_metaData,
			m_srv.GetAddressOf()
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceView::CreateMipMapTGATextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceView::CreateMipMapTGATextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method.",
				HresultUtility::GetHresultMessage(hr),
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceView::CreateMipMapTGATextureFromFile Method.\n\
				DirectX::CreateShaderResourceView Method.",
				hr
			);
#endif
		}
	}
}

void DXShaderResourceView::CreateTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context) {
	Debug::BracketBegin("DXShaderResourceView::CreateTextureFromFile");

	Debug::Log("Path[", m_path, "]");
	Debug::Log("Name[", FileUtility::Utility::FileName(m_path), "]");
	Debug::Log("Extension[", FileUtility::Utility::FileExtension(m_path), "]");

	auto ext = FileUtility::Utility::FileExtension(m_path);
	m_wpath = Utility::ToWideStr(m_path);
	m_metaData = DXTextureMetaData::GetTextureMetaData(m_path);
	m_size = DXTextureMetaData::GetTextureSize(m_path);

	// DirectX内のメソッド名と被るためthisを使用している
	if (!m_path.empty()) {
		if (ext == "dds") {
			CreateDDSTextureFromFile(device,	context);
		}
		else if (ext == "tga") {
			CreateTGATextureFromFile(device, context);
		}
		else {
			CreateWICTextureFromFile(device, context);
		}
	}
	else {
		Debug::LogError("Not exist file path.");
	}

	Debug::BracketEnd();
}

void DXShaderResourceView::CreateMipMapTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context) {
	Debug::BracketBegin("DXShaderResourceView::CreateMipMapTextureFromFile");

	Debug::Log("Path[", m_path, "]");
	Debug::Log("Name[", FileUtility::Utility::FileName(m_path), "]");
	Debug::Log("Extension[", FileUtility::Utility::FileExtension(m_path), "]");

	auto ext = FileUtility::Utility::FileExtension(m_path);
	m_wpath = Utility::ToWideStr(m_path);
	m_metaData = DXTextureMetaData::GetTextureMetaData(m_path);
	m_size = DXTextureMetaData::GetTextureSize(m_path);

	// DirectX内のメソッド名と被るためthisを使用している
	if (!m_path.empty()) {
		if (ext == "dds") {
			CreateMipMapDDSTextureFromFile(device, context);
		}
		else if (ext == "tga") {
			CreateMipMapTGATextureFromFile(device, context);
		}
		else {
			CreateMipMapWICTextureFromFile(device, context);
		}
	}
	else {
		Debug::LogError("Not exist file path.");
	}

	Debug::BracketEnd();
}

void DXShaderResourceView::SetRegistNumber(const int_t& registNumber) {
	m_registNumber = registNumber;
}

void DXShaderResourceView::SetFilePath(const std::string& path) {
	m_path = path;
}

bool DXShaderResourceView::HasTexture() {
	return m_srv != nullptr;
}

ComObject::ShaderResourceView DXShaderResourceView::GetSRV() {
	return m_srv;
}

std::string DXShaderResourceView::GetFilePath() {
	return m_path;
}

std::wstring DXShaderResourceView::GetFilePathWide() {
	return m_wpath;
}

DXVector2Int DXShaderResourceView::GetSize() {
	return m_size;
}

void DXShaderResourceView::VSSetShaderResources(DirectXDeviceContext* context) {
	context->VSSetShaderResources(
		m_registNumber,
		1,
		m_srv.GetAddressOf()
	);
}

void DXShaderResourceView::PSSetShaderResources(DirectXDeviceContext* context) {
	context->PSSetShaderResources(
		m_registNumber,
		1,
		m_srv.GetAddressOf()
	);
}
