#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXVector.h"
#include"DirectXUtilityModule/DirectXMatrix.h"
#include"DirectXUtilityModule/DirectXColor.h"


// DirectX�̕��������C�g�������N���X
class DirectionalLight {
private:
	DXMatrix4x4 m_viewMatrix;
	DXMatrix4x4 m_projectionMatrix;
	
	// �v���W�F�N�V�����s����\������p�����[�^
	DXVector2 m_screenSize;
	float_t m_screenAspect;
	float_t m_fov;
	float_t m_nearClip;
	float_t m_farClip;

	// �r���[�s����\������p�����[�^
	DXVector3 m_position;
	DXVector3 m_lookAt;
	DXVector3 m_upVector;

	DXVector3 m_direction;
	DXColor m_color;
public:
	DirectionalLight();
	~DirectionalLight();
private:
	void CreateViewMatrix();
	void CreateProjectionMatrix();
public:
	void Initialize();
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	void SetScreenSize(DXVector2 screenSize);
	void SetFov(float_t fov);
	void SetNearClip(float_t nearClip);
	void SetFarClip(float_t farClip);
	void SetViewDirection(const DXVector3& direction);
	void SetUpVector(const DXVector3& direction);
	void SetLightColor(const DXColor& color);
	DXVector2 GetScreenSize();
	DXVector3 GetViewDirection();
	DXColor GetLightColor();
public:
	DXMatrix4x4 GetViewMatrix();
	DXMatrix4x4 GetProjectionMatrix();
};