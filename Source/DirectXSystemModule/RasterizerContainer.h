#pragma once

// MyFile
#include"Rasterizer.h"

class DXRasterizerContainer {
private:
	DXRasterizer m_noneSolid;	// カリングなしのソリッドモデル
	DXRasterizer m_frontSolid;	// 表面カリングありのソリッドモデル
	DXRasterizer m_backSolid;	// 裏面カリングありのソリッドモデル
	DXRasterizer m_noneWire;	// カリングなしのワイヤーフレームモデル
	DXRasterizer m_frontWire;	// 表面カリングありのワイヤーフレームモデル
	DXRasterizer m_backWire;	// 裏面カリングありのワイヤーフレームモデル
private:
	static DXRasterizerContainer& GetInstance();
	static void CreateNoneSolid(DirectXDevice* device);
	static void CreateFrontSolid(DirectXDevice* device);
	static void CreateBackSolid(DirectXDevice* device);
	static void CreateNoneWire(DirectXDevice* device);
	static void CreateFrontWire(DirectXDevice* device);
	static void CreateBackWire(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static void SetNoneSolid(DirectXDeviceContext* context);
	static void SetFrontSolid(DirectXDeviceContext* context);
	static void SetBackSolid(DirectXDeviceContext* context);
	static void SetNoneWire(DirectXDeviceContext* context);
	static void SetFrontWire(DirectXDeviceContext* context);
	static void SetBackWire(DirectXDeviceContext* context);
};