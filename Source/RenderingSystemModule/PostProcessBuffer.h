#pragma once

// MyFile
#include"RenderTarget.h"

// �|�X�g�v���Z�X�p�̃o�b�t�@
class DXPostProcessBuffer {
private:
	DXRenderTarget m_rim;		// �������C�g�p
	DXRenderTarget m_bloom;		// �u���[���G�t�F�N�g�p
	DXRenderTarget m_vignette;	// �r�l�b�g�G�t�F�N�g�p
	DXRenderTarget m_ssao;		// SSAO�i�X�N���[���X�y�[�X�A���r�G���g�I�N���[�W�����j�p
	DXRenderTarget m_ssr;		// SSR�i�X�N���[���X�y�[�X���t���N�V�����p
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