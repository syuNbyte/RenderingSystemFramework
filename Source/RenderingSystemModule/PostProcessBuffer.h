#pragma once

// MyFile
#include"RenderTarget.h"

// ポストプロセス用のバッファ
class DXPostProcessBuffer {
private:
	DXRenderTarget m_rim;		// リムライト用
	DXRenderTarget m_bloom;		// ブルームエフェクト用
	DXRenderTarget m_vignette;	// ビネットエフェクト用
	DXRenderTarget m_ssao;		// SSAO（スクリーンスペースアンビエントオクルージョン）用
	DXRenderTarget m_ssr;		// SSR（スクリーンスペースリフレクション用
private:
	static DXPostProcessBuffer& GetInstance();
	static void CreateRimRenderTarget(DirectXDevice* device);
	static void CreateBloomRenderTarget(DirectXDevice* device);
	static void CreateVignetteRenderTarget(DirectXDevice* device);
	static void CreateSSAORenderTarget(DirectXDevice* device);
	static void CreateSSRRenderTarget(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static ComObject::RenderTargetView GetBloomRTV();
	static ComObject::RenderTargetView GetVignetteRTV();
	static ComObject::ShaderResourceView GetBloomSRV();
	static ComObject::ShaderResourceView GetVignetteSRV();
};