#pragma once





#include<windows.h>
#include<system_error>





// �}�N���̒�`
#pragma region



// DirectXLibrary�̃f�t�H���g�ō쐬����Ă���F�X�Ȃ��̂��g�p����ۂɎg�p����
#define DefaultDXL 0

// �z����g�p���Ȃ��ꍇ�̃X�^�[�g�X���b�g�̃f�t�H���g�ԍ�
#define NotArrayStartSlot 0

// 16�o�C�g�ɃA���C�������g����@
// ���萔�o�b�t�@�Ƀf�[�^��n���ۂ�16�o�C�g���ƂɃf�[�^�̋��E����؂��Ă��邽�߃f�[�^�����E�ɍ��킹��16�o�C�g�A���C�������g���s���Ă���
#define ALIGN16 _declspec(align(16))

// �߂�l�̎󂯎�����������nodiscard�������}�N��������������
#define NoDiscard [[nodiscard]]

// �񐄏��̃v���O�����ł��邱�Ƃ��R���p�C���ɓ`����
#define Deprecated [[deprecated]]	// �Q�l�T�C�g�Fhttps://cpprefjp.github.io/lang/cpp14/deprecated_attr.html

// �񐄏��̃v���O�����ł��邱�Ƃ��R���p�C���ɓ`���񐄏��R�����g������������
#define DeprecatedComment( comment ) [[deprecated(comment)]] 

// �g�p�񐄏��̃I�u�W�F�N�g�ɂ��鑮���\��
#define MaybeUnused [[maybe_unused]]

//�g�p���Ȃ��v���O�����Ɏg�p����ifndef�p�}�N��
#define UNUSED_PROGRAM

// �w�肵���֐��Ƀp�u���b�N�ȃA�N�Z�X�w��q��t�^����
#define PublicFunc public:

// �w�肵���֐��Ƀv���C�x�[�g�ȃA�N�Z�X�w��q��t�^����
#define PrivateFunc private:

// �w�肵���֐��Ƀv���e�N�g�ȃA�N�Z�X�w��q��t�^����
#define ProtectedFunc protected:

// �w�肵���֐��̂���ȏ�̃I�[�o�[���C�h���֎~����
#define OverrideFinal override final;

// �w�肵���N���X�̂���ȏ�̌p�����֎~����
#define FinalClass( className ) class className final



#pragma endregion Define Macro

// MessageBox�̕\�����Ȃ�������
#define NO_MESSAGE_BOX

// �֐��}�N���̒�`
#pragma region



// HRESULT�^�ɓ����Ă���G���[����std::string�^�ɒu������
#define GetHRESULTMessage( hr ) std::system_category().message(hr)

// HRESULT�^�̖߂�l�����s�������ꍇ�ɓ���if��
#define IF_HR(hr) if(FAILED(hr))

// �G���[���b�Z�[�W���o��
#if !defined(NO_MESSAGE_BOX)
#ifdef _DEBUG
#define ErrorMessage(error_message) MessageBox(nullptr, error_message, "error", MB_OK)
#else
#define ErrorMessage(error_message)
#endif
#else
#define ErrorMessage(error_message)
#endif

// HRESULT�^�ɓ����Ă���G���[���b�Z�[�W�����b�Z�[�W�{�b�N�X�ɕ\��
#define ErrorMessageHR( hr ) { \
	std::string error_message = GetHRESULTMessage(hr);\
	ErrorMessage( error_message.c_str() );\
}

// �ǂ̕����ŃG���[���N�����̂������������G���[���b�Z�[�W
#define RegionErrorMessage( region_str, error_message ) ErrorMessage(region_str##" : "##error_message)

// HRESULT�^�̖߂�l�����s�������ꍇ�ɃG���[���b�Z�[�W���o�͂���
#define IF_ErrorMessage( hr, error_message ) if( FAILED(hr) ){ ErrorMessage(error_message); }

// ErrorMessageStorage�N���X�ɃG���[���b�Z�[�W��ǉ�
#define AddErrorMessageLogger( class_name, error_type, error_message ){ \
\
	ErrorMessageLogger::AddErrorMessage(\
		class_name,\
		error_type,\
		file_name_macro,\
		std::to_string(line_num_macro),\
		function_name_macro,\
		error_message\
	);\
\
}

//�ȈՓI�ɉ񐔃��[�v���s�����߂̃}�N��
#define ForNum( loop_num ) for( int i = 0; i < loop_num; i++ )

// ���S��ComPtr��������邽�߂̃}�N��
#define SafeReleaseCom( com_object ) {\
	if( com_object ){\
		com_object->Release(); \
		com_object.Detach(); \
	}\
}

// ���S�Ƀf�[�^��������邽�߂̃}�N��
#define SafeRelease( object ) {\
 if( object != nullptr ){\
	object->Release();\
	object = nullptr;\
 }\
}


// �O�����Z�q�����s����}�N��
#define TernaryOperator( _if_, _true_, _false_ ) _if_ ? _true_ : _false_

// ���̔z��̗v�f�����擾����
#define ArrayCount( _array_ ) _countof(_array_)



#pragma endregion Define Function Macro