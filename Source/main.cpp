
// Windows
#include<Windows.h>

// MyFile
#include"Application.h"

int APIENTRY WinMain(
	_In_		HINSTANCE 	hInstance, 		// �A�v���P�[�V�����̃n���h��
	_In_opt_ HINSTANCE 	hPrevInstance,	// ����Windows�o�[�W�����ł͎g���Ȃ�
	_In_		LPSTR 		lpszArgs, 		// �N�����̈����i������j
	_In_		int 			nWinMode			// �E�C���h�E�\�����[�h
) {
	Application::SetWinMainParam(hInstance, hPrevInstance, lpszArgs, nWinMode);
	Application::Initialize();
	Application::MainProcess();
	Application::Finalize();

	//return Application::UnInit();
	return 0;

}