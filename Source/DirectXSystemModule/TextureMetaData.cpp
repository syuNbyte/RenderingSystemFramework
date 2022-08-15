
// TNLIB
#include"Debug.hpp"
#include"Utility.hpp"
#include"FileUtility.hpp"

// MyFile
#include"TextureMetaData.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

TexMetadata DXTextureMetaData::GetWICMetaData(const std::string& path) {
	auto wpath = Utility::ToWideStr(path);
	TexMetadata metaData;

	auto hr = GetMetadataFromWICFile(
		wpath.c_str(),
		0,
		metaData
	);

	IF_HRS(hr) {
		Debug::Log("DXTextureMetaData::GetWICMetaData Method Success.");
		return metaData;
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXTextureMetaData::GetWICMetaData Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXTextureMetaData::GetWICMetaData Method.\n",
			hr
		);
#endif
		return metaData;
	}
}

TexMetadata DXTextureMetaData::GetDDSMetaData(const std::string& path) {
	auto wpath = Utility::ToWideStr(path);
	TexMetadata metaData;

	auto hr = GetMetadataFromDDSFile(
		wpath.c_str(),
		0,
		metaData
	);

	IF_HRS(hr) {
		Debug::Log("DXTextureMetaData::GetDDSMetaData Method Success.");
		return metaData;
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXTextureMetaData::GetDDSMetaData Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXTextureMetaData::GetDDSMetaData Method.\n",
			hr
		);
#endif
		return metaData;
	}
}

TexMetadata DXTextureMetaData::GetTGAMetaData(const std::string& path) {
	auto wpath = Utility::ToWideStr(path);
	TexMetadata metaData;

	auto hr = GetMetadataFromTGAFile(
		wpath.c_str(),
		metaData
	);

	IF_HRS(hr) {
		Debug::Log("DXTextureMetaData::GetTGAMetaData Method Success.");
		return metaData;
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXTextureMetaData::GetTGAMetaData Method.\n",
			HresultUtility::GetHresultMessage(hr),
			"\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXTextureMetaData::GetTGAMetaData Method.\n",
			hr
		);
#endif
		return metaData;
	}
}

TexMetadata DXTextureMetaData::GetTextureMetaData(const std::string& path) {
	auto wpath = Utility::ToWideStr(path);
	auto ext = FileUtility::Utility::FileExtension(path);

	if (ext == "dds") {
		return DXTextureMetaData::GetDDSMetaData(path);
	}
	else if (ext == "tga") {
		return DXTextureMetaData::GetTGAMetaData(path);
	}
	else {
		return DXTextureMetaData::GetWICMetaData(path);
	}
}

DXVector2Int DXTextureMetaData::GetTextureSize(const std::string& path) {
	auto metaData = DXTextureMetaData::GetTextureMetaData(path);
	return DXVector2Int(metaData.width, metaData.height);
}

int_t DXTextureMetaData::GetTextureMipMapLevel(const std::string& path) {
	auto metaData = DXTextureMetaData::GetTextureMetaData(path);
	return metaData.mipLevels;
}
