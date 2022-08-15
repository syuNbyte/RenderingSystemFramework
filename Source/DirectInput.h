#pragma once


#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

#include<array>

class DirectInput {

private:

	static inline const int key_max_num = 256;

	HWND hwnd;

	LPDIRECTINPUT8			direct_input_interface = nullptr;				// IDirectInput8インターフェースへのポインタ
	LPDIRECTINPUTDEVICE8	direct_input_device_keyboard = nullptr;		// IDirectInputDevice8インターフェースへのポインタ(キーボード)
	LPDIRECTINPUTDEVICE8	direct_input_device_mouse = nullptr;			// IDirectInputDevice8インターフェースへのポインタ(マウス)
	std::array<BYTE, DirectInput::key_max_num> key_state;					// キーボードの状態を受け取るワーク
	std::array<BYTE, DirectInput::key_max_num> key_state_trigger;		// キーボードの状態を受け取るワーク
	std::array<BYTE, DirectInput::key_max_num> key_state_repeat;		// キーボードの状態を受け取るワーク
	std::array<BYTE, DirectInput::key_max_num> key_state_release;		// キーボードの状態を受け取るワーク
	std::array<int, DirectInput::key_max_num>	key_state_repeat_cnt;	// キーボードのリピートカウンタ
	DIMOUSESTATE2	mouse_state;													// マウスの状態を受け取るワーク
	DIMOUSESTATE2	mouse_state_trigger;											// マウスの状態を受け取るワーク
	POINT				mouse_point;													// マウスの現在座標
	int				mouse_wheel;													// マウスホイールの回転量


private:

	static DirectInput* GetInstance();

private:

	static void InitKeyboard();
	static void UnInitKeyboard();

	static void InitMause();
	static void UnInitMouse();

public:

	//キーボードの状態を更新
	static void UpdateKeyboard();

	//各種キーボード取得関数
	static bool GetKeyboardPress( int key );		// 押している間
	static bool GetKeyboardTrigger( int key );	// 押された瞬間
	static bool GetKeyboardRepeat( int key );		// 一定時間以上押し続けられている場合
	static bool GetKeyboardRelease( int key );	// 離された瞬間

public:

	//マウスの状態を更新
	static void UpdateMouse();

	//マウスの各種ボタンの状態を取得
	static bool GetMouseLeftPress();
	static bool GetMouseLeftTrigger();
	static bool GetMouseRightPress();
	static bool GetMouseRightTrigger();
	static bool GetMouseCenterPress();
	static bool GetMouseCenterTrigger();

	//各軸の移動
	static long GetMouseAxisX();
	static long GetMouseAxisY();
	static long GetMouseAxisZ();

	//ウィンドウの左上座標からの座標
	static long GetMouseX();
	static long GetMouseY();

	// マウスホイールの回転量を取得する
	static int GetMouseWheelRotate();
	

public:

	//キーボード、マウスの状態を更新
	static void Initalize();
	static void Update();
	static void Finalize();



};