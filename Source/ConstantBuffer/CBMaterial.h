#pragma once

// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXColor.h"
#include"DirectXSystemModule/ConstantBuffer.h"

// �}�e���A���萔�o�b�t�@�p�̃f�[�^
struct CBMaterialData{
	DXColor albedo;		// �F�f�[�^
	DXColor material;		// r : ���t�l�X, g : ���^���l�X, b : �X�y�L�����[, a : �G�~�b�V�u
	//float_t roughness;	// �\�ʂ̑e��
	//float_t metalness;	// �\�ʂ̋�����
};

// �����x�[�X�����_�����O�̃}�e���A���p�����[�^��ݒ肷�邽�߂̒萔�o�b�t�@
class CBMaterial {
private:
	CBMaterialData m_data;
	DXConstantBuffer m_cbuf;
private:
	static CBMaterial& GetInstance();
	static void CreateConstantBuffer(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static void SetAlbedo(const DXColor& albedo);
	static void SetRoughness(const float_t& roughness);
	static void SetMetalness(const float_t& metalness);
	static void SetSpecular(const float_t& specular);
	static void SetEmissive(const float_t& emissive);
	static void UpdateConstantBuffer(DirectXDeviceContext* context);
	static void VSSetConstantBuffer(DirectXDeviceContext* context);
	static void PSSetConstantBuffer(DirectXDeviceContext* context);
};