#pragma once

// STL
#include<vector>

// MyFile
#include"RenderTarget.h"

// DirectX�Œx�������_�����O����ۂ̃e�N�X�`����ێ�����l
class DXGraphicBuffer {
private:
	DXRenderTarget m_albedo;					// �F�e�N�X�`���i�F�jR : �ԐF, G : �ΐF, B : �F, A : �����x
	DXRenderTarget m_normal;					// �@���e�N�X�`���i�@���jR : X�����̃x�N�g��, G : Y�����̃x�N�g��, B : Z�����̃x�N�g��
	DXRenderTarget m_position;					//	���W�e�N�X�`���i���W�jR : X���W, G : Y���W, B : Z���W
	DXRenderTarget m_depth;						// �[�x�e�N�X�`���i�[�x�l�jR : �[�x�l
	DXRenderTarget m_material;					// �����f�[�^�e�N�X�`���i���t�l�X�ƃ��^���l�X�jR : ���t�l�X, G : ���^���l�X
	DXRenderTarget m_brightness;				// �P�x�f�[�^�e�N�X�`���i�P�x�jR : �P�x
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

// GraphicBuffer��p�����v�Z���ʂ̃e�N�X�`����ێ�����ꎞ�o�b�t�@
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