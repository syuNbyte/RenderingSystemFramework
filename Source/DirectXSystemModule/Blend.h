#pragma once

// STL
#include<array>

// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"

// DirectX�̃u�����h���@�������N���X
class DXBlend {
protected:
	ComObject::BlendState m_state;
protected:
	BlendDesc m_desc;
protected:
	std::array<float_t, 4> m_factor;	// �u�����f�B���O�W��
	uint_t m_mask;							// �T���v�����O�̃}�X�N�l
public:
	DXBlend();
	DXBlend(const DXBlend& blend);
	DXBlend(DXBlend&& blend);
public:
	// Blend���쐬����
	void Create(DirectXDevice* device, const BlendDesc& desc);

	// �u�����f�B���O�W����ݒ肷��
	void SetBlendFactor(float_t factor[4]);

	// �T���v�����O�̃}�X�N�n��ݒ肷��
	void SetBlendMask(const uint_t& mask);
public:
	// OMSetBlendState�̃��b�p�[
	void OMSetBlendState(DirectXDeviceContext* context);
};