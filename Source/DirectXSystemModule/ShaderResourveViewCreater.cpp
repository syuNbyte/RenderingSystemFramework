
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
#include"ShaderResourveViewCreater.h"
#include"TextureMetaData.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

void DXShaderResourceViewCreater::CreateWICTextureFromFile(const std::string& path, DirectXDevice* device, DirectXDeviceContext* context, ShaderResourceView** srv, Resource** resource) {
	auto wpath = Utility::ToWideStr(path);

	auto hr = DirectX::CreateWICTextureFromFile(
		device,
		context,
		wpath.c_str(),
		resource,
		srv
	);

	IF_HRS(hr) {
		Debug::Log("DXShaderResourceViewCreater::CreateWICTextureFromFile Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXShaderResourceViewCreater::CreateWICTextureFromFile Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXShaderResourceViewCreater::CreateWICTextureFromFile Method.\n",
			hr
		);
#endif
	}
}

void DXShaderResourceViewCreater::CreateDDSTextureFromFile(const std::string& path, DirectXDevice* device, DirectXDeviceContext* context, ShaderResourceView** srv, Resource** resource) {
	auto wpath = Utility::ToWideStr(path);

	auto hr = DirectX::CreateDDSTextureFromFile(
		device,
		context,
		wpath.c_str(),
		resource,
		srv
	);

	IF_HRS(hr) {
		Debug::Log("DXShaderResourceViewCreater::CreateDDSTextureFromFile Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXShaderResourceViewCreater::CreateDDSTextureFromFile Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXShaderResourceViewCreater::CreateDDSTextureFromFile Method.\n",
			hr
		);
#endif
	}
}

void DXShaderResourceViewCreater::CreateTGATextureFromFile(const std::string& path, DirectXDevice* device, DirectXDeviceContext* context, ShaderResourceView** srv, Resource** resource) {
	auto wpath = Utility::ToWideStr(path);
	auto metaData = DXTextureMetaData::GetTextureMetaData(path);
	auto image = std::make_unique<DirectX::ScratchImage>();

	// TGAファイルのロード
	{
		auto hr = DirectX::LoadFromTGAFile(
			wpath.c_str(),
			&metaData,
			*image
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceViewCreater::CreateTGATextureFromFile Method.\n",
				"DirectX::LoadFromTGAFile Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceViewCreater::CreateTGATextureFromFile Method.\n",
				"DirectX::LoadFromTGAFile Method.",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceViewCreater::CreateTGATextureFromFile Method.\n\
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
			metaData,
			resource
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceViewCreater::CreateTGATextureFromFile Method.\n",
				"DirectX::CreateTexture Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceViewCreater::CreateTGATextureFromFile Method.\n",
				"DirectX::CreateTexture Method.",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceViewCreater::CreateTGATextureFromFile Method.\n\
				DirectX::CreateTexture Method.",
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
			metaData,
			srv
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceViewCreater::CreateTGATextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceViewCreater::CreateTGATextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method.",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceViewCreater::CreateTGATextureFromFile Method.\n\
				DirectX::CreateShaderResourceView Method.",
				hr
			);
#endif
		}
	}
}

void DXShaderResourceViewCreater::CreateMipMapWICTextureFromFile(const std::string& path, DirectXDevice* device, DirectXDeviceContext* context, ShaderResourceView** srv, Resource** resource) {
	auto wpath = Utility::ToWideStr(path);
	auto metaData = DXTextureMetaData::GetTextureMetaData(path);
	auto image = std::make_unique<DirectX::ScratchImage>();

	// WICファイルのロード
	{
		auto hr = DirectX::LoadFromWICFile(
			wpath.c_str(),
			DirectX::WIC_FLAGS_NONE,
			&metaData,
			*image
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceViewCreater::CreateMipMapWICTextureFromFile Method.\n",
				"DirectX::LoadFromWICFile Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceViewCreater::CreateMipMapWICTextureFromFile Method.\n",
				"DirectX::LoadFromWICFile Method.",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceViewCreater::CreateMipMapWICTextureFromFile Method.\n\
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
				"DXShaderResourceViewCreater::CreateMipMapWICTextureFromFile Method.\n",
				"DirectX::GenerateMpMaps Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceViewCreater::CreateMipMapWICTextureFromFile Method.\n",
				"DirectX::GenerateMpMaps Method.",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceViewCreater::CreateMipMapWICTextureFromFile Method.\n\
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
			metaData,
			srv
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceViewCreater::CreateMipMapWICTextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceViewCreater::CreateMipMapWICTextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method.",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceViewCreater::CreateMipMapWICTextureFromFile Method.\n\
				DirectX::CreateShaderResourceView Method.",
				hr
			);
#endif
		}
	}
}

