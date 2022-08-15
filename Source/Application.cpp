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
	// WinMain関数の引数
	m_hInstance = GetModuleHandle(0);
	m_hPrevInstance = nullptr;
	m_lpszArgs = nullptr;
	m_nWinMode = WindowModeType::Default;
	
	// ウィンドウ作成時に使用するパラメータ
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

	// メモリリークを検知
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	// ウィンドウの作成を行う
	_CreateWindowClass();
	_CreateWindowHandle();
	_SetWindowPosition();
	
	// ウインドウを表示する
	ShowWindow(m_windowHandle, m_nWinMode);
	UpdateWindow(m_windowHandle);

	// ログで詳細を表示する
	Debug::Log("ClassName[", m_className, "]");
	Debug::Log("WindowName[", m_windowName, "]");
	Debug::Log("WindowSize[", m_windowSize, "]");
}

void Application::_CreateWindowClass() {
	// ウインドウクラス情報のセット
	m_windowClass = {
		sizeof(WNDCLASSEX),									// 構造体のサイズ
		0,															// ウインドウスタイル
		Application::WindowProcedure,						// ウインドウメッセージ関数
		0,															// エキストラなし
		0,															// エキストラなし
		m_hInstance,								// インスタンス値のセット
		LoadIcon((HINSTANCE)NULL, IDI_APPLICATION),	// ラージアイコン
		LoadCursor((HINSTANCE)NULL, IDC_ARROW),		// カーソルスタイル
		(HBRUSH)(COLOR_WINDOW + 1),						// ウインドウのクライアント領域の背景色を設定
		0,															// メニューなし
		m_className.c_str(),									// クラス名
		LoadIcon((HINSTANCE)NULL, IDI_WINLOGO)			// スモールアイコン
	};

	// ウィンドウクラスの登録
	RegisterClassEx(&m_windowClass);
}

void Application::_CreateWindowHandle() {
	m_windowHandle = CreateWindow(
		m_className.c_str(),								// ウィンドウクラスの名前
		m_windowName.c_str(),							// タイトル
		WS_CAPTION | WS_SYSMENU,						// ウィンドウスタイル
		0, 0,													// ウィンドウ位置 縦, 横(あとで中央に移動させます)
		m_windowSize.x, m_windowSize.y,				// ウィンドウサイズ
		HWND_DESKTOP,										// 親ウィンドウなし
		(HMENU)NULL,										// メニューなし
		m_hInstance,										// インスタンスハンドル
		(LPVOID)NULL										// 追加引数なし
	);								
}

void Application::_SetWindowPosition() {

	// テンポラリ
	Math::Vector::Vector2Int windowSize;
	RECT windowRect;
	RECT cliendRect;

	// ウインドウのクライアントサイズを再計算（Metricsだけでは、フレームデザインでクライアント領域サイズが変わってしまうので）
	GetWindowRect(m_windowHandle, &windowRect);
	GetClientRect(m_windowHandle, &cliendRect);
	windowSize.x = (windowRect.right - windowRect.left) - (cliendRect.right - cliendRect.left) + m_windowSize.x;
	windowSize.y = (windowRect.bottom - windowRect.top) - (cliendRect.bottom - cliendRect.top) + m_windowSize.y;

	// ウインドウのクライアントサイズを再設定
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

		{// Windowsの処理
			if (instance.m_processMessage.message == WM_QUIT) {// PostQuitMessage()が呼ばれて、WM_QUITメッセージが来たらループ終了
				return ProcessMessageType::QuitMessage;
			}
			else {
				// メッセージの翻訳と送出
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
			// キーボード押下	
			switch (wParam) {
				case VK_ESCAPE:
				{
					// ウインドウを終了させる
					DestroyWindow(hwnd);
				}
				break;
			}
		}
		break;
		case WM_DESTROY:
		{						// ウインドウ終了時
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
		// コンソールウィンドウの更新
		ConsoleWindow::Update(instance.m_nWinMode);

		// 入力系統のバッファを更新
		DirectInput::Update();

		// レンダリングの事前処理
		auto c = 1.0f / 255.0f;
		DXBackBuffer::Clear(DirectXManager::GetImmediateContext().Get(), DXColor(c * 30, c * 70, c * 128, 1.0f));
		DXDepthStencilDefault::Clear(DirectXManager::GetImmediateContext().Get());

		// 描画テスト
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

		// レンダリングの終了処理
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

