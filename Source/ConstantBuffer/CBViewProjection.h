#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXMatrix.h"
#include"DirectXSystemModule/ConstantBuffer.h"

// カメラ用のビュー行列とプロジェクション行列の定数バッファに送るデータをまとめた構造体
struct ViewProjection {
	DXMatrix4x4 view;
	DXMatrix4x4 projection;
};

// カメラ用のビュー行列とプロジェクション行列の定数バッファ
class CBViewProjection {
private:
	ViewProjection m_data;
	DXConstantBuffer m_cbuf;
private:
	static CBViewProjection& GetInstance();
	static void CreateConstantBuffer(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static void SetViewMatrix(const DXMatrix4x4& viewMatrix);
	static void SetProjectionMatrix(const DXMatrix4x4& projectionMatrix);
	static void UpdateConstantBuffer(DirectXDeviceContext* context);
	static void VSSetConstantBuffer(DirectXDeviceContext* context);
	static void PSSetConstantBuffer(DirectXDeviceContext* context);
};