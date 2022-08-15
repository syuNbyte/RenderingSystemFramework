
// Windows
#include<Windows.h>

// MyFile
#include"Application.h"

int APIENTRY WinMain(
	_In_		HINSTANCE 	hInstance, 		// アプリケーションのハンドル
	_In_opt_ HINSTANCE 	hPrevInstance,	// 今のWindowsバージョンでは使われない
	_In_		LPSTR 		lpszArgs, 		// 起動時の引数（文字列）
	_In_		int 			nWinMode			// ウインドウ表示モード
) {
	Application::SetWinMainParam(hInstance, hPrevInstance, lpszArgs, nWinMode);
	Application::Initialize();
	Application::MainProcess();
	Application::Finalize();

	//return Application::UnInit();
	return 0;

}