void DXShaderResourceViewCreater::CreateMipMapDDSTextureFromFile(const std::string& path, DirectXDevice* device, DirectXDeviceContext* context, ShaderResourceView** srv, Resource** resource) {
	auto wpath = Utility::ToWideStr(path);
	auto metaData = DXTextureMetaData::GetTextureMetaData(path);
	auto image = std::make_unique<DirectX::ScratchImage>();

	// DDSファイルのロード
	{
		auto hr = DirectX::LoadFromDDSFile(
			wpath.c_str(),
			DirectX::DDS_FLAGS_NONE,
			&metaData,
			*image
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceViewCreater::CreateMipMapDDSTextureFromFile Method.\n",
				"DirectX::LoadFromDDSFile Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceViewCreater::CreateMipMapDDSTextureFromFile Method.\n",
				"DirectX::LoadFromDDSFile Method.",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceViewCreater::CreateMipMapDDSTextureFromFile Method.\n\
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
				"DXShaderResourceViewCreater::CreateMipMapDDSTextureFromFile Method.\n",
				"DirectX::GenerateMpMaps Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceViewCreater::CreateMipMapDDSTextureFromFile Method.\n",
				"DirectX::GenerateMpMaps Method.",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceViewCreater::CreateMipMapDDSTextureFromFile Method.\n\
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
			metaData,
			srv
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceViewCreater::CreateMipMapDDSTextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceViewCreater::CreateMipMapDDSTextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method.",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceViewCreater::CreateMipMapDDSTextureFromFile Method.\n\
				DirectX::CreateShaderResourceView Method.",
				hr
			);
#endif
		}
	}
}

void DXShaderResourceViewCreater::CreateMipMapTGATextureFromFile(const std::string& path, DirectXDevice* device, DirectXDeviceContext* context, ShaderResourceView** srv, Resource** resource) {
	auto wpath = Utility::ToWideStr(path);
	auto metaData = DXTextureMetaData::GetTextureMetaData(path);
	auto image = std::make_unique<DirectX::ScratchImage>();

	// TGAファイルのロード
	{
		auto hr = DirectX::LoadFromTGAFile(
			wpath.c_str(),
			&metaData,
			*image
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceViewCreater::CreateMipMapTGATextureFromFile Method.\n",
				"DirectX::LoadFromTGAFile Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceViewCreater::CreateMipMapTGATextureFromFile Method.\n",
				"DirectX::LoadFromTGAFile Method.",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceViewCreater::CreateMipMapTGATextureFromFile Method.\n\
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
				"DXShaderResourceViewCreater::CreateMipMapTGATextureFromFile Method.\n",
				"DirectX::GenerateMpMaps Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceViewCreater::CreateMipMapTGATextureFromFile Method.\n",
				"DirectX::GenerateMpMaps Method.",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceViewCreater::CreateMipMapTGATextureFromFile Method.\n\
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
			metaData,
			srv
		);

		IF_HRS(hr) {
			Debug::Log(
				"DXShaderResourceViewCreater::CreateMipMapTGATextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method Success."
			);
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXShaderResourceViewCreater::CreateMipMapTGATextureFromFile Method.\n",
				"DirectX::CreateShaderResourceView Method.",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXShaderResourceViewCreater::CreateMipMapTGATextureFromFile Method.\n\
				DirectX::CreateShaderResourceView Method.",
				hr
			);
#endif
		}
	}
}

void DXShaderResourceViewCreater::CreateShaderResourceView(const std::string& path, DirectXDevice* device, DirectXDeviceContext* context, ShaderResourceView** srv, Resource** resource) {
	Debug::BracketBegin("DXShaderResourceViewCreater::CreateShaderResourceView");

	Debug::Log("Path[", path, "]");
	Debug::Log("Name[", FileUtility::Utility::FileName(path), "]");
	Debug::Log("Extension[", FileUtility::Utility::FileExtension(path), "]");

	auto ext = FileUtility::Utility::FileExtension(path);

	if (ext == "dds") {
		DXShaderResourceViewCreater::CreateDDSTextureFromFile(
			path,
			device,
			context,
			srv,
			resource
		);
	}
	else if (ext == "tga") {
		DXShaderResourceViewCreater::CreateTGATextureFromFile(
			path,
			device,
			context,
			srv,
			resource
		);
	}
	else {
		DXShaderResourceViewCreater::CreateWICTextureFromFile(
			path,
			device,
			context,
			srv,
			resource
		);
	}

	Debug::BracketEnd();
}

void DXShaderResourceViewCreater::CreateMipMapShaderResourceView(const std::string& path, DirectXDevice* device, DirectXDeviceContext* context, ShaderResourceView** srv, Resource** resource) {
	Debug::BracketBegin("DXShaderResourceViewCreater::CreateMipMapShaderResourceView");

	auto ext = FileUtility::Utility::FileExtension(path);

	if (ext == "dds") {
		DXShaderResourceViewCreater::CreateMipMapDDSTextureFromFile(
			path,
			device,
			context,
			srv,
			resource
		);
	}
	else if (ext == "tga") {
		DXShaderResourceViewCreater::CreateMipMapTGATextureFromFile(
			path,
			device,
			context,
			srv,
			resource
		);
	}
	else {
		DXShaderResourceViewCreater::CreateMipMapWICTextureFromFile(
			path,
			device,
			context,
			srv,
			resource
		);
	}

	Debug::BracketEnd();
}
