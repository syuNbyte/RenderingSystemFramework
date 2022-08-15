#pragma once

// MyFile
#include"DirectXDataTypes.h"

// 一般的によく使われるブレンド方法の状態を保持するクラス
class BlendStateContainer {
private:
	ComObject::BlendState m_none;					// ブレンドステート無効化
	ComObject::BlendState m_default;				// デフォルト
	ComObject::BlendState m_srcAlpha;			// ソースαブレンド
	ComObject::BlendState m_alpha;				// αブレンド
	ComObject::BlendState m_alphaToCoverage;	// アルファトゥカバレッジ
	ComObject::BlendState m_add;					// 加算合成ブレンド
	ComObject::BlendState m_addAlpha;			// 加算合成ブレンド（透過有り）
	ComObject::BlendState m_sub;					// 減算合成ブレンド
	ComObject::BlendState m_mul;					// 乗算合成ブレンド
	ComObject::BlendState m_align;				// 線形合成ブレンド
	ComObject::BlendState m_screen;				// スクリーン合成ブレンド
	ComObject::BlendState m_reverse;				// リバース合成ブレンド
private:
	BlendDesc m_noneDesc;							// ブレンドステート無効化
	BlendDesc m_defaultDesc;						// デフォルト
	BlendDesc m_srcAlphaDesc;						// ソースαブレンド
	BlendDesc m_alphaDesc;							// αブレンド
	BlendDesc m_alphaToCoverageDesc;				// アルファトゥカバレッジ
	BlendDesc m_addDesc;								// 加算合成ブレンド
	BlendDesc m_addAlphaDesc;						// 加算合成ブレンド（透過有り）
	BlendDesc m_subDesc;								// 減算合成ブレンド
	BlendDesc m_mulDesc;								// 乗算合成ブレンド
	BlendDesc m_alignDesc;							// 線形合成ブレンド
	BlendDesc m_screenDesc;							// スクリーン合成ブレンド
	BlendDesc m_reverseDesc;						// リバース合成ブレンド
private:
	// Commandパターンを使用したほうが凝集度は下がるが今はしない
	static BlendStateContainer& GetInstance();
	static void CreateNone();
	static void CreateDefault();
	static void CreateSrcAlpha();
	static void CreateAlpha();
	static void CreateAlphaToCoverage();
	static void CreateAdd();
	static void CreateAddAlpha();
	static void CreateSub();
	static void CreateMul();
	static void CreateAlign();
	static void CreateScreen();
	static void CreateReverse();
public:
	static void Initialize();
	static ComObject::BlendState GetNone();
	static ComObject::BlendState GetDefault();
	static ComObject::BlendState GetSrcAlpha();
	static ComObject::BlendState GetAlpha();
	static ComObject::BlendState GetAlphaToCoverage();
	static ComObject::BlendState GetAdd();
	static ComObject::BlendState GetAddAlpha();
	static ComObject::BlendState GetSub();
	static ComObject::BlendState GetMul();
	static ComObject::BlendState GetAlign();
	static ComObject::BlendState GetScreen();
	static ComObject::BlendState GetReverse();
};