#pragma once

// LibraryInclude
#pragma comment(lib, "DirectXTex.lib")

// STL
#include<string>

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"

// DirectXÇÃShaderResourceViewÇÃçÏê¨ÇàµÇ§ÉNÉâÉX
class DXShaderResourceViewCreater {
private:
	static void CreateWICTextureFromFile(
		const std::string& path,
		DirectXDevice* device,
		DirectXDeviceContext* context,
		ShaderResourceView** srv,
		Resource** resource
	);

	static void CreateDDSTextureFromFile(
		const std::string& path,
		DirectXDevice* device,
		DirectXDeviceContext* context,
		ShaderResourceView** srv,
		Resource** resource
	);

	static void CreateTGATextureFromFile(
		const std::string& path,
		DirectXDevice* device,
		DirectXDeviceContext* context,
		ShaderResourceView** srv,
		Resource** resource
	);

	static void CreateMipMapWICTextureFromFile(
		const std::string& path,
		DirectXDevice* device,
		DirectXDeviceContext* context,
		ShaderResourceView** srv,
		Resource** resource
	);

	static void CreateMipMapDDSTextureFromFile(
		const std::string& path,
		DirectXDevice* device,
		DirectXDeviceContext* context,
		ShaderResourceView** srv,
		Resource** resource
	);

	static void CreateMipMapTGATextureFromFile(
		const std::string& path,
		DirectXDevice* device,
		DirectXDeviceContext* context,
		ShaderResourceView** srv,
		Resource** resource
	);

public:
	static void CreateShaderResourceView(
		const std::string& path,
		DirectXDevice* device,
		DirectXDeviceContext* context,
		ShaderResourceView** srv,
		Resource** resource
	);

	static void CreateMipMapShaderResourceView(
		const std::string& path,
		DirectXDevice* device,
		DirectXDeviceContext* context,
		ShaderResourceView** srv,
		Resource** resource
	);
};