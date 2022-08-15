#pragma once

// LibraryInclude
#pragma comment(lib, "DirectXTex.lib")

// DirectXTex
#include"DirectXTex.h"

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXVector.h"

// DirectXのテクスチャに関するメタデータを扱うクラス
class DXTextureMetaData {
private:
	static TexMetadata GetWICMetaData(const std::string& path);
	static TexMetadata GetDDSMetaData(const std::string& path);
	static TexMetadata GetTGAMetaData(const std::string& path);
public:
	static TexMetadata GetTextureMetaData(const std::string& path);
	static DXVector2Int GetTextureSize(const std::string& path);
	static int_t GetTextureMipMapLevel(const std::string& path);
};