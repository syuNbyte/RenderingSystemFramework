#pragma once
// �ėp�I�Ȋ֐����C�u����



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



// �ėp�I�Ȋ֐����C�u����
namespace Utility {

	constexpr std::array<char_t, 10> HalfWidthDigit = { { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'} };

//===================================================
// ���l�֌W
//===================================================

	// ���������߂�
#pragma region Digit

	// ���������_�ȏ㕔���̐��l�̌��������߂�
	template<class T>
	inline int_t Digit( T value ) {

		// �Q�l�T�C�g�Fhttps://ohimaru.hatenablog.com/entry/2014/10/20/005322

		return static_cast< int_t >( std::log10( static_cast< double_t >( value ) ) ) + 1;

	}

	// ���������_�ȉ��̌��������߂�
	// �������^�͕s��
	template<class T, class FloatingPoint = std::enable_if_t<std::is_floating_point_v<T>>>
	inline int_t UnderFloatDigit( T value ) {

		// �Q�l�T�C�g�Fhttps://www.paveway.info/entry/2020/09/12/cli_getdigit

		auto string = Utility::_Internal_ToStringFloat( 
			value,							// ���ۂ̐��l
			std::chars_format::fixed,	// ���l�𕂓������_���\�L�̕�����ɕϊ�����
			100								// ���������_�ȉ�100���܂őΉ�������
		);

		//string.find_first_of(�w�蕶����)		= �w�蕶����̍ŏ��̕����̂ЂƂO�̈ʒu
		//string.find_first_of(�w�蕶����) + 1 = �w�蕶����̍ŏ��̕����̈ʒu
		//string.find_first_of(�w�蕶����) + 2 = �w�蕶����̍ŏ��̕����̂ЂƂ�̈ʒu
		auto ret = string.length() - ( string.find_first_of( "." ) + 1 );

		return ret;

	}

	// �S�̂̌��������߂�
	template<class T, std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
	inline int_t AllDigit( T value ) {
		// ���l�^�ȊO�̏ꍇ
		static_assert( false, "Use the numeric type." );
	}

	// �S�̂̌��������߂�
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline int_t AllDigit( T value ) {
		return Utility::Digit( value ) + Utility::UnderFloatDigit( value );
	}

	// �S�̂̌��������߂�
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline int_t AllDigit( T value ) {
		return Utility::Digit( value );
	}

#pragma endregion Digit

	// �w�肵�����̐��l���擾
#pragma region DigitNum

	// ���l�^�ȊO�̏ꍇ
	template<class T, std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
	inline int_t DigitNum( T value, int_t digit ) {
		// ���l�^�ȊO�̏ꍇ
		static_assert( false, "Use the numeric type." );
	}

	// �w�肵�����̐��l�𐮐��l����擾
	// value : ���l�f�[�^
	// digit : �擾���錅�@���O�͂P�̈ʂ̌����w���A-�P�͕��������_�ȉ����ʂ��w��
	// �߂�l : �\���\�Ȕ͈͂̒l�Ȃ炻�̌��̐��l�i0�`9�j�A�G���[�̏ꍇ��-�P��Ԃ�
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline int_t DigitNum( T value, int_t digit ) {

		// ���������_�^�p�I�[�o�[���[�h

		// ���f�[�^
		auto upperDigit = Utility::Digit( value );				// �������̌���
		auto underDigit = Utility::UnderFloatDigit( value );	// ���������_�ȉ��̌���

		// ������f�[�^
		auto string = Utility::_Internal_ToStringFloat( value, std::chars_format::fixed );

		// ������f�[�^����w�肵�����̐��l�����݂̂𐔒l�^�ɕϊ����ĕԂ�
		if( Math::Sign( digit ) > 0 ) {
			// �I�����Ă��錅��񂪐��̐��l�̏ꍇ
			return Utility::CharTo( string[upperDigit - digit - 1] );	// upperDigit + digit����0�����������_�̏����_��\���Ă��܂����߁u+1�v���Ă���
		}
		else {
			// �I�����Ă��錅��񂪕��̐��l�̏ꍇ
			// �I�����Ă��錅��񂪕\���\�Ȕ͈͂̌���񂾂����ꍇ
			if( Math::Abs( digit ) <= underDigit ) {
				return Utility::CharTo( string[upperDigit + Math::Abs( digit )] );
			}
			else {
				// �\���\�Ȕ͈͂̌����ł͂Ȃ������ꍇ
				return -1;
			}
		}

	}

	// �w�肵�����̐��l�𐮐��l����擾
	// value : ���l�f�[�^
	// digit : �擾���錅�@���O�͂P�̈ʂ̌����w���A-�P�͕��������_�ȉ����ʂ��w��
	// �߂�l : �\���\�Ȕ͈͂̒l�Ȃ炻�̌��̐��l�i0�`9�j�A�G���[�̏ꍇ��-�P��Ԃ�
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline int_t DigitNum( T value, int_t digit ) {

		// �����^�p�I�[�o�[���[�h

		// ���f�[�^
		auto upperDigit = Utility::Digit( value );				// �������̌���

		// ������f�[�^
		auto string = Utility::_Internal_ToStringInt( value );

		// ������f�[�^����w�肵�����̐��l�����݂̂𐔒l�^�ɕϊ����ĕԂ�
		if( Math::Sign( digit ) >= 0 ) {
			// �I�����Ă��錅��񂪐��̐��l�̏ꍇ
			return Utility::CharTo( string[upperDigit - digit - 1] );	// upperDigit + digit����0�����������_�̏����_��\���Ă��܂����߁u+1�v���Ă���
		}
		else {
			// �\���\�Ȕ͈͂̌����ł͂Ȃ������ꍇ
			return -1;
		}

	}

#pragma endregion DigitNum

//===================================================
// ������֌W
//===================================================

	// ���l�𕶎���֕ϊ�����
#pragma region ToString

	// ������֕ϊ�����
	//     value : �ϊ����鐔�l�f�[�^�i���������_�^�j
	//    format : �f�[�^�̕ϊ�����
	// precision : �o�͂��镶����̐��x
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline std::string _Internal_ToStringFloat( 
		T value,
		std::chars_format format = std::chars_format::general,
		int_t precision = 4
	) {

		std::string string;												// �f�[�^������std::string
		string.resize( 100 );

#if _HAS_COMPLETE_CHARCONV	//C++���̎d�l����������Ă���Ȃ猅�����w��ł�������g�p����
		// �ϊ�����
		auto result = std::to_chars(
			string.data(),
			string.data() + string.length(),
			value,
			format,
			precision
		);
#else
		// ���������_��precision�ʂ��l�̌ܓ�����
		value = Math::Round( value, -precision );

		// �l���[�����ǂ������肵�Ēl������������
		value = Math::ZeroIfZero( value );

		// �ϊ�����
		// ���������_�ȉ�
		auto result = std::to_chars(
			string.data(),
			string.data() + string.length(),
			value,
			format
		);
#endif

		// �����񒷂ɃT�C�Y�����킹��
		string.resize( string.find_first_of( '\0' ) );

		// ������̖��������l�Ȃ�
		if( std::all_of( string.end() - 1, string.end(), std::isdigit ) ) {
			// ���������_�̒l�ł�1.0f��1.0�Ȃǂ̂悤�ȕ��������_�ȉ���0�����Ȃ��ꍇ�́u1�v�Ƃ��ĕ\������Ă��܂��̂�
			// ���̏ꍇ�́u.0�v�𕶎���Ƃ��Ēǉ�����
			if( string.find( '.' ) == std::string::npos ) string.append( ".0" );
		}

#if _DEBUG

		try {
			if( result.ec == std::errc {} ) {
				// �Ȃɂ��G���[���Ȃ��ꍇ
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

	// ������֕ϊ�����
	//     value : �ϊ����鐔�l�f�[�^�i�����^�j
	// precision : �o�͂��镶����̐��x
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline std::string _Internal_ToStringInt( T value, int_t base = 10 ) {

		std::string string;												// �f�[�^������std::string
		string.resize( 100 );

#if _HAS_COMPLETE_CHARCONV	//C++���̎d�l����������Ă���Ȃ猅�����w��ł�������g�p����
		// �ϊ�����
		auto result = std::to_chars(
			string.data(),
			string.data() + string.length(),
			value,
			base
		);
#else
		// �ϊ�����
		auto result = std::to_chars(
			string.data(),
			string.data() + string.length(),
			value,
			base
		);
#endif

		// �����񒷂ɃT�C�Y�����킹��
		string.resize( string.find_first_of( '\0' ) );

#if _DEBUG

		try {
			if( result.ec == std::errc {} ) {
				// �Ȃɂ��G���[���Ȃ��ꍇ
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

	// ������֕ϊ�����
	template<class T, std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
	inline std::string ToString( T value ) {
		// ���l�^�ȊO�̏ꍇ
		//static_assert( false, "Use the numeric type." );
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	// ������֕ϊ�����
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline std::string ToString( T value ) {
		return Utility::_Internal_ToStringFloat( value );
	}

	// ������֕ϊ�����
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline std::string ToString( T value ) {
		return Utility::_Internal_ToStringInt( value );
	}

	// ������֕ϊ�����
	template<class T, std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
	inline std::string ToStringFloat( T value, std::chars_format format = std::chars_format::general, int_t precision = 5 ) {
		// ���l�^�ȊO�̏ꍇ
		//static_assert( false, "Use the numeric type." );
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	// ���������_�^�𕂓������_�^������֕ϊ�����
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline std::string ToStringFloat( T value, std::chars_format format = std::chars_format::general, int_t precision = 5 ) {
		value = Math::Round(value, -precision);
		return Utility::_Internal_ToStringFloat( value, format, precision );
	}

	// �����^�𕂓������_�^������֕ϊ�����
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline std::string ToStringFloat( T value, std::chars_format format = std::chars_format::general, int_t precision = 5 ) {
		long_double_t v = value;
		v = Math::Round( v, -precision );
		return Utility::_Internal_ToStringFloat( v, format, precision );
	}

	// ������֕ϊ�����
	template<class T, std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
	inline std::string ToStringInt( T value ) {
		// ���l�^�ȊO�̏ꍇ
		//static_assert( false, "Use the numeric type." );
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	// �����^�𐮐��^������֕ϊ�����
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline std::string ToStringInt( T value ) {
		return Utility::_Internal_ToStringInt( value );
	}

	// ���������_�^�𐮐��^������֕ϊ�����
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline std::string ToStringInt( T value ) {
		longlong_t v = value;
		return Utility::_Internal_ToStringInt( v );
	}

#pragma endregion ToString

	// ������𐔒l�֕ϊ�����
#pragma region StringTo

	// �����񂩂畂�������_�^���l�֕ϊ�����
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
				// �Ȃɂ��G���[���Ȃ��ꍇ
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

	// �����񂩂琮���^���l�֕ϊ�����
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
				// �Ȃɂ��G���[���Ȃ��ꍇ
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

	// �����񂩂琔�l�֕ϊ�����
	template<class T, std::enable_if_t<!std::is_arithmetic_v<T>>* = nullptr>
	inline T StringTo( std::string string ) {
		// ���l�^�ȊO�̏ꍇ
		static_assert( false, "Use the numeric type." );
	}

	// �����񂩂琔�l�֕ϊ�����
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline T StringTo( std::string string ) {
		return Utility::StringToFloat<T>( string );
	}

	// �����񂩂琔�l�֕ϊ�����
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline T StringTo( std::string string ) {
		return Utility::StringToInt<T>( string );
	}


#pragma endregion StringTo

	// �������琔�l�֕ϊ�����
#pragma region CharTo

	// �������琔�l�֕ϊ�����
	inline int_t CharTo( char_t chara ) {
		// Ascii�R�[�h���l�ɕϊ�
		auto ascii = static_cast< int_t >( chara );
		
		// Ascii�R�[�h���̐��l��\�������Ȃ�-48�������l��Ԃ��A����ȊO�Ȃ�G���[�l-�P��Ԃ�
		return ( std::isdigit(chara) ) ? ascii - 48 : -1;
	}

#pragma endregion CharTo

	// �i���ϊ�
#pragma region RadixTransformation

	// �������2�i�����l�ɕϊ�
	inline longlong_t StringToBin( std::string string ) {
		return std::strtoll( string.c_str(), nullptr, 2 );
	}

	// �������8�i�����l�ɕϊ�
	inline longlong_t StringToOct( std::string string ) {
		return std::strtoll( string.c_str(), nullptr, 8 );
	}

	// �������10�i�����l�ɕϊ�
	inline longlong_t StringToDecimal( std::string string ) {
		return std::strtoll( string.c_str(), nullptr, 10 );
	}

	// �������16�i�����l�ɕϊ�
	inline longlong_t StringToHex( std::string string ) {
		return std::strtoll( string.c_str(), nullptr, 16 );
	}

#pragma endregion RadixTransformation

	// �ϊ�
#pragma region Convert

	// �Q�l�T�C�g
	// http://www.slis.tsukuba.ac.jp/~fujisawa.makoto.fu/cgi-bin/wiki/index.php?string%A4%CB%A4%E8%A4%EB%CA%B8%BB%FA%CE%F3%BD%E8%CD%FD#b00d8ac3
	// https://qiita.com/Sugar_8/items/e35aae130fbc92fe4103

	// �}���`�o�C�g����������C�h������ɕϊ�
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

	// ���C�h��������}���`�o�C�g������ɕϊ��@�����C�h������ = UNICODE������
	inline std::string ToMultibyteStr( std::wstring wstring ) {
		::setlocale( LC_CTYPE, "JPN" );
#if !defined(WINAPI)
#pragma warning(disable:4996)	// Start disabling 4996 errors.
		std::string dst;
		size_t size = wstring.length()*MB_CUR_MAX + 1;    // �}���`�o�C�g������ł̍ő�T�C�Y
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

	// �����������ǂ����𔻒肷��
#pragma region IsDigitString

	// ���̕��������������ʂ���
	inline bool IsDigitString(char_t chara) {
		return std::isdigit(chara);
	}

#pragma endregion IsDigitString

	// ����������琔����T���o��
#pragma region FindDigitString

	// �w�肵���ꏊ�ȍ~�ɔ��p�����������ŏ��Ɍ��������ꏊ��Ԃ��B
	// ������Ȃ����std::string::npos��Ԃ�
	inline std::size_t FindFirstDigitString(std::string string, std::size_t pos = 0) {
		// ���p��������������
		auto findPos = string.find_first_of("0123456789", pos);
		if (findPos != std::string::npos) return findPos;

		// ������Ȃ����std::string::npos��Ԃ�
		return std::string::npos;
	}

	// �w�肵���ꏊ�ȍ~�ɔ��p�����������Ō�Ɍ��������ꏊ��Ԃ��B
	// ������Ȃ����std::string::npos��Ԃ�
	inline std::size_t FindLastDigitString(std::string string, std::size_t pos = std::string::npos) {
		// ���p��������������
		auto findPos = string.find_last_of("0123456789", pos);
		if (findPos != std::string::npos) return findPos;

		// ������Ȃ����std::string::npos��Ԃ�
		return std::string::npos;
	}

	// �擪���猩�Ĉ�ԍŏ��Ɍ�������������������擾����
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

	// �������猩�Ĉ�ԍŏ��Ɍ�������������������擾����
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

	// �͈͓��ɓ��ꕶ���񂪑��݂������Ԃ�
#pragma region FindStringNum

	// �͈͓��ɓ��ꕶ���񂪑��݂������Ԃ�
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

	// �ړ����Ɋւ��郁�\�b�h
#pragma region Related to Prefix

	// ��������ɓ���̐ړ��������݂��邩�ǂ����𔻒肷��
	inline bool StartWithPrefix(const std::string& string, const std::string& prefix) {
		return string.find(prefix) == 0;
	}

	// �͈͓��̕�����̒�����suffix������ŊJ�n���Ă��镶����݂̂𔲂��o��
	template<class Iter>
	inline std::vector<std::string> FindStartWithPrefix(Iter begin, Iter end, const std::string& prefix) {
		std::vector<std::string> ret;
		for (auto iter = begin; iter != end; iter++) {
			if (Utility::StartWithPrefix(*iter, prefix)) ret.emplace_back(*iter);
		}
		return ret;
	}

#pragma endregion Related to String Prefix

	// �ڔ����Ɋւ��郁�\�b�h
#pragma region Related to Suffix

	// ��������ɓ���̐ړ��������݂��邩�ǂ����𔻒肷��
	inline bool EndWithSuffix(const std::string& string, const std::string& suffix) {
		return string.rfind(suffix) == (string.length() - suffix.length());
	}

	// �͈͓��̕�����̒�����prefix������ŏI�����Ă��镶����݂̂𔲂��o��
	template<class Iter>
	inline std::vector<std::string> FindEndWithPrefix(Iter begin, Iter end, const std::string& suffix) {
		std::vector<std::string> ret;
		for (auto iter = begin; iter != end; iter++) {
			if (Utility::EndWithSuffix(*iter, suffix)) ret.emplace_back(*iter);
		}
		return ret;
	}

#pragma endregion Related to String Suffix

	// �A��
#pragma region SerialNumber

	// �͈͓��ɑ��݂��Ȃ��A�ԕ�����𐶐�����
	template<class Iter>
	inline std::string SerialNumber(Iter begin, Iter end, const std::string& base, const std::string& split = " ") {

		static_assert(
			std::is_same_v<
				typename Iter::value_type,
				std::string
			>,
			"Utility::SerialNumber Method StaticAssert[Iter::value_type not equal std::string]"
			);

		bool baseExist = false;	// base�Ɗ��S��v���镶���񂪑��݂��邩����
		ulonglong_t count = 0ULL;
		std::string findStrBase = base + split;

		// base�Ɗ��S��v���镶���񂪑��݂��邩�ǂ����𔻒肷��
		for (auto iter = begin; iter != end; iter++) {
			if (*iter == base) {
				baseExist = true;
			}
		}

		// �x�[�X�����񂪑��݂��Ȃ��Ȃ炻�̂܂܂̕������Ԃ�
		if (!baseExist) {
			return base;
		}

		// base + split + �����̕������0���珇�Ɍ������Ă���������Ȃ������ԍ����g�p����
		for (int_t j = 0;; j++) {

			auto findStr = findStrBase + Utility::ToStringInt(j);

			auto iter = std::find_if(
				begin,
				end,
				[&findStr](const std::string& str) { return str == findStr; }
			);

			// base + split + �����̒��Ō�����Ȃ��g�ݍ��킹������Α����ɂ��̕������Ԃ�
			if (iter == end) {
				return findStr;
			}
		}
	}

#pragma endregion SerialNumber

	// ������̒u���Ɋւ��郁�\�b�h
#pragma region Related to String Substitution

	// ��������ōŏ��Ɍ�����������̕�������w�蕶����ɒu������
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

	// ��������������̕������S�Ďw�蕶����ɒu������
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

	// ������̍폜�Ɋւ��郁�\�b�h
#pragma region Related Remove

	// �����������ŏ��Ɍ�����������̕�������폜����
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

	// ��������������̕���������ׂč폜����
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