#pragma once
// 汎用的な関数ライブラリ



#include<iostream>
#include<sstream>
#include<string>
#include<charconv>
#include<stdexcept>
#include<functional>
#include<algorithm>
#include<cctype>
#include<vector>

#include"SafetyDataType.h"
#include"Math.hpp"



// 汎用的な関数ライブラリ
namespace Utility {

	constexpr std::array<char_t, 10> HalfWidthDigit = { { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'} };

//===================================================
// 数値関係
//===================================================

	// 桁数を求める
#pragma region Digit

	// 浮動小数点以上部分の数値の桁数を求める
	template<class T>
	inline int_t Digit( T value ) {

		// 参考サイト：https://ohimaru.hatenablog.com/entry/2014/10/20/005322

		return static_cast< int_t >( std::log10( static_cast< double_t >( value ) ) ) + 1;

	}

	// 浮動小数点以下の桁数を求める
	// ※整数型は不可
	template<class T, class FloatingPoint = std::enable_if_t<std::is_floating_point_v<T>>>
	inline int_t UnderFloatDigit( T value ) {

		// 参考サイト：https://www.paveway.info/entry/2020/09/12/cli_getdigit

		auto string = Utility::_Internal_ToStringFloat( 
			value,							// 実際の数値
			std::chars_format::fixed,	// 数値を浮動小数点数表記の文字列に変換する
			100								// 浮動小数点以下100桁まで対応させる
		);

		//string.find_first_of(指定文字列)		= 指定文字列の最初の文字のひとつ前の位置
		//string.find_first_of(指定文字列) + 1 = 指定文字列の最初の文字の位置
		//string.find_first_of(指定文字列) + 2 = 指定文字列の最初の文字のひとつ後の位置
		auto ret = string.length() - ( string.find_first_of( "." ) + 1 );

		return ret;

	}

	// 全体の桁数を求める
	template<class T, std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
	inline int_t AllDigit( T value ) {
		// 数値型以外の場合
		static_assert( false, "Use the numeric type." );
	}

	// 全体の桁数を求める
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline int_t AllDigit( T value ) {
		return Utility::Digit( value ) + Utility::UnderFloatDigit( value );
	}

	// 全体の桁数を求める
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline int_t AllDigit( T value ) {
		return Utility::Digit( value );
	}

#pragma endregion Digit

	// 指定した桁の数値を取得
#pragma region DigitNum

	// 数値型以外の場合
	template<class T, std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
	inline int_t DigitNum( T value, int_t digit ) {
		// 数値型以外の場合
		static_assert( false, "Use the numeric type." );
	}

	// 指定した桁の数値を整数値から取得
	// value : 数値データ
	// digit : 取得する桁　※０は１の位の桁を指し、-１は浮動小数点以下第一位を指す
	// 戻り値 : 表現可能な範囲の値ならその桁の数値（0～9）、エラーの場合は-１を返す
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline int_t DigitNum( T value, int_t digit ) {

		// 浮動小数点型用オーバーロード

		// 桁データ
		auto upperDigit = Utility::Digit( value );				// 整数部の桁数
		auto underDigit = Utility::UnderFloatDigit( value );	// 浮動小数点以下の桁数

		// 文字列データ
		auto string = Utility::_Internal_ToStringFloat( value, std::chars_format::fixed );

		// 文字列データから指定した桁の数値部分のみを数値型に変換して返す
		if( Math::Sign( digit ) > 0 ) {
			// 選択している桁情報が正の数値の場合
			return Utility::CharTo( string[upperDigit - digit - 1] );	// upperDigit + digitだと0が浮動小数点の小数点を表してしまうため「+1」している
		}
		else {
			// 選択している桁情報が負の数値の場合
			// 選択している桁情報が表現可能な範囲の桁情報だった場合
			if( Math::Abs( digit ) <= underDigit ) {
				return Utility::CharTo( string[upperDigit + Math::Abs( digit )] );
			}
			else {
				// 表現可能な範囲の桁情報ではなかった場合
				return -1;
			}
		}

	}

	// 指定した桁の数値を整数値から取得
	// value : 数値データ
	// digit : 取得する桁　※０は１の位の桁を指し、-１は浮動小数点以下第一位を指す
	// 戻り値 : 表現可能な範囲の値ならその桁の数値（0～9）、エラーの場合は-１を返す
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline int_t DigitNum( T value, int_t digit ) {

		// 整数型用オーバーロード

		// 桁データ
		auto upperDigit = Utility::Digit( value );				// 整数部の桁数

		// 文字列データ
		auto string = Utility::_Internal_ToStringInt( value );

		// 文字列データから指定した桁の数値部分のみを数値型に変換して返す
		if( Math::Sign( digit ) >= 0 ) {
			// 選択している桁情報が正の数値の場合
			return Utility::CharTo( string[upperDigit - digit - 1] );	// upperDigit + digitだと0が浮動小数点の小数点を表してしまうため「+1」している
		}
		else {
			// 表現可能な範囲の桁情報ではなかった場合
			return -1;
		}

	}

#pragma endregion DigitNum

//===================================================
// 文字列関係
//===================================================

	// 数値を文字列へ変換する
#pragma region ToString

	// 文字列へ変換する
	//     value : 変換する数値データ（浮動小数点型）
	//    format : データの変換方式
	// precision : 出力する文字列の精度
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline std::string _Internal_ToStringFloat( 
		T value,
		std::chars_format format = std::chars_format::general,
		int_t precision = 4
	) {

		std::string string;												// データを入れるstd::string
		string.resize( 100 );

#if _HAS_COMPLETE_CHARCONV	//C++側の仕様が実装されているなら桁数が指定できる方を使用する
		// 変換する
		auto result = std::to_chars(
			string.data(),
			string.data() + string.length(),
			value,
			format,
			precision
		);
#else
		// 浮動小数点第precision位を四捨五入する
		value = Math::Round( value, -precision );

		// 値がゼロかどうか判定して値を書き換える
		value = Math::ZeroIfZero( value );

		// 変換する
		// 浮動小数点以下
		auto result = std::to_chars(
			string.data(),
			string.data() + string.length(),
			value,
			format
		);
#endif

		// 文字列長にサイズを合わせる
		string.resize( string.find_first_of( '\0' ) );

		// 文字列の末尾が数値なら
		if( std::all_of( string.end() - 1, string.end(), std::isdigit ) ) {
			// 浮動小数点の値でも1.0fや1.0などのような浮動小数点以下が0しかない場合は「1」として表示されてしまうので
			// その場合は「.0」を文字列として追加する
			if( string.find( '.' ) == std::string::npos ) string.append( ".0" );
		}

#if _DEBUG

		try {
			if( result.ec == std::errc {} ) {
				// なにもエラーがない場合
				return string;
			}
			else {
				throw std::logic_error( "missing convert value." );
			}
		}
		catch( const std::logic_error error ) {
			std::cout << error.what() << std::endl;
		}

#else

		if( result.ec == std::errc {} ) {
			return string;
		}
		else {
			return string;
		}

#endif

	}

	// 文字列へ変換する
	//     value : 変換する数値データ（整数型）
	// precision : 出力する文字列の制度
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline std::string _Internal_ToStringInt( T value, int_t base = 10 ) {

		std::string string;												// データを入れるstd::string
		string.resize( 100 );

#if _HAS_COMPLETE_CHARCONV	//C++側の仕様が実装されているなら桁数が指定できる方を使用する
		// 変換する
		auto result = std::to_chars(
			string.data(),
			string.data() + string.length(),
			value,
			base
		);
#else
		// 変換する
		auto result = std::to_chars(
			string.data(),
			string.data() + string.length(),
			value,
			base
		);
#endif

		// 文字列長にサイズを合わせる
		string.resize( string.find_first_of( '\0' ) );

#if _DEBUG

		try {
			if( result.ec == std::errc {} ) {
				// なにもエラーがない場合
				return string;
			}
			else {
				throw std::logic_error( "missing convert value." );
			}
		}
		catch( const std::logic_error error ) {
			std::cout << error.what() << std::endl;
			Debug::LogError( error.what() );
		}

#else

		if( result.ec == std::errc {} ) {
			return string;
		}
		else {
			return string;
		}

#endif

	}

	// 文字列へ変換する
	template<class T, std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
	inline std::string ToString( T value ) {
		// 数値型以外の場合
		//static_assert( false, "Use the numeric type." );
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	// 文字列へ変換する
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline std::string ToString( T value ) {
		return Utility::_Internal_ToStringFloat( value );
	}

	// 文字列へ変換する
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline std::string ToString( T value ) {
		return Utility::_Internal_ToStringInt( value );
	}

	// 文字列へ変換する
	template<class T, std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
	inline std::string ToStringFloat( T value, std::chars_format format = std::chars_format::general, int_t precision = 5 ) {
		// 数値型以外の場合
		//static_assert( false, "Use the numeric type." );
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	// 浮動小数点型を浮動小数点型文字列へ変換する
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline std::string ToStringFloat( T value, std::chars_format format = std::chars_format::general, int_t precision = 5 ) {
		value = Math::Round(value, -precision);
		return Utility::_Internal_ToStringFloat( value, format, precision );
	}

	// 整数型を浮動小数点型文字列へ変換する
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline std::string ToStringFloat( T value, std::chars_format format = std::chars_format::general, int_t precision = 5 ) {
		long_double_t v = value;
		v = Math::Round( v, -precision );
		return Utility::_Internal_ToStringFloat( v, format, precision );
	}

	// 文字列へ変換する
	template<class T, std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
	inline std::string ToStringInt( T value ) {
		// 数値型以外の場合
		//static_assert( false, "Use the numeric type." );
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	// 整数型を整数型文字列へ変換する
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline std::string ToStringInt( T value ) {
		return Utility::_Internal_ToStringInt( value );
	}

	// 浮動小数点型を整数型文字列へ変換する
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline std::string ToStringInt( T value ) {
		longlong_t v = value;
		return Utility::_Internal_ToStringInt( v );
	}

#pragma endregion ToString

	// 文字列を数値へ変換する
#pragma region StringTo

	// 文字列から浮動小数点型数値へ変換する
	template<class T = long_double_t, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline T StringToFloat( std::string string ) {

		T value;

		auto result = std::from_chars(
			string.data(),
			string.data() + string.length(),
			value,
			std::chars_format::general
		);

#if _DEBUG

		try {
			if( result.ec == std::errc {} ) {
				// なにもエラーがない場合
				return value;
			}
			else {
				throw std::logic_error( "missing convert value." );
			}
		}
		catch( const std::logic_error error ) {
			std::cout << error.what() << std::endl;
		}

#else

		if( result.ec == std::errc {} ) {
			return value;
		}
		else {
			return T();
		}

#endif

	}

	// 文字列から整数型数値へ変換する
	template<class T = longlong_t, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline T StringToInt( std::string string, int_t base = 10 ) {

		T value;

		auto result = std::from_chars(
			string.data(),
			string.data() + string.length(),
			value,
			base
		);

#if _DEBUG

		try {
			if( result.ec == std::errc {} ) {
				// なにもエラーがない場合
				return value;
			}
			else {
				throw std::logic_error( "missing convert value." );
			}
		}
		catch( const std::logic_error error ) {
			std::cout << error.what() << std::endl;
		}

#else

		if( result.ec == std::errc {} ) {
			return value;
		}
		else {
			return T();
		}

#endif

	}

	// 文字列から数値へ変換する
	template<class T, std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
	inline T StringTo( std::string string ) {
		// 数値型以外の場合
		static_assert( false, "Use the numeric type." );
	}

	// 文字列から数値へ変換する
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline T StringTo( std::string string ) {
		return Utility::StringToFloat<T>( string );
	}

	// 文字列から数値へ変換する
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline T StringTo( std::string string ) {
		return Utility::StringToInt<T>( string );
	}


#pragma endregion StringTo

	// 文字から数値へ変換する
#pragma region CharTo

	// 文字から数値へ変換する
	inline int_t CharTo( char_t chara ) {
		// Asciiコード数値に変換
		auto ascii = static_cast< int_t >( chara );
		
		// Asciiコード内の数値を表す文字なら-48した数値を返し、それ以外ならエラー値-１を返す
		return ( std::isdigit(chara) ) ? ascii - 48 : -1;
	}

#pragma endregion CharTo

	// 進数変換
#pragma region RadixTransformation

	// 文字列を2進数数値に変換
	inline longlong_t StringToBin( std::string string ) {
		return std::strtoll( string.c_str(), nullptr, 2 );
	}

	// 文字列を8進数数値に変換
	inline longlong_t StringToOct( std::string string ) {
		return std::strtoll( string.c_str(), nullptr, 8 );
	}

	// 文字列を10進数数値に変換
	inline longlong_t StringToDecimal( std::string string ) {
		return std::strtoll( string.c_str(), nullptr, 10 );
	}

	// 文字列を16進数数値に変換
	inline longlong_t StringToHex( std::string string ) {
		return std::strtoll( string.c_str(), nullptr, 16 );
	}

#pragma endregion RadixTransformation

	// 変換
#pragma region Convert

	// 参考サイト
	// http://www.slis.tsukuba.ac.jp/~fujisawa.makoto.fu/cgi-bin/wiki/index.php?string%A4%CB%A4%E8%A4%EB%CA%B8%BB%FA%CE%F3%BD%E8%CD%FD#b00d8ac3
	// https://qiita.com/Sugar_8/items/e35aae130fbc92fe4103

	// マルチバイト文字列をワイド文字列に変換
	inline std::wstring ToWideStr( std::string string ) {
		::setlocale( LC_CTYPE, "JPN" );
#if !defined(WINAPI)
#pragma warning(disable:4996)	// Start disabling 4996 errors.
		std::wstring dst;
		size_t size = string.length() + 1;
		wchar_t* wcs = new wchar_t[size];
		mbstowcs( wcs, string.c_str(), size );
		dst = wcs;
		delete[] wcs;
		return dst;
#elif defined(WINAPI) 
		std::vector<wchar_t> dest( string.length() + 16 );
		std::size_t converted;
		::mbstowcs_s( &converted, dest.data(), dest.size(), string.c_str(), _TRUNCATE );
		return std::wstring( dest.data() );
#pragma warning(default:4996)	// End disabling 4996 errors.
#endif
	}

	// ワイド文字列をマルチバイト文字列に変換　※ワイド文字列 = UNICODE文字列
	inline std::string ToMultibyteStr( std::wstring wstring ) {
		::setlocale( LC_CTYPE, "JPN" );
#if !defined(WINAPI)
#pragma warning(disable:4996)	// Start disabling 4996 errors.
		std::string dst;
		size_t size = wstring.length()*MB_CUR_MAX + 1;    // マルチバイト文字列での最大サイズ
		char *mbs = new char[size];
		wcstombs( mbs, wstring.c_str(), size );
		dst = mbs;
		delete[] mbs;
		return dst;
#elif defined(WINAPI) 
		std::vector<char> dest( wstring.length() + 16 );
		std::size_t converted;
		::wcstombs_s( &converted, dest.data(), dest.size(), wstring.c_str(), _TRUNCATE );
		return std::string( dest.data() );
#pragma warning(default:4996)	// End disabling 4996 errors.
#endif
	}

#pragma endregion Convert

	// 数字文字かどうかを判定する
#pragma region IsDigitString

	// この文字が数字か判別する
	inline bool IsDigitString(char_t chara) {
		return std::isdigit(chara);
	}

#pragma endregion IsDigitString

	// 文字列内から数字を探し出す
#pragma region FindDigitString

	// 指定した場所以降に半角数字文字が最初に見つかった場所を返す。
	// 見つからなければstd::string::nposを返す
	inline std::size_t FindFirstDigitString(std::string string, std::size_t pos = 0) {
		// 半角数字を検索する
		auto findPos = string.find_first_of("0123456789", pos);
		if (findPos != std::string::npos) return findPos;

		// 見つからなければstd::string::nposを返す
		return std::string::npos;
	}

	// 指定した場所以降に半角数字文字が最後に見つかった場所を返す。
	// 見つからなければstd::string::nposを返す
	inline std::size_t FindLastDigitString(std::string string, std::size_t pos = std::string::npos) {
		// 半角数字を検索する
		auto findPos = string.find_last_of("0123456789", pos);
		if (findPos != std::string::npos) return findPos;

		// 見つからなければstd::string::nposを返す
		return std::string::npos;
	}

	// 先頭から見て一番最初に見つかった数字文字列を取得する
	inline std::string GetFindFirstDigitString(std::string string, std::size_t pos = 0) {
		auto findPos = Utility::FindFirstDigitString(string, pos);
		if (findPos != std::string::npos) {
			ulonglong_t count = 1ULL;
			while (true) {
				if (std::isdigit(string[findPos + count])) {
					++count;
				}
				else {
					return string.substr(findPos, count);
				}
			}
		}
		else {
			return std::string();
		}
	}

	// 末尾から見て一番最初に見つかった数字文字列を取得する
	inline std::string GetFindLastDigitString(std::string string, std::size_t pos = std::string::npos) {
		auto findPos = Utility::FindLastDigitString(string, pos);
		if (findPos != std::string::npos) {
			ulonglong_t count = 1ULL;
			while (true) {
				if (std::isdigit(string[findPos - count])) {
					++count;
				}
				else {
					return string.substr(findPos - count + 1, count);
				}
			}
		}
		else {
			return std::string();
		}
	}

#pragma endregion FindDigitString

	// 範囲内に同一文字列が存在する個数を返す
#pragma region FindStringNum

	// 範囲内に同一文字列が存在する個数を返す
	template<class Iter>
	inline std::size_t FindStringNum(Iter begin, Iter end, const std::string& base) {
		std::size_t count = 0ULL;
		for (auto iter = begin; iter != end; iter++) {
			if (iter->find(base) != std::string::npos) {
				++count;
			}
		}
		return count;
	}

#pragma endregion FindStringNum

	// 接頭辞に関するメソッド
#pragma region Related to Prefix

	// 文字列内に特定の接頭辞が存在するかどうかを判定する
	inline bool StartWithPrefix(const std::string& string, const std::string& prefix) {
		return string.find(prefix) == 0;
	}

	// 範囲内の文字列の中からsuffix文字列で開始している文字列のみを抜き出す
	template<class Iter>
	inline std::vector<std::string> FindStartWithPrefix(Iter begin, Iter end, const std::string& prefix) {
		std::vector<std::string> ret;
		for (auto iter = begin; iter != end; iter++) {
			if (Utility::StartWithPrefix(*iter, prefix)) ret.emplace_back(*iter);
		}
		return ret;
	}

#pragma endregion Related to String Prefix

	// 接尾辞に関するメソッド
#pragma region Related to Suffix

	// 文字列内に特定の接頭辞が存在するかどうかを判定する
	inline bool EndWithSuffix(const std::string& string, const std::string& suffix) {
		return string.rfind(suffix) == (string.length() - suffix.length());
	}

	// 範囲内の文字列の中からprefix文字列で終了している文字列のみを抜き出す
	template<class Iter>
	inline std::vector<std::string> FindEndWithPrefix(Iter begin, Iter end, const std::string& suffix) {
		std::vector<std::string> ret;
		for (auto iter = begin; iter != end; iter++) {
			if (Utility::EndWithSuffix(*iter, suffix)) ret.emplace_back(*iter);
		}
		return ret;
	}

#pragma endregion Related to String Suffix

	// 連番
#pragma region SerialNumber

	// 範囲内に存在しない連番文字列を生成する
	template<class Iter>
	inline std::string SerialNumber(Iter begin, Iter end, const std::string& base, const std::string& split = " ") {

		static_assert(
			std::is_same_v<
				typename Iter::value_type,
				std::string
			>,
			"Utility::SerialNumber Method StaticAssert[Iter::value_type not equal std::string]"
			);

		bool baseExist = false;	// baseと完全一致する文字列が存在するか判定
		ulonglong_t count = 0ULL;
		std::string findStrBase = base + split;

		// baseと完全一致する文字列が存在するかどうかを判定する
		for (auto iter = begin; iter != end; iter++) {
			if (*iter == base) {
				baseExist = true;
			}
		}

		// ベース文字列が存在しないならそのままの文字列を返す
		if (!baseExist) {
			return base;
		}

		// base + split + 数字の文字列を0から順に検索していき見つからなかった番号を使用する
		for (int_t j = 0;; j++) {

			auto findStr = findStrBase + Utility::ToStringInt(j);

			auto iter = std::find_if(
				begin,
				end,
				[&findStr](const std::string& str) { return str == findStr; }
			);

			// base + split + 数字の中で見つからない組み合わせがあれば即座にその文字列を返す
			if (iter == end) {
				return findStr;
			}
		}
	}

#pragma endregion SerialNumber

	// 文字列の置換に関するメソッド
#pragma region Related to String Substitution

	// 文字列内で最初に見つかった特定の文字列を指定文字列に置換する
	inline std::string ReplaceString(std::string string, const std::string& findStr, const std::string& replaceStr) {
		auto pos = string.find(findStr);
		if (pos != std::string::npos) {
			auto begin = std::next(string.begin(), pos);
			auto end = std::next(begin, findStr.size());
			string.replace(begin, end, replaceStr);
			return string;
		}
		else {
			return string;
		}
	}

	// 文字列内から特定の文字列を全て指定文字列に置換する
	inline std::string ReplaceStringAll(std::string string, const std::string& findStr, const std::string& replaceStr) {
		while (true) {
			auto pos = string.find(findStr);
			if (pos != std::string::npos) {
				auto begin = std::next(string.begin(), pos);
				auto end = std::next(begin, findStr.size());
				string.replace(begin, end, replaceStr);
			}
			else {
				return string;
			}
		}
	}

#pragma endregion Related to String Substitution

	// 文字列の削除に関するメソッド
#pragma region Related Remove

	// 文字列内から最初に見つかった特定の文字列を削除する
	inline std::string RemoveString(std::string string, const std::string& removeStr) {
		auto pos = string.find(removeStr);
		if (pos != std::string::npos) {
			auto begin = std::next(string.begin(), pos);
			auto end = std::next(begin, removeStr.size());
			string.erase(begin, end);
			return string;
		}
		else {
			return string;
		}
	}

	// 文字列内から特定の文字列をすべて削除する
	inline std::string RemoveStringAll(std::string string, const std::string& removeStr) {
		while (true) {
			auto pos = string.find(removeStr);
			if (pos != std::string::npos) {
				auto begin = std::next(string.begin(), pos);
				auto end = std::next(begin, removeStr.size());
				string.erase(begin, end);
			}
			else {
				return string;
			}
		}
	}

#pragma endregion Related to String Remove

}