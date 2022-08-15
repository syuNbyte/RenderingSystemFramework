#pragma once

#include<windows.h>

//ShowWindow�֐��̃p�����[�^�Ƃ��ēn���l
enum WindowModeType {

	Hide = SW_HIDE,								//0	�E�B���h�E���\���ɂ���
	Normal = SW_SHOWNORMAL,						//1	�E�B���h�E���A�N�e�B�u�ɂ��ĕ\������@���E�B���h�E���ŏ����E�ő剻����Ă���ꍇ�͌��̃E�B���h�E�̏�Ԃɖ߂�
	Minimized = SW_SHOWMINIMIZED,				//2	�E�B���h�E���ŏ���
	Maximized = SW_SHOWMAXIMIZED,				//3	�E�B���h�E���ő剻
	No_activate = SW_SHOWNOACTIVATE,			//4	�E�B���h�E���A�N�e�B�u�ɂ͂����ɕ\��
	Show = SW_SHOW,								//5	�E�B���h�E���A�N�e�B�u�ɂ��Č��݂̈ʒu�ƃT�C�Y�ŕ\��
	MinAndNextZOrderWindow = SW_MINIMIZE,	//6	�E�B���h�E���ŏ������āA���̃E�B���h�E�̎��ɍőO�ʂɈʒu����E�B���h�E(����Z�I�[�_�[�ɂ���g�b�v���x���E�B���h�E)���A�N�e�B�u�ɂ���
	MinNoActive = SW_SHOWMINNOACTIVE,		//7	�E�B���h�E�͔�A�N�e�B�u�ɂ��čŏ������ꂽ��Ԃŕ\��
	NA = SW_SHOWNA,								//8	�E�B���h�E���A�N�e�B�u�ɂ��Č��݂̈ʒu�ƃT�C�Y�ŕ\��
	Restore = SW_RESTORE,						//9	�E�B���h�E���A�N�e�B�u�ɂ��ĕ\������@���E�B���h�E���ŏ����E�ő剻����Ă���ꍇ�͌��̃E�B���h�E�̏�Ԃɖ߂�
	Default = SW_SHOWDEFAULT,					//10	�A�v���P�[�V�����N������CreateProcess�֐��ɐݒ肳�ꂽ�l���g�p����
	ForceMinimize = SW_FORCEMINIMIZE			//11	���̃X���b�h����E�B���h�E���ŏ�������ꍇ�Ɏg�p����

};