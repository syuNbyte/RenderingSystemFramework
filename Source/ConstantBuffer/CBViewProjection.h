#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXMatrix.h"
#include"DirectXSystemModule/ConstantBuffer.h"

// �J�����p�̃r���[�s��ƃv���W�F�N�V�����s��̒萔�o�b�t�@�ɑ���f�[�^���܂Ƃ߂��\����
struct ViewProjection {
	DXMatrix4x4 view;
	DXMatrix4x4 projection;
};

// �J�����p�̃r���[�s��ƃv���W�F�N�V�����s��̒萔�o�b�t�@
class CBViewProjection {
private:
	ViewProjection m_data;
	DXConstantBuffer m_cbuf;
private:
	static CBViewProjection& GetInstance();
	static void CreateConstantBuffer(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static void SetViewMatrix(const DXMatrix4x4& viewMatrix);
	static void SetProjectionMatrix(const DXMatrix4x4& projectionMatrix);
	static void UpdateConstantBuffer(DirectXDeviceContext* context);
	static void VSSetConstantBuffer(DirectXDeviceContext* context);
	static void PSSetConstantBuffer(DirectXDeviceContext* context);
};