#pragma once

// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXVector.h"
#include"DirectXUtilityModule/DirectXMatrix.h"
#include"DirectXUtilityModule/DirectXColor.h"
#include"DirectXSystemModule/ConstantBuffer.h"

// ディレクショナルライト用のデータ
struct CBDirectionalLightData {
	DXMatrix4x4Align viewMatrix;
	DXMatrix4x4Align projectionMatrix;
	DXVector3Align direction;
	DXColorAlign lightColor;
	DXColorAlign ambientLightColor;
};

// ディレクショナルライト用の定数バッファ
class CBDirectionalLight {
private:
	CBDirectionalLightData m_data;
	DXConstantBuffer m_cbuf;
private:
	static CBDirectionalLight& GetInstance();
	static void CreateConstantBuffer(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static void SetViewMatrix(const DXMatrix4x4& matrix);
	static void SetProjectionMatrix(const DXMatrix4x4& matrix);
	static void SetDirection(const DXVector3& lightDirection);
	static void SetDirectionalLightColor(const DXColor& color);
	static void SetAmbientLightColor(const DXColor& color);
	static void UpdateConstantBuffer(DirectXDeviceContext* context);
	static void VSSetConstantBuffer(DirectXDeviceContext* context);
	static void PSSetConstantBuffer(DirectXDeviceContext* context);
};