
// TNLIB
#include"Debug.hpp"
#include"ErrorType.h"

// MyFile
#include"DirectInput.h"
#include"Application.h"
#include"UtilityMacro.h"

DirectInput* DirectInput::GetInstance() {

	static DirectInput direct_input;

	return &direct_input;

}


#pragma region


void DirectInput::InitKeyboard() {

	auto direct_input = DirectInput::GetInstance();
	auto hwnd = Application::GetWindowHandle();
	direct_input->hwnd = hwnd;

	// デバイスオブジェクトを作成
	auto hr = direct_input->direct_input_interface->CreateDevice(
		GUID_SysKeyboard,
		&direct_input->direct_input_device_keyboard,
		NULL
	);

	if (FAILED(hr) || direct_input->direct_input_device_keyboard == nullptr) {

		//キーボードが存在しない
		MessageBox(hwnd, "Not exist Keyboard", "error", MB_ICONWARNING);

		Debug::LogWarning(
			typeid(DirectInput).name(),
			ErrorType::failed_to_exist_elemet,
			GetHRESULTMessage(hr)
		);

	}

	// データフォーマットを設定
	hr = direct_input->direct_input_device_keyboard->SetDataFormat(&c_dfDIKeyboard);


	IF_HR(hr) {

		//キーボードフォーマットを設定できなかった
		MessageBox(hwnd, "Setting Keyboard format error", "error", MB_ICONWARNING);

		Debug::LogWarning(
			typeid(DirectInput).name(),
			ErrorType::setting_failed,
			GetHRESULTMessage(hr)
		);

	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = direct_input->direct_input_device_keyboard->SetCooperativeLevel(hwnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));

	IF_HR(hr) {

		//キーボードに協調モードを設定できなかった
		MessageBox(hwnd, "Setting Keyboard Cooperation Mode error", "error", MB_ICONWARNING);

		Debug::LogWarning(
			typeid(DirectInput).name(),
			ErrorType::setting_failed,
			GetHRESULTMessage(hr)
		);

	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	direct_input->direct_input_device_keyboard->Acquire();

}

void DirectInput::UnInitKeyboard() {
	auto direct_input = DirectInput::GetInstance();

	if (direct_input->direct_input_device_keyboard) {
		direct_input->direct_input_device_keyboard->Unacquire();
		direct_input->direct_input_device_keyboard->Release();
		direct_input->direct_input_device_keyboard = NULL;
	}
}


void DirectInput::UpdateKeyboard() {

	HRESULT hr;
	auto direct_input = DirectInput::GetInstance();
	BYTE key_state_old[256];

	// 前回のデータを保存
	memcpy(key_state_old, direct_input->key_state.data(), DirectInput::key_max_num);

	// デバイスからデータを取得
	hr = direct_input->direct_input_device_keyboard->GetDeviceState(
		sizeof(direct_input->key_state),
		direct_input->key_state.data()
	);


	if (SUCCEEDED(hr)) {
		for (int cnt = 0; cnt < DirectInput::key_max_num; cnt++) {
			// トリガーとリリースを取得
			direct_input->key_state_trigger[cnt] = (direct_input->key_state[cnt] ^ key_state_old[cnt]) & direct_input->key_state[cnt];
			direct_input->key_state_release[cnt] = (direct_input->key_state[cnt] ^ key_state_old[cnt]) & key_state_old[cnt];

			// キーが押されているならリピートの判定処理
			if (direct_input->key_state[cnt]) {
				if (direct_input->key_state_repeat_cnt[cnt] < 20) {
					direct_input->key_state_repeat_cnt[cnt]++;
					// 「初回入力」もしくは「ボタンを押してから20フレーム経過」
					// した場合、リピート用配列のデータを有効にする
					//if(  ||  )
					// 上記条件以外はリピート用配列のデータを無効にする
					//else
					if ((direct_input->key_state_repeat_cnt[cnt] == 1) || direct_input->key_state_repeat_cnt[cnt] >= 20) {
						direct_input->key_state_repeat[cnt] = direct_input->key_state[cnt];
					}
					else {
						direct_input->key_state_repeat[cnt] = 0;
					}
				}
			}
			else {
				direct_input->key_state_repeat_cnt[cnt] = 0;
				direct_input->key_state_repeat[cnt] = 0;
			}
		}
	}
	else {
		//このelse内の処理結果はあまり関係ない
		//実際にはhr型にはWin32のHRESULT型が戻り値で返されているというエラーが出ているがキーボードアクセスには何ら支障がないため大丈夫
		//そのため、以下のエラーメッセージ部分の処理をコメントアウトしている

		// キーボードへのアクセス権を取得
		hr = direct_input->direct_input_device_keyboard->Acquire();

		// キーボードへのアクセス権の取得に失敗
		//IF_ErrorMessage(hr, "UnSuccess Get Keyboard AccessPass");
	}

}

bool DirectInput::GetKeyboardPress(int key) {
	return ((DirectInput::GetInstance()->key_state[key] & 0x80) != 0);
}

bool DirectInput::GetKeyboardTrigger(int key) {
	return ((DirectInput::GetInstance()->key_state_trigger[key] & 0x80) != 0);
}

bool DirectInput::GetKeyboardRepeat(int key) {
	return ((DirectInput::GetInstance()->key_state_repeat[key] & 0x80) != 0);
}

bool DirectInput::GetKeyboardRelease(int key) {
	return ((DirectInput::GetInstance()->key_state_release[key] & 0x80) != 0);
}

#pragma endregion Keyboard


#pragma region


void DirectInput::InitMause() {


	auto direct_input = DirectInput::GetInstance();
	auto hwnd = Application::GetWindowHandle();
	direct_input->hwnd = hwnd;

	// デバイスオブジェクトを作成
	auto hr = direct_input->direct_input_interface->CreateDevice(
		GUID_SysMouse,
		&direct_input->direct_input_device_mouse,
		NULL
	);

	if (FAILED(hr) || direct_input->direct_input_device_mouse == NULL) {
		//マウスがない
		MessageBox(hwnd, "Not exist Mouse", "error", MB_ICONWARNING);

		Debug::LogWarning(

			typeid(DirectInput).name(),
			ErrorType::failed_to_exist_elemet,
			GetHRESULTMessage(hr)

		);

	}

	// データフォーマットを設定
	hr = direct_input->direct_input_device_mouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr)) {
		//マウスのフォーマットを設定できなかった
		MessageBox(hwnd, "Not Setting Mouse format error", "error", MB_ICONWARNING);


		Debug::LogWarning(

			typeid(DirectInput).name(),
			ErrorType::setting_failed,
			GetHRESULTMessage(hr)

		);

	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = direct_input->direct_input_device_mouse->SetCooperativeLevel(hwnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr)) {
		//マウスの協調モードを設定できなかった
		MessageBox(hwnd, "Not Setting Mouse Cooperation Mode error", "error", MB_ICONWARNING);

		Debug::LogWarning(

			typeid(DirectInput).name(),
			ErrorType::setting_failed,
			GetHRESULTMessage(hr)

		);

	}

	// デバイスの設定
	{
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL;

		hr = direct_input->direct_input_device_mouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	direct_input->direct_input_device_mouse->Acquire();

}



