#pragma once

// MyFile
#include"DirectXUtilityModule//DirectXDataTypes.h"

// DirectX�̃T���v���[�������N���X
class DXSampler {
protected:
	ComObject::SamplerState m_state;
protected:
	SamplerDesc m_desc;
protected:
	int_t m_registNumber;
public:
	DXSampler();
	DXSampler(const DXSampler& sampler);
	DXSampler(DXSampler&& sampler);
public:
	// �T���v���[���쐬����
	void Create(DirectXDevice* device, const SamplerDesc& desc);

	// �V�F�[�_�[���Ŏg�p����Ă���T���v���[�X�e�[�g�̔ԍ���ݒ肷��
	void SetRegistNumber(const int_t& registNumber);

	// �ێ�����T���v���[�X�e�[�g���擾����
	ComObject::SamplerState GetSamplerState();
public:
	// ���_�V�F�[�_�[���̃T���v���[�̏���ݒ肷��
	void VSSetSampler(DirectXDeviceContext* context);

	// �s�N�Z���V�F�[�_�[���̃T���v���[�̏���ݒ肷��
	void PSSetSampler(DirectXDeviceContext* context);
};