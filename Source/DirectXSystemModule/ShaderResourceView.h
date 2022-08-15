#pragma once

// LibraryInclude
#pragma comment(lib, "DirectXTex.lib")

// DirectXTex
#include"DirectXTex.h"

// STL
#include<string>

// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXVector.h"

// DirectXのShaderResourceViewを扱うクラス
class DXShaderResourceView {
protected:
	ComObject::ShaderResourceView m_srv;	// 画像データ
	DirectX::TexMetadata m_metaData;			// 画像のメタデータ
protected:
	int_t m_registNumber;						// テクスチャバッファの使用しているレジスタ番号
private:
	std::string m_path;							// テクスチャのディレクトリパス（マルチバイト）
	std::wstring m_wpath;						// テクスチャのディレクトリパス（ワイド）
	DXVector2Int m_size;							// テクスチャの画像サイズ
private:
	void CreateWICTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);
	void CreateDDSTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);
	void CreateTGATextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);
	void CreateMipMapWICTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);
	void CreateMipMapDDSTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);
	void CreateMipMapTGATextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);
public:
	// 通常のテクスチャを作成する
	void CreateTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);

	// みっぷマップテクスチャを作成する
	void CreateMipMapTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);

	// シェーダーファイル内で使用されているテクスチャバッファの使用番号を設定する
	void SetRegistNumber(const int_t& registNumber);

	// 使用するテクスチャファイルのディレクトリパスを設定する
	void SetFilePath(const std::string& path);

	// このテクスチャが有効なデータを保持しているかを判定する
	bool HasTexture();

	// このテクスチャのShaderResourceViewを取得する
	ComObject::ShaderResourceView GetSRV();

	// この画像のディレクトリパスを取得する
	std::string GetFilePath();
	std::wstring GetFilePathWide();

	// この画像のサイズを取得する
	DXVector2Int GetSize();
public:
	// 頂点シェーダーにこのテクスチャを設定する
	void VSSetShaderResources(DirectXDeviceContext* context);

	// ピクセルシェーダーに子のテクスチャを設定する
	void PSSetShaderResources(DirectXDeviceContext* context);
};