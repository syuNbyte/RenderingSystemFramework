
// Windows
#include<windows.h>

// MyFile
#include"ConsoleWindow.h"
#include"Application.h"




ConsoleWindow& ConsoleWindow::GetInstance() {
	static ConsoleWindow consoleWindow;
	return consoleWindow;
}

void ConsoleWindow::Initialize() {
	auto& instance = ConsoleWindow::GetInstance();
	
	// ウィンドウハンドルを取得する
	instance.m_windowHandle = Application::GetWindowHandle();

	// コンソールウィンドウ用のメモリを確保
	::AllocConsole();

	// コンソールへの各種入力に対応させる
	::freopen_s( &instance.m_fp, "CON", "w", stdin );	// 標準入力を受け取れるようにする
	::freopen_s( &instance.m_fp, "CON", "w", stdout );	// 標準出力を受け取れるようにする
	::freopen_s( &instance.m_fp, "CON", "w", stderr );	// 標準エラーを受け取れるようにする

	Debug::Log("Console Corresponding input", "stdin, stdout, stderr");
}

void ConsoleWindow::Update( int_t windowMode ) {
	auto& instance = ConsoleWindow::GetInstance();
	::ShowWindow( instance.m_windowHandle, windowMode );
	::UpdateWindow( instance.m_windowHandle );
}

void ConsoleWindow::Finalize() {
	auto& instance = ConsoleWindow::GetInstance();
	::fclose( instance.m_fp );
	::FreeConsole();
}
