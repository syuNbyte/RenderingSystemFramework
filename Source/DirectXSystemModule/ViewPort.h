#pragma once

// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXVector.h"

class DXViewPort {
private:
	ViewPort m_viewPort;
public:
	DXViewPort();
	DXViewPort(const DXViewPort& viewPort);
	DXViewPort(DXViewPort&& viewPort);
	~DXViewPort();
public:
	void SetViewPortTopLeftPos(const DXVector2& pos);						// ���݂̉�ʃT�C�Y�̂����̂ǂ͈̔͂ɕ`�悷�邩��ݒ肷��
	void SetViewPortBottomRightPos(const DXVector2& pos);					// ���݂̉�ʃT�C�Y�̂����̂ǂ͈̔͂ɕ`�悷�邩��ݒ肷��
	void SetViewPortDepthRange(float_t minDepth, float_t maxDepth);	// �r���[�|�[�g�Ŏg�p�����[�x�l�͈̔�
public:
	DXVector2 GetViewPortSize();
	DXVector2 GetViewPortTopLeftPos();
	DXVector2 GetViewPortBottomRightPos();
	float_t GetViewPortMinDepth();
	float_t GetViewPortMaxDepth();
	void RSSetViewPort(DirectXDeviceContext* context);
};