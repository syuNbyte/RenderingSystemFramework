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

	LPDIRECTINPUT8			direct_input_interface = nullptr;				// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8	direct_input_device_keyboard = nullptr;		// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
	LPDIRECTINPUTDEVICE8	direct_input_device_mouse = nullptr;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�}�E�X)
	std::array<BYTE, DirectInput::key_max_num> key_state;					// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	std::array<BYTE, DirectInput::key_max_num> key_state_trigger;		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	std::array<BYTE, DirectInput::key_max_num> key_state_repeat;		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	std::array<BYTE, DirectInput::key_max_num> key_state_release;		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
	std::array<int, DirectInput::key_max_num>	key_state_repeat_cnt;	// �L�[�{�[�h�̃��s�[�g�J�E���^
	DIMOUSESTATE2	mouse_state;													// �}�E�X�̏�Ԃ��󂯎�郏�[�N
	DIMOUSESTATE2	mouse_state_trigger;											// �}�E�X�̏�Ԃ��󂯎�郏�[�N
	POINT				mouse_point;													// �}�E�X�̌��ݍ��W
	int				mouse_wheel;													// �}�E�X�z�C�[���̉�]��


private:

	static DirectInput* GetInstance();

private:

	static void InitKeyboard();
	static void UnInitKeyboard();

	static void InitMause();
	static void UnInitMouse();

public:

	//�L�[�{�[�h�̏�Ԃ��X�V
	static void UpdateKeyboard();

	//�e��L�[�{�[�h�擾�֐�
	static bool GetKeyboardPress( int key );		// �����Ă����
	static bool GetKeyboardTrigger( int key );	// �����ꂽ�u��
	static bool GetKeyboardRepeat( int key );		// ��莞�Ԉȏ㉟���������Ă���ꍇ
	static bool GetKeyboardRelease( int key );	// �����ꂽ�u��

public:

	//�}�E�X�̏�Ԃ��X�V
	static void UpdateMouse();

	//�}�E�X�̊e��{�^���̏�Ԃ��擾
	static bool GetMouseLeftPress();
	static bool GetMouseLeftTrigger();
	static bool GetMouseRightPress();
	static bool GetMouseRightTrigger();
	static bool GetMouseCenterPress();
	static bool GetMouseCenterTrigger();

	//�e���̈ړ�
	static long GetMouseAxisX();
	static long GetMouseAxisY();
	static long GetMouseAxisZ();

	//�E�B���h�E�̍�����W����̍��W
	static long GetMouseX();
	static long GetMouseY();

	// �}�E�X�z�C�[���̉�]�ʂ��擾����
	static int GetMouseWheelRotate();
	

public:

	//�L�[�{�[�h�A�}�E�X�̏�Ԃ��X�V
	static void Initalize();
	static void Update();
	static void Finalize();



};