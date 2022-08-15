#pragma once

// STL
#include<fstream>

// TNLIB
#include"SafetyDataType.h"

// �R���\�[���E�B���h�E���g�p���邽�߂̃N���X
class ConsoleWindow {

private:

	FILE* m_fp;
	HWND m_windowHandle;

private:

	static ConsoleWindow& GetInstance();

public:

	// �R���\�[���E�B���h�E�̏���������
	static void Initialize();

	// �R���\�[���E�B���h�E�̍X�V�����@��WinMain�֐��̑�4�����̃E�B���h�E���[�h���󂯎��
	static void Update( int_t windoeMode );

	// �R���\�[���E�B���h�E�̏I������
	static void Finalize();

};
