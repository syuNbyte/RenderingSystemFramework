#pragma once

// TNLIB
#include"Debug.hpp"
#include"Counter.hpp"
#include"Utility.hpp"
#include"Color.hpp"

// MyFile
#include"Application.h"
#include"ConsoleWindow.h"
#include"DirectInput.h"
#include"DirectXSystemModule/DirectXManager.h"
#include"RenderingSystemModule/BackBuffer.h"
#include"DirectXSystemModule/DepthStencil.h"
#include"DirectXUtilityModule/DirectXColor.h"

// Example
#include"Example/RenderingExample.h"
#include"Example/DeferredRenderingExample.h"


Application::Application() {
	// WinMain�֐��̈���
	m_hInstance = GetModuleHandle(0);
	m_hPrevInstance = nullptr;
	m_lpszArgs = nullptr;
	m_nWinMode = WindowModeType::Default;
	
	// �E�B���h�E�쐬���Ɏg�p����p�����[�^
	m_className = "ClassName";
	m_windowName = "WindowName";
	m_windowSize.Set(16 * 50, 9 * 50);
	m_windowClass = WNDCLASSEX();
	m_windowHandle = HWND();
	m_processMessage = MSG();
}

Application& Application::GetInstance() {
	static Application application;
	return application;
}

void Application::CreateApplicationWindow() {

	// ���������[�N�����m
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	// �E�B���h�E�̍쐬���s��
	_CreateWindowClass();
	_CreateWindowHandle();
	_SetWindowPosition();
	
	// �E�C���h�E��\������
	ShowWindow(m_windowHandle, m_nWinMode);
	UpdateWindow(m_windowHandle);

	// ���O�ŏڍׂ�\������
	Debug::Log("ClassName[", m_className, "]");
	Debug::Log("WindowName[", m_windowName, "]");
	Debug::Log("WindowSize[", m_windowSize, "]");
}

void Application::_CreateWindowClass() {
	// �E�C���h�E�N���X���̃Z�b�g
	m_windowClass = {
		sizeof(WNDCLASSEX),									// �\���̂̃T�C�Y
		0,															// �E�C���h�E�X�^�C��
		Application::WindowProcedure,						// �E�C���h�E���b�Z�[�W�֐�
		0,															// �G�L�X�g���Ȃ�
		0,															// �G�L�X�g���Ȃ�
		m_hInstance,								// �C���X�^���X�l�̃Z�b�g
		LoadIcon((HINSTANCE)NULL, IDI_APPLICATION),	// ���[�W�A�C�R��
		LoadCursor((HINSTANCE)NULL, IDC_ARROW),		// �J�[�\���X�^�C��
		(HBRUSH)(COLOR_WINDOW + 1),						// �E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
		0,															// ���j���[�Ȃ�
		m_className.c_str(),									// �N���X��
		LoadIcon((HINSTANCE)NULL, IDI_WINLOGO)			// �X���[���A�C�R��
	};

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&m_windowClass);
}

void Application::_CreateWindowHandle() {
	m_windowHandle = CreateWindow(
		m_className.c_str(),								// �E�B���h�E�N���X�̖��O
		m_windowName.c_str(),							// �^�C�g��
		WS_CAPTION | WS_SYSMENU,						// �E�B���h�E�X�^�C��
		0, 0,													// �E�B���h�E�ʒu �c, ��(���ƂŒ����Ɉړ������܂�)
		m_windowSize.x, m_windowSize.y,				// �E�B���h�E�T�C�Y
		HWND_DESKTOP,										// �e�E�B���h�E�Ȃ�
		(HMENU)NULL,										// ���j���[�Ȃ�
		m_hInstance,										// �C���X�^���X�n���h��
		(LPVOID)NULL										// �ǉ������Ȃ�
	);								
}

void Application::_SetWindowPosition() {

	// �e���|����
	Math::Vector::Vector2Int windowSize;
	RECT windowRect;
	RECT cliendRect;

	// �E�C���h�E�̃N���C�A���g�T�C�Y���Čv�Z�iMetrics�����ł́A�t���[���f�U�C���ŃN���C�A���g�̈�T�C�Y���ς���Ă��܂��̂Łj
	GetWindowRect(m_windowHandle, &windowRect);
	GetClientRect(m_windowHandle, &cliendRect);
	windowSize.x = (windowRect.right - windowRect.left) - (cliendRect.right - cliendRect.left) + m_windowSize.x;
	windowSize.y = (windowRect.bottom - windowRect.top) - (cliendRect.bottom - cliendRect.top) + m_windowSize.y;

	// �E�C���h�E�̃N���C�A���g�T�C�Y���Đݒ�
	SetWindowPos(
		m_windowHandle,
		NULL,
		GetSystemMetrics(SM_CXSCREEN) / 2 - windowSize.x / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - windowSize.y / 2,
		windowSize.x - 1,
		windowSize.y - 1,
		SWP_NOZORDER
	);
}

