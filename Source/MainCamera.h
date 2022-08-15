#pragma once

// MyFile
#include"Camera.h"
#include"DirectXUtilityModule//DirectXDataTypes.h"

// ƒƒCƒ“ƒJƒƒ‰‚Æ‚µ‚Äˆµ‚¤ƒJƒƒ‰‚ğİ’è‚·‚é
class MainCamera {
private:
	Camera m_camera;
private:
	static MainCamera& GetInstance();
private:
	static void CreateCamera();
public:
	static void Initialize();
	static void SetCameraPositionConstantBuffer(DirectXDeviceContext* context);
	static void SetCameraViewDirectionConstantBuffer(DirectXDeviceContext* context);
	static void SetCameraViewPortSize(DirectXDeviceContext* context);
	static void SetViewMatrixConstantBuffer(DirectXDeviceContext* context);
	static void SetPerspectiveProjectionMatrixConstantBuffer(DirectXDeviceContext* context);
	static void SetOrthoProjectionMatrixConstantBuffer(DirectXDeviceContext* context);
	static void UpdateConstantBuffer(DirectXDeviceContext* context);
	static void VSSetConstantBuffer(DirectXDeviceContext* context);
	static void PSSetConstantBuffer(DirectXDeviceContext* context);
public:
	static Camera& GetCamera();
};