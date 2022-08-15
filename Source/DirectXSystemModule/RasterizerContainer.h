#pragma once

// MyFile
#include"Rasterizer.h"

class DXRasterizerContainer {
private:
	DXRasterizer m_noneSolid;	// �J�����O�Ȃ��̃\���b�h���f��
	DXRasterizer m_frontSolid;	// �\�ʃJ�����O����̃\���b�h���f��
	DXRasterizer m_backSolid;	// ���ʃJ�����O����̃\���b�h���f��
	DXRasterizer m_noneWire;	// �J�����O�Ȃ��̃��C���[�t���[�����f��
	DXRasterizer m_frontWire;	// �\�ʃJ�����O����̃��C���[�t���[�����f��
	DXRasterizer m_backWire;	// ���ʃJ�����O����̃��C���[�t���[�����f��
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