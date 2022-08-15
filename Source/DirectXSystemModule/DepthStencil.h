#pragma once

// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"

// DirectXのDepthStencilを扱うクラス
class DXDepthStencil {
private:
	ComObject::Texture2D m_tex;
	ComObject::DepthStencilState m_state;
	ComObject::DepthStencilView m_dsv;
	ComObject::ShaderResourceView m_srv;
private:
	DepthBufferDesc m_texDesc;
	DepthStencilDesc m_stateDesc;
	DepthStencilViewDesc m_dsvDesc;
	ShaderResourceViewDesc m_srvDesc;
private:
	void CreateTexture2D(DirectXDevice* device);
	void CreateDepthStencilState(DirectXDevice* device);
	void CreateDepthStencilView(DirectXDevice* device);
	void CreateShaderResourceView(DirectXDevice* device);
public:
	void Create(DirectXDevice* device);
	void Clear(DirectXDeviceContext* context);
	void SetTexture2DDesc(const Texture2DDesc& desc);
	void SetDepthStencilDesc(const DepthStencilDesc& desc);
	void SetDepthStencilViewDesc(const DepthStencilViewDesc& desc);
	void SetShaderResourceViewDesc(const ShaderResourceViewDesc& desc);
public:
	ComObject::DepthStencilState GetDepthStencilState();
	ComObject::DepthStencilView GetDSV();
	ComObject::ShaderResourceView GetSRV();
};

class DXDepthStencilDefault {
private:
	DXDepthStencil m_depthStencil;
private:
	static DXDepthStencilDefault& GetInstance();
	static void CreateDepthStencil(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static void Clear(DirectXDeviceContext* context);	// 深度ステンシルビューを0でクリア
	static ComObject::DepthStencilState GetDepthStencilState();
	static ComObject::DepthStencilView GetDSV();
public:
	static void DepthBufferON(DirectXDeviceContext* context);
	static void DepthBufferOFF(DirectXDeviceContext* context);
};