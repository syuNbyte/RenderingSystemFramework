
// DirectXTK
#include"WICTextureLoader.h"

// TNLIB
#include"Debug.hpp"
#include"Utility.hpp"

// MyFile
#include"Texture.h"
#include"TextureMetaData.h"
#include"ShaderResourveViewCreater.h"
#include"DirectXUtilityModule/DirectXVector.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

void DXTexture::Create(DirectXDevice* device, DirectXDeviceContext* context, const std::string& path) {
	Debug::BracketBegin("DXTexture::Create");

	m_path = path;
	m_size = DXTextureMetaData::GetTextureSize(path);
	
	ComObject::Resource resource;
	DXShaderResourceViewCreater::CreateShaderResourceView(
		path,
		device,
		context,
		m_srv.GetAddressOf(),
		resource.GetAddressOf()
	);

	Debug::BracketEnd();
}

void DXTexture::SetRegistNumber(const int_t& registNumber) {
	m_registNumber = registNumber;
}

bool DXTexture::HasTexture() {
	return m_srv != nullptr;
}

ComObject::ShaderResourceView DXTexture::GetSRV() {
	return m_srv;
}

std::string DXTexture::GetFilePath() {
	return m_path;
}

DXVector2Int DXTexture::GetSize() {
	return m_size;
}

void DXTexture::VSSetShaderResources(DirectXDeviceContext* context) {
	context->VSSetShaderResources(
		m_registNumber,
		1,
		m_srv.GetAddressOf()
	);
}

void DXTexture::PSSetShaderResources(DirectXDeviceContext* context) {
	context->PSSetShaderResources(
		m_registNumber,
		1,
		m_srv.GetAddressOf()
	);
}
