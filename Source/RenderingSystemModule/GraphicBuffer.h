#pragma once

// STL
#include<vector>

// MyFile
#include"RenderTarget.h"

// DirectXで遅延レンダリングする際のテクスチャを保持する値
class DXGraphicBuffer {
private:
	DXRenderTarget m_albedo;					// 色テクスチャ（色）R : 赤色, G : 緑色, B : 青色, A : 透明度
	DXRenderTarget m_normal;					// 法線テクスチャ（法線）R : X方向のベクトル, G : Y方向のベクトル, B : Z方向のベクトル
	DXRenderTarget m_position;					//	座標テクスチャ（座標）R : X座標, G : Y座標, B : Z座標
	DXRenderTarget m_depth;						// 深度テクスチャ（深度値）R : 深度値
	DXRenderTarget m_material;					// 質感データテクスチャ（ラフネスとメタルネス）R : ラフネス, G : メタルネス
	DXRenderTarget m_brightness;				// 輝度データテクスチャ（輝度）R : 輝度
private:
	static DXGraphicBuffer& GetInstance();
	static void CreateAlbedoRenderTarget(DirectXDevice* device);
	static void CreateNormalRenderTarget(DirectXDevice* device);
	static void CreatePositionRenderTarget(DirectXDevice* device);
	static void CreateDepthRenderTarget(DirectXDevice* device);
	static void CreateMaterialRenderTarget(DirectXDevice* device);
	static void CreateBrightnessRenderTarget(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static void ClearRenderTargetView(DirectXDeviceContext* context);
	static ComObject::RenderTargetView GetAlbedoRTV();
	static ComObject::RenderTargetView GetNormalRTV();
	static ComObject::RenderTargetView GetPositionRTV();
	static ComObject::RenderTargetView GetDepthRTV();
	static ComObject::RenderTargetView GetMaterialRTV();
	static ComObject::RenderTargetView GetBrightnessRTV();
	static ComObject::ShaderResourceView GetAlbedoSRV();
	static ComObject::ShaderResourceView GetNormalSRV();
	static ComObject::ShaderResourceView GetPositionSRV();
	static ComObject::ShaderResourceView GetDepthSRV();
	static ComObject::ShaderResourceView GetMaterialSRV();
	static ComObject::ShaderResourceView GetBrightnessSRV();
	static std::vector<RenderTargetView*> GetRTVs();
};

// GraphicBufferを用いた計算結果のテクスチャを保持する一時バッファ
class DXGraphicBufferResult {
private:
	std::array<DXRenderTarget, 2> m_result;
	int_t m_write;
	int_t m_read;
private:
	static DXGraphicBufferResult& GetInstance();
	static void CreateResultRenderTarget(DirectXDevice* device);
public:
	static void Initailize(DirectXDevice* device);
	static void ClearRenderTargetView(DirectXDeviceContext* context);
	static ComObject::RenderTargetView GetResultRTV1();
	static ComObject::ShaderResourceView GetResultSRV1();
	static ComObject::RenderTargetView GetResultRTV2();
	static ComObject::ShaderResourceView GetResultSRV2();
public:
	static void SwitchRenderTarget();
	static DXRenderTarget& GetWriteRenderTarget();
	static DXRenderTarget& GetReadRenderTarget();
};