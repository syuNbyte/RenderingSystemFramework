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
	void SetViewPortTopLeftPos(const DXVector2& pos);						// 現在の画面サイズのうちのどの範囲に描画するかを設定する
	void SetViewPortBottomRightPos(const DXVector2& pos);					// 現在の画面サイズのうちのどの範囲に描画するかを設定する
	void SetViewPortDepthRange(float_t minDepth, float_t maxDepth);	// ビューポートで使用される深度値の範囲
public:
	DXVector2 GetViewPortSize();
	DXVector2 GetViewPortTopLeftPos();
	DXVector2 GetViewPortBottomRightPos();
	float_t GetViewPortMinDepth();
	float_t GetViewPortMaxDepth();
	void RSSetViewPort(DirectXDeviceContext* context);
};