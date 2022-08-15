#pragma once
// �V�X�e���֘A���C�u����





#include<guiddef.h>
#include<combaseapi.h>
#include<string>
#include<unordered_map>
#include<chrono>
#include<ctime>
#include<utility>
#include<regex>

#include"SafetyDataType.h"
#include"Utility.hpp"

// �V�X�e���֘A���C�u����
namespace System {

#pragma region GUID

	using GuidStr = std::string;

	//* GUID�̕�����𐶐�����
	//* @param int fmt ����������̏���ID
	//* @return string GUID�^NULL�F�������s
	inline GuidStr GetGUIDStr( int_t fmt = 0 ) {

		// �Q�l�T�C�g�Fhttps://www.pahoo.org/e-soul/webtech/cpp01/cpp01-10-01.shtm

		//char*�o�b�t�@�T�C�Y
		#define SIZE_BUFF 512

		// GUID�擾�p
		GUID guid;
		
		//�G���[�E���b�Z�[�W�i�[�p
		std::string ErrorMessage;
		
		// �o�b�t�@
		static char buffer[SIZE_BUFF + 1];
		
		//����������̏���
		const char* format[] = {
			 "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
			 "%08lX%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",
			 "{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
			 "{%08lX%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X}"
		};

		if( ( fmt < 0 ) || ( fmt > static_cast<int_t>(sizeof( format )) ) ) {
			ErrorMessage = "����������̏������s��";
			return "";
		}

		//GUID�̐����iWindows API���p�j
		if( S_OK != ::CoCreateGuid( &guid ) ) {
			ErrorMessage = "GUID�����Ɏ��s";
			return "";
		}
		else {
			// �o�b�t�@�Ɋi�[
			snprintf( 
				buffer,
				SIZE_BUFF,
				format[fmt],
				guid.Data1,
				guid.Data2,
				guid.Data3,
				guid.Data4[0],
				guid.Data4[1],
				guid.Data4[2],
				guid.Data4[3],
				guid.Data4[4],
				guid.Data4[5],
				guid.Data4[6],
				guid.Data4[7]
			);
		}

		return static_cast<GuidStr>(buffer);
	}

	// GUID���擾����
	inline longlong_t GetGUID( int_t fmt = 0 ) {
		auto guidStr = System::GetGUIDStr( fmt );

		std::size_t pos = 0;

		std::regex strRegex( R"(([0-9A-Fa-f]+)(-)([0-9A-Fa-f]+)(-)([0-9A-Fa-f]+)(-)([0-9A-Fa-f]+)(-)([0-9A-Fa-f]+))" );
		std::smatch sm;
		if( std::regex_search( guidStr, sm, strRegex ) ) {
			//guidStr = sm[1].str() + sm[3].str() + sm[5].str() + sm[7].str() + sm[9].str();

		// 16�i���������10�i��������ɕϊ����ĕԂ�
		//return Utility::StringToHex( guidStr );
		auto _1 = Utility::StringToHex( sm[1].str() );
		auto _3 = Utility::StringToHex( sm[3].str() );
		auto _5 = Utility::StringToHex( sm[5].str() );
		auto _7 = Utility::StringToHex( sm[7].str() );
		auto _9 = Utility::StringToHex( sm[9].str() );
		//auto ret = _1 * _3 * _5 * _7 * _9;
		//auto ret = _9 / ( _1 + _3 + _5 + _7 );
		//auto ret = _1 * _3 * _5 * _7 / _9;
		auto ret = _1 * _3 * _5 * _7 * _9;
		return ret;

		}


	}

#pragma endregion GUID

#pragma region DateTime

	namespace Date {

		//===================================================
		// DateTime���\�b�h�ō쐬�������փA�N�Z�X����ۂ̒萔
		//===================================================
		constexpr const char* Year = "year";					// ����N���̏����擾����
		constexpr const char* Month = "month";					// �������̏����擾����
		constexpr const char* Day = "day";						// �������̏����擾����
		constexpr const char* Hour = "hour";					// �������̏����擾����
		constexpr const char* Minute = "minute";				// �������̏����擾����
		constexpr const char* Second = "second";				// ���b���̏����擾����
		constexpr const char* Week = "week";					// ���j�����̏����擾����
		constexpr const char* DayForYear = "day for week";	// ��N��ʂ��ĉ����ڂ��̏����擾����

		// ���݂̓������擾����
		// ���R�s�[�⃀�[�u�ŕԂ��ƃ����^�C���G���[���N�����̂Œl�n��
		inline std::unordered_map<std::string, std::string> DateTime() {

			std::unordered_map<std::string, std::string> dateTime;

			// ���������擾
			std::time_t nowTime= std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
			std::tm* now = std::localtime( &nowTime );

			// ���������i�[
			dateTime.emplace( Date::Year, Utility::ToString( 1900 + now->tm_year ) );
			dateTime.emplace( Date::Month, Utility::ToString( now->tm_mon + 1 ) );
			dateTime.emplace( Date::Day, Utility::ToString( now->tm_mday ) );
			dateTime.emplace( Date::Hour, Utility::ToString( now->tm_hour ) );
			dateTime.emplace( Date::Minute, Utility::ToString( now->tm_min ) );
			dateTime.emplace( Date::Second, Utility::ToString( now->tm_sec ) );
			dateTime.emplace( Date::Week, Utility::ToString( now->tm_wday ) );
			dateTime.emplace( Date::DayForYear, Utility::ToString( now->tm_yday ) );

			return dateTime;

		}

		// ���݂̔N������yyyy/mm/dd��Ԃ̕�����Ŏ擾
		// ���R�s�[�⃀�[�u�ŕԂ��ƃ����^�C���G���[���N�����̂Œl�n��
		inline std::string YYYY_MM_SS() {
			// ���������擾
			time_t nowTime = std::time( nullptr );
			std::tm* now = std::localtime( &nowTime );
			auto year = Utility::ToString( 1900 + now->tm_year );
			auto month = Utility::ToString( now->tm_mon + 1 );
			auto day = Utility::ToString( now->tm_mday );

			return year + "/" + month + "/" + day;
		}

		// ���݂̎����b��hh:mm:ss��Ԃ̕�����Ŏ擾
		// ���R�s�[�⃀�[�u�ŕԂ��ƃ����^�C���G���[���N�����̂Œl�n��
		inline std::string HH_MM_SS() {
			// ���������擾
			time_t nowTime = std::time( nullptr );
			std::tm* now = std::localtime( &nowTime );

			return std::string(
				Utility::ToString( now->tm_hour )
				+
				":"
				+
				Utility::ToString( now->tm_min )
				+
				":"
				+
				Utility::ToString( now->tm_sec )
			);
		}

	}

#pragma endregion DateTime

}