#pragma once

#include<string>

namespace ErrorType {

	using ErrorType = const char*;
	using ErrorLocation = const char*;


	constexpr ErrorType failed_to_create_element = "�v�f�̍쐬�Ɏ��s���܂���";
	constexpr ErrorType failed_to_add_element = "�v�f�̒ǉ��Ɏ��s���܂���";
	constexpr ErrorType failed_to_exist_elemet = "�v�f�����݂��܂���";
	constexpr ErrorType failed_to_get_element = "�v�f�̎擾�Ɏ��s���܂���";
	constexpr ErrorType failed_to_load_element = "�v�f�̓ǂݍ��݂Ɏ��s���܂���";
	constexpr ErrorType failed_to_write_element = "�v�f�̏������݂Ɏ��s���܂���";

	constexpr ErrorType unknown = "�G���[���R���s��";
	constexpr ErrorType setting_failed = "�ݒ�Ɏ��s���܂���";
	constexpr ErrorType function_exe_failed = "�֐��̎��s�Ɏ��s���܂���";

}

template<class ClassType>
class ErrorClass {
public:
	// �N���X����std::string�^�ɕϊ����Ď擾����
	std::string name() {
		return std::string( typeid( ClassType ).name() );
	}
};