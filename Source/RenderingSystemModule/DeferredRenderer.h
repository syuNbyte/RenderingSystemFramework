#pragma once

// MyFile
#include"DirectXUtilityModule//DirectXDataTypes.h"
#include"DirectXSystemModule/VertexShader.h"
#include"DirectXSystemModule/PixelShader.h"

// 遅延レンダリング用のレンダラー
class DeferredRenderer {
private:

private:
	bool m_depthEnable; // 深度を使用するかどうか
private:
	static DeferredRenderer& GetInstance();
public:
	static void Initialize(DirectXDevice* device, SwapChain* swapChain);
	static void Rendering(DirectXDeviceContext* context);
	static void BackBufferClear(DirectXDeviceContext* context);
	static void GraphicBufferClear(DirectXDeviceContext* context);
	static void SetDepthFlag(bool flag);
	static bool GetDepthFlag();
public:
	static void OMSetBackBuffer(DirectXDeviceContext* context);
	static void OMSetGraphicBuffer(DirectXDeviceContext* context);
public:
	static ComObject::ShaderResourceView GetAlbedoSRV();
	static ComObject::ShaderResourceView GetNormalSRV();
	static ComObject::ShaderResourceView GetPositionSRV();
	static ComObject::ShaderResourceView GetDepthSRV();
};