#pragma once
// システム関連ライブラリ





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

// システム関連ライブラリ
namespace System {

#pragma region GUID

	using GuidStr = std::string;

	//* GUIDの文字列を生成する
	//* @param int fmt 生成文字列の書式ID
	//* @return string GUID／NULL：生成失敗
	inline GuidStr GetGUIDStr( int_t fmt = 0 ) {

		// 参考サイト：https://www.pahoo.org/e-soul/webtech/cpp01/cpp01-10-01.shtm

		//char*バッファサイズ
		#define SIZE_BUFF 512

		// GUID取得用
		GUID guid;
		
		//エラー・メッセージ格納用
		std::string ErrorMessage;
		
		// バッファ
		static char buffer[SIZE_BUFF + 1];
		
		//生成文字列の書式
		const char* format[] = {
			 "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
			 "%08lX%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",
			 "{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
			 "{%08lX%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X}"
		};

		if( ( fmt < 0 ) || ( fmt > static_cast<int_t>(sizeof( format )) ) ) {
			ErrorMessage = "生成文字列の書式が不正";
			return "";
		}

		//GUIDの生成（Windows API利用）
		if( S_OK != ::CoCreateGuid( &guid ) ) {
			ErrorMessage = "GUID生成に失敗";
			return "";
		}
		else {
			// バッファに格納
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

	// GUIDを取得する
	inline longlong_t GetGUID( int_t fmt = 0 ) {
		auto guidStr = System::GetGUIDStr( fmt );

		std::size_t pos = 0;

		std::regex strRegex( R"(([0-9A-Fa-f]+)(-)([0-9A-Fa-f]+)(-)([0-9A-Fa-f]+)(-)([0-9A-Fa-f]+)(-)([0-9A-Fa-f]+))" );
		std::smatch sm;
		if( std::regex_search( guidStr, sm, strRegex ) ) {
			//guidStr = sm[1].str() + sm[3].str() + sm[5].str() + sm[7].str() + sm[9].str();

		// 16進数文字列を10進数文字列に変換して返す
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
		// DateTimeメソッドで作成した情報へアクセスする際の定数
		//===================================================
		constexpr const char* Year = "year";					// 西暦何年かの情報を取得する
		constexpr const char* Month = "month";					// 何月かの情報を取得する
		constexpr const char* Day = "day";						// 何日かの情報を取得する
		constexpr const char* Hour = "hour";					// 何時かの情報を取得する
		constexpr const char* Minute = "minute";				// 何分かの情報を取得する
		constexpr const char* Second = "second";				// 何秒かの情報を取得する
		constexpr const char* Week = "week";					// 何曜日かの情報を取得する
		constexpr const char* DayForYear = "day for week";	// 一年を通して何日目かの情報を取得する

		// 現在の日時を取得する
		// ※コピーやムーブで返すとランタイムエラーを起こすので値渡し
		inline std::unordered_map<std::string, std::string> DateTime() {

			std::unordered_map<std::string, std::string> dateTime;

			// 日時情報を取得
			std::time_t nowTime= std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
			std::tm* now = std::localtime( &nowTime );

			// 日時情報を格納
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

		// 現在の年月日をyyyy/mm/dd状態の文字列で取得
		// ※コピーやムーブで返すとランタイムエラーを起こすので値渡し
		inline std::string YYYY_MM_SS() {
			// 日時情報を取得
			time_t nowTime = std::time( nullptr );
			std::tm* now = std::localtime( &nowTime );
			auto year = Utility::ToString( 1900 + now->tm_year );
			auto month = Utility::ToString( now->tm_mon + 1 );
			auto day = Utility::ToString( now->tm_mday );

			return year + "/" + month + "/" + day;
		}

		// 現在の時分秒をhh:mm:ss状態の文字列で取得
		// ※コピーやムーブで返すとランタイムエラーを起こすので値渡し
		inline std::string HH_MM_SS() {
			// 日時情報を取得
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