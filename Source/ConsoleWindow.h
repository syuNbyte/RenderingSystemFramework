#pragma once

// STL
#include<fstream>

// TNLIB
#include"SafetyDataType.h"

// コンソールウィンドウを使用するためのクラス
class ConsoleWindow {

private:

	FILE* m_fp;
	HWND m_windowHandle;

private:

	static ConsoleWindow& GetInstance();

public:

	// コンソールウィンドウの初期化処理
	static void Initialize();

	// コンソールウィンドウの更新処理　※WinMain関数の第4引数のウィンドウモードを受け取る
	static void Update( int_t windoeMode );

	// コンソールウィンドウの終了処理
	static void Finalize();

};
