#pragma once

// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXVector.h"
#include"DirectXUtilityModule/DirectXMatrix.h"
#include"DirectXUtilityModule/DirectXColor.h"
#include"DirectXSystemModule/ConstantBuffer.h"

// 定数バッファに渡すデータ
struct CBCameraParamData{
	DXVector3Align cameraPosition;
	DXVector3Align cameraViewDirection;
	DXVector2IntAlign viewPortSize;
	DXMatrix4x4Align viewMatrix;
	DXMatrix4x4Align projectionMatrix;
};

// カメラのパラメータ用の定数バッファ
class CBCameraParam {
	CBCameraParamData m_data;
	DXConstantBuffer m_cbuf;
private:
	static CBCameraParam& GetInstance();
	static void CreateConstantBuffer(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static void SetCameraPosition(const DXVector3& position);
	static void SetCameraViewDirection(const DXVector3& direction);
	static void SetViewPortSize(const DXVector2Int& size);
	static void SetViewMatrix(const DXMatrix4x4& matrix);
	static void SetProjectionMatrix(const DXMatrix4x4& matrix);
	static void UpdateConstantBuffer(DirectXDeviceContext* context);
	static void VSSetConstantBuffer(DirectXDeviceContext* context);
	static void PSSetConstantBuffer(DirectXDeviceContext* context);
};