void DirectInput::UnInitMouse() {
	auto direct_input = DirectInput::GetInstance();

	if (direct_input->direct_input_device_mouse) {
		direct_input->direct_input_device_mouse->Unacquire();
		direct_input->direct_input_device_mouse->Release();
		direct_input->direct_input_device_mouse = NULL;
	}
}



void DirectInput::UpdateMouse() {

	DIMOUSESTATE2 mouse_state_old;
	auto direct_input = DirectInput::GetInstance();

	GetCursorPos(&direct_input->mouse_point);
	ScreenToClient(direct_input->hwnd, &direct_input->mouse_point);

	// 前回のデータを保存
	mouse_state_old = direct_input->mouse_state;

	// デバイスからデータを取得
	auto hr = direct_input->direct_input_device_mouse->GetDeviceState(sizeof(direct_input->mouse_state), &direct_input->mouse_state);


	if (SUCCEEDED(hr)) {
		// トリガーの取得
		//direct_input->mouse_stateTrigger.lX = 
		//direct_input->mouse_stateTrigger.lY = 
		//direct_input->mouse_stateTrigger.lZ = 
		for (int cnt = 0; cnt < 8; cnt++) {
			direct_input->mouse_state_trigger.rgbButtons[cnt] = ((mouse_state_old.rgbButtons[cnt] ^ direct_input->mouse_state.rgbButtons[cnt]) & direct_input->mouse_state.rgbButtons[cnt]);
		}
	}
	else {
		direct_input->direct_input_device_mouse->Acquire();
	}

}


#pragma region


bool DirectInput::GetMouseLeftPress() {
	return ((DirectInput::GetInstance()->mouse_state.rgbButtons[0] & 0x80) != 0);
}

bool DirectInput::GetMouseLeftTrigger() {
	return ((DirectInput::GetInstance()->mouse_state_trigger.rgbButtons[0] & 0x80) != 0);
}

bool DirectInput::GetMouseRightPress() {
	return ((DirectInput::GetInstance()->mouse_state.rgbButtons[1] & 0x80) != 0);
}

bool DirectInput::GetMouseRightTrigger() {
	return ((DirectInput::GetInstance()->mouse_state_trigger.rgbButtons[1] & 0x80) != 0);
}

bool DirectInput::GetMouseCenterPress() {
	return ((DirectInput::GetInstance()->mouse_state.rgbButtons[2] & 0x80) != 0);
}

bool DirectInput::GetMouseCenterTrigger() {
	return ((DirectInput::GetInstance()->mouse_state_trigger.rgbButtons[2] & 0x80) != 0);
}


#pragma endregion MouseButton


#pragma region


long DirectInput::GetMouseAxisX() {
	return DirectInput::GetInstance()->mouse_state.lX;
}



long DirectInput::GetMouseAxisY() {
	return DirectInput::GetInstance()->mouse_state.lY;
}



long DirectInput::GetMouseAxisZ() {
	return DirectInput::GetInstance()->mouse_state.lZ;
}


#pragma endregion MouseMoveAxis


#pragma region


long DirectInput::GetMouseX() {
	return DirectInput::GetInstance()->mouse_point.x;
}

long DirectInput::GetMouseY() {
	return DirectInput::GetInstance()->mouse_point.y;
}


#pragma endregion MousePosition


#pragma endregion Mouse




void DirectInput::Initalize() {
	DirectInput8Create(
		Application::GetHandleInstance(),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		reinterpret_cast<void**>(&DirectInput::GetInstance()->direct_input_interface),
		nullptr
	);
	DirectInput::InitKeyboard();
	DirectInput::InitMause();
}



void DirectInput::Update() {
	DirectInput::UpdateKeyboard();
	DirectInput::UpdateMouse();
}



void DirectInput::Finalize() {
	DirectInput::UnInitKeyboard();
	DirectInput::UnInitMouse();
}
