#pragma once

// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"DirectXUtilityModule/DirectXVector.h"
#include"DirectXUtilityModule/DirectXMatrix.h"

// �J�����Ɋւ��鏈�����܂Ƃ߂�����
class Camera {
private:
	DXMatrix4x4 m_lookToView;
	DXMatrix4x4 m_lookAtView;
	DXMatrix4x4 m_perspectiveProjection;
	DXMatrix4x4 m_orthoProjection;

	// �v���W�F�N�V�����s����\������p�����[�^
	DXVector2 m_screenSize;
	float_t m_screenAspect;
	float_t m_fov;
	float_t m_nearClip;
	float_t m_farClip;

	// �r���[�s����\������p�����[�^
	DXVector3 m_position;
	DXVector3 m_lookAt;		// lookAtView�s��̃p�����[�^
	DXVector3 m_direction;	// lookToView�s��̃p�����[�^
	DXVector3 m_upVector;

	// �v���W�F�N�V�����s��ɉe����^����p�����[�^
	float_t m_zoom;
public:
	Camera();
	~Camera();
private:
	void CreateLookToViewMatrix();
	void CreateLookAtViewMatrix();
	void CreatePerspectiveProjectionMatrix();
	void CreateOrthoProjectionMatrix();
public:
	void Initialize();
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	void SetScreenSize(DXVector2 screenSize);
	void SetFov(float_t fov);
	void SetNearClip(float_t nearClip);
	void SetFarClip(float_t farClip);
	void SetZoom(float_t zoom);
	void SetPosition(DXVector3 position);
	void SetLookAtPosition(DXVector3 position);
	void SetViewDirection(DXVector3 direction);
	void SetUpVector(DXVector3 upVector);
public:
	void SetRelativeZoom(float_t zoom);
	void SetRelativePosition(DXVector3 position);
	void SetRelativeViewDirection(DXVector3 direction);
public:
	DXVector2 GetScreenSize();
	float_t GetNearClip();
	float_t GetFarClip();
	float_t GetZoom();
	DXVector3 GetPosition();
	DXVector3 GetViewDirection();
	DXVector3 GetUpVector();
public:
	DXMatrix4x4 GetLookAtViewMatrix();
	DXMatrix4x4 GetLookToViewMatrix();
	DXMatrix4x4 GetPerspectiveProjectionMatrix();
	DXMatrix4x4 GetOrthoProjectionMatrix();
};


