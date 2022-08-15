#pragma once

// STL
#include<string>
#include<memory>

// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXVector.h"

// DirectXのテクスチャを扱うクラス
class DXTexture {
private:
	ComObject::ShaderResourceView m_srv;	// 画像データ
protected:
	int_t m_registNumber;						// テクスチャバッファの使用しているレジスタ番号
private:
	std::string m_path;							// テクスチャのディレクトリパス
	DXVector2Int m_size;							// テクスチャの画像サイズ
public:
	// 作成する
	void Create(DirectXDevice* device, DirectXDeviceContext* context, const std::string& path);

	// シェーダーファイル内で使用されているテクスチャバッファの使用番号を設定する
	void SetRegistNumber(const int_t& registNumber);

	// このテクスチャが有効なデータを保持しているかを判定する
	bool HasTexture();

	// このテクスチャのShaderResourceViewを取得する
	ComObject::ShaderResourceView GetSRV();

	// この画像のディレクトリパスを取得する
	std::string GetFilePath();

	// この画像のサイズを取得する
	DXVector2Int GetSize();
public:
	// 頂点シェーダーにこのテクスチャを設定する
	void VSSetShaderResources(DirectXDeviceContext* context);

	// ピクセルシェーダーに子のテクスチャを設定する
	void PSSetShaderResources(DirectXDeviceContext* context);
};