ProcessMessageType Application::ProcessMessage() {
	auto& instance = Application::GetInstance();

	if (PeekMessage(&instance.m_processMessage, nullptr, 0, 0, PM_REMOVE) != 0) {

		{// Windows�̏���
			if (instance.m_processMessage.message == WM_QUIT) {// PostQuitMessage()���Ă΂�āAWM_QUIT���b�Z�[�W�������烋�[�v�I��
				return ProcessMessageType::QuitMessage;
			}
			else {
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&instance.m_processMessage);
				DispatchMessage(&instance.m_processMessage);

				return ProcessMessageType::MessageOccurred;
			}
		}

	}
	else {
		return ProcessMessageType::MessageNone;
	}
}

LRESULT WINAPI Application::WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_KEYDOWN:
		{				
			// �L�[�{�[�h����	
			switch (wParam) {
				case VK_ESCAPE:
				{
					// �E�C���h�E���I��������
					DestroyWindow(hwnd);
				}
				break;
			}
		}
		break;
		case WM_DESTROY:
		{						// �E�C���h�E�I����
			PostQuitMessage(0);
		}
		break;
		default:
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return 0;
}

HINSTANCE Application::GetHandleInstance() {
	auto& instance = Application::GetInstance();
	return instance.m_hInstance;
}

HWND Application::GetWindowHandle() {
	auto& instance = Application::GetInstance();
	return instance.m_windowHandle;
}

Math::Vector::Vector2Int Application::GetWindowSize() {
	auto& instance = Application::GetInstance();
	return instance.m_windowSize;
}

void Application::SetWindowName(const std::string& windowName) {
	auto& instance = Application::GetInstance();
	instance.m_windowName = windowName;
	SetWindowText(instance.m_windowHandle, windowName.c_str());
}

void Application::SetWindowSize(const Math::Vector::Vector2Int& windowSize) {
	auto& instance = Application::GetInstance();
	instance.m_windowSize = windowSize;
	instance._SetWindowPosition();
}

void Application::SetWindowMode(WindowModeType windowMode) {
	auto& instance = Application::GetInstance();
	instance.m_nWinMode = windowMode;
	//SendMessage(instance.m_windowHandle, WM_SYSCOMMAND, windowMode, NULL);
	ShowWindow(instance.m_windowHandle, windowMode);
}

void Application::SetWinMainParam(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgs, int_t nWinMode) {
	auto& instance = Application::GetInstance();
	instance.m_hInstance = hInstance;
	instance.m_hPrevInstance = hPrevInstance;
	instance.m_lpszArgs = lpszArgs;
	instance.m_nWinMode = nWinMode;
}

void Application::Initialize() {
	Debug::SetLogSaveFlag(true);
	Debug::SetDispOnConsoleWindow(true);
	Debug::SetDispOnOutputWindow(true);

	Debug::BracketBegin("Application::Initialize");

	auto& instance = Application::GetInstance();
	ConsoleWindow::Initialize();
	instance.CreateApplicationWindow();
	DirectInput::Initalize();
	DirectXManager::Initialize();

	Debug::BracketEnd();
}

void Application::MainProcess() {
	auto& instance = Application::GetInstance();

	while (Application::ProcessMessage() != ProcessMessageType::QuitMessage) {
		// �R���\�[���E�B���h�E�̍X�V
		ConsoleWindow::Update(instance.m_nWinMode);

		// ���͌n���̃o�b�t�@���X�V
		DirectInput::Update();

		// �����_�����O�̎��O����
		auto c = 1.0f / 255.0f;
		DXBackBuffer::Clear(DirectXManager::GetImmediateContext().Get(), DXColor(c * 30, c * 70, c * 128, 1.0f));
		DXDepthStencilDefault::Clear(DirectXManager::GetImmediateContext().Get());

		// �`��e�X�g
		//RenderingTriangle();
		//RenderingTexture();
		//DeferredRendering();
		{
			static bool flag = false;

			if (DirectInput::GetKeyboardTrigger(DIK_F1)) {
				flag = (flag) ? false : true;
			}
			if (flag) {
				DefaultExample();
			}
			else {
				//DefaultExample();
				//LaplacianFilter();
				//GaussianFilter();
				//PhysicsBasedRendering();
				RenderingPassExample();
			}

		}

		// �����_�����O�̏I������
		DirectXManager::GetSwapChain()->Present(0, 0);

	}
}

void Application::Finalize() {
	Debug::BracketBegin("Application::Finalize");

	auto& instance = Application::GetInstance();
	ConsoleWindow::Finalize();
	DirectInput::Finalize();
	DirectXManager::Finalize();

	Debug::BracketEnd();
}

