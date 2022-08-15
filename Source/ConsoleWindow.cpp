
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
	
	// �E�B���h�E�n���h�����擾����
	instance.m_windowHandle = Application::GetWindowHandle();

	// �R���\�[���E�B���h�E�p�̃��������m��
	::AllocConsole();

	// �R���\�[���ւ̊e����͂ɑΉ�������
	::freopen_s( &instance.m_fp, "CON", "w", stdin );	// �W�����͂��󂯎���悤�ɂ���
	::freopen_s( &instance.m_fp, "CON", "w", stdout );	// �W���o�͂��󂯎���悤�ɂ���
	::freopen_s( &instance.m_fp, "CON", "w", stderr );	// �W���G���[���󂯎���悤�ɂ���

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
