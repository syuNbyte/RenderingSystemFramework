#pragma once

// MyFile
#include"Blend.h"

// 一般的によく使われるブレンド方法を保持するクラス
class DXBlendContainer {
private:
	DXBlend m_none;				// ブレンドステート無効化
	DXBlend m_default;			// デフォルト
	DXBlend m_srcAlpha;			// ソースαブレンド
	DXBlend m_alpha;				// αブレンド
	DXBlend m_alphaToCoverage;	// アルファトゥカバレッジ
	DXBlend m_add;					// 加算合成ブレンド
	DXBlend m_addAlpha;			// 加算合成ブレンド（透過あり）
	DXBlend m_sub;					// 減算合成ブレンド
	DXBlend m_mul;					// 乗算合成ブレンド
	DXBlend m_align;				// 線形合成ブレンド
	DXBlend m_screen;				// スクリーン合成ブレンド
	DXBlend m_reverse;			// リバース合成ブレンド
private:
	static DXBlendContainer& GetInstance();
	static void CreateNone(DirectXDevice* device);
	static void CreateDefault(DirectXDevice* device);
	static void CreateSrcAlpha(DirectXDevice* device);
	static void CreateAlpha(DirectXDevice* device);
	static void CreateAlphaToCoverage(DirectXDevice* device);
	static void CreateAdd(DirectXDevice* device);
	static void CreateAddAlpha(DirectXDevice* device);
	static void CreateSub(DirectXDevice* device);
	static void CreateMul(DirectXDevice* device);
	static void CreateAlign(DirectXDevice* device);
	static void CreateScreen(DirectXDevice* device);
	static void CreateReverse(DirectXDevice* device);
public:
	static void Initialize(DirectXDevice* device);
	static DXBlend GetNoneBlend();
	static DXBlend GetDefaultBlend();
	static DXBlend GetSrcAlphaBlend();
	static DXBlend GetAlphaBlend();
	static DXBlend GetAlphaToCoverageBlend();
	static DXBlend GetAddBlend();
	static DXBlend GetAddAlphaBlend();
	static DXBlend GetSubBlend();
	static DXBlend GetMulBlend();
	static DXBlend GetAlignBlend();
	static DXBlend GetScreenBlend();
	static DXBlend GetReverseBlend();
};