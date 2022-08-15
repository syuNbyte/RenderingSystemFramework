#pragma once

// Windows
#include<windows.h>

// TNLIB
#include"Math.hpp"

// MyFile
#include"ProcessMessageType.h"
#include"WindowModeType.h"

class Application {

private:

	// WinMain�֐��̈���
	HINSTANCE m_hInstance;
	HINSTANCE m_hPrevInstance;
	LPSTR m_lpszArgs;
	int_t m_nWinMode;

	// �E�B���h�E�̍쐬���Ɏg�p����p�����[�^
	std::string m_className;
	std::string m_windowName;
	Math::Vector::Vector2Int m_windowSize;
	WNDCLASSEX m_windowClass;
	HWND m_windowHandle;
	MSG m_processMessage;

private:
	
	Application();

	static Application& GetInstance();

	// �E�B���h�E�̍쐬
	void CreateApplicationWindow();
	void _CreateWindowClass();
	void _CreateWindowHandle();
	void _SetWindowPosition();

	// ���b�Z�[�W�֘A�̏���
	static ProcessMessageType ProcessMessage();

	// �E�B���h�E�v���V�[�W���֐�
	static LRESULT WINAPI WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

public:

	static HINSTANCE GetHandleInstance();
	static HWND GetWindowHandle();
	static Math::Vector::Vector2Int GetWindowSize();

	static void SetWindowName(const std::string& windowName);
	static void SetWindowSize(const Math::Vector::Vector2Int& windowSize);
	static void SetWindowMode(WindowModeType windowMode);

public:

	static void SetWinMainParam(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszArgs, _In_ int_t nWinMode);
	static void Initialize();
	static void MainProcess();
	static void Finalize();

};