
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

	// �f�o�C�X�I�u�W�F�N�g���쐬
	auto hr = direct_input->direct_input_interface->CreateDevice(
		GUID_SysKeyboard,
		&direct_input->direct_input_device_keyboard,
		NULL
	);

	if (FAILED(hr) || direct_input->direct_input_device_keyboard == nullptr) {

		//�L�[�{�[�h�����݂��Ȃ�
		MessageBox(hwnd, "Not exist Keyboard", "error", MB_ICONWARNING);

		Debug::LogWarning(
			typeid(DirectInput).name(),
			ErrorType::failed_to_exist_elemet,
			GetHRESULTMessage(hr)
		);

	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = direct_input->direct_input_device_keyboard->SetDataFormat(&c_dfDIKeyboard);


	IF_HR(hr) {

		//�L�[�{�[�h�t�H�[�}�b�g��ݒ�ł��Ȃ�����
		MessageBox(hwnd, "Setting Keyboard format error", "error", MB_ICONWARNING);

		Debug::LogWarning(
			typeid(DirectInput).name(),
			ErrorType::setting_failed,
			GetHRESULTMessage(hr)
		);

	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = direct_input->direct_input_device_keyboard->SetCooperativeLevel(hwnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));

	IF_HR(hr) {

		//�L�[�{�[�h�ɋ������[�h��ݒ�ł��Ȃ�����
		MessageBox(hwnd, "Setting Keyboard Cooperation Mode error", "error", MB_ICONWARNING);

		Debug::LogWarning(
			typeid(DirectInput).name(),
			ErrorType::setting_failed,
			GetHRESULTMessage(hr)
		);

	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
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

	// �O��̃f�[�^��ۑ�
	memcpy(key_state_old, direct_input->key_state.data(), DirectInput::key_max_num);

	// �f�o�C�X����f�[�^���擾
	hr = direct_input->direct_input_device_keyboard->GetDeviceState(
		sizeof(direct_input->key_state),
		direct_input->key_state.data()
	);


	if (SUCCEEDED(hr)) {
		for (int cnt = 0; cnt < DirectInput::key_max_num; cnt++) {
			// �g���K�[�ƃ����[�X���擾
			direct_input->key_state_trigger[cnt] = (direct_input->key_state[cnt] ^ key_state_old[cnt]) & direct_input->key_state[cnt];
			direct_input->key_state_release[cnt] = (direct_input->key_state[cnt] ^ key_state_old[cnt]) & key_state_old[cnt];

			// �L�[��������Ă���Ȃ烊�s�[�g�̔��菈��
			if (direct_input->key_state[cnt]) {
				if (direct_input->key_state_repeat_cnt[cnt] < 20) {
					direct_input->key_state_repeat_cnt[cnt]++;
					// �u������́v�������́u�{�^���������Ă���20�t���[���o�߁v
					// �����ꍇ�A���s�[�g�p�z��̃f�[�^��L���ɂ���
					//if(  ||  )
					// ��L�����ȊO�̓��s�[�g�p�z��̃f�[�^�𖳌��ɂ���
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
		//����else���̏������ʂ͂��܂�֌W�Ȃ�
		//���ۂɂ�hr�^�ɂ�Win32��HRESULT�^���߂�l�ŕԂ���Ă���Ƃ����G���[���o�Ă��邪�L�[�{�[�h�A�N�Z�X�ɂ͉���x�Ⴊ�Ȃ����ߑ��v
		//���̂��߁A�ȉ��̃G���[���b�Z�[�W�����̏������R�����g�A�E�g���Ă���

		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		hr = direct_input->direct_input_device_keyboard->Acquire();

		// �L�[�{�[�h�ւ̃A�N�Z�X���̎擾�Ɏ��s
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

	// �f�o�C�X�I�u�W�F�N�g���쐬
	auto hr = direct_input->direct_input_interface->CreateDevice(
		GUID_SysMouse,
		&direct_input->direct_input_device_mouse,
		NULL
	);

	if (FAILED(hr) || direct_input->direct_input_device_mouse == NULL) {
		//�}�E�X���Ȃ�
		MessageBox(hwnd, "Not exist Mouse", "error", MB_ICONWARNING);

		Debug::LogWarning(

			typeid(DirectInput).name(),
			ErrorType::failed_to_exist_elemet,
			GetHRESULTMessage(hr)

		);

	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = direct_input->direct_input_device_mouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr)) {
		//�}�E�X�̃t�H�[�}�b�g��ݒ�ł��Ȃ�����
		MessageBox(hwnd, "Not Setting Mouse format error", "error", MB_ICONWARNING);


		Debug::LogWarning(

			typeid(DirectInput).name(),
			ErrorType::setting_failed,
			GetHRESULTMessage(hr)

		);

	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = direct_input->direct_input_device_mouse->SetCooperativeLevel(hwnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr)) {
		//�}�E�X�̋������[�h��ݒ�ł��Ȃ�����
		MessageBox(hwnd, "Not Setting Mouse Cooperation Mode error", "error", MB_ICONWARNING);

		Debug::LogWarning(

			typeid(DirectInput).name(),
			ErrorType::setting_failed,
			GetHRESULTMessage(hr)

		);

	}

	// �f�o�C�X�̐ݒ�
	{
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL;

		hr = direct_input->direct_input_device_mouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
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

	// �O��̃f�[�^��ۑ�
	mouse_state_old = direct_input->mouse_state;

	// �f�o�C�X����f�[�^���擾
	auto hr = direct_input->direct_input_device_mouse->GetDeviceState(sizeof(direct_input->mouse_state), &direct_input->mouse_state);


	if (SUCCEEDED(hr)) {
		// �g���K�[�̎擾
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
