#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXMatrix.h"
#include"DirectXSystemModule/ConstantBuffer.h"

// ���[���h�s��萔�o�b�t�@�p�̃f�[�^
struct CBWorldData {
	DXMatrix4x4 world;
};

// ���[���h�s���萔�o�b�t�@�ɑ���
class CBWorld {
private:
	CBWorldData m_data;
	DXConstantBuffer m_cbuf;
private:
	static CBWorld& GetInstance();
	static void CreateConstantBuffer(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static void SetWorldMatrix(const DXMatrix4x4& worldMatrix);
	static void UpdateConstantBuffer(DirectXDeviceContext* context);
	static void VSSetConstantBuffer(DirectXDeviceContext* context);
	static void PSSetConstantBuffer(DirectXDeviceContext* context);
};