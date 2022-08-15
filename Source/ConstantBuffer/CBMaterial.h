#pragma once

// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXColor.h"
#include"DirectXSystemModule/ConstantBuffer.h"

// マテリアル定数バッファ用のデータ
struct CBMaterialData{
	DXColor albedo;		// 色データ
	DXColor material;		// r : ラフネス, g : メタルネス, b : スペキュラー, a : エミッシブ
	//float_t roughness;	// 表面の粗さ
	//float_t metalness;	// 表面の金属感
};

// 物理ベースレンダリングのマテリアルパラメータを設定するための定数バッファ
class CBMaterial {
private:
	CBMaterialData m_data;
	DXConstantBuffer m_cbuf;
private:
	static CBMaterial& GetInstance();
	static void CreateConstantBuffer(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static void SetAlbedo(const DXColor& albedo);
	static void SetRoughness(const float_t& roughness);
	static void SetMetalness(const float_t& metalness);
	static void SetSpecular(const float_t& specular);
	static void SetEmissive(const float_t& emissive);
	static void UpdateConstantBuffer(DirectXDeviceContext* context);
	static void VSSetConstantBuffer(DirectXDeviceContext* context);
	static void PSSetConstantBuffer(DirectXDeviceContext* context);
};