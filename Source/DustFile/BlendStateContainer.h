#pragma once

// MyFile
#include"DirectXDataTypes.h"

// ��ʓI�ɂ悭�g����u�����h���@�̏�Ԃ�ێ�����N���X
class BlendStateContainer {
private:
	ComObject::BlendState m_none;					// �u�����h�X�e�[�g������
	ComObject::BlendState m_default;				// �f�t�H���g
	ComObject::BlendState m_srcAlpha;			// �\�[�X���u�����h
	ComObject::BlendState m_alpha;				// ���u�����h
	ComObject::BlendState m_alphaToCoverage;	// �A���t�@�g�D�J�o���b�W
	ComObject::BlendState m_add;					// ���Z�����u�����h
	ComObject::BlendState m_addAlpha;			// ���Z�����u�����h�i���ߗL��j
	ComObject::BlendState m_sub;					// ���Z�����u�����h
	ComObject::BlendState m_mul;					// ��Z�����u�����h
	ComObject::BlendState m_align;				// ���`�����u�����h
	ComObject::BlendState m_screen;				// �X�N���[�������u�����h
	ComObject::BlendState m_reverse;				// ���o�[�X�����u�����h
private:
	BlendDesc m_noneDesc;							// �u�����h�X�e�[�g������
	BlendDesc m_defaultDesc;						// �f�t�H���g
	BlendDesc m_srcAlphaDesc;						// �\�[�X���u�����h
	BlendDesc m_alphaDesc;							// ���u�����h
	BlendDesc m_alphaToCoverageDesc;				// �A���t�@�g�D�J�o���b�W
	BlendDesc m_addDesc;								// ���Z�����u�����h
	BlendDesc m_addAlphaDesc;						// ���Z�����u�����h�i���ߗL��j
	BlendDesc m_subDesc;								// ���Z�����u�����h
	BlendDesc m_mulDesc;								// ��Z�����u�����h
	BlendDesc m_alignDesc;							// ���`�����u�����h
	BlendDesc m_screenDesc;							// �X�N���[�������u�����h
	BlendDesc m_reverseDesc;						// ���o�[�X�����u�����h
private:
	// Command�p�^�[�����g�p�����ق����ÏW�x�͉����邪���͂��Ȃ�
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