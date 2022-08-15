#pragma once

// MyFile
#include"DirectionalLight.h"
#include"DirectXUtilityModule//DirectXDataTypes.h"
#include"RenderingSystemModule/RenderTarget.h"
#include"DirectXSystemModule/DepthStencil.h"


class MainDirectionalLight {
private:
	DXRenderTarget m_rt;			// ライト視点の深度マップ用レンダーターゲット
	DXDepthStencil m_ds;			// ライト視点の深度マップ用深度ステンシル
	DirectionalLight m_light;	// 方向性ライト用
private:
	static MainDirectionalLight& GetInstance();
	static void CreateDirectionalLight();
	static void CreateDepthRenderTarget(DirectXDevice* device);
	static void CreateDepthStencil(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static void SetLightDirectionConstantBuffer(DirectXDeviceContext* context);
	static void SetLightColorConstantBuffer(DirectXDeviceContext* context);
	static void SetAmbientColorConstantBuffer(DirectXDeviceContext* context);
	static void SetViewMatrixConstantBuffer(DirectXDeviceContext* context);
	static void SetProjectionMatrixConstantBuffer(DirectXDeviceContext* context);
	static void UpdateConstantBuffer(DirectXDeviceContext* context);
	static void VSSetConstantBuffer(DirectXDeviceContext* context);
	static void PSSetConstantBuffer(DirectXDeviceContext* context);
public:
	static DirectionalLight& GetLight();
	static DXRenderTarget& GetRenderTarget();
	static DXDepthStencil& GetDepthStencil();
};