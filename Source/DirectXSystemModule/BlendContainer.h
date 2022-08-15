#pragma once

// MyFile
#include"Blend.h"

// ��ʓI�ɂ悭�g����u�����h���@��ێ�����N���X
class DXBlendContainer {
private:
	DXBlend m_none;				// �u�����h�X�e�[�g������
	DXBlend m_default;			// �f�t�H���g
	DXBlend m_srcAlpha;			// �\�[�X���u�����h
	DXBlend m_alpha;				// ���u�����h
	DXBlend m_alphaToCoverage;	// �A���t�@�g�D�J�o���b�W
	DXBlend m_add;					// ���Z�����u�����h
	DXBlend m_addAlpha;			// ���Z�����u�����h�i���߂���j
	DXBlend m_sub;					// ���Z�����u�����h
	DXBlend m_mul;					// ��Z�����u�����h
	DXBlend m_align;				// ���`�����u�����h
	DXBlend m_screen;				// �X�N���[�������u�����h
	DXBlend m_reverse;			// ���o�[�X�����u�����h
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