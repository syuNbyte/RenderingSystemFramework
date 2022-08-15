#pragma once

// MyFile
#include"Camera.h"
#include"DirectXUtilityModule//DirectXDataTypes.h"

// �x�������_�����O�p�̒�_�J����
class DeferredRenderingCamera {
private:
	Camera m_camera;
private:
	static DeferredRenderingCamera& GetInstance();
public:
	static void Initialize();
	static void SetViewMatrixConstantBuffer(DirectXDeviceContext* context);
	static void SetPerspectiveProjectionMatrixConstantBuffer(DirectXDeviceContext* context);
	static void SetOrthoProjectionMatrixConstantBuffer(DirectXDeviceContext* context);
	static void VSSetConstantBuffer(DirectXDeviceContext* context);
	static void PSSetConstantBuffer(DirectXDeviceContext* context);
};