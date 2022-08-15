#pragma once


#include<cinttypes>	// int~_t型, uint~_t型を使用するために必要
#include<cmath>		// float_t型, double_t型を使用するために必要

#pragma region


// int_t
using Char = int8_t;
using char_t = int8_t;
using Short = int16_t;
using short_t = int16_t;
using Int = int32_t;
using int_t = int32_t;
using Long = long;
using long_t = long;
using LLong = int64_t;
using longlong_t = int64_t;

// uint_t
using UChar = uint8_t;
using uchar_t = uint8_t;
using UShort = uint16_t;
using ushort_t = uint16_t;
using UInt = uint32_t;
using uint_t = uint32_t;
using uLong = unsigned long;
using ulong_t = unsigned long;
using ULLong = uint64_t;
using ulonglong_t = uint64_t;

// float_t, double_tは別ファイルで定義されているため定義していない
using LongDouble = long double;
using long_double_t = long double;


#pragma endregion Default Datatype





// 自分が分かりやすいように使うためのデータ型
#pragma region



// デフォルトで定義されているデータ型を使いやすくするためのエイリアス
using Pointer = void*;	// 汎用ポインタ型（どのポインタ型でも代入可能）
using Size = size_t;		// size_t型を名前変更しただけ


// Util系クラスで使用する管理ID
using ID = uint32_t;
using index = uint32_t;		// インデックス型


using if_index = int32_t;	// 条件判定も使えるインデックス
#define if_index_error -1	// if_index型をの使用時に使用するエラー型



#pragma endregion using Alias Easy to Understand DataType





// パーセント文字列リテラル
#pragma region



constexpr float operator""per( unsigned long long value ) {

	return static_cast< float >( value );

}


constexpr float operator""per( long double value ) {

	return static_cast< float >( value );

}



#pragma endregion percentage literal operator
