#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXMatrix.h"
#include"DirectXSystemModule/ConstantBuffer.h"

// ワールド行列定数バッファ用のデータ
struct CBWorldData {
	DXMatrix4x4 world;
};

// ワールド行列を定数バッファに送る
class CBWorld {
private:
	CBWorldData m_data;
	DXConstantBuffer m_cbuf;
private:
	static CBWorld& GetInstance();
	static void CreateConstantBuffer(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static void SetWorldMatrix(const DXMatrix4x4& worldMatrix);
	static void UpdateConstantBuffer(DirectXDeviceContext* context);
	static void VSSetConstantBuffer(DirectXDeviceContext* context);
	static void PSSetConstantBuffer(DirectXDeviceContext* context);
};