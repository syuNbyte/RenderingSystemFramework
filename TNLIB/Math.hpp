#pragma once
// 実際にC++のライブラリを使用すると
// 無駄なincludeが発生するし、いちいち探さなくてはならない
// それを解消するためのライブラリ
// UnityのMathfライブラリを参考に作成している
// Unity.Mathf : https://docs.unity3d.com/jp/current/ScriptReference/Mathf.html

// TODO
// クラス内メソッド以外は全てののメソッドにinline を付けないとLINK2005エラーを送出するため付けること



//======================================================================================================
// 座標系指定マクロ
//======================================================================================================
// ※注意：マクロを定義する際はこのヘッダのインクルード宣言よりも前に定義しないと意味がない、
// 　　　　なので使用するファイルの最上部で宣言する事を推奨する
// 
// 左手座標系を明示的に使用する際に適用するマクロ：#define _MATH_LIB_LH_COORD
// 右手座標系を明示的に使用する際に適用するマクロ：#define _MATH_LIB_RH_COORD
//======================================================================================================

// 明示的に使用する座標系マクロが定義されているかを判定し定義されていなければ以下のマクロを定義する
#if !defined(_MATH_LIB_LH_COORD) && !defined(_MATH_LIB_RH_COORD)
#define _UNDEFINED_MATH_LIB_COORD
#endif

// 内部的で使用するマクロを定義する
#if defined(_MATH_LIB_LH_COORD)
// 左手座標系マクロが定義されているなら内部的な左手座標系マクロを定義
#define _LH_COORD
#define MATH_LIB_COORD "Left Hand Coord"
#elif defined(_MATH_LIB_RH_COORD)
// 右手座標系マクロが定義されているなら内部的な右手座標系マクロを定義
#define _RH_COORD
#define MATH_LIB_COORD "Right Hand Coord"
#elif defined(_UNDEFINED_MATH_LIB_COORD)
// どちらの座標系かを定義されていない場合は強制的に左手座標系を適用する
#define _LH_COORD
#define MATH_LIB_COORD "Left Hand Coord"
#endif

// 左手座標系かを判定する内部マクロを定義
#ifdef _LH_COORD
#define _IF_LH_COORD 1
#else
#define _IF_LH_COORD 0
#endif

// 右手座標系かを判定する内部マクロを定義
#ifdef _RH_COORD
#define _IF_RH_COORD 1
#else
#define _IF_RH_COORD 0
#endif



#include<array> // PerlinNoiseで使用する
#include<cmath>
#include<random>
#include<limits>
#include<algorithm>
#include<numeric>
#include<cfenv>
#include<type_traits>
#include<stdexcept>

#include"SafetyDataType.h"
#include"Utility.hpp"
#include"TypeTraitsEx.hpp"
#include"FileUtility.hpp"
#include"Debug.hpp"

#undef min
#undef max

// 数学計算ライブラリ
namespace Math {

//===================================================
// 変数
//===================================================

#pragma region Variable

	// π
	template<class T>
	constexpr T Pi = T( 3.141592653589793238462643383279 );

	// 1/π（πの逆数）
	template<class T>
	constexpr T InvPi = T( 1.0 / 3.141592653589793238462643383279 );

	// π/2
	template<class T>
	constexpr T PiDiv2 = T(3.141592653589793238462643383279 / 2.0);

	// π/4
	template<class T>
	constexpr T PiDiv4 = T(3.141592653589793238462643383279 / 4.0);


	// 度からラジアンに変換する
	template<class T>
	constexpr T Deg2Rad = T( 0.017453293 );

	// ラジアンから度に変換する
	template<class T>
	constexpr T Rad2Deg = T( 57.29577951 );

	// 正の無限大
	template<class T>
	inline constexpr T Infinity() noexcept {
		return std::numeric_limits<T>::infinity();
	};

// 負の無限大
	template<class T>
	inline constexpr T negativeInfinity() noexcept {
		return -std::numeric_limits<T>::infinity();
	};

// イプシロン
	template<class T>
	inline constexpr T Epsilon() noexcept {
		return std::numeric_limits<T>::epsilon();
	};

// NaN（Not a Number）数値では表現できない場合に使用する
	template<class T>
	inline constexpr T NaN() noexcept {
		return std::numeric_limits<T>::quiet_NaN();
	};

// 最小の表現可能な数値
	template<class T>
	inline constexpr T MinValue() noexcept {
		return std::numeric_limits<T>::min();
	};

// 最大の表現可能な数値
	template<class T>
	inline constexpr T MaxValue() noexcept {
		return std::numeric_limits<T>::max();
	};

#pragma endregion Variable

//===================================================
// メソッド
//===================================================

	// sin：正弦を求める
#pragma region Sin

	// 引数がラジアンで表現されるsin関数
	template<class T>
	inline T Sin( T radian ) {
		return std::sin( radian );
	}

	// 引数がラジアンで表現されるsin関数
	template<>
	inline float_t Sin( float_t radian ) {
		return std::sin( radian );
	}

	// 引数がラジアンで表現されるsin関数
	template<>
	inline double_t Sin( double_t radian ) {
		return std::sin( radian );
	}

	// 引数がラジアンで表現されるsin関数
	template<>
	inline long_double_t Sin( long_double_t radian ) {
		return std::sinl( radian );
	}


	// 引数が度で表現されるsin関数
	template<class T>
	inline T SinDeg( T degree ) {
		return std::sin( degree * Math::Rad2Deg<T> );
	}

	// 引数が度で表現されるsin関数
	template<>
	inline float_t SinDeg( float_t degree ) {
		return std::sin( degree * Math::Rad2Deg<float_t> );
	}

	// 引数が度で表現されるsin関数
	template<>
	inline double_t SinDeg( double_t degree ) {
		return std::sin( degree * Math::Rad2Deg<double_t> );
	}

	// 引数が度で表現されるsin関数
	template<>
	inline long_double_t SinDeg( long_double_t degree ) {
		return std::sinl( degree * Math::Rad2Deg<long_double_t> );
	}

#pragma endregion Sin

	// cos：余弦を求める
#pragma region Cos

	// 引数がラジアンで表現されるcos関数
	template<class T>
	inline T Cos( T value ) {
		return std::cos( value );
	}

	// 引数がラジアンで表現されるcos関数
	template<>
	inline float_t Cos( float_t value ) {
		return std::cosf( value );
	}

	// 引数がラジアンで表現されるcos関数
	template<>
	inline double_t Cos( double_t value ) {
		return std::cos( value );
	}

	// 引数がラジアンで表現されるcos関数
	template<>
	inline long_double_t Cos( long_double_t value ) {
		return std::cosl( value );
	}


	// 引数が度で表現されるsin関数
	template<class T>
	inline T CosDeg( T degree ) {
		return std::cos( degree * Math::Rad2Deg<T> );
	}

	// 引数が度で表現されるcos関数
	template<>
	inline float_t CosDeg( float_t degree ) {
		return std::cos( degree * Math::Rad2Deg<float_t> );
	}

	// 引数が度で表現されるcos関数
	template<>
	inline double_t CosDeg( double_t degree ) {
		return std::cos( degree * Math::Rad2Deg<double_t> );
	}

	// 引数が度で表現されるcos関数
	template<>
	inline long_double_t CosDeg( long_double_t degree ) {
		return std::cosl( degree * Math::Rad2Deg<long_double_t> );
	}

#pragma endregion Cos

	// tan：傾きを求める
#pragma region Tan

	// 引数がラジアンで表現されるtan関数
	template<class T>
	inline T Tan( T value ) {
		return std::tan( value );
	}

	// 引数がラジアンで表現されるtan関数
	template<>
	inline float_t Tan( float_t value ) {
		return std::tanf( value );
	}

	// 引数がラジアンで表現されるtan関数
	template<>
	inline double_t Tan( double_t value ) {
		return std::tan( value );
	}

	// 引数がラジアンで表現されるtan関数
	template<>
	inline long_double_t Tan( long_double_t value ) {
		return std::tanl( value );
	}

	// 引数が度で表現されるtan関数
	template<class T>
	inline T TanDeg( T degree ) {
		return std::tan( degree * Math::Rad2Deg<T> );
	}

	// 引数が度で表現されるtan関数
	template<>
	inline float_t TanDeg( float_t degree ) {
		return std::tan( degree * Math::Rad2Deg<float_t> );
	}

	// 引数が度で表現されるtan関数
	template<>
	inline double_t TanDeg( double_t degree ) {
		return std::tan( degree * Math::Rad2Deg<double_t> );
	}

	// 引数が度で表現されるtan関数
	template<>
	inline long_double_t TanDeg( long_double_t degree ) {
		return std::tanl( degree * Math::Rad2Deg<long_double_t> );
	}

#pragma endregion Tan

	// Arc sin
#pragma region Asin

	// Arc sin
	template<class T>
	inline T Asin( T value ) {
		return std::asin( value );
	}

	template<>
	inline float_t Asin( float_t value ) {
		return std::asin( value );
	}

	template<>
	inline double_t Asin( double_t value ) {
		return std::asin( value );
	}

	template<>
	inline long_double_t Asin( long_double_t value ) {
		return std::asinl( value );
	}

#pragma endregion Asin

	// Arc cos
#pragma region Acos

	// Arc cos
	template<class T>
	inline T Acos( T value ) {
		return std::acos( value );
	}

	template<>
	inline float_t Acos( float_t value ) {
		return std::acos( value );
	}

	template<>
	inline double_t Acos( double_t value ) {
		return std::acos( value );
	}

	template<>
	inline long_double_t Acos( long_double_t value ) {
		return std::acosl( value );
	}

#pragma endregion Acos

	// Arc tan, Atan2
#pragma region Atan

	// Arc tan
	template<class T>
	inline T Atan( T value ) {
		return std::atan( value );
	}

	template<>
	inline float_t Atan( float_t value ) {
		return std::atan( value );
	}

	template<>
	inline double_t Atan( double_t value ) {
		return std::atan( value );
	}

	template<>
	inline long_double_t Atan( long_double_t value ) {
		return std::atanl( value );
	}


	// 2次元ベクトルから角度を求める
	template<class T>
	inline T Atan2( T y, T x ) {
		return std::atan2( y, x );
	}

	// 2次元ベクトルから角度を求める
	template<>
	inline float_t Atan2( float_t y, float_t x ) {
		return std::atan2f( y, x );
	}

	// 2次元ベクトルから角度を求める
	template<>
	inline double_t Atan2( double_t y, double_t x ) {
		return std::atan2( y, x );
	}

	// 2次元ベクトルから角度を求める
	template<>
	inline long_double_t Atan2( long_double_t y, long_double_t x ) {
		return std::atan2l( y, x );
	}

#pragma endregion Atan

	// 合計を求める
#pragma region Sum

	// 合計を求める
	// ※このメソッドはコンテナクラスのイテレータを使用してその範囲の集計結果を取得する
	// beginIter : 集計開始場所のイテレータ
	// lastIter : 集計終了場所のイテレータ
	template<class T, class Init>
	inline T Sum( Init beginIter, Init lastIter ) {

		// イテレータ内の要素がデフォルト構築可能ではない場合エラー
		static_assert( 
			std::is_default_constructible_v<typename Init::value_type>,
			"The type is not default constructible."
			);
		// T&型にInit::value_type型に代入可能ではない場合エラー
		// ※イテレータ内のデータ型が戻り値型に代入可能ではない場合エラー
		static_assert(
			std::is_assignable_v<std::add_lvalue_reference_t<T>, typename Init::value_type>,
			"The data type in the iterator is not changeable to the data type of the return value."
			);

		typename Init::value_type ret = typename Init::value_type();
		for( auto iter = beginIter; iter != lastIter; iter++ ) ret = ret + *iter;
		return static_cast<T>(ret);
	}

	// 合計を求める
	// ※このメソッドはコンテナクラスのイテレータを使用してその範囲の集計結果を取得する
	// beginIter : 集計開始場所のイテレータ
	// lastIter : 集計終了場所のイテレータ
	// init : 集計データの初期値
	template<class T, class Init>
	inline T Sum( Init beginIter, Init lastIter, typename Init::value_type init ) {

		// T&型にInit::value_type型に代入可能ではない場合エラー
		// ※イテレータ内のデータ型が戻り値型に代入可能ではない場合エラー
		static_assert(
			std::is_assignable_v<std::add_lvalue_reference_t<T>, typename Init::value_type>,
			"The data type in the iterator is not changeable to the data type of the return value."
			);

		typename Init::value_type ret = init;
		for( auto iter = beginIter; iter != lastIter; iter++ ) ret = ret + *iter;
		return static_cast< T >( ret );
	}

	// 条件に合致する値の合計を求める
	// ※このメソッドはコンテナクラスのイテレータを使用してその範囲の集計結果を取得する
	// beginIter : 集計開始場所のイテレータ
	// lastIter : 集計終了場所のイテレータ
	// cond : 集計する際に使用する条件判定式：// この式を改造すること「[]( const decltype( v )::value_type& value ) {return true;}」
	template<class T, class Init, class Cond>
	inline T SumIf( Init beginIter, Init lastIter, Cond cond ) {

		// イテレータ内の要素がデフォルト構築可能ではない場合エラー
		static_assert(
			std::is_default_constructible_v<typename Init::value_type>,
			"The type is not default constructible."
			);
		// T&型にInit::value_type型に代入可能ではない場合エラー
		// ※イテレータ内のデータ型が戻り値型に代入可能ではない場合エラー
		static_assert(
			std::is_assignable_v<std::add_lvalue_reference_t<T>, typename Init::value_type>,
			"The data type in the iterator is not changeable to the data type of the return value."
			);

		typename Init::value_type ret = typename Init::value_type();
		for( auto iter = beginIter; iter != lastIter; iter++ ) if( cond( *iter ) ) ret = ret + *iter;
		return static_cast< T >( ret );
	}

	// 条件に合致する値の合計を求める
	// ※このメソッドはコンテナクラスのイテレータを使用してその範囲の集計結果を取得する
	// beginIter : 集計開始場所のイテレータ
	// lastIter : 集計終了場所のイテレータ
	// init : 集計データの初期値
	// cond : 集計する際に使用する条件判定式：// この式を改造すること「[]( const decltype( v )::value_type& value ) {return true;}」
	template<class T, class Init, class Cond>
	inline T SumIf( Init beginIter, Init lastIter, typename Init::value_type init, Cond cond ) {

		// T&型にInit::value_type型に代入可能ではない場合エラー
		// ※イテレータ内のデータ型が戻り値型に代入可能ではない場合エラー
		static_assert(
			std::is_assignable_v<std::add_lvalue_reference_t<T>, typename Init::value_type>,
			"The data type in the iterator is not changeable to the data type of the return value."
			);

		typename Init::value_type ret = init;
		for( auto iter = beginIter; iter != lastIter; iter++ ) if( cond( *iter ) ) ret = ret + *iter;
		return static_cast< T >( ret );
	}

#pragma endregion Sum

	// 平均を求める
#pragma region Average

	// 平均を求める
	// ※このメソッドはコンテナクラスのイテレータを使用してその範囲の集計結果を取得する
	// ※このメソッドは数値型用の特殊化ver
	// beginIter : 集計開始場所のイテレータ
	// lastIter : 集計終了場所のイテレータ
	template<class T, class Init>
	inline T Average( Init beginIter, Init lastIter ) {

		// イテレータ内の要素がデフォルト構築可能ではない場合エラー
		static_assert(
			std::is_default_constructible_v<typename Init::value_type>,
			"The type is not default constructible."
			);
		// T&型にInit::value_type型に代入可能ではない場合エラー
		// ※イテレータ内のデータ型が戻り値型に代入可能ではない場合エラー
		static_assert(
			std::is_assignable_v<std::add_lvalue_reference_t<T>, typename Init::value_type>,
			"The data type in the iterator is not changeable to the data type of the return value."
			);

		typename Init::value_type ret = typename Init::value_type();
		std::size_t size = 0ULL;
		for( auto iter = beginIter; iter != lastIter; iter++ ) {
			++size;
			ret = ret + *iter;
		}
		return static_cast< T >( ret ) / static_cast< double_t >( size );
	}

	// 平均を求める
	// ※このメソッドはコンテナクラスのイテレータを使用してその範囲の集計結果を取得する
	// ※このメソッドは数値型用の特殊化ver
	// beginIter : 集計開始場所のイテレータ
	// lastIter : 集計終了場所のイテレータ
	// init : 集計データの初期値
	template<class T, class Init>
	inline T Average( Init beginIter, Init lastIter, typename Init::value_type init ) {

		// T&型にInit::value_type型に代入可能ではない場合エラー
		// ※イテレータ内のデータ型が戻り値型に代入可能ではない場合エラー
		static_assert(
			std::is_assignable_v<std::add_lvalue_reference_t<T>, typename Init::value_type>,
			"The data type in the iterator is not changeable to the data type of the return value."
			);

		typename Init::value_type ret = init;
		std::size_t size = 0ULL;
		for( auto iter = beginIter; iter != lastIter; iter++ ) {
			++size;
			ret = ret + *iter;
		}
		return static_cast< T >( ret ) / static_cast< double_t >( size );
	}

	// 条件に合致する値の平均を求める
	// ※このメソッドはコンテナクラスのイテレータを使用してその範囲の集計結果を取得する
	// ※このメソッドは数値型用の特殊化ver
	// beginIter : 集計開始場所のイテレータ
	// lastIter : 集計終了場所のイテレータ
	// cond : 集計する際に使用する条件判定式：// この式を改造すること「[]( const decltype( v )::value_type& value ) {return true;}」
	template<class T, class Init, class Cond>
	inline T AverageIf( Init beginIter, Init lastIter, Cond cond ) {

		// イテレータ内の要素がデフォルト構築可能ではない場合エラー
		static_assert(
			std::is_default_constructible_v<typename Init::value_type>,
			"The type is not default constructible."
			);
		// T&型にInit::value_type型に代入可能ではない場合エラー
		// ※イテレータ内のデータ型が戻り値型に代入可能ではない場合エラー
		static_assert(
			std::is_assignable_v<std::add_lvalue_reference_t<T>, typename Init::value_type>,
			"The data type in the iterator is not changeable to the data type of the return value."
			);

		typename Init::value_type ret = typename Init::value_type();
		std::size_t size = 0ULL;
		for( auto iter = beginIter; iter != lastIter; iter++ ) {
			if( cond( *iter ) ) {
				++size;
				ret = ret + *iter;
			}
		}
		return static_cast< T >( ret ) / static_cast< double_t >( size );
	}

	// 条件に合致する値の平均を求める
	// ※このメソッドはコンテナクラスのイテレータを使用してその範囲の集計結果を取得する
	// ※このメソッドは数値型用の特殊化ver
	// beginIter : 集計開始場所のイテレータ
	// lastIter : 集計終了場所のイテレータ
	// init : 集計データの初期値
	// cond : 集計する際に使用する条件判定式：// この式を改造すること「[]( const decltype( v )::value_type& value ) {return true;}」
	template<class T, class Init, class Cond>
	inline T AverageIf( Init beginIter, Init lastIter, typename Init::value_type init, Cond cond ) {

		// T&型にInit::value_type型に代入可能ではない場合エラー
		// ※イテレータ内のデータ型が戻り値型に代入可能ではない場合エラー
		static_assert(
			std::is_assignable_v<std::add_lvalue_reference_t<T>, typename Init::value_type>,
			"The data type in the iterator is not changeable to the data type of the return value."
			);

		typename Init::value_type ret = init;
		std::size_t size = 0ULL;
		for( auto iter = beginIter; iter != lastIter; iter++ ) {
			if( cond( *iter ) ) {
				++size;
				ret = ret + *iter;
			}
		}
		return static_cast< T >( ret ) / static_cast< double_t >( size );
	}

#pragma endregion Average

	// 累乗
#pragma region Pow

	// 値を累乗した結果を求める
	// value : 累乗させる値
	// n : 累乗させる回数
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T Pow( T value, T n ) {
		return static_cast< T >( std::powl( value, n ) );
	}

	// 値を2乗した結果を求める
	// value : 累乗させる値
	template<class T>
	inline T Pow2( T value ) {
		return value * value;
	}

#pragma endregion Pow

	// 階乗
#pragma region Factorial

	// n回階乗した結果を求める
	template<class T = double_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T Factorial( int_t n ) {
		T tmp = 1;
		for( T i = 2; i < n; i++ ) tmp *= i;

		return tmp;
	}

#pragma endregion Factorial

	// 何倍すれば目的の数値になるかを求める
#pragma region Target multiply value

	// 何倍すれば目的の数値になるかを求める
	template<class T = double_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T TargetMultiply( T value, T targetValue ) {
		return targetValue / value;
	}

#pragma endregion Target multiply value

	// 余り
#pragma region Mod

	// 余りを計算する
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline T Mod( T value, T modBase ) {
		return value % modBase;
	}

	// 余りを計算する
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline T Mod( T value, T modBase ) {
		return value - ( Math::Floor( value / modBase ) * modBase );
	}

	// 数値が余剰を求める数より大きい場合、その数の余りを返す
	// 余剰を求める数よりも小さい場合、0を返す
	// value：数値
	// modBase：余剰を求める数
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T GreaterMod( T value, T modBase ) {
		return ( Math::GreaterEqual( value, modBase ) ) ? Math::Mod<T>( value, modBase ) : static_cast< T >( 0.0 );
	}

#pragma endregion Mod

	// 分数
#pragma region Fraction

	// 分数の計算を行う
	// 戻り値：分数の計算結果
	// molecule : 分子
	// denominator : 分母
	template<class T>
	inline T Fraction( T molecule, T denominator ) {
		return static_cast<T>(1.0) / denominator * molecule;
	}

	// valueを分数倍した結果を返す
	// 戻り値：分数倍した結果
	// value : もとの値
	// molecule : 分子
	// denominator : 分母
	template<class T>
	inline T FractionValue( T value, T molecule, T denominator ) {
		return static_cast< T >( 1.0 ) / denominator * molecule * value;
	}

#pragma endregion Fraction

	// 差分
#pragma region Difference

	// 差分を計算する
	template<class T>
	inline T Difference( T a, T b ) {
		return Math::Abs( a - b );
	}

#pragma endregion Difference

	// 平方根の計算（√）
#pragma region Sqrt

	// 平方根の計算を行います（√）
	template<class T>
	inline T Sqrt( T value ) {
		return static_cast< T >( std::sqrt( value ) );
	}

	// 高速な平方根の計算を行います（√）
	// ※通常の平方根の計算よりも高速ですが精度が小数第4位程度までしか保証されていません
	template<class T>
	inline T FastSqrt( T value ) {

		//高速根号( ルート )計算：http://takashiijiri.com/study/miscs/fastsqrt.html

		float_t xHalf = 0.5f * value;
		int_t   tmp = 0x5F3759DF - ( *( int_t* )&value >> 1 ); //initial guess
		float_t xRes = *( float* )&tmp;

		xRes *= ( 1.5f - ( xHalf * xRes * xRes ) );
		xRes *= ( 1.5f - ( xHalf * xRes * xRes ) );//コメントアウトを外すと精度が上がる

		return xRes * value;

	}

#pragma endregion Sqrt

	// 絶対値
#pragma region Abs

	// 絶対値を求める
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr, std::enable_if_t<std::is_signed_v<T>>* = nullptr>
	inline T Abs( T value ) {
		return std::abs( value );
	}

	// 絶対値を求める
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr, std::enable_if_t<std::is_unsigned_v<T>>* = nullptr>
	inline T Abs( T value ) {
		// 符号なし整数型はそもそも絶対値なのでそのまま返す
		return value;
	}

	// 絶対値を求める
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline T Abs( T value ) {
		return static_cast< T >( std::abs( value ) );
	}

	// 絶対値を求める
	template<>
	inline float_t Abs( float_t value ) {
		return std::fabsf( value );
	}

#pragma endregion Abs

	// 大きさを比較して返す
#pragma region Min, Max

	// 数値の大きさを比較し、小さい方の数値を取得
	template<class T>
	inline T Min( T a, T b ) {
		return ( Math::LessEqual<T>( a, b ) ) ? a : b;
	}

	// 数値の大きさを比較し、大きい方の数値を取得
	template<class T>
	inline T Max( T a, T b ) {
		return ( Math::GreaterEqual<T>( a, b ) ) ? a : b;
	}

#pragma endregion Min, Max

	// 最大公約数・最小公倍数
#pragma region Gcd, Lcm

	// 二つの数の最大公約数を求める
	template<
		class T,
		class U,
		TypeTraitsEx::IsNonBoolArithmetic2<T,U> = nullptr
	>
	inline TypeTraitsEx::CommonType<T, U> Gcd( T a, U b ) {
		using commonType = TypeTraitsEx::CommonType<T, U>;
		auto mod = Math::Mod<commonType>( a, b );
		if( Math::Equal<commonType>(mod, 0) ) {
			return( b );
		}
		else {
			return( Math::Gcd( b, mod ) );
		}
	}

	// 二つの数の最小公倍数を求める
	template<
		class T,
		class U,
		TypeTraitsEx::IsNonBoolArithmetic2<T, U> = nullptr,
		TypeTraitsEx::IsIntegral2<T, U> = nullptr
	>
	inline ulonglong_t Lcm( T a, U b ) {
		a = Math::Abs( a );
		b = Math::Abs( b );
		return a * b / Math::Gcd<ulonglong_t>( a, b );
	}

	// 二つの数の最小公倍数を求める
	template<
		class T,
		class U,
		TypeTraitsEx::IsNonBoolArithmetic2<T, U> = nullptr,
		TypeTraitsEx::EitherIsFloatingPoint<T, U> = nullptr
	>
	inline long_double_t Lcm( T a, U b ) {

		long_double_t ad = Math::Abs(a);
		long_double_t bd = Math::Abs(b);

		// 浮動小数点以下の桁数が多い方の桁数を取得
		auto underDigit = Math::Max(
			Utility::UnderFloatDigit( ad ),
			Utility::UnderFloatDigit( bd )
		);

		// 浮動小数点以下の桁数分、10の累乗を行う
		auto pow10 = Math::Pow(
			10,
			underDigit
		);

		// 10の累乗倍する
		ad *= pow10;
		bd *= pow10;

		return Math::Lcm( 
			Math::FloorToInt<ulonglong_t>(ad), Math::FloorToInt<ulonglong_t>(bd) 
		) / static_cast< longlong_t >( pow10 );


		// 一応別verの処理をバックアップ
		{
			//long_double_t ad = Math::Abs( a );
			//long_double_t bd = Math::Abs( b );

			//// 浮動小数点以下の桁数が多い方の桁数を取得
			//auto underDigit = Math::Max(
			//	Utility::UnderFloatDigit( ad ),
			//	Utility::UnderFloatDigit( bd )
			//);

			//// 浮動小数点以下の桁数分、10の累乗を行う
			//auto pow10 = Math::Pow(
			//	10,
			//	underDigit
			//);

			//// 10の累乗倍する
			//ad *= pow10;
			//bd *= pow10;

			//// デフォルトの浮動小数点丸め方式を取得
			//auto defType = std::fegetround();
			//std::fesetround( FE_TONEAREST );

			//// 現在の丸め方式で整数型に変換
			//ad = std::nearbyintl( ad );
			//bd = std::nearbyintl( bd );

			//// 元の浮動小数点丸め方式に戻す
			//std::fesetround( defType );
			//return Math::Lcm( static_cast< ulonglong_t >( ad ), static_cast< ulonglong_t >( bd ) ) / static_cast< ulonglong_t >( pow10 );
		}

	}

#pragma endregion Gcd, Lcm

	// 角度の変換
#pragma region ConvertAngle

	// 度からラジアンに変換する
	template<class T>
	inline T ToRadian( T degree ) {
		return degree * Math::Deg2Rad<T>;
	}

	// ラジアンから度に変換する
	template<class T>
	inline T ToDegree( T radian ) {
		return radian * Math::Rad2Deg<T>;
	}

	// +180～-180で表現されている角度を360度で表現する
	template<class T>
	inline T ToAroundAngle( T degree ) {
		return ( Math::GreaterEqual<T>( degree, 0.0 ) ) ? degree : 360.0 + static_cast< double_t >( degree );
	}

	// 360度で表現されている角度を+180～-180で表現する
	template<class T>
	inline T ToHalfAngle( T degree ) {
		return ( Math::LessEqual<T>( degree, 180.0 ) ) ? degree : static_cast< double_t >( degree ) - 360.0;
	}

	// 角度を±0～+360度以内に制限する
	template<class T>
	inline T ClampAngle( T degree ) {
		if( Math::GreaterEqual<T>( degree, 0.0 ) ) {
			return ( Math::LessEqual( degree, 360.0 ) ) ? degree : static_cast< double_t >( degree ) - 360.0;
		}
		else {
			return 360.0 - static_cast< double_t >( degree );
		}
	}

	// x, yから角度を求める
	template<class T>
	inline T Atan2Angle( T x, T y ) {
		return Math::ToAroundAngle( Math::Atan2( y, x ) * Math::Rad2Deg<T> );
	}

#pragma endregion ConvertAngle

	// 符号判定を行い、+1・-1を返す関数
#pragma region Sign

	// 符号判定を行い、0以上なら+1を、0未満なら-1を返す
	template<class T>
	inline T Sign( T value ) {
		return Math::GreaterEqual<T>( value, static_cast<T>(0.0) ) ? +1 : -1;
	}

	// 符号判定を行い、0以上なら-1を、0未満なら+1を返す
	template<class T>
	inline T InverseSign( T value ) {
		return Math::GreaterEqual<T>( value, static_cast< T >( 0.0 ) ) ? -1 : +1;
	}

#pragma endregion Sign

	// ゼロかどうか判定しゼロならゼロを返す関数
#pragma region ZeroIfZero

	// ゼロかどうか判定しゼロならゼロを返し
	// ゼロではないなら元の値を返す
	template<class T>
	inline T ZeroIfZero( T value ) {
		return Math::IsZero<T>( value ) ? static_cast< T >( 0.0 ) : value;
	}

#pragma endregion ZeroIfZero

	// 線形補間関連（linear interpolation)
#pragma region Lerp

	// a～b間の線形補間を行います。
	// 戻り値：a～bの間の数値
	template<class T>
	inline T Lerp01( T a, T b, T t ) {
		return a + Math::Clamp01( t ) * ( b - a );
	}

	// a～b間の線形補間を行います。
	// デフォルト戻り値：a～bの間の数値　※Lerp01と同等処理
	// min max を設定した場合の戻り値：a～bの差分 * Math::Clamp(t, min, max);
	template<class T>
	inline T Lerp( T a, T b, T t, T min = 0.0, T max = 1.0 ) {
		return a + Math::Clamp(t, min, max) * ( b - a );
	}

	// a～b間の線形補間を行います
	// 戻り値：a～bの差分 * t
	template<class T>
	inline T LerpUnclamped( T a, T b, T t ) {
		return a + t * ( b - a );
	}

	// a～b間を0～1の間に線形補間を行います。
	// 戻り値 : 0～1の間の数値
	template<class T>
	inline T InverseLerp01( T a, T b, T value ) {
		return Math::Clamp01(( value - a ) / ( b - a ));
	}

	// a～b間を0～1の間に線形補間を行います。
	// デフォルト戻り値：0～1の間の数値
	// 戻り値 : min maxの間の数値
	template<class T>
	inline T InverseLerp( T a, T b, T value, T min = 0.0, T max = 1.0 ) {
		return Math::Clamp( ( value - a ) / ( b - a ), min, max );
	}

	// a～b間を0～1の間に線形補間を行います。
	// tに制限はありません
	// 戻り値 : 0～1の間の数値
	template<class T>
	inline T InverseLerpUnclamped( T a, T b, T value ) {
		return ( value - a ) / ( b - a );
	}

#pragma endregion Lerp

	// スムーズな補間を行う
#pragma region SmoothStep

	template<class T>
	inline T SmoothStep( T min, T max, T t ) {
		t = Math::Clamp01<T>( t );
		t = t * t * t;
		return ( max - min ) * t ;
	}

	template<class T>
	inline T SmoothStep01( T t ) {
		t = Math::Clamp01<T>(t);
		return t * t * t;
	}

#pragma endregion SmoothStep

	// この数値以上で最近傍の整数値で表現可能な数値を取得する
#pragma region Ceil

	// この数値以上で最近傍の整数値で表現可能な浮動小数点数値を取得する
	template<class T, TypeTraitsEx::IsFloatingPoint<T> = nullptr>
	inline T Ceil( T value ) {
		return std::ceil( static_cast< T >( value ) );
	}

	template<class T, TypeTraitsEx::IsNonBoolIntegral<T> = nullptr>
	inline T Ceil( T value ) {
		return std::ceil( static_cast< T >( value ) );
	}

	// この数値以上で最近傍の整数値を取得する
	template<
		class T = longlong_t,
		class U,
		TypeTraitsEx::IsNonBoolIntegral<T> = nullptr,
		TypeTraitsEx::IsFloatingPoint<U> = nullptr
	>
	inline T CeilToInt( U value ) {
		return std::ceil( static_cast< U >( value ) );
	}

	template<class T, TypeTraitsEx::IsNonBoolIntegral<T> = nullptr>
	inline T CeilToInt( T value ) {
		// そのまま返す
		return value;
	}

#pragma endregion Ceil

	// この数値以下で最近傍の整数値で表現可能な数値を取得する
#pragma region Floor

	// この数値以下で最近傍の整数値で表現可能な浮動小数点数地を取得する
	template<class T, TypeTraitsEx::IsFloatingPoint<T> = nullptr>
	inline T Floor( T value ) {
		return std::floor( static_cast< T >( value ) );
	}

	// この数値以下で最近傍の整数値で表現可能な浮動小数点数地を取得する
	template<class T, TypeTraitsEx::IsNonBoolIntegral<T> = nullptr>
	inline T Floor( T value ) {
		// そのまま返す
		return value;
	}

	// この数値以下で最近傍の整数値を取得する
	template<
		class T = longlong_t,
		class U,
		TypeTraitsEx::IsNonBoolIntegral<T> = nullptr,
		TypeTraitsEx::IsFloatingPoint<U> = nullptr
	>
	inline T FloorToInt( U value ) {
		return std::floor( static_cast< U >( value ) );
	}

	// この数値以下で最近傍の整数値を取得する
	template<class T, TypeTraitsEx::IsNonBoolIntegral<T> = nullptr>
	inline T FloorToInt( T value ) {
		// そのまま返す
		return value;
	}

#pragma endregion Floor

	// 数値を範囲内に制限する
#pragma region Clamp

	// std::clampがClmap01の際に例外を送出したので
	// 例外内容は評価不可能な数値が渡されたことによるものとのことですが、
	// 評価不可能な数値ではないので
	// 自前で両方とも再実装しました

	// min～maxの範囲に間に値を制限します
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T Clamp( T value, T min, T max ) {
		if( value <= min ) return min;
		if( value >= max ) return max;
		return value;
	}

	// 0.0～1.0の範囲に間に値を制限します　※飽和関数
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T Clamp01( T value ) {
		return Math::Clamp( value, static_cast< T >( 0.0 ), static_cast< T >( 1.0 ) );
	}

	// 0以上に値を制限します
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T ClampToPlus( T value ) {
		return Math::Clamp( value, static_cast< T >( 0.0 ), Math::MaxValue<T>() );
	}

	// 0未満に値を制限します
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline T ClampToMinus( T value ) {
		return Math::Clamp( value, Math::MinValue<T>(), -Math::Epsilon<T>() );
	}

	// 0未満に値を制限します
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline T ClampToMinus( T value ) {
		return Math::Clamp( value, Math::MinValue<T>(), -1 );
	}

	// min以上に値を制限します
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T ClampToMin( T value, T min ) {
		return Math::Clamp( value, min, Math::MaxValue<T>() );
	}

	// max以下に値を制限します
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T ClampToMax( T value, T max ) {
		return Math::Clamp( value, Math::MinValue<T>(), max );
	}

#pragma endregion Clamp

		// 四捨五入、切り上げ、切り捨て
#pragma region Round, RoundUp, RoundDown

	// 全てのRound系メソッドで同じアルゴリズムを使用する
	// 一部使用する関数によって内容が異なるためその部分に使用する関数は設定できるようにしてある
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline double _Internal_Round_Method( T value, int_t digit, std::function<double_t( double_t )> _roundFunc ) {

		if( Math::Sign( digit ) > 0 && digit != 0 ) {
			// 選択桁が浮動小数点以上の場合
			// 10の-n乗
			auto mul = std::pow( 10, -digit );
			return _roundFunc( value * mul ) / mul;
		}
		else if( Math::Sign( digit ) < 0 && digit != 0 ) {

#if _DEBUG

			try {
				// 浮動小数点型の場合のみ計算
				if( Math::IsFloat( value ) ) {
					// 選択桁が浮動小数点以下の場合
					// 10の+n乗
					auto mul = std::pow( 10, -digit );
					return _roundFunc( value * mul ) / mul;
				}
				else {
					throw std::runtime_error( "\
						Attempted to perform a floating - point_t calculation even though it is not a floating - point_t type\
						" );
				}
			}
			catch( const std::runtime_error& error ) {
				std::cout << error.what() << std::endl;
				// そのまま返す
				return value;
			}

#else

			// 浮動小数点型の場合のみ計算
			if( Math::IsFloat( value ) ) {
				// 選択桁が浮動小数点以下の場合
				// 10の+n乗
				auto mul = std::pow( 10, -digit );
				return _roundFunc( value * mul ) / mul;
			}
			else {
				// そのまま返す
				return value;
			}

#endif
		}
		else {
			// 選択桁が０の場合
			return _roundFunc( value );
		}

	}

	// 選択された桁で四捨五入を行う
	// value : 数値データ
	// digit : 取得する桁　※０は１の位の桁を指し、-１は浮動小数点以下第一位を指す
	// 桁指定がない場合はデフォルトで1の位を四捨五入する
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T Round( T value, int_t digit = 0 ) {
		return Math::_Internal_Round_Method( value, digit, []( double_t v ) {return std::round( v ); } );
	}

	// 選択された桁で切り上げを行う
	// value : 数値データ
	// digit : 取得する桁　※０は１の位の桁を指し、-１は浮動小数点以下第一位を指す
	// 桁指定がない場合はデフォルトで1の位を切り上げる
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T RoundUp( T value, int_t digit = 0 ) {
		return Math::_Internal_Round_Method( value, digit, []( double_t v ) {return std::ceil( v ); } );
	}

	// 選択された桁で切り捨てを行う
	// value : 数値データ
	// digit : 取得する桁　※０は１の位の桁を指し、-１は浮動小数点以下第一位を指す
	// 桁指定がない場合はデフォルトで1の位を切り捨てる
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T RoundDown( T value, int_t digit = 0 ) {
		return static_cast< T >( Math::_Internal_Round_Method( value, digit, []( double_t v ) {return std::floor( v ); } ) );
	}


#pragma endregion Round, RoundUp, RoundDown

	// 乱数を生成する
#pragma region Random

	// 乱数を生成する
	// min : 乱数の最小分布範囲
	// max : 乱数の最大分布範囲
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline T Random( T min, T max ) {

		// cppref:uniform_int_distribution：https://cpprefjp.github.io/reference/random/uniform_int_distribution.html

		// 64bit
#if _WIN64
		std::random_device random_device;						// 常にランダムなシード値を作成するデバイス
		std::mt19937_64 random_engine( random_device() );	// 64ビット版メルセンヌツイスター
		std::uniform_int_distribution<> dist( min, max );	// 整数型の指定範囲で一様分布を行う
#else
		std::random_device random_device;						// 常にランダムなシード値を作成するデバイス
		std::mt19937 random_engine( random_device() );		// 32ビット版メルセンヌツイスター
		std::uniform_int_distribution<> dist( min, max );	// 整数型の指定範囲で一様分布を行う
#endif

		return dist( random_engine );
	}

	// 乱数を生成する
	// min : 乱数の最小分布範囲
	// max : 乱数の最大分布範囲
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline T Random( T min, T max ) {

		// cppref:uniform_real_distribution：https://cpprefjp.github.io/reference/random/uniform_real_distribution.html

#if _WIN64
		std::random_device random_device;						// 常にランダムなシード値を作成するデバイス
		std::mt19937_64 random_engine( random_device() );	// 64ビット版メルセンヌツイスター
		std::uniform_real_distribution<> dist( min, max );	// 浮動小数点型の指定範囲で一様分布を行う
#else
		std::random_device random_device;						// 常にランダムなシード値を作成するデバイス
		std::mt19937 random_engine( random_device() );		// 32ビット版メルセンヌツイスター
		std::uniform_real_distribution<> dist( min, max );	// 浮動小数点型の指定範囲で一様分布を行う
#endif

		return dist( random_engine );
	}


	// xorshift128のアルゴリズムを使用した乱数生成器
	inline ulong_t RandomXorShift128() {

		// 以下のサイトにはxorshift128についての解説も載っている
		// 固定シード値の参考サイト：https://tech.ckme.co.jp/cpp/cpp_xorshift.shtml
		// 　可変シード値にする場合：http://hexadrive.sblo.jp/article/63660775.html

		// 固定シード値
		static ulong_t x = 123456789;
		static ulong_t y = 362436069;
		static ulong_t z = 521288629;
		static ulong_t w = 88675123;

		ulong_t t;
		t = ( x ^ ( x << 11 ) );
		x = y;
		y = z;
		z = w;

		return( w = ( w ^ ( w >> 19 ) ) ^ ( t ^ ( t >> 8 ) ) );
	}

	// xorshift128のアルゴリズムを使用し0.0～1.0の範囲で乱数を取得する
	inline double_t Random01XorShift128() {
		return static_cast< double_t >( Math::RandomXorShift128() ) / static_cast< double_t >( Math::MaxValue<ulong_t>() );
	}

	// 0.0～1.0の範囲で乱数を取得する
	// template param : <T = 浮動小数点型のデータ型, Resolution = 分解能>
	template<
		class T = double_t,
		std::size_t Resolution = std::numeric_limits<T>::digits,
		std::enable_if_t<std::is_floating_point_v<T>>* = nullptr
	>
		inline T Random01() {

			// cppref_generate_canonical : https://cpprefjp.github.io/reference/random/generate_canonical.html

#if _WIN64
		std::random_device random_device;
		std::mt19937_64 random_engine( random_device() );
#else
		std::random_device random_device;
		std::mt19937 random_engine( random_device() );
#endif
		return std::generate_canonical<T, Resolution>( random_engine );
	}



#pragma endregion Random

	// パーリンノイズを生成する
#pragma region PerlinNoise

	template<class T = double_t>
	class _PerlinNoiseClass {

	private:

		//メンバ変数
		using Pint = std::uint_fast8_t;
		std::array<Pint, 512> p { {} };
	public:

		constexpr _PerlinNoiseClass() = default;
		explicit _PerlinNoiseClass( const std::uint_fast32_t seed_ ) {
			this->setSeed( seed_ );
		}
	private:

		constexpr T getFade( const T t_ ) const noexcept {
			return t_ * t_ * t_ * ( t_ * ( t_ * 6 - 15 ) + 10 );
		}

		constexpr T getLerp( const T t_, const T a_, const T b_ ) const noexcept {
			return a_ + t_ * ( b_ - a_ );
		}

		constexpr T makeGrad( const Pint hash_, const T u_, const T v_ ) const noexcept {
			return ( ( ( hash_ & 1 ) == 0 ) ? u_ : -u_ ) + ( ( ( hash_ & 2 ) == 0 ) ? v_ : -v_ );
		}

		constexpr T makeGrad( const Pint hash_, const T x_, const T y_, const T z_ ) const noexcept {
			return this->makeGrad( hash_, hash_ < 8 ? x_ : y_, hash_ < 4 ? y_ : hash_ == 12 || hash_ == 14 ? x_ : z_ );
		}

		constexpr T getGrad( const Pint hash_, const T x_, const T y_, const T z_ ) const noexcept {
			return this->makeGrad( hash_ & 15, x_, y_, z_ );
		}

		T setNoise( T x_ = 0.0, T y_ = 0.0, T z_ = 0.0 ) const noexcept {
			const std::size_t x_int { static_cast< std::size_t >( static_cast< std::size_t >( std::floor( x_ ) ) & 255 ) };
			const std::size_t y_int { static_cast< std::size_t >( static_cast< std::size_t >( std::floor( y_ ) ) & 255 ) };
			const std::size_t z_int { static_cast< std::size_t >( static_cast< std::size_t >( std::floor( z_ ) ) & 255 ) };
			x_ -= std::floor( x_ );
			y_ -= std::floor( y_ );
			z_ -= std::floor( z_ );
			const T u { this->getFade( x_ ) };
			const T v { this->getFade( y_ ) };
			const T w { this->getFade( z_ ) };
			const std::size_t a0 { static_cast< std::size_t >( this->p[x_int] + y_int ) };
			const std::size_t a1 { static_cast< std::size_t >( this->p[a0] + z_int ) };
			const std::size_t a2 { static_cast< std::size_t >( this->p[a0 + 1] + z_int ) };
			const std::size_t b0 { static_cast< std::size_t >( this->p[x_int + 1] + y_int ) };
			const std::size_t b1 { static_cast< std::size_t >( this->p[b0] + z_int ) };
			const std::size_t b2 { static_cast< std::size_t >( this->p[b0 + 1] + z_int ) };

			return this->getLerp(
				w,
				this->getLerp( v,
					this->getLerp( u, this->getGrad( this->p[a1], x_, y_, z_ ), this->getGrad( this->p[b1], x_ - 1, y_, z_ ) ),
					this->getLerp( u, this->getGrad( this->p[a2], x_, y_ - 1, z_ ), this->getGrad( this->p[b2], x_ - 1, y_ - 1, z_ ) )
				),
				this->getLerp( v,
					this->getLerp( u, this->getGrad( this->p[a1 + 1], x_, y_, z_ - 1 ), this->getGrad( this->p[b1 + 1], x_ - 1, y_, z_ - 1 ) ),
					this->getLerp( u, this->getGrad( this->p[a2 + 1], x_, y_ - 1, z_ - 1 ), this->getGrad( this->p[b2 + 1], x_ - 1, y_ - 1, z_ - 1 ) )
				)
			);
		}
	public:

		//SEED値を設定する
		void setSeed( const std::uint_fast32_t seed_ ) {
			for( std::size_t i {}; i < 256; ++i ) {
				this->p[i] = static_cast< Pint >( i );
			}

			//p[0～255]の範囲をシャッフル
			std::shuffle(
				this->p.begin(),
				this->p.begin() + 256,
				std::default_random_engine( seed_ )
			);

			for( std::size_t i {}; i < 256; ++i ) {
				this->p[256 + i] = this->p[i];
			}
		}

		T setOctaveNoise( const std::size_t octaves_, T x_ ) const noexcept {
			T noise_value {};
			T amp{ 1.0 };
			for( std::size_t i {}; i < octaves_; ++i ) {
				noise_value += this->setNoise( x_ ) * amp;
				x_ *= 2.0;
				amp *= 0.5;
			}
			return noise_value;
		}
		T setOctaveNoise( const std::size_t octaves_, T x_, T y_ ) const noexcept {
			T noise_value {};
			T amp{ 1.0 };
			for( std::size_t i {}; i < octaves_; ++i ) {
				noise_value += this->setNoise( x_, y_ ) * amp;
				x_ *= 2.0;
				y_ *= 2.0;
				amp *= 0.5;
			}
			return noise_value;
		}
		T setOctaveNoise( const std::size_t octaves_, T x_, T y_, T z_ ) const noexcept {
			T noise_value {};
			T amp{ 1.0 };
			for( std::size_t i {}; i < octaves_; ++i ) {
				noise_value += this->setNoise( x_, y_, z_ ) * amp;
				x_ *= 2.0;
				y_ *= 2.0;
				z_ *= 2.0;
				amp *= 0.5;
			}
			return noise_value;
		}


		//オクターブ無しノイズを取得する
		template <typename... Args>
		T noise( const Args... args_ ) const noexcept {
			return this->setNoise( args_... ) * 0.5 + 0.5;
		}

		//オクターブ有りノイズを取得する
		template <typename... Args>
		T octaveNoise( const std::size_t octaves_, const Args... args_ ) const noexcept {
			return this->setOctaveNoise( octaves_, args_... ) * 0.5 + 0.5;
		}

	};

	// パーリンノイズを使用する　※範囲は0.0～1.0の範囲で指定する
	inline double_t PerlinNoise( double_t x = 0.0, double_t y = 0.0, double_t z = 0.0 ) {
		static const _PerlinNoiseClass<double_t> perlin( Math::Random<uint_t>( 0, 10000 ) ); // シード値をランダムに設定
		return perlin.noise(
			Math::Clamp01( x ),
			Math::Clamp01( y ),
			Math::Clamp01( z )
		);
	}

	inline double_t PerlinNoiseOctave( const std::size_t octaves, double_t x = 0.0, double_t y = 0.0, double_t z = 0.0 ) {
		static const _PerlinNoiseClass<double_t> perlin( Math::Random<uint_t>( 0, 10000 ) ); // シード値をランダムに設定
		return perlin.octaveNoise(
			octaves,
			Math::Clamp01( x ),
			Math::Clamp01( y ),
			Math::Clamp01( z )
		);
	}

#pragma endregion PerlinNoise

//===================================================
// 判定メソッド
//===================================================

#pragma region JudghMethod

	// この数値が正の無限大・負の無限大かを判定する
	// ※正負どちらかの無限大ならtrue
	template<class T>
	inline bool IsInfinity( T value ) {
		return std::isinf( value );
	}

	// この数値がNaN（Not a Number）かを判定する
	// ※NaNならtrue
	template<class T>
	inline bool IsNaN( T value ) {
		return std::isnan( value );
	}

	// この数値が0に近似しているか判定する
	// ゼロならtrue
	template<class T>
	inline bool IsZero( T value, T epsilon = Math::Epsilon<T>() ) {
		return Math::Equal<T>( value, static_cast< T >( 0.0 ), epsilon );
	}

	// この数値が有限な数値かを判定する
	// ※正負どちらの無限大でもなく、NaN（Not a Number）でもない場合true
	template<class T>
	inline bool IsFinite( T value ) {
		return std::isfinite( value );
	}

	// この数値が偶数かを判定する
	// 偶数ならtrue
	// ※0も偶数のためtrueとなる：詳しくは「0の偶奇性」で検索
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline bool IsEven( T value ) {
		return ( value % 2 == 0 );
	}

	// この数値が奇数かを判定する
	// 奇数ならtrue
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline bool IsOddnum( T value ) {
		return ( value % 2 == 1 );
	}

	// 数値型かどうかを判定する
	// 数値型ならtrue
	template<class T>
	inline bool IsNumber( T value ) {
		return ( std::is_arithmetic_v<T> ) ? true : false;
	}

	// この数値が整数型かどうかを判定する
	// 整数型ならtrue
	template<class T>
	inline bool IsInt( T value ) {
		return ( std::is_integral_v<T> ) ? true : false;
	}

	// この数値が浮動小数点型かどうかを判定する
	// 浮動小数点型ならtrue
	template<class T>
	inline bool IsFloat( T value ) {
		return ( std::is_floating_point_v<std::remove_reference_t<T>> ) ? true : false;
	}

	// この数値がある数の倍数かどうかを判定する
	// valueがmulBaseの倍数ならtrue
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline bool IsMultiply( T value, T mulBase ) {
		return ( mulBase % value == 0 ) ? true : false;
	}

	// この数値がある数の倍数かどうかを判定する
	// valueがmulBaseの倍数ならtrue
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline bool IsMultiply( T value, T mulBase ) {
		return ( Math::Mod( value, mulBase ) == 0 ) ? true : false;
	}

	// この数値が正の数かどうかを判定する　※0も正の数に入る
	// 数値が正の数ならtrue
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool IsPlus( T value ) {
		return ( Math::Sign( value ) > 0 );
	}

	// この数値が負の数かどうかを判定する　※0の場合は正の数に入るためfalseとなる
	// 数値が負の数ならtrue
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool IsMinus( T value ) {
		return ( Math::Sign( value ) < 0 );
	}


#pragma endregion JudghMethod

//===================================================
// 数値比較
//===================================================

	// 比較を行う
#pragma region Compare

	// 誤差を考慮した等値判定（=）
	// aとbの誤差がepsilon以下なら同じ
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool Equal( T a, T b, T epsilon = Math::Epsilon<T>() ) {
		return ( Math::Abs( a - b ) <= epsilon );
	}

	// 誤差を考慮した非等値判定（!=）
	// aとbの誤差がepsilon以下なら同じ
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool NotEqual( T a, T b, T epsilon = Math::Epsilon<T>() ) {
		return !( Math::Abs( a - b ) <= epsilon );
	}

	// 誤差を考慮した大なり判定（>）
	// aとbの誤差がepsilon以上かつ、aの方が大きいならtrue
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool Greater( T a, T b, T epsilon = Math::Epsilon<T>() ) {
		return ( a > b && Math::NotEqual( a, b, epsilon ) );
	}

	// 誤差を考慮した大なりイコール判定（>=）
	// aとbの誤差がepsilon以下かつ、aの方が大きい・同値ならtrue
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool GreaterEqual( T a, T b, T epsilon = Math::Epsilon<T>() ) {
		return ( a >= b || Math::Equal( a, b, epsilon ) );
	}

	// 誤差を考慮した小なり判定（<）
	// aとbの誤差がepsilon以上かつ、aの方が小さいならtrue
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool Less( T a, T b, T epsilon = Math::Epsilon<T>() ) {
		return ( a < b && Math::NotEqual( a, b, epsilon ) );
	}

	// 誤差を考慮した小なりイコール判定（<=）
	// aとbの誤差がepsilon以下かつ、aの方が小さい・同値ならtrue
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool LessEqual( T a, T b, T epsilon = Math::Epsilon<T>() ) {
		return ( a <= b || Math::Equal( a, b, epsilon ) );
	}

	// 誤差を考慮した大なり小なり判定
	// selfがminより大きくmaxより小さいかを判定する
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool GreaterLess(T min, T self, T max, T epsilon = Math::Epsilon<T>()) {
		return (Math::Less(min, self, epsilon) && Math::Less(self, max, epsilon));
	}

	// 誤差を考慮した大なりイコール小なりイコール判定
	// selfがmin以上でmax以下かを判定する
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool GreaterLessEqual(T min, T self, T max, T epsilon = Math::Epsilon<T>()) {
		return (Math::LessEqual(min, self, epsilon) && Math::LessEqual(self, max, epsilon));
	}

#pragma endregion Compare

//===================================================
// 基本的な数値計算
//===================================================

	// 基本的な数値計算
#pragma region Basic Numerical Calculations

	// 加算
	// 戻り値：a + b
	template<class T>
	inline T Add( T a, T b ) {
		return a + b;
	}

	// 減算
	// 戻り値：a - b
	template<class T>
	inline T Sub( T a, T b ) {
		return a - b;
	}

	// 乗算
	// 戻り値：a * b
	template<class T>
	inline T Mul( T a, T b ) {
		return a * b;
	}

	// 除算
	// 戻り値：a / b
	template<class T>
	inline T Div( T a, T b ) {
		return a / b;
	}

#pragma endregion Basic Numerical Calculations

	// ベクトル・行列などで使用される汎用的なクラス・メソッド
#pragma region VecMathUtil

	// 回転順番を定義する列挙定数
	enum class RotationOrder {
		XYZ,
		XZY,
		YXZ,
		YZX,
		ZXY,
		ZYX
	};

#pragma endregion VecMathUtil


	// ベクトルライブラリ
#pragma region VectorLib

	// Reference
	// 参考サイト
	// https://zenn.dev/ryutorion/articles/game-math-vector

	// ベクトルライブラリ
	namespace Vector {

		// 前方宣言
		template<class T>
		class Vector1t;
		template<class T>
		class Vector2t;
		template<class T>
		class Vector3t;
		template<class T>
		class Vector4t;

		// 一次元のベクトルを表すデータ型
		template<class T>
		class Vector1t {

		public:

			// x要素のデータ型
			using type = T;

		public:

#pragma region member

			T x;

#pragma endregion member

		public:

#pragma region constructor

			Vector1t() : x( 0 ) {};
			Vector1t( const Vector1t& vec1 ) {
				x = vec1.x;
			}
			Vector1t( Vector1t&& vec1 ) {
				x = vec1.x;
			}

			Vector1t( T x ) : x( x ) {};

			// 型変換を行い要素を構築する（型変換機能付きコンストラクタ）
			template<class U>
			Vector1t( Vector1t<U> const& vec1 ) {
				x = static_cast< T >( vec1.x );
			}

#pragma endregion constructor

		public:

#pragma region operator

		//===========================
		// 代入演算子
		//===========================

			// 型変換を行い代入する
			template<class U>
			Vector1t& operator=( Vector1t<U> const& vec1 ) noexcept {
				x = static_cast< T >( vec1.x );
				return *this;
			}

			Vector1t& operator=( const Vector1t& vec1 ) noexcept {
				x = vec1.x;
				return *this;
			}

			Vector1t& operator=( T scalar ) noexcept {
				x = scalar;
				return *this;
			}

		//===========================
		// ()演算子　※数値を設定する
		//===========================

			Vector1t& operator()( T scalar ) noexcept {
				x = scalar;
				return *this;
			}

			Vector1t& operator()( const Vector1t& vec1 ) noexcept {
				x = vec1.x;
				return *this;
			}

		//===========================
		// 単項演算子
		//===========================

			Vector1t operator+() const noexcept {
				return *this;
			}

			Vector1t operator-() const noexcept {
				return Vector1t( -x );
			}

		//===========================
		// 算術演算子
		//===========================

			Vector1t operator+( T scalar ) const noexcept {
				return Vector1t( x + scalar );
			}

			Vector1t operator+( const Vector1t& vec1 ) const noexcept {
				return Vector1t( x + vec1.x );
			}

			Vector1t operator-( T scalar ) const noexcept {
				return Vector1t( x - scalar );
			}

			Vector1t operator-( const Vector1t& vec1 ) const noexcept {
				return Vector1t( x - vec1.x );
			}

			Vector1t operator*( T scalar ) const noexcept {
				return Vector1t( x * scalar );
			}

			Vector1t operator*( const Vector1t& vec1 ) {
				return Vector1t( x * vec1.x );
			}

			Vector1t operator/( T scalar ) const noexcept {
				return Vector1t( x / scalar );
			}

			Vector1t operator/( const Vector1t& vec1 ) const noexcept {
				return Vector1t( x / vec1.x );
			}

		//===========================
		// 複合代入演算子
		//===========================

			Vector1t& operator+=( T scalar ) noexcept {
				*this = *this + scalar;
				return *this;
			}

			Vector1t& operator+=( const Vector1t& vec1 ) noexcept {
				*this = *this + vec1;
				return *this;
			}

			Vector1t& operator-=( T scalar ) noexcept {
				*this = *this - scalar;
				return *this;
			}

			Vector1t& operator-=( const Vector1t& vec1 ) noexcept {
				*this = *this - vec1;
				return *this;
			}

			Vector1t& operator*=( T scalar )noexcept {
				*this = *this * scalar;
				return *this;
			}

			Vector1t& operator*=( const Vector1t& vec1 ) noexcept {
				*this = *this * vec1;
				return *this;
			}

			Vector1t& operator/=( T scalar )noexcept {
				*this = *this / scalar;
				return *this;
			}

			Vector1t& operator/=( const Vector1t& vec1 ) noexcept {
				*this = *this / vec1;
				return *this;
			}

		//===========================
		// 比較演算子
		//===========================

			bool operator==( const Vector1t& vec1 ) const noexcept {
				return Equal<T>(vec1);
			}

			bool operator!=( const Vector1t& vec1 ) const noexcept {
				return NotEqual<T>(vec1);
			}

			//===========================
			// ストリーム出力
			//===========================

			friend std::ostream& operator<<( std::ostream& stream, const Vector1t& vec1 ) {
				stream << vec1.ToString();
				return stream;
			}

#pragma endregion operator

		public:

#pragma region method

			// 全ての成分に同一の値を設定
			Vector1t& Set( T scalar ) noexcept {
				x = scalar;
				return *this;
			}

			// 全ての成分に同一の値を加算
			Vector1t& SetRelative(T diff) noexcept {
				x += diff;
				return *this;
			}

			// 逆ベクトルを取得
			Vector1t& Inverse() noexcept {
				*this = -*this;
				return *this;
			}

			// このベクトルからtoVectorまでのベクトルを求める
			Vector1t GoToVector( const Vector1t& toVector ) const noexcept {
				return toVector - *this;
			}

			// データ型を変換したVector1t型を返す
			template<class U>
			Vector1t<U> TypeConvert() const noexcept {
				return Vector1t<U>( *this );
			}

			// 使用されているデータ型名を取得
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// ベクトルの値を文字列に変換
			std::string ToString() const noexcept {
				return std::string(
					"Vector1( "
					+
					Utility::ToString( x )
					+
					" )"
				);
			}

			// このベクトルの成分内にNaNが入っているかを調べる
			bool HasNaN() const noexcept {
				return Math::IsNaN(x);
			}

			// このベクトルの成分内に正の無限大/負の無限大が入っているかを調べる
			bool HasInfinity() const noexcept {
				return Math::IsInfinity(x);
			}

			// 与えられたベクトルがこのベクトルと正確に等しい場合、trueを返します
			template<class U = T>
			bool Equal( const Vector1t& vec1, U epsilon = 0.001 ) const noexcept {
				return Math::Equal<U>(x, vec1.x, epsilon);
			}

			// 与えられたベクトルがこのベクトルと正確に等しくない場合、trueを返します
			template<class U = T>
			bool NotEqual(const Vector1t& vec1, U epsilon = 0.001) const noexcept {
				return Math::NotEqual<U>(x, vec1.x, epsilon);
			}

			// 自身のデータ型と比較
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// このクラスのメモリサイズを返す
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

			// シリアライズを行う
			void Serialize( std::string filePath ) const noexcept {
				FileUtility::Text::LoadAndWrite(
					{
						"Type : ", TypeName(), "\n",
						ToString()
					},
					filePath
				);
			}

#pragma endregion method

		};

		// 二次元ベクトルを表すデータ型
		template<class T>
		class Vector2t {

		public:

			// x, y要素のデータ型
			using type = T;

		public:

#pragma region member

			union {
				struct {
					T x;
					T y;
				};
				struct {
					std::array<T, 2> vec;
				};
			};

#pragma endregion member
			
		public:

#pragma region constructor

			Vector2t() : x( 0 ), y( 0 ) {};
			Vector2t( const Vector2t& vec2 ) {
				vec = vec2.vec;
			};
			Vector2t( Vector2t&& vec2 ) {
				vec = vec2.vec;
			}
			Vector2t( T x, T y ) : x( x ), y( y ) {};

			// 型変換を行い要素を構築する（型変換機能付きコンストラクタ）
			template<class U>
			Vector2t( Vector2t<U> const& vec2 ) {
				x = static_cast< T >( vec2.x );
				y = static_cast< T >( vec2.y );
			}

#pragma endregion constructor

		public:

#pragma region operator

		//===========================
		// 代入演算子
		//===========================

			// 型変換を行い代入する
			template<class U>
			Vector2t& operator=( Vector2t<U> const& vec2 ) noexcept {
				x = static_cast< T >( vec2.x );
				y = static_cast< T >( vec2.y );
				return *this;
			}

			Vector2t& operator=( const Vector2t& vec2 ) noexcept {
				vec = vec2.vec;
				return *this;
			}

			Vector2t& operator=( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				return *this;
			}

		//===========================
		// ()演算子　※数値を設定する
		//===========================

			Vector2t& operator()( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				return *this;
			}

			Vector2t& operator()( T newX, T newY ) noexcept {
				x = newX;
				y = newY;
				return *this;
			}

			Vector2t& operator()( const Vector2t& vec2 ) noexcept {
				x = vec2.x;
				y = vec2.y;
				return *this;
			}

		//===========================
		// []演算子　※[0]、[1] を使用して x や y 成分にアクセスします
		//===========================

			T& operator[]( int_t i ) {
				return vec[i];
			}

		//===========================
		// 単項演算子
		//===========================

			Vector2t operator+() const noexcept {
				return *this;
			}

			Vector2t operator-() const noexcept {
				return Vector2t( -x, -y );
			}

		//===========================
		// 算術演算子
		//===========================

			Vector2t operator+( T scalar ) const noexcept {
				return Vector2t( x + scalar, y + scalar );
			}

			Vector2t operator+( const Vector2t& vec2 ) const noexcept {
				return Vector2t( x + vec2.x, y + vec2.y );
			}

			Vector2t operator-( T scalar ) const noexcept {
				return Vector2t( x - scalar, y - scalar );
			}

			Vector2t operator-( const Vector2t& vec2 ) const noexcept {
				return Vector2t( x - vec2.x, y - vec2.y );
			}

			Vector2t operator*( T scalar ) const noexcept {
				return Vector2t( x * scalar, y * scalar );
			}

			Vector2t operator*( const Vector2t& vec2 ) {
				return Vector2t( x * vec2.x, y * vec2.y );
			}

			Vector2t operator/( T scalar ) const noexcept {
				return Vector2t( x / scalar, y / scalar );
			}

			Vector2t operator/( const Vector2t& vec2 ) const noexcept {
				return Vector2t( x / vec2.x, y / vec2.y );
			}

		//===========================
		// 複合代入演算子
		//===========================

			Vector2t& operator+=( T scalar ) noexcept {
				*this = *this + scalar;
				return *this;
			}

			Vector2t& operator+=( const Vector2t& vec2 ) noexcept {
				*this = *this + vec2;
				return *this;
			}

			Vector2t& operator-=( T scalar ) noexcept {
				*this = *this - scalar;
				return *this;
			}

			Vector2t& operator-=( const Vector2t& vec2 ) noexcept {
				*this = *this - vec2;
				return *this;
			}

			Vector2t& operator*=( T scalar )noexcept {
				*this = *this * scalar;
				return *this;
			}

			Vector2t& operator*=( const Vector2t& vec2 ) noexcept {
				*this = *this * vec2;
				return *this;
			}

			Vector2t& operator/=( T scalar )noexcept  {
				*this = *this / scalar;
				return *this;
			}

			Vector2t& operator/=( const Vector2t& vec2 ) noexcept {
				*this = *this / vec2;
				return *this;
			}

		//===========================
		// 比較演算子
		//===========================

			bool operator==( const Vector2t& vec2 ) const noexcept {
				return Equal<T>(vec2);
			}

			bool operator!=( const Vector2t& vec2 ) const noexcept {
				return NotEqual<T>(vec2);
			}

			//===========================
			// ストリーム出力
			//===========================

			friend std::ostream& operator<<( std::ostream& stream, const Vector2t& vec2 ) {
				stream << vec2.ToString();
				return stream;
			}

#pragma endregion operator

		public:

#pragma region static param

			// Vector2( 0.0, 1.0 )
			static constexpr Vector2t Up() noexcept {
				return Vector2t( 0.0, 1.0 );
			};
			// Vector2( 0.0, -1.0 )
			static constexpr Vector2t Down() noexcept {
				return Vector2t( 0.0, -1.0 );
			};
			// Vector2( 1.0, 0.0 )
			static constexpr Vector2t Right() noexcept {
				return Vector2t( 1.0, 0.0 );
			};
			// Vector2( -1.0, 0.0 )
			static constexpr Vector2t Left() noexcept {
				return Vector2t( -1.0, 0.0 );
			};
			// Vector2( 0.0, 0.0 )
			static constexpr Vector2t Zero() noexcept {
				return Vector2t( 0.0, 0.0 );
			}
			// Vector2( 1.0, 1.0 )
			static constexpr Vector2t One() noexcept {
				return Vector2t( 1.0, 1.0 );
			}
			// Vector2( Infinity, Infinity )
			static constexpr Vector2t PositiveInfinity() noexcept {
				return Vector2t( Math::Infinity<T>(), Math::Infinity<T>() );
			}
			// Vector2( -Infinity, -Infinity )
			static constexpr Vector2t NegativeInfinity() noexcept {
				return Vector2t( Math::negativeInfinity<T>(), Math::negativeInfinity<T>() );
			}
			// NaNベクトル　※主にエラー値として返す
			static constexpr Vector2t NaN() noexcept {
				return Vector2t( Math::NaN<T>(), Math::NaN<T>() );
			}

#pragma endregion static param

		public:

#pragma region static method

			//static T Length( Vector2t vec2 ) {

			//}

			static Vector2t Lerp( const Vector2t& a, const Vector2t& b, T t ) noexcept {
				// a から b までのベクトルのスカラー倍したものをaベクトルに加算する
				return a + ( a.GoToVector( b ) * Math::Clamp01<T>( t ) );
			}

			static Vector2t LerpUnclamped( const Vector2t& a, const Vector2t& b, T t ) noexcept {
				// a から b までのベクトルのスカラー倍したものをaベクトルに加算する
				return a + ( a.GoToVector( b ) * t );
			}

			static void Serialize( const Vector2t& vec2, std::string filePath ) noexcept {
				FileUtility::Text::LoadAndWrite(
					{
						"Type : ", vec2.TypeName(), "\n",
						vec2.ToString()
					},
					filePath
				);
			}

#pragma endregion static method

		public:

#pragma region method

			// 全ての成分に同一の値を設定
			Vector2t& Set( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				return *this;
			}

			// 各軸の成分ごとに値を設定
			Vector2t& Set( T newX, T newY ) noexcept {
				x = newX;
				y = newY;
				return *this;
			}

			// ベクトルの値をコピー
			Vector2t& Set( const Vector2t& vec2 ) noexcept {
				x = vec2.x;
				y = vec2.y;
				return *this;
			}

			// 全ての成分に同一の値を加算
			Vector2t& SetRelative( T diff ) noexcept {
				x += diff;
				y += diff;
				return *this;
			}

			// 各軸の成分ごとに値を加算
			Vector2t& SetRelative( T diffX, T diffY ) noexcept {
				x += diffX;
				y += diffY;
				return *this;
			}

			// ベクトルの値を加算
			Vector2t& SetRelative( const Vector2t& vec2 ) noexcept {
				x += vec2.x;
				y += vec2.y;
				return *this;
			}

			// ベクトルの長さを取得
			T Length() const noexcept {
				return Math::FastSqrt( SqrLength() );
			}

			// ベクトルの長さの2乗を取得
			T SqrLength() const noexcept {
				return ( x * x + y * y );
			}

			// ベクトル同士の距離を取得
			T Distance( const Vector2t& vec2 ) const noexcept {
				return GoToVector( vec2 ).Length();
			}

			// ベクトル同士の距離の2乗を取得
			T SqrDistance( const Vector2t& vec2 ) const noexcept {
				return GoToVector( vec2 ).SqrLength();
			}

			// 逆ベクトルを取得
			Vector2t& Inverse() noexcept {
				*this = -*this;
				return *this;
			}

			Vector2t InverseVector() const noexcept {
				return -*this;
			}

			// このベクトルを正規化する　※このベクトルが変化する
			Vector2t& Normalize() noexcept {
				*this /= Length();
				return *this;
			}

			// このベクトルを正規化した結果を返す　※このベクトル自体は変化しない
			Vector2t NormalizedVector() const noexcept {
				return *this / Length();
			}

			// このベクトルとのドット積の結果を返す
			T Dot( const Vector2t& vec2 ) const noexcept {
				return ( x * vec2.x + y * vec2.y );
			}

			// このベクトルとのクロス積の結果を返す
			T Cross( const Vector2t& vec2 ) const noexcept {
				return x * vec2.y - y * vec2.x;
			}

			// このベクトルとの間の角度を求める
			T Angle( const Vector2t& vec2 ) {
				return Math::Acos( Dot( vec2 ) );
			}

			// このベクトルからtoVectorまでのベクトルを求める
			Vector2t GoToVector( const Vector2t& toVector ) const noexcept {
				return toVector - *this;
			}

			// Vector3t<U>型に変換する
			template<class U = T>
			Math::Vector::Vector3t<U> ToVector3() const noexcept {
				return Math::Vector::Vector3t<U>(
					x,
					y,
					0
					);
			}

			// データ型を変換したVector2t型を返す
			template<class U>
			Vector2t<U> TypeConvert() const noexcept {
				return Vector2t<U>( *this );
			}

			// 使用されているデータ型名を取得
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// ベクトルの値を文字列に変換
			std::string ToString() const noexcept {
				return std::string(
					"Vector2( "
					+
					Utility::ToString( x )
					+
					", "
					+
					Utility::ToString( y )
					+
					" )"
				);
			}

			// このベクトルの成分内にNaNが入っているかを調べる
			bool HasNaN() const noexcept {
				for (int_t i = 0; i < 2; i++) {
					if (Math::IsNaN(vec[i])) return true;
				}
				return false;
			}

			// このベクトルの成分内に正の無限大/負の無限大が入っているかを調べる
			bool HasInfinity() const noexcept {
				for (int_t i = 0; i < 2; i++) {
					if (Math::IsInfinity(vec[i])) return true;
				}
				return false;
			}

			// 与えられたベクトルがこのベクトルと正確に等しい場合、trueを返します
			template<class U = T>
			bool Equal( const Vector2t& vec2, U epsilon = 0.001) const noexcept {
				for (int_t i = 0; i < 2; i++) {
					if (Math::NotEqual<U>(vec[i], vec2.vec[i], epsilon)) {
						return false;
					}
				}
				return true;
			}

			// 与えられたベクトルがこのベクトルと正確に等しくない場合、trueを返します
			template<class U = T>
			bool NotEqual(const Vector2t& vec2, U epsilon = 0.001) const noexcept {
				for (int_t i = 0; i < 2; i++) {
					if (Math::NotEqual<U>(vec[i], vec2.vec[i], epsilon)) {
						return true;
					}
				}
				return false;
			}

			// 自身のデータ型と比較
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// このクラスのメモリサイズを返す
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

			// シリアライズを行う
			void Serialize( std::string filePath ) const noexcept {
				FileUtility::Text::LoadAndWrite(
					{
						"Type : ", TypeName(), "\n",
						ToString()
					},
					filePath
				);
			}

#pragma endregion method

		};

		// 三次元ベクトルを表すデータ型
		template<class T>
		class Vector3t {

		public:

			// x, y, z要素のデータ型
			using type = T;

		public:

#pragma region member

			union {
				struct {
					T x;
					T y;
					T z;
				};
				struct {
					std::array<T, 3> vec;
				};
			};

#pragma endregion member

		public:

#pragma region constructor

			Vector3t() : x(0), y(0), z(0) {};
			Vector3t( const Vector3t& vec3 ) {
				vec = vec3.vec;
			};
			Vector3t( Vector3t&& vec3 ) {
				vec = vec3.vec;
			}
			Vector3t( T x, T y, T z ) : x( x ), y( y ), z( z ) {};

			// 型変換を行い要素を構築する（型変換機能付きコンストラクタ）
			template<class U>
			Vector3t(Vector3t<U> const& vec3 ) {
				x = static_cast< T >( vec3.x );
				y = static_cast< T >( vec3.y );
				z = static_cast< T >( vec3.z );
			}

#pragma endregion constructor
			
		public:

#pragma region operator

		//===========================
		// 代入演算子
		//===========================

			// 型変換を行い代入する
			template<class U>
			Vector3t& operator=( Vector3t<U> const& vec3 ) noexcept {
				x = static_cast< T >( vec3.x );
				y = static_cast< T >( vec3.y );
				z = static_cast< T >( vec3.z );
				return *this;
			}

			Vector3t& operator=( const Vector3t& vec3 ) noexcept {
				vec = vec3.vec;
				return *this;
			}

			Vector3t& operator=( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				z = scalar;
				return *this;
			}

		//===========================
		// ()演算子　※数値を設定する
		//===========================

			Vector3t& operator()( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				z = scalar;
				return *this;
			}

			Vector3t& operator()( T newX, T newY, T newZ ) noexcept {
				x = newX;
				y = newY;
				z = newZ;
				return *this;
			}

			Vector3t& operator()( const Vector3t& vec3 ) noexcept {
				x = vec3.x;
				y = vec3.y;
				z = vec3.z;
				return *this;
			}

		//===========================
		// []演算子　※[0]、[1]、[2] を使用して x や y や z 成分にアクセスします
		//===========================

			T& operator[]( int_t i ) {
				return vec[i];
			}

		//===========================
		// 単項演算子
		//===========================

			Vector3t operator+() const noexcept {
				return *this;
			}

			Vector3t operator-() const noexcept {
				return Vector3t( -x, -y, -z );
			}

		//===========================
		// 算術演算子
		//===========================

			Vector3t operator+( T scalar ) const noexcept {
				return Vector3t( x + scalar, y + scalar, z + scalar );
			}

			Vector3t operator+( const Vector3t& vec3 ) const noexcept {
				return Vector3t( x + vec3.x, y + vec3.y, z + vec3.z );
			}

			Vector3t operator-( T scalar ) const noexcept {
				return Vector3t( x - scalar, y - scalar, z - scalar );
			}

			Vector3t operator-( const Vector3t& vec3 ) const noexcept {
				return Vector3t( x - vec3.x, y - vec3.y, z - vec3.z );
			}

			Vector3t operator*( T scalar ) const noexcept {
				return Vector3t( x * scalar, y * scalar, z * scalar );
			}

			Vector3t operator*( const Vector3t& vec3 ) const noexcept {
				return Vector3t( x * vec3.x, y * vec3.y, z * vec3.z );
			}

			Vector3t operator/( T scalar ) const noexcept {
				return Vector3t( x / scalar, y / scalar, z / scalar );
			}

			Vector3t operator/( const Vector3t& vec3 ) const noexcept {
				return Vector3t( x / vec3.x, y / vec3.y, z / vec3.z );
			}

		//===========================
		// 複合代入演算子
		//===========================

			Vector3t& operator+=( T scalar ) noexcept {
				*this = *this + scalar;
				return *this;
			}

			Vector3t& operator+=( const Vector3t& vec3 ) noexcept {
				*this = *this + vec3;
				return *this;
			}

			Vector3t& operator-=( T scalar ) noexcept {
				*this = *this - scalar;
				return *this;
			}

			Vector3t& operator-=( const Vector3t& vec3 ) noexcept {
				*this = *this - vec3;
				return *this;
			}

			Vector3t& operator*=( T scalar ) noexcept {
				*this = *this * scalar;
				return *this;
			}

			Vector3t& operator*=( const Vector3t& vec3 ) noexcept {
				*this = *this * vec3;
				return *this;
			}

			Vector3t& operator/=( T scalar ) noexcept {
				*this = *this / scalar;
				return *this;
			}

			Vector3t& operator/=( const Vector3t& vec3 ) noexcept {
				*this = *this / vec3;
				return *this;
			}

		//===========================
		// 比較演算子
		//===========================

			bool operator==( const Vector3t& vec3 ) const noexcept {
				return Equal<T>(vec3);
			}

			bool operator!=( const Vector3t& vec3 ) const noexcept {
				return NotEqual<T>(vec3);
			}

		//===========================
		// ストリーム出力
		//===========================

			friend std::ostream& operator<<( std::ostream& stream, const Vector3t& vec3 ) {
				stream << vec3.ToString();
				return stream;
			}

#pragma endregion operator

		public:

#pragma region static param

			// Vector3( 0.0, 1.0, 0.0 )
			static constexpr Vector3t Up() noexcept {
				return Vector3t( 0.0, 1.0, 0.0 );
			};
			// Vector3( 0.0, -1.0, 0.0 )
			static constexpr Vector3t Down() noexcept {
				return Vector3t( 0.0, -1.0, 0.0 );
			};
			// Vector3( 1.0, 0.0, 0.0 )
			static constexpr Vector3t Right() noexcept {
				return Vector3t( 1.0, 0.0, 0.0 );
			};
			// Vector3( -1.0, 0.0, 0.0 )
			static constexpr Vector3t Left() noexcept {
				return Vector3t( -1.0, 0.0, 0.0 );
			};
			// Vector3( 0.0, 0.0, 1.0 )
			static constexpr Vector3t Forward() noexcept {
				return Vector3t( 0.0, 0.0, 1.0 );
			};
			// Vector3( 0.0, 0.0, -1.0 )
			static constexpr Vector3t Back() noexcept {
				return Vector3t( 0.0, 0.0, -1.0 );
			};
			// Vector3( 0.0, 0.0, 0.0 )
			static constexpr Vector3t Zero() noexcept {
				return Vector3t( 0.0, 0.0, 0.0 );
			}
			// Vector3( 1.0, 1.0, 1.0 )
			static constexpr Vector3t One() noexcept {
				return Vector3t( 1.0, 1.0, 1.0 );
			}
			// Vector3( Infinity, Infinity, Infinity )
			static constexpr Vector3t PositiveInfinity() noexcept {
				return Vector3t( Math::Infinity<T>(), Math::Infinity<T>(), Math::Infinity<T>() );
			}
			// Vector3( -Infinity, -Infinity, -Infinity )
			static constexpr Vector3t NegativeInfinity() noexcept {
				return Vector3t( Math::negativeInfinity<T>(), Math::negativeInfinity<T>(), Math::negativeInfinity<T>() );
			}
			// NaNベクトル　※主にエラー値として返す
			static constexpr Vector3t NaN() noexcept {
				return Vector3t( Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>() );
			}

#pragma endregion static param

		public:

#pragma region static method

			// 3次元ベクトル間の線形補間
			static Vector3t Lerp( const Vector3t& a, const Vector3t& b, T t ) noexcept {
				// a から b までのベクトルのスカラー倍したものをaベクトルに加算する
				return a + ( a.GoToVector( b ) * Math::Clamp01<T>(t) );
			}

			// 3次元ベクトル間の線形補間
			static Vector3t LerpUnclamped( const Vector3t& a, const Vector3t& b, T t ) noexcept {
				// a から b までのベクトルのスカラー倍したものをaベクトルに加算する
				return a + ( a.GoToVector( b ) * t );
			}

#pragma endregion static method

		public:

#pragma region method

			// 全ての成分に同一の値を設定
			Vector3t& Set( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				z = scalar;
				return *this;
			}

			// 各軸の成分ごとに値を設定
			Vector3t& Set( T newX, T newY, T newZ ) noexcept {
				x = newX;
				y = newY;
				z = newZ;
				return *this;
			}

			// ベクトルの値をコピー
			Vector3t& Set( const Vector3t& vec3 ) noexcept {
				x = vec3.x;
				y = vec3.y;
				z = vec3.z;
				return *this;
			}

			// 全ての成分に同一の値を加算
			Vector3t& SetRelative(T diff) noexcept {
				x += diff;
				y += diff;
				z += diff;
				return *this;
			}

			// 各軸の成分ごとに値を加算
			Vector3t& SetRelative(T diffX, T diffY, T diffZ) noexcept {
				x += diffX;
				y += diffY;
				z += diffZ;
				return *this;
			}

			// ベクトルの値を加算
			Vector3t& SetRelative(const Vector3t& vec3) noexcept {
				x += vec3.x;
				y += vec3.y;
				z += vec3.z;
				return *this;
			}

			// ベクトルの長さを取得
			T Length() const noexcept {
				return Math::FastSqrt( SqrLength() );
			}

			// ベクトルの長さの2乗を取得
			T SqrLength() const noexcept {
				return ( x * x + y * y + z * z );
			}

			// ベクトル同士の距離を取得
			T Distance( const Vector3t& vec3 ) const noexcept {
				return GoToVector( vec3 ).Length();
			}

			// ベクトル同士の距離の2乗を取得
			T SqrDistance( const Vector3t& vec3 ) const noexcept {
				return GoToVector( vec3 ).SqrLength();
			}

			// このベクトルを逆ベクトルにする　※このベクトルが変化する
			Vector3t& Inverse() noexcept {
				*this = -*this;
				return *this;
			}

			// このベクトルを逆ベクトルにした結果を返す　※このベクトル自体は変化しない
			Vector3t InverseVector() const noexcept {
				return -*this;
			}

			// このベクトルを正規化する　※このベクトルが変化する
			Vector3t& Normalize() noexcept {
				*this /= Length();
				return *this;
			}

			// このベクトルを正規化した結果を返す　※このベクトル自体は変化しない
			Vector3t NormalizedVector() const noexcept {
				return *this / Length();
			}

			// このベクトルとのドット積の結果を返す
			T Dot( const Vector3t& vec3 ) const noexcept {
				return (x * vec3.x + y * vec3.y + z * vec3.z);
			}

			// このベクトルとのクロス積の結果を返す
			Vector3t Cross( const Vector3t& vec3 ) const noexcept {
				return Vector3t(
					y * vec3.z - z * vec3.y,
					z * vec3.x - x * vec3.z,
					x * vec3.y - y * vec3.x
				);
			}

			// このベクトルとの間の角度を求める
			T Angle( const Vector3t& vec3 ) {
				return Math::Acos( Dot( vec3 ) );
			}

			// このベクトルからtoVectorまでのベクトルを求める
			Vector3t GoToVector( const Vector3t& toVector ) const noexcept {
				return toVector - *this;
			}

			// Vector2t<U>型に変換する
			template<class U = T>
			Math::Vector::Vector2t<U> ToVector2() const noexcept {
				return Math::Vector::Vector2t<U>(
					x,
					y
					);
			}

			// Vector4t<U>型に変換する
			template<class U = T>
			Math::Vector::Vector4t<U> ToVector4() const noexcept {
				return Math::Vector::Vector4t<U>(
					x,
					y,
					z,
					0
					);
			}

			// データ型を変換したVector3t型を返す
			template<class U>
			Vector3t<U> TypeConvert() const noexcept {
				return Vector3t<U>( *this );
			}

			// 使用されているデータ型名を取得
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// ベクトルの値を文字列に変換
			std::string ToString() const noexcept {
				return std::string(
					"Vector3( "
					+
					Utility::ToString( x )
					+
					", "
					+
					Utility::ToString( y )
					+
					", "
					+
					Utility::ToString( z )
					+
					" )"
				);
			}

			// このベクトルの成分内にNaNが入っているかを調べる
			bool HasNaN() const noexcept {
				for (int_t i = 0; i < 3; i++) {
					if (Math::IsNaN(vec[i])) return true;
				}
				return false;
			}

			// このベクトルの成分内に正の無限大/負の無限大が入っているかを調べる
			bool HasInfinity() const noexcept {
				for (int_t i = 0; i < 3; i++) {
					if (Math::IsInfinity(vec[i])) return true;
				}
				return false;
			}

			// 与えられたベクトルがこのベクトルと正確に等しい場合、trueを返します
			template<class U = T>
			bool Equal( const Vector3t& vec3, U epsilon = 0.001) const noexcept {
				for (int_t i = 0; i < 3; i++) {
					if (Math::NotEqual<U>(vec[i], vec3.vec[i], epsilon)) {
						return false;
					}
				}
				return true;
			}

			// 与えられたベクトルがこのベクトルと正確に等しくない場合、trueを返します
			template<class U = T>
			bool NotEqual(const Vector3t& vec3, U epsilon = 0.001) const noexcept {
				for (int_t i = 0; i < 3; i++) {
					if (Math::NotEqual<U>(vec[i], vec3.vec[i], epsilon)) {
						return true;
					}
				}
				return false;
			}

			// 自身のデータ型と比較
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// このクラスのメモリサイズを返す
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

			// シリアライズを行う
			void Serialize( std::string filePath ) const noexcept {
				FileUtility::Text::LoadAndWrite(
					{
						"Type : ", TypeName(), "\n",
						ToString()
					},
					filePath
				);
			}

#pragma endregion method

		};

		// 四次元ベクトルを表すデータ型
		template<class T>
		class Vector4t {

		public:

			// x, y, z, w要素のデータ型
			using type = T;

		public:

#pragma region member

			union {
				struct {
					T x;
					T y;
					T z;
					T w;
				};
				struct {
					std::array<T, 4> vec;
				};
			};

#pragma endregion member

		public:

#pragma region constructor

			Vector4t() : x( 0 ), y( 0 ), z( 0 ), w( 0 ) {};
			Vector4t( const Vector4t& vec4 ) {
				vec = vec4.vec;
			};
			Vector4t( Vector4t&& vec4 ) {
				vec = vec4.vec;
			}
			Vector4t( T x, T y, T z, T w ) : x( x ), y( y ), z( z ), w( w ) {};

			// 型変換を行い要素を構築する（型変換機能付きコンストラクタ）
			template<class U>
			Vector4t( Vector4t<U> const& vec4 ) {
				x = static_cast< T >( vec4.x );
				y = static_cast< T >( vec4.y );
				z = static_cast< T >( vec4.z );
				w = static_cast< T >( vec4.w );
			}

#pragma endregion constructor

		public:

#pragma region operator

		//===========================
		// 代入演算子
		//===========================

			// 型変換を行い代入する
			template<class U>
			Vector4t& operator=( Vector4t<U> const& vec4 ) noexcept {
				x = static_cast< T >( vec4.x );
				y = static_cast< T >( vec4.y );
				z = static_cast< T >( vec4.z );
				w = static_cast< T >( vec4.w );
				return *this;
			}

			Vector4t& operator=( const Vector4t& vec4 ) noexcept {
				vec = vec4.vec;
				return *this;
			}

			Vector4t& operator=( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				z = scalar;
				w = scalar;
				return *this;
			}

		//===========================
		// ()演算子　※数値を設定する
		//===========================

			Vector4t& operator()( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				z = scalar;
				w = scalar;
				return *this;
			}

			Vector4t& operator()( T newX, T newY, T newZ, T newW ) noexcept {
				x = newX;
				y = newY;
				z = newZ;
				w = newW;
				return *this;
			}

			Vector4t& operator()( const Vector4t& vec4 ) noexcept {
				x = vec4.x;
				y = vec4.y;
				z = vec4.z;
				w = vec4.w;
				return *this;
			}

		//===========================
		// []演算子　※[0]、[1]、[2]、[3] を使用して x や y や z や w 成分にアクセスします
		//===========================

			T& operator[]( int_t i ) {
				return vec[i];
			}

		//===========================
		// 比較演算子
		//===========================

			bool operator==( const Vector4t& vec4 ) const noexcept {
				return Equal<T>(vec4);
			}

			bool operator!=( const Vector4t& vec4 ) const noexcept {
				return NotEqual<T>(vec4);
			}

			//===========================
			// ストリーム出力
			//===========================

			friend std::ostream& operator<<( std::ostream& stream, const Vector4t& vec4 ) {
				stream << vec4.ToString();
				return stream;
			}

#pragma endregion operator

		public:

#pragma region static param

			// Vector4( 0.0, 0.0, 0.0, 0.0 )
			static constexpr Vector4t Zero() noexcept {
				return Vector4t( 0.0, 0.0, 0.0, 0.0 );
			}
			// Vector4( 1.0, 1.0, 1.0, 1.0 )
			static constexpr Vector4t One() noexcept {
				return Vector4t( 1.0, 1.0, 1.0, 1.0 );
			}
			// NaNベクトル　※主にエラー値として返す
			static constexpr Vector4t NaN() noexcept {
				return Vector4t( Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>() );
			}

#pragma endregion static param

		public:

#pragma region method

			// 全ての成分に同一の値を設定
			Vector4t& Set( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				z = scalar;
				w = scalar;
				return *this;
			}

			// 各軸の成分ごとに値を設定
			Vector4t& Set( T newX, T newY, T newZ, T newW ) noexcept {
				x = newX;
				y = newY;
				z = newZ;
				w = newW;
				return *this;
			}

			// ベクトルの値をコピー
			Vector4t& Set( const Vector4t& vec4 ) noexcept {
				x = vec4.x;
				y = vec4.y;
				z = vec4.z;
				w = vec4.w;
				return *this;
			}

			// 全ての成分に同一の値を加算
			Vector4t& SetRelative(T diff) noexcept {
				x += diff;
				y += diff;
				z += diff;
				w += diff;
				return *this;
			}

			// 各軸の成分ごとに値を加算
			Vector4t& SetRelative(T diffX, T diffY, T diffZ, T diffW) noexcept {
				x += diffX;
				y += diffY;
				z += diffZ;
				w += diffW;
				return *this;
			}

			// ベクトルの値を加算
			Vector4t& SetRelative(const Vector4t& vec4) noexcept {
				x += vec4.x;
				y += vec4.y;
				z += vec4.z;
				w += vec4.w;
				return *this;
			}

			// Vector3t<U>型に変換する
			template<class U = T>
			Math::Vector::Vector3t<U> ToVector3() const noexcept {
				return Math::Vector::Vector3t<U>(
					x,
					y,
					z
					);
			}

			// データ型を変換したVector4t型を返す
			template<class U>
			Vector4t<U> TypeConvert() const noexcept {
				return Vector4t<U>( *this );
			}

			// 使用されているデータ型名を取得
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// ベクトルの値を文字列に変換
			std::string ToString() const noexcept {
				return std::string(
					"Vector4( "
					+
					Utility::ToString( x )
					+
					", "
					+
					Utility::ToString( y )
					+
					", "
					+
					Utility::ToString( z )
					+
					", "
					+
					Utility::ToString( w )
					+
					" )"
				);
			}

			// このベクトルの成分内にNaNが入っているかを調べる
			bool HasNaN() const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if (Math::IsNaN(vec[i])) return true;
				}
				return false;
			}

			// このベクトルの成分内に正の無限大/負の無限大が入っているかを調べる
			bool HasInfinity() const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if (Math::IsInfinity(vec[i])) return true;
				}
				return false;
			}

			// 与えられたベクトルがこのベクトルと正確に等しい場合、trueを返します
			template<class U = T>
			bool Equal( const Vector4t& vec4, U epsilon = 0.001) const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if (Math::NotEqual<U>(vec[i], vec4.vec[i], epsilon)) {
						return false;
					}
				}
				return true;
			}

			// 与えられたベクトルがこのベクトルと正確に等しいない場合、trueを返します
			template<class U = T>
			bool NotEqual(const Vector4t& vec4, U epsilon = 0.001) const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if (Math::NotEqual<U>(vec[i], vec4.vec[i], epsilon)) {
						return true;
					}
				}
				return false;
			}

			// 自身のデータ型と比較
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// このクラスのメモリサイズを返す
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

			// シリアライズを行う
			void Serialize( std::string filePath ) const noexcept {
				FileUtility::Text::LoadAndWrite(
					{
						"Type : ", TypeName(), "\n",
						ToString()
					},
					filePath
				);
			}

#pragma endregion method

		};

		//===================================================
		// float_t型とint_t型でエイリアス宣言
		//===================================================

		using Vector1 = Vector1t<float_t>;
		using Vector1Int = Vector1t<int_t>;

		using Vector2 = Vector2t<float_t>;
		using Vector2Int = Vector2t<int_t>;

		using Vector3 = Vector3t<float_t>;
		using Vector3Int = Vector3t<int_t>;

		using Vector4 = Vector4t<float_t>;
		using Vector4Int = Vector4t<int_t>;

	}

#pragma endregion VectorLib


	// 行列ライブラリ
#pragma region MatrixLib

	// Reference

	// 参考サイト
	// https://light11.hatenadiary.com/entry/2019/01/07/213408
	// https://zenn.dev/ryutorion/articles/game-math-matrix
	// https://nogson2.hatenablog.com/entry/2017/05/22/222937?amp=1
	// https://imagingsolution.net/math/

	// 行列式
	// http://www.slis.tsukuba.ac.jp/~fujisawa.makoto.fu/cgi-bin/wiki/index.php?4x4%A4%DE%A4%C7%A4%CE%B5%D5%B9%D4%CE%F3%A4%CE%C4%BE%C0%DC%B2%F2%CB%A1
	// 2x2：https://risalc.info/src/determinant-two-by-two.html
	// 3x3：https://risalc.info/src/determinant-three-by-three.html

	// 逆行列
	// http://www.slis.tsukuba.ac.jp/~fujisawa.makoto.fu/cgi-bin/wiki/index.php?4x4%A4%DE%A4%C7%A4%CE%B5%D5%B9%D4%CE%F3%A4%CE%C4%BE%C0%DC%B2%F2%CB%A1
	// https://takun-physics.net/12554/
	// http://cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/tech23.html

	// 検算
	// https://matrixcalc.org/ja/

	// 行列ライブラリ
	namespace Matrix {

		// 前方宣言
		template<class T>
		class Matrix2x2t;
		template<class T>
		class Matrix3x3t;
		template<class T>
		class Matrix4x4t;

		// 2x2行列
		template<class T>
		class Matrix2x2t {

		public:

			// 行列要素のデータ型
			using type = T;

			// 2x2行列の一行分の成分
			using Row = std::array<T, 2>;

			// 2x2行列の一列分の成分
			using Column = std::array<T, 2>;

			// 配列でデータを保持する2行2列のデータ型　※1次元目が行、2次元目が列のデータ
			using Matrix2x2Array = std::array<Matrix2x2t::Row, 2>;

		public:

#pragma region member

			// R : Row(行), C : Column(列)
			union {
				struct {
					// 各成分を個別に扱う場合に使用
					T m11, m12;			// 1行目の各成分
					T m21, m22;			// 2行目の各成分
				};
				struct {
					// 行ごとの成分を三次元ベクトルとして扱う場合に使用
					Math::Vector::Vector2t<T> rowVector1;	// 1行目の成分のベクトル
					Math::Vector::Vector2t<T> rowVector2;	// 2行目の成分のベクトル
				};
				struct {
					// 行ごとの成分を配列として扱う場合に使用
					Matrix2x2t::Row row1;	// 1行目の成分の配列
					Matrix2x2t::Row row2;	// 2行目の成分の配列
				};
				struct {
					// 成分全体に対する1次元配列
					std::array<T, 4> matrix;
				};
				struct {
					// 成分全体に対する2次元配列　※1次元目が行、2次元目が列のデータ
					Matrix2x2t::Matrix2x2Array matrix2x2;
				};
			};

#pragma endregion member

		public:

#pragma region constructor

			// デフォルトで単位行列
			Matrix2x2t() :
				m11( 1 ), m12( 0 ),
				m21( 0 ), m22( 1 ) {};
			Matrix2x2t( const Matrix2x2t& matrix2x2 ) {
				matrix = matrix2x2.matrix;
			};
			Matrix2x2t( Matrix2x2t&& matrix2x2 ) {
				matrix = matrix2x2.matrix;
			}
			Matrix2x2t( T m11, T m12, T m21, T m22 ) :
				m11( m11 ), m12( m12 ),
				m21( m21 ), m22( m22 ) {};

			// 型変換を行い要素を構築する（型変換機能付きコンストラクタ）
			template<class U>
			Matrix2x2t( Matrix2x2t<U> const& matrix2x2 ) {
				for( int i = 0; i < 4; i++ ) {
					matrix[i] = static_cast< T >( matrix2x2.matrix[i] );
				}
			}

#pragma endregion constructor

		public:

#pragma region operator 

		//===========================
		// 代入演算子
		//===========================

			// 型変換を行い代入する
			template<class U>
			Matrix2x2t& operator=( Matrix2x2t<U> const& matrix2x2 ) noexcept {
				for( int i = 0; i < 4; i++ ) {
					matrix[i] = static_cast< T >( matrix2x2.matrix[i] );
				}
				return *this;
			}

			Matrix2x2t& operator=( const Matrix2x2t& matrix2x2 ) noexcept {
				for( int i = 0; i < 4; i++ ) {
					matrix[i] = matrix2x2.matrix[i];
				}
				return *this;
			}

		//===========================
		// 算術演算子
		//===========================

			constexpr Matrix2x2t operator+( const Matrix2x2t& matrix2x2 ) const noexcept {
				return Matrix2x2t(
					m11 + matrix2x2.m11, m12 + matrix2x2.m12,
					m21 + matrix2x2.m21, m22 + matrix2x2.m22
				);
			}

			constexpr Matrix2x2t operator-( const Matrix2x2t& matrix2x2 ) const noexcept {
				return Matrix2x2t(
					m11 - matrix2x2.m11, m12 - matrix2x2.m12,
					m21 - matrix2x2.m21, m22 - matrix2x2.m22
				);
			}

			constexpr Matrix2x2t operator*( const Matrix2x2t& matrix2x2 ) const noexcept {

				// 行要素と列要素の乗算
				auto mul = []( const Matrix2x2t::Row& row, const Matrix2x2t::Column& column ) {
					T ret = 0;
					for( int i = 0; i < 2; i++ ) {
						ret += row[i] * column[i];
					}
					return ret;
				};

				// 一時変数
				auto r1 = GetRow( 0 );
				auto r2 = GetRow( 1 );
				auto c1 = matrix2x2.GetColumn( 0 );
				auto c2 = matrix2x2.GetColumn( 1 );

				return Matrix2x2t(
					mul( r1, c1 ), mul( r1, c2 ),
					mul( r2, c1 ), mul( r2, c2 )
				);

			}

			constexpr Math::Vector::Vector2t<T> operator*( const Math::Vector::Vector2t<T>& vec2 ) const noexcept {

				// 列要素と2次元ベクトルの乗算
				auto mul = []( const Matrix2x2t::Column& column, const Math::Vector::Vector2t<T>& _vec2 ) {
					T ret = 0;
					for( int i = 0; i < 2; i++ ) {
						ret += column[i] * _vec2.vec[i];
					}
					return ret;
				};

				// 一時変数
				auto c1 = GetRow( 0 );
				auto c2 = GetRow( 1 );

				return Math::Vector::Vector2t<T>(
					mul( c1, vec2 ),
					mul( c2, vec2 )
					);

			}

		//===========================
		// 複合代入演算子
		//===========================

			Matrix2x2t& operator+=( const Matrix2x2t& matrix2x2 ) noexcept {
				*this = *this + matrix2x2;
				return *this;
			}

			Matrix2x2t& operator-=( const Matrix2x2t& matrix2x2 ) noexcept {
				*this = *this - matrix2x2;
				return *this;
			}

			Matrix2x2t& operator*=( const Matrix2x2t& matrix2x2 ) noexcept {
				*this = *this * matrix2x2;
				return *this;
			}

		//===========================
		// 比較演算子
		//===========================

			bool operator==( const Matrix2x2t& matrix2x2 ) const noexcept {
				return Equal<T>(matrix2x2);
			}

			bool operator!=( const Matrix2x2t& matrix2x2 ) const noexcept {
				return NotEqual<T>(matrix2x2);
			}

			//===========================
			// ストリーム出力
			//===========================

			friend std::ostream& operator<<( std::ostream& stream, const Matrix2x2t& matrix2x2 ) {
				stream << matrix2x2.ToString();
				return stream;
			}

#pragma endregion operator

		public:

#pragma region static param

			// 単位行列
			static constexpr Matrix2x2t Identity() noexcept {
				return Matrix2x2t(
					1.0, 0.0,
					0.0, 1.0
				);
			}

			// ゼロ行列
			static constexpr Matrix2x2t Zero() noexcept {
				return Matrix2x2t(
					0.0, 0.0,
					0.0, 0.0
				);
			}

			// NaN行列　※主にエラー値として返す
			static constexpr Matrix2x2t NaN() noexcept {
				return Matrix2x2t(
					Math::NaN<T>(), Math::NaN<T>(),
					Math::NaN<T>(), Math::NaN<T>()
				);
			}

#pragma endregion static param

		public:

#pragma region static method


#pragma endregion static method

		public:

#pragma region method

			// 自身の行列を転置する
			Matrix2x2t& Transpose() noexcept {
				*this = TransposeMatrix();
				return *this;
			}

			// 自身の行列を転置した結果を返す
			Matrix2x2t TransposeMatrix() const noexcept {
				return Matrix2x2t(
					m11, m21,
					m12, m22
					);
			}

			// 自身の行列の逆行列を返す
			Matrix2x2t InverseMatrix() const noexcept {
				// 行列式の結果値
				auto det = Determinant();

				// 行列式の結果値が0なら逆行列は存在しない
				if( Math::Equal<T>( det, 0.0, 0.001 ) ) {
#if _DEBUG
					try {
						throw std::runtime_error( "Matrix2x2::InverseMatrix : Unable to compute inverse matrix." );
					}
					catch( const std::runtime_error& error ) {
						// 弱警告を行う
						Debug::LogWarning( error.what() );
						// NaN行列を返す
						return Matrix2x2t::NaN();
					}
#else
					// 弱警告を行う
					Debug::LogWarning( "Matrix2x2::InverseMatrix : Unable to compute inverse matrix." );
					// NaN行列を返す
					return Matrix2x2t::NaN();
#endif
				}
				else {
					auto invDet = static_cast<T>(1.0) / det;
					return Matrix2x2t<T>(
						m22 * invDet,
						m12 * -invDet,
						m21 * -invDet,
						m11 * invDet
						);
				}
			}

			// 自身の行列式の結果を返す
			T Determinant() const noexcept {
				return T(
					m11 * m22 - m12 * m21
				);
			}

			// 指定した行の要素を取得する
			Matrix2x2t::Row GetRow( uint_t row ) const {
				switch( row ) {
					case 0:
					{
						return { { m11, m12 } };
					}
					break;
					case 1:
					{
						return { { m21, m22 } };
					}
					break;
					default:
					{
						try {
							// 要素外アクセス
							throw std::runtime_error( "Matrix3x3::GetRowMethod : index out of range." );
						}
						catch( const std::runtime_error& error ) {
							Debug::LogError( error.what() );
						}
					}
					break;
				}
			}

			// 指定した列の要素を取得する
			Matrix2x2t::Column GetColumn( uint_t column ) const {
				switch( column ) {
					case 0:
					{
						return { { m11, m21 } };
					}
					break;
					case 1:
					{
						return { { m12, m22 } };
					}
					break;
					default:
					{
						try {
							// 要素外アクセス
							throw std::runtime_error( "Matrix3x3::GetCoumnMethod : index out of range." );
						}
						catch( const std::runtime_error& error ) {
							std::cout << error.what() << std::endl;
							Debug::LogError( error.what() );
						}
					}
					break;
				}
			}

			// 列のデータを直線状に並べたものを取得する
			std::array<T, 4> ColumnMajorOrder() const noexcept {
				return {
					// 1列目
					m11, m21,
					// 2列目
					m12, m22
				};
			}

			// 行のデータを直線状に並べたものを取得する
			std::array<T, 4> RowMajorOrder() const noexcept {
				return {
					// 1行目
					m11, m12,
					// 2行目
					m21, m22
				};
			}

			// Matrix3x3t<U>型に変換する
			template<class U = T>
			Matrix3x3t<U> ToMatrix3x3() const noexcept {
				return Matrix3x3t<U>(
					// 1行目
					m11, m12, 0,
					// 2行目
					m21, m22, 0,
					// 3行目
					0, 0, 1
					);
			}

			// データ型を変換したMatrix2x2t型を返す
			template<class U>
			Matrix2x2t<U> TypeConvert() const noexcept {
				return Matrix2x2t<U>( *this );
			}

			// 使用されているデータ型名を取得
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// 行列の値を文字列に変換
			std::string ToString() const noexcept {
				// std::string("1.(")としているのはひとつ前の行の" )\n"と文字列同士を結合するためで
				// こうしないと、const char*同士の加算となってしまうのでコンパイルエラーとなる
				return std::string(
					"Matrix2x2 {\n" +
					std::string( " 1.( " ) + Utility::ToString( m11 ) + ", " + Utility::ToString( m12 ) + " )\n" +
					std::string( " 2.( " ) + Utility::ToString( m21 ) + ", " + Utility::ToString( m22 ) + " )\n" +
					std::string("}")
				);
			}

			// この行列の成分内にNaNが入っているかを調べる
			bool HasNaN() const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if (Math::IsNaN(matrix[i])) return true;
				}
				return false;
			}

			// この行列の成分内に正の無限大/負の無限大が入っているかを調べる
			bool HasInfinity() const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if (Math::IsInfinity(matrix[i])) return true;
				}
				return false;
			}

			// 与えられた行列がこの行列と正確に等しい場合、trueを返します
			template<class U = T>
			bool Equal( const Matrix2x2t& matrix2x2, U epsilon = 0.001) const noexcept {
				for (int i = 0; i < 4; i++) {
					if (Math::NotEqual<U>(matrix[i], matrix2x2.matrix[i], epsilon)) {
						return false;
					}
				}
				return true;
			}

			// 与えられた行列がこの行列と正確に等しくない場合、trueを返します
			template<class U = T>
			bool NotEqual(const Matrix2x2t& matrix2x2, U epsilon = 0.001) const noexcept {
				for (int i = 0; i < 4; i++) {
					if (Math::NotEqual<U>(matrix[i], matrix2x2.matrix[i], epsilon)) {
						return true;
					}
				}
				return false;
			}

			// 自身のデータ型と比較
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// このクラスのメモリサイズを返す
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

			// シリアライズを行う
			void Serialize( std::string filePath ) const noexcept {
				FileUtility::Text::LoadAndWrite(
					{
						"Type : ", TypeName(), "\n",
						ToString()
					},
					filePath
				);
			}

#pragma endregion method

		};

		// 3x3行列
		template<class T>
		class Matrix3x3t {

		public:

			// 行列要素のデータ型
			using type = T;

			// 3x3行列の一行分の成分
			using Row = std::array<T, 3>;

			// 3x3行列の一列分の成分
			using Column = std::array<T, 3>;

			// 配列でデータを保持する3行3列のデータ型　※1次元目が行、2次元目が列のデータ
			using Matrix3x3Array = std::array<Matrix3x3t::Row, 3>;

		public:

#pragma region member

			// R : Row(行), C : Column(列)
			union {
				struct {
					// 各成分を個別に扱う場合に使用
					T m11, m12, m13;			// 1行目の各成分
					T m21, m22, m23;			// 2行目の各成分
					T m31, m32, m33;			// 3行目の各成分
				};
				struct {
					// 行ごとの成分を三次元ベクトルとして扱う場合に使用
					Math::Vector::Vector3t<T> rowVector1;	// 1行目の成分のベクトル
					Math::Vector::Vector3t<T> rowVector2;	// 2行目の成分のベクトル
					Math::Vector::Vector3t<T> rowVector3;	// 3行目の成分のベクトル
				};
				struct {
					// 行ごとの成分を配列として扱う場合に使用
					Matrix3x3t::Row row1;	// 1行目の成分の配列
					Matrix3x3t::Row row2;	// 2行目の成分の配列
					Matrix3x3t::Row row3;	// 3行目の成分の配列
				};
				struct {
					// 成分全体に対する1次元配列
					std::array<T, 9> matrix;
				};
				struct {
					// 成分全体に対する2次元配列　※1次元目が行、2次元目が列のデータ
					Matrix3x3t::Matrix3x3Array matrix3x3;
				};
			};

#pragma endregion member

		public:

#pragma region constructor

			// デフォルトで単位行列
			Matrix3x3t() :
				m11( 1 ), m12( 0 ), m13( 0 ),
				m21( 0 ), m22( 1 ), m23( 0 ),
				m31( 0 ), m32( 0 ), m33( 1 ) {};
			Matrix3x3t( const Matrix3x3t& matrix3x3 ) {
				matrix = matrix3x3.matrix;
			};
			Matrix3x3t( Matrix3x3t&& matrix3x3 ) {
				matrix = matrix3x3.matrix;
			}
			Matrix3x3t( T m11, T m12, T m13, T m21, T m22, T m23, T m31, T m32, T m33 ) :
				m11( m11 ), m12( m12 ), m13( m13 ),
				m21( m21 ), m22( m22 ), m23( m23 ),
				m31( m31 ), m32( m32 ), m33( m33 ) {};

			// 型変換を行い要素を構築する（型変換機能付きコンストラクタ）
			template<class U>
			Matrix3x3t( Matrix3x3t<U> const& matrix3x3 ) {
				for( int i = 0; i < 9; i++ ) {
					matrix[i] = static_cast< T >( matrix3x3.matrix[i] );
				}
			}

#pragma endregion constructor

		public:

#pragma region operator 

		//===========================
		// 代入演算子
		//===========================

			// 型変換を行い代入する
			template<class U>
			Matrix3x3t& operator=( Matrix3x3t<U> const& matrix3x3 ) noexcept {
				for( int i = 0; i < 9; i++ ) {
					matrix[i] = static_cast< T >( matrix3x3.matrix[i] );
				}
				return *this;
			}

			Matrix3x3t& operator=( const Matrix3x3t& matrix3x3 ) noexcept {
				for( int i = 0; i < 9; i++ ) {
					matrix[i] = matrix3x3.matrix[i];
				}
				return *this;
			}

		//===========================
		// 算術演算子
		//===========================

			constexpr Matrix3x3t operator+( const Matrix3x3t& matrix3x3 ) const noexcept {
				return Matrix3x3t(
					m11 + matrix3x3.m11, m12 + matrix3x3.m12, m13 + matrix3x3.m13,
					m21 + matrix3x3.m21, m22 + matrix3x3.m22, m23 + matrix3x3.m23,
					m31 + matrix3x3.m31, m32 + matrix3x3.m32, m33 + matrix3x3.m33
				);
			}

			constexpr Matrix3x3t operator-( const Matrix3x3t& matrix3x3 ) const noexcept {
				return Matrix3x3t(
					m11 - matrix3x3.m11, m12 - matrix3x3.m12, m13 - matrix3x3.m13,
					m21 - matrix3x3.m21, m22 - matrix3x3.m22, m23 - matrix3x3.m23,
					m31 - matrix3x3.m31, m32 - matrix3x3.m32, m33 - matrix3x3.m33
				);
			}

			constexpr Matrix3x3t operator*( const Matrix3x3t& matrix3x3 ) const noexcept {

				// 行要素と列要素の乗算
				auto mul = []( const Matrix3x3t::Row& row, const Matrix3x3t::Column& column ) {
					T ret = 0;
					for( int i = 0; i < 3; i++ ) {
						ret += row[i] * column[i];
					}
					return ret;
				};

				// 一時変数
				auto r1 = GetRow( 0 );
				auto r2 = GetRow( 1 );
				auto r3 = GetRow( 2 );
				auto c1 = matrix3x3.GetColumn( 0 );
				auto c2 = matrix3x3.GetColumn( 1 );
				auto c3 = matrix3x3.GetColumn( 2 );

				return Matrix3x3t(
					mul( r1, c1 ), mul( r1, c2 ), mul( r1, c3 ),
					mul( r2, c1 ), mul( r2, c2 ), mul( r2, c3 ),
					mul( r3, c1 ), mul( r3, c2 ), mul( r3, c3 )
				);

			}

			constexpr Math::Vector::Vector3t<T> operator*( const Math::Vector::Vector3t<T>& vec3 ) const noexcept {

				// 列要素と3次元ベクトルの乗算
				auto mul = []( const Matrix3x3t::Column& column, const Math::Vector::Vector3t<T>& _vec3 ) {
					T ret = 0;
					for( int i = 0; i < 3; i++ ) {
						ret += column[i] * _vec3.vec[i];
					}
					return ret;
				};

				// 一時変数
				auto c1 = GetRow( 0 );
				auto c2 = GetRow( 1 );
				auto c3 = GetRow( 2 );

				return Math::Vector::Vector3t<T>(
					mul( c1, vec3 ),
					mul( c2, vec3 ),
					mul( c3, vec3 )
					);

			}

		//===========================
		// 複合代入演算子
		//===========================

			Matrix3x3t& operator+=( const Matrix3x3t& matrix3x3 ) noexcept {
				*this = *this + matrix3x3;
				return *this;
			}

			Matrix3x3t& operator-=( const Matrix3x3t& matrix3x3 ) noexcept {
				*this = *this - matrix3x3;
				return *this;
			}

			Matrix3x3t& operator*=( const Matrix3x3t& matrix3x3 ) noexcept {
				*this = *this * matrix3x3;
				return *this;
			}

		//===========================
		// 比較演算子
		//===========================

			bool operator==( const Matrix3x3t& matrix3x3 ) const noexcept {
				return Equal<T>(matrix3x3);
			}

			bool operator!=( const Matrix3x3t& matrix3x3 ) const noexcept {
				return NotEqual<T>(matrix3x3);
			}

			//===========================
			// ストリーム出力
			//===========================

			friend std::ostream& operator<<( std::ostream& stream, const Matrix3x3t& matrix3x3 ) {
				stream << matrix3x3.ToString();
				return stream;
			}

#pragma endregion operator

		public:

#pragma region static param

			// 単位行列
			static constexpr Matrix3x3t Identity() noexcept {
				return Matrix3x3t(
					1.0, 0.0, 0.0,
					0.0, 1.0, 0.0,
					0.0, 0.0, 1.0
				);
			}

			// ゼロ行列
			static constexpr Matrix3x3t Zero() noexcept {
				return Matrix3x3t(
					0.0, 0.0, 0.0,
					0.0, 0.0, 0.0,
					0.0, 0.0, 0.0
				);
			}

			// NaN行列　※主にエラー値として返す
			static constexpr Matrix3x3t NaN() noexcept {
				return Matrix3x3t(
					Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>(),
					Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>(),
					Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>()
				);
			}

#pragma endregion static param

		public:

#pragma region static method


#pragma endregion static method

		public:

#pragma region method

			// 自身の行列を転置する
			Matrix3x3t& Transpose() noexcept {
				*this = TransposeMatrix();
				return *this;
			}

			// 自身の行列を転置した結果を返す
			Matrix3x3t TransposeMatrix() const noexcept {
				return Matrix3x3t(
					m11, m21, m31,
					m12, m22, m32,
					m13, m23, m33
				);
			}

			// 自身の行列の逆行列を取得する
			Matrix3x3t InverseMatrix() const noexcept {
				// 行列式の結果値
				auto det = Determinant();

				// 行列式の結果値が0なら逆行列は存在しない
				if( Math::Equal<T>( det, 0.0, 0.001 ) ) {
#if _DEBUG
					try {
						throw std::runtime_error( "Matrix3x3::InverseMatrix : Unable to compute inverse matrix." );
					}
					catch( const std::runtime_error& error ) {
						// 弱警告を行う
						Debug::LogWarning( error.what() );
						// NaN行列を返す
						return Matrix3x3t::NaN();
					}
#else
					// 弱警告を行う
					Debug::LogWarning( "Matrix3x3::InverseMatrix : Unable to compute inverse matrix." );
					// NaN行列を返す
					return Matrix3x3t::NaN();
#endif
				}
				else {
					auto invDet = static_cast< T >( 1.0 ) / det;
					return Matrix3x3t<T>(
						invDet * ( m22 * m33 - m23 * m32 ),
						invDet * ( m13 * m32 - m12 * m33 ),
						invDet * ( m12 * m23 - m13 * m22 ),
						invDet * ( m23 * m31 - m21 * m33 ),
						invDet * ( m11 * m33 - m13 * m31 ),
						invDet * ( m13 * m21 - m11 * m23 ),
						invDet * ( m21 * m32 - m22 * m31 ),
						invDet * ( m12 * m31 - m11 * m32 ),
						invDet * ( m11 * m22 - m12 * m21 )
						);
				}

			}
			
			// 自身の行列式の結果を返す
			T Determinant() const noexcept {

				auto c11 = Matrix2x2t<T>(
					m22, m32,
					m23, m33
					).Determinant();

				auto c12 = Matrix2x2t<T>(
					m21, m31,
					m23, m33
					).Determinant();

				auto c13 = Matrix2x2t<T>(
					m21, m31,
					m22, m32
					).Determinant();

				return m11 * c11 - m12 * c12 + m13 * c13;

			}

			// 指定した行の要素を取得する
			Matrix3x3t::Row GetRow( uint_t row ) const {
				switch( row ) {
					case 0:
					{
						return { { m11, m12, m13 } };
					}
					break;
					case 1:
					{
						return { { m21, m22, m23 } };
					}
					break;
					case 2:
					{
						return { { m31, m32, m33 } };
					}
					break;
					default:
					{
						try {
							// 要素外アクセス
							throw std::runtime_error( "Matrix3x3::GetRowMethod : index out of range." );
						}
						catch( const std::runtime_error& error ) {
							std::cout << error.what() << std::endl;
							Debug::LogError( error.what() );
						}
					}
					break;
				}
			}

			// 指定した列の要素を取得する
			Matrix3x3t::Column GetColumn( uint_t column ) const {
				switch( column ) {
					case 0:
					{
						return { { m11, m21, m31 } };
					}
					break;
					case 1:
					{
						return { { m12, m22, m32 } };
					}
					break;
					case 2:
					{
						return { { m13, m23, m33 } };
					}
					break;
					default:
					{
						try {
							// 要素外アクセス
							throw std::runtime_error( "Matrix3x3::GetCoumnMethod : index out of range." );
						}
						catch( const std::runtime_error& error ) {
							std::cout << error.what() << std::endl;
							Debug::LogError( error.what() );
						}
					}
					break;
				}
			}

			// 列のデータを直線状に並べたものを取得する
			std::array<T, 9> ColumnMajorOrder() const noexcept {
				return {
					// 1列目
					m11, m21, m31,
					// 2列目
					m12, m22, m32,
					// 3列目
					m13, m23, m33
				};
			}

			// 行のデータを直線状に並べたものを取得する
			std::array<T, 9> RowMajorOrder() const noexcept {
				return {
					// 1行目
					m11, m12, m13,
					// 2行目
					m21, m22, m23,
					// 3行目
					m31, m32, m33
				};
			}

			// Matrix2x2t<U>型に変換する
			template<class U = T>
			Matrix2x2t<U> ToMatrix2x2() const noexcept {
				return Matrix2x2t<U>(
					m11, m12,
					m21, m22
					);
			}

			// Matrix4x4t<U>型に変換する
			template<class U = T>
			Matrix4x4t<U> ToMatrix4x4() const noexcept {
				return Matrix4x4t<U>(
					// 1行目
					m11, m12, m13, 0,
					// 2行目
					m21, m22, m23, 0,
					// 3行目
					m31, m32, m33, 0,
					// 4行目
					0, 0, 0, 1
					);
			}

			// データ型を変換したMatrix3x3t型を返す
			template<class U>
			Matrix3x3t<U> TypeConvert() const noexcept {
				return Matrix3x3t<U>( *this );
			}

			// 使用されているデータ型名を取得
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// 行列の値を文字列に変換
			std::string ToString() const noexcept {
				// std::string("1.(")としているのはひとつ前の行の" )\n"と文字列同士を結合するためで
				// こうしないと、const char*同士の加算となってしまうのでコンパイルエラーとなる
				return std::string(
					"Matrix3x3 {\n" +
					std::string(" 1.( ") + Utility::ToString( m11 ) + ", " + Utility::ToString( m12 ) + ", " + Utility::ToString( m13 ) + " )\n" +
					std::string(" 2.( ") + Utility::ToString( m21 ) + ", " + Utility::ToString( m22 ) + ", " + Utility::ToString( m23 ) + " )\n" +
					std::string(" 3.( ") + Utility::ToString( m31 ) + ", " + Utility::ToString( m32 ) + ", " + Utility::ToString( m33 ) + " )\n" +
					std::string("}")
				);
			}

			// この行列の成分内にNaNが入っているかを調べる
			bool HasNaN() const noexcept {
				for (int_t i = 0; i < 9; i++) {
					if (Math::IsNaN(matrix[i])) return true;
				}
				return false;
			}

			// この行列の成分内に正の無限大/負の無限大が入っているかを調べる
			bool HasInfinity() const noexcept {
				for (int_t i = 0; i < 9; i++) {
					if (Math::IsInfinity(matrix[i])) return true;
				}
				return false;
			}

			// 与えられた行列がこの行列と正確に等しい場合、trueを返します
			template<class U = T>
			bool Equal( const Matrix3x3t& matrix3x3, U epsilon = 0.001 ) const noexcept {
				for (int i = 0; i < 9; i++) {
					if (Math::NotEqual<U>(matrix[i], matrix3x3.matrix[i], epsilon)) {
						return false;
					}
				}
				return true;
			}

			// 与えられた行列がこの行列と正確に等しくない場合、trueを返します
			template<class U = T>
			bool NotEqual(const Matrix3x3t& matrix3x3, U epsilon = 0.001) const noexcept {
				for (int i = 0; i < 9; i++) {
					if (Math::NotEqual<U>(matrix[i], matrix3x3.matrix[i], epsilon)) {
						return true;
					}
				}
				return false;
			}

			// 自身のデータ型と比較
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// このクラスのメモリサイズを返す
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

			// シリアライズを行う
			void Serialize( std::string filePath ) const noexcept {
				FileUtility::Text::LoadAndWrite(
					{
						"Type : ", TypeName(), "\n",
						ToString()
					},
					filePath
				);
			}

#pragma endregion method

		};

		// 4x4行列
		template<class T>
		class Matrix4x4t {

		public:

			// 行列要素のデータ型
			using type = T;

			// 4x4行列の一行分の成分
			using Row = std::array<T, 4>;

			// 4x4行列の一列分の成分
			using Column = std::array<T, 4>;

			// 配列でデータを保持する3行3列のデータ型　※1次元目が行、2次元目が列のデータ
			using Matrix4x4Array = std::array<Matrix4x4t::Row, 4>;

		public:

#pragma region member

			// R : Row(行), C : Column(列)
			union {
				struct {
					// 各成分を個別に扱う場合に使用
					T m11, m12, m13, m14;			// 1行目の各成分
					T m21, m22, m23, m24;			// 2行目の各成分
					T m31, m32, m33, m34;			// 3行目の各成分
					T m41, m42, m43, m44;			// 4行目の各成分
				};
				struct {
					// 行ごとの成分を四次元ベクトルとして扱う場合に使用
					Math::Vector::Vector4t<T> rowVector1;	// 1行目の成分のベクトル
					Math::Vector::Vector4t<T> rowVector2;	// 2行目の成分のベクトル
					Math::Vector::Vector4t<T> rowVector3;	// 3行目の成分のベクトル
					Math::Vector::Vector4t<T> rowVector4;	// 4行目の成分のベクトル
				};
				struct {
					// 行ごとの成分を配列として扱う場合に使用
					Matrix4x4t::Row row1;	// 1行目の成分の配列
					Matrix4x4t::Row row2;	// 2行目の成分の配列
					Matrix4x4t::Row row3;	// 3行目の成分の配列
					Matrix4x4t::Row row4;	// 4行目の成分の配列
				};
				struct {
					// 成分全体に対する1次元配列
					std::array<T, 16> matrix;
				};
				struct {
					// 成分全体に対する2次元配列　※1次元目が行、2次元目が列のデータ
					Matrix4x4t::Matrix4x4Array matrix4x4;
				};
			};

#pragma endregion member

		public:

#pragma region constructor

			// デフォルトで単位行列
			Matrix4x4t() :
				m11( 1 ), m12( 0 ), m13( 0 ), m14( 0 ),
				m21( 0 ), m22( 1 ), m23( 0 ), m24( 0 ),
				m31( 0 ), m32( 0 ), m33( 1 ), m34( 0 ),
				m41( 0 ), m42( 0 ), m43( 0 ), m44( 1 ) {};
			Matrix4x4t( const Matrix4x4t& matrix4x4 ) {
				matrix = matrix4x4.matrix;
			};
			Matrix4x4t( Matrix4x4t&& matrix4x4 ) {
				matrix = matrix4x4.matrix;
			}
			Matrix4x4t( T m11, T m12, T m13, T m14, T m21, T m22, T m23, T m24, T m31, T m32, T m33, T m34, T m41, T m42, T m43, T m44 ) :
				m11( m11 ), m12( m12 ), m13( m13 ), m14( m14 ),
				m21( m21 ), m22( m22 ), m23( m23 ), m24( m24 ),
				m31( m31 ), m32( m32 ), m33( m33 ), m34( m34 ),
				m41( m41 ), m42( m42 ), m43( m43 ), m44( m44 ) {};

			// 型変換を行い要素を構築する（型変換機能付きコンストラクタ）
			template<class U>
			Matrix4x4t( Matrix4x4t<U> const& matrix4x4 ) {
				for( int i = 0; i < 16; i++ ) {
					matrix[i] = static_cast< T >( matrix4x4.matrix[i] );
				}
			}

#pragma endregion constructor

		public:

#pragma region operator 

		//===========================
		// 代入演算子
		//===========================

			// 型変換を行い代入する
			template<class U>
			Matrix4x4t& operator=( Matrix4x4t<U> const& matrix4x4 ) noexcept {
				for( int i = 0; i < 16; i++ ) {
					matrix[i] = static_cast< T >( matrix4x4.matrix[i] );
				}
				return *this;
			}

			Matrix4x4t& operator=( const Matrix4x4t& matrix4x4 ) noexcept {
				for( int i = 0; i < 16; i++ ) {
					matrix[i] = matrix4x4.matrix[i];
				}
				return *this;
			}

		//===========================
		// 算術演算子
		//===========================

			constexpr Matrix4x4t operator+( const Matrix4x4t& matrix4x4 ) const noexcept {
				return Matrix4x4t(
					m11 + matrix4x4.m11, m12 + matrix4x4.m12, m13 + matrix4x4.m13, m14 + matrix4x4.m14,
					m21 + matrix4x4.m21, m22 + matrix4x4.m22, m23 + matrix4x4.m23, m24 + matrix4x4.m24,
					m31 + matrix4x4.m31, m32 + matrix4x4.m32, m33 + matrix4x4.m33, m34 + matrix4x4.m34,
					m41 + matrix4x4.m41, m42 + matrix4x4.m42, m43 + matrix4x4.m43, m44 + matrix4x4.m44
				);
			}

			constexpr Matrix4x4t operator-( const Matrix4x4t& matrix4x4 ) const noexcept {
				return Matrix4x4t(
					m11 - matrix4x4.m11, m12 - matrix4x4.m12, m13 - matrix4x4.m13, m14 - matrix4x4.m14,
					m21 - matrix4x4.m21, m22 - matrix4x4.m22, m23 - matrix4x4.m23, m24 - matrix4x4.m24,
					m31 - matrix4x4.m31, m32 - matrix4x4.m32, m33 - matrix4x4.m33, m34 - matrix4x4.m34,
					m41 - matrix4x4.m41, m42 - matrix4x4.m42, m43 - matrix4x4.m43, m44 - matrix4x4.m44
				);
			}

			constexpr Matrix4x4t operator*( const Matrix4x4t& matrix4x4 ) const noexcept {

				// 行要素と列要素の乗算
				auto mul = []( const Matrix4x4t::Row& row, const Matrix4x4t::Column& column ) {
					T ret = 0;
					for( int i = 0; i < 4; i++ ) {
						ret += row[i] * column[i];
					}
					return ret;
				};

				// 一時変数
				auto r1 = GetRow( 0 );
				auto r2 = GetRow( 1 );
				auto r3 = GetRow( 2 );
				auto r4 = GetRow( 3 );
				auto c1 = matrix4x4.GetColumn( 0 );
				auto c2 = matrix4x4.GetColumn( 1 );
				auto c3 = matrix4x4.GetColumn( 2 );
				auto c4 = matrix4x4.GetColumn( 3 );

				return Matrix4x4t(
					mul( r1, c1 ), mul( r1, c2 ), mul( r1, c3 ), mul( r1, c4 ),
					mul( r2, c1 ), mul( r2, c2 ), mul( r2, c3 ), mul( r2, c4 ),
					mul( r3, c1 ), mul( r3, c2 ), mul( r3, c3 ), mul( r3, c4 ),
					mul( r4, c1 ), mul( r4, c2 ), mul( r4, c3 ), mul( r4, c4 )
				);

			}

			constexpr Math::Vector::Vector4t<T> operator*( const Math::Vector::Vector4t<T>& vec4 ) const noexcept {

				// 列要素と4次元ベクトルの乗算
				auto mul = []( const Matrix4x4t::Column& column, const Math::Vector::Vector4t<T>& _vec4 ) {
					T ret = 0;
					for( int i = 0; i < 4; i++ ) {
						ret += column[i] * _vec4.vec[i];
					}
					return ret;
				};

				// 一時変数
				auto c1 = GetRow( 0 );
				auto c2 = GetRow( 1 );
				auto c3 = GetRow( 2 );
				auto c4 = GetRow( 3 );

				return Math::Vector::Vector4t<T>(
					mul( c1, vec4 ),
					mul( c2, vec4 ),
					mul( c3, vec4 ),
					mul( c4, vec4 )
					);

			}

		//===========================
		// 複合代入演算子
		//===========================

			Matrix4x4t& operator+=( const Matrix4x4t& matrix4x4 ) noexcept {
				*this = *this + matrix4x4;
				return *this;
			}

			Matrix4x4t& operator-=( const Matrix4x4t& matrix4x4 ) noexcept {
				*this = *this - matrix4x4;
				return *this;
			}

			Matrix4x4t& operator*=( const Matrix4x4t& matrix4x4 ) noexcept {
				*this = *this * matrix4x4;
				return *this;
			}

		//===========================
		// 比較演算子
		//===========================

			bool operator==( const Matrix4x4t& matrix4x4 ) const noexcept {
				return Equal<T>(matrix4x4);
			}

			bool operator!=( const Matrix4x4t& matrix4x4 ) const noexcept {
				return NotEqual<T>(matrix4x4);
			}

			//===========================
			// ストリーム出力
			//===========================

			friend std::ostream& operator<<( std::ostream& stream, const Matrix4x4t& matrix4x4 ) {
				stream << matrix4x4.ToString();
				return stream;
			}

#pragma endregion operator

		public:

#pragma region static param

			// 単位行列
			static constexpr Matrix4x4t Identity() noexcept {
				return Matrix4x4t(
					1.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0
				);
			}

			// ゼロ行列
			static constexpr Matrix4x4t Zero() noexcept {
				return Matrix4x4t(
					0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 0.0, 0.0
				);
			}

			// NaN行列　※主にエラー値として返す
			static constexpr Matrix4x4t NaN() noexcept {
				return Matrix4x4t(
					Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>(),
					Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>(),
					Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>(),
					Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>()
				);
			}

#pragma endregion static param

		public:

#pragma region static method


#pragma endregion static method

		public:

#pragma region method

			// 自身の行列を転置する
			Matrix4x4t& Transpose() noexcept {
				*this = TransposeMatrix();
				return *this;
			}

			// 自身の行列を転置した結果を返す
			Matrix4x4t TransposeMatrix() const noexcept {
				return Matrix4x4t(
					m11, m21, m31, m41,
					m12, m22, m32, m42,
					m13, m23, m33, m43,
					m14, m24, m34, m44
				);
			}

			// 自身の行列の逆行列を取得する
			Matrix4x4t InverseMatrix() const {
				// 行列式の結果値
				auto det = Determinant();

				// 行列式の結果値が0なら逆行列は存在しない
				if( Math::Equal<T>( det, 0.0, 0.001 ) ) {
#if _DEBUG
					try {
						throw std::runtime_error( "Matrix4x4::InverseMatrix : Unable to compute inverse matrix." );
					}
					catch( const std::runtime_error& error ) {
						// 弱警告を行う
						Debug::LogWarning( error.what() );
						// NaN行列を返す
						return Matrix4x4t::NaN();
					}
#else
					// 弱警告を行う
					Debug::LogWarning( "Matrix4x4::InverseMatrix : Unable to compute inverse matrix." );
					// NaN行列を返す
					return Matrix4x4t::NaN();
#endif
				}
				else {
					auto invDet = static_cast< T >( 1.0 ) / det;
					return Matrix4x4t<T>(
						invDet * ( m22*m33*m44 + m23 * m34*m42 + m24 * m32*m43 - m22 * m34*m43 - m23 * m32*m44 - m24 * m33*m42 ),
						invDet * ( m12*m34*m43 + m13 * m32*m44 + m14 * m33*m42 - m12 * m33*m44 - m13 * m34*m42 - m14 * m32*m43 ),
						invDet * ( m12*m23*m44 + m13 * m24*m42 + m14 * m22*m43 - m12 * m24*m43 - m13 * m22*m44 - m14 * m23*m42 ),
						invDet * ( m12*m24*m33 + m13 * m22*m34 + m14 * m23*m32 - m12 * m23*m34 - m13 * m24*m32 - m14 * m22*m33 ),

						invDet * ( m21*m34*m43 + m23 * m31*m44 + m24 * m33*m41 - m21 * m33*m44 - m23 * m34*m41 - m24 * m31*m43 ),
						invDet * ( m11*m33*m44 + m13 * m34*m41 + m14 * m31*m43 - m11 * m34*m43 - m13 * m31*m44 - m14 * m33*m41 ),
						invDet * ( m11*m24*m43 + m13 * m21*m44 + m14 * m23*m41 - m11 * m23*m44 - m13 * m24*m41 - m14 * m21*m43 ),
						invDet * ( m11*m23*m34 + m13 * m24*m31 + m14 * m21*m33 - m11 * m24*m33 - m13 * m21*m34 - m14 * m23*m31 ),

						invDet * ( m21*m32*m44 + m22 * m34*m41 + m24 * m31*m42 - m21 * m34*m42 - m22 * m31*m44 - m24 * m32*m41 ),
						invDet * ( m11*m34*m42 + m12 * m31*m44 + m14 * m32*m41 - m11 * m32*m44 - m12 * m34*m41 - m14 * m31*m42 ),
						invDet * ( m11*m22*m44 + m12 * m24*m41 + m14 * m21*m42 - m11 * m24*m42 - m12 * m21*m44 - m14 * m22*m41 ),
						invDet * ( m11*m24*m32 + m12 * m21*m34 + m14 * m22*m31 - m11 * m22*m34 - m12 * m24*m31 - m14 * m21*m32 ),

						invDet * ( m21*m33*m42 + m22 * m31*m43 + m23 * m32*m41 - m21 * m32*m43 - m22 * m33*m41 - m23 * m31*m42 ),
						invDet * ( m11*m32*m43 + m12 * m33*m41 + m13 * m31*m42 - m11 * m33*m42 - m12 * m31*m43 - m13 * m32*m41 ),
						invDet * ( m11*m23*m42 + m12 * m21*m43 + m13 * m22*m41 - m11 * m22*m43 - m12 * m23*m41 - m13 * m21*m42 ),
						invDet * ( m11*m22*m33 + m12 * m23*m31 + m13 * m21*m32 - m11 * m23*m32 - m12 * m21*m33 - m13 * m22*m31 )
						);
				}

			}

			// 自身の行列式の結果を返す
			T Determinant() const noexcept {

				auto c11 = Matrix3x3t<T>(
					m22, m23, m24,
					m32, m33, m34,
					m42, m43, m44
					).Determinant();

				auto c12 = -Matrix3x3t<T>(
					m21, m23, m24,
					m31, m33, m34,
					m41, m43, m44
					).Determinant();

				auto c13 = Matrix3x3t<T>(
					m21, m22, m24,
					m31, m32, m34,
					m41, m42, m44
					).Determinant();

				auto c14 = -Matrix3x3t<T>(
					m21, m22, m23,
					m31, m32, m33,
					m41, m42, m43
					).Determinant();

				return ( m11 * c11 + m12 * c12 + m13 * c13 + m14 * c14 );

			}

			// 指定した行の要素を取得する
			Matrix4x4t::Row GetRow( uint_t row ) const {
				switch( row ) {
					case 0:
					{
						return { { m11, m12, m13, m14 } };
					}
					break;
					case 1:
					{
						return { { m21, m22, m23, m24 } };
					}
					break;
					case 2:
					{
						return { { m31, m32, m33, m34 } };
					}
					break;
					case 3:
					{
						return { { m41, m42, m43, m44 } };
					}
					break;
					default:
					{
						try {
							// 要素外アクセス
							throw std::runtime_error( "Matrix4x4::GetRowMethod : index out of range." );
						}
						catch( const std::runtime_error& error ) {
							std::cout << error.what() << std::endl;
							Debug::LogError( error.what() );
						}
					}
					break;
				}
			}

			// 指定した列の要素を取得する
			Matrix4x4t::Column GetColumn( uint_t column ) const {
				switch( column ) {
					case 0:
					{
						return { { m11, m21, m31, m41 } };
					}
					break;
					case 1:
					{
						return { { m12, m22, m32, m42 } };
					}
					break;
					case 2:
					{
						return { { m13, m23, m33, m43 } };
					}
					break;
					case 3:
					{
						return { { m14, m24, m34, m44 } };
					}
					break;
					default:
					{
						try {
							// 要素外アクセス
							throw std::runtime_error( "Matrix4x4::GetCoumnMethod : index out of range." );
						}
						catch( const std::runtime_error& error ) {
							std::cout << error.what() << std::endl;
							Debug::LogError( error.what() );
						}
					}
					break;
				}
			}

			// 列のデータを直線状に並べたものを取得する
			std::array<T, 12> ColumnMajorOrder() const noexcept {
				return {
					// 1列目
					m11, m21, m31, m41,
					// 2列目
					m12, m22, m32, m42,
					// 3列目
					m13, m23, m33, m43,
					// 4列目
					m14, m24, m34, m44
				};
			}

			// 行のデータを直線状に並べたものを取得する
			std::array<T, 12> RowMajorOrder() const noexcept {
				return {
					// 1行目
					m11, m12, m13, m14,
					// 2行目
					m21, m22, m23, m24,
					// 3行目
					m31, m32, m33, m34,
					// 3行目
					m41, m42, m43, m44,
				};
			}

			// Matrix3x3t<U>型に変換する
			template<class U = T>
			Matrix3x3t<T> ToMatrix3x3() const noexcept {
				return Matrix3x3t<T>(
					// 1行目
					m11, m12, m13,
					// 2行目
					m21, m22, m23,
					// 3行目
					m31, m32, m33
					);
			}

			// データ型を変換したMatrix4x4t型を返す
			template<class U>
			Matrix4x4t<U> TypeConvert() const noexcept {
				return Matrix4x4t<U>( *this );
			}

			// 使用されているデータ型名を取得
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// 行列の値を文字列に変換
			std::string ToString() const noexcept {
				// std::string("1.(")としているのはひとつ前の行の" )\n"と文字列同士を結合するためで
				// こうしないと、const char*同士の加算となってしまうのでコンパイルエラーとなる
				return std::string(
					"Matrix4x4 {\n" +
					std::string( " 1.( " ) + Utility::ToString( m11 ) + ", " + Utility::ToString( m12 ) + ", " + Utility::ToString( m13 ) + ", " + Utility::ToString( m14 ) + " )\n" +
					std::string( " 2.( " ) + Utility::ToString( m21 ) + ", " + Utility::ToString( m22 ) + ", " + Utility::ToString( m23 ) + ", " + Utility::ToString( m24 ) + " )\n" +
					std::string( " 3.( " ) + Utility::ToString( m31 ) + ", " + Utility::ToString( m32 ) + ", " + Utility::ToString( m33 ) + ", " + Utility::ToString( m34 ) + " )\n" +
					std::string( " 4.( " ) + Utility::ToString( m41 ) + ", " + Utility::ToString( m42 ) + ", " + Utility::ToString( m43 ) + ", " + Utility::ToString( m44 ) + " )\n" +
					std::string( "}" )
				);
			}

			// この行列の成分内にNaNが入っているかを調べる
			bool HasNaN() const noexcept {
				for (int_t i = 0; i < 16; i++) {
					if (Math::IsNaN(matrix[i])) return true;
				}
				return false;
			}

			// この行列の成分内に正の無限大/負の無限大が入っているかを調べる
			bool HasInfinity() const noexcept {
				for (int_t i = 0; i < 16; i++) {
					if (Math::IsInfinity(matrix[i])) return true;
				}
				return false;
			}

			// 与えられた行列がこの行列と正確に等しい場合、trueを返します
			template<class U = T>
			bool Equal( const Matrix4x4t& matrix4x4, U epsilon = 0.001 ) const noexcept {
				for (int i = 0; i < 16; i++) {
					if (Math::NotEqual<U>( matrix[i], matrix4x4.matrix[i], epsilon) ) {
						return false;
					}
				}
				return true;
			}

			// 与えられた行列がこの行列と正確に等しくない場合、trueを返します
			template<class U = T>
			bool NotEqual(const Matrix4x4t& matrix4x4, U epsilon = 0.001) const noexcept {
				for (int i = 0; i < 16; i++) {
					if (Math::NotEqual<U>(matrix[i], matrix4x4.matrix[i], epsilon) ) {
						return true;
					}
				}
				return false;
			}

			// 自身のデータ型と比較
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// このクラスのメモリサイズを返す
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

			// シリアライズを行う
			void Serialize( std::string filePath ) const noexcept {
				FileUtility::Text::LoadAndWrite(
					{
						"Type : ", TypeName(), "\n",
						ToString()
					},
					filePath
				);
			}

#pragma endregion method

		};

		// アフィン変換
		namespace Affine {

			template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
			Matrix4x4t<T> RotationX( T degreeRotate ) {
				return Matrix4x4t<T>(
					1.0, 0.0, 0.0, 0.0,
					0.0, Math::Cos( Math::ToRadian<T>( degreeRotate ) ), Math::Sin( Math::ToRadian<T>( degreeRotate ) ), 0.0,
					0.0, -Math::Sin( Math::ToRadian<T>( degreeRotate ) ), Math::Cos( Math::ToRadian<T>( degreeRotate ) ), 0.0,
					0.0, 0.0, 0.0, 1.0
				);
			}

			template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
			Matrix4x4t<T> RotationY( T degreeRotate ) {
				return Matrix4x4t<T>(
					Math::Cos( Math::ToRadian<T>( degreeRotate ) ), 0.0, -Math::Sin( Math::ToRadian<T>( degreeRotate ) ), 0.0,
					0.0, 1.0, 0.0, 0.0,
					Math::Sin( Math::ToRadian<T>( degreeRotate ) ), 0.0, Math::Cos( Math::ToRadian<T>( degreeRotate ) ), 0.0,
					0.0, 0.0, 0.0, 1.0
				);
			}

			template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
			Matrix4x4t<T> RotationZ( T degreeRotate ) {
				return Matrix4x4t<T>(
					Math::Cos( Math::ToRadian<T>( degreeRotate ) ), Math::Sin( Math::ToRadian<T>( degreeRotate ) ), 0.0, 0.0,
					-Math::Sin( Math::ToRadian<T>( degreeRotate ) ), Math::Cos( Math::ToRadian<T>( degreeRotate ) ), 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0
				);
			}

			template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
			Matrix4x4t<T> RotationAll( T degreeRotateX, T degreeRotateY, T degreeRotateZ, Math::RotationOrder eulerOrder ) {
				auto cos_x = Math::Cos( Math::ToRadian<T>( degreeRotateX ) );
				auto cos_y = Math::Cos( Math::ToRadian<T>( degreeRotateY ) );
				auto cos_z = Math::Cos( Math::ToRadian<T>( degreeRotateZ ) );
				auto sin_x = Math::Sin( Math::ToRadian<T>( degreeRotateX ) );
				auto sin_y = Math::Sin( Math::ToRadian<T>( degreeRotateY ) );
				auto sin_z = Math::Sin( Math::ToRadian<T>( degreeRotateZ ) );

				// 指定された計算順番で計算する
				switch( eulerOrder ) {

					case RotationOrder::XYZ:
					{
						return Matrix4x4t<T>(
							cos_y * cos_z, sin_x * sin_y * cos_z + cos_x * sin_z, -cos_x * sin_y * cos_z + sin_x * sin_z, 0.0,
							-cos_y * sin_z, -sin_x * sin_y * sin_z + cos_x * cos_z, cos_x * sin_y * sin_z + sin_x * cos_z, 0.0,
							sin_y, -sin_x * cos_y, cos_x * cos_y, 0.0,
							0.0, 0.0, 0.0, 1.0
						);
					}
					break;
					case RotationOrder::XZY:
					{
						return Matrix4x4t<T>(
							cos_y * cos_z, cos_x * cos_y * sin_z + sin_x * sin_y, sin_x * cos_y * sin_z - cos_x * sin_y, 0.0,
							-sin_z, cos_x * cos_z, sin_x * cos_z, 0.0,
							sin_y * cos_z, cos_x * sin_y * sin_z - sin_x * cos_y, sin_x * sin_y * sin_z + cos_x * cos_y, 0.0,
							0.0, 0.0, 0.0, 1.0
						);
					}
					break;
					case RotationOrder::YXZ:
					{
						return Matrix4x4t<T>(
							sin_x * sin_y * sin_z + cos_y * cos_z, cos_x * sin_z, sin_x * cos_y * sin_z - sin_y * cos_z, 0.0,
							sin_x * sin_y * cos_z - cos_y * sin_z, cos_x * cos_z, sin_x * cos_y * cos_z + sin_y * sin_z, 0.0,
							cos_x * sin_y, -sin_x, cos_x * cos_y, 0.0,
							0.0, 0.0, 0.0, 1.0
						);
					}
					break;
					case RotationOrder::YZX:
					{
						return Matrix4x4t<T>(
							cos_y * cos_z, sin_z, -sin_y * cos_z, 0.0,
							-cos_x * cos_y * sin_z + sin_x * sin_y, cos_x * cos_z, cos_x * sin_y * sin_z + sin_x * cos_y, 0.0,
							sin_x * cos_y * sin_z + cos_x * sin_y, -sin_x * cos_z, -sin_x * sin_y * sin_z + cos_x * cos_y, 0.0,
							0.0, 0.0, 0.0, 1.0
						);
					}
					break;
					case RotationOrder::ZXY:
					{
						return Matrix4x4t<T>(
							-sin_x * sin_y * sin_z + cos_y * cos_z, sin_x * sin_y * cos_z + cos_y * sin_z, -cos_x * sin_y, 0.0,
							-cos_x * sin_z, cos_x * cos_z, sin_x, 0.0,
							sin_x * cos_y * sin_z + sin_y * cos_z, -sin_x * cos_y * cos_z + sin_y * sin_z, cos_x * cos_y, 0.0,
							0.0, 0.0, 0.0, 1.0
						);
					}
					break;
					case RotationOrder::ZYX:
					{
						return Matrix4x4t<T>(
							cos_y * cos_z, cos_y * sin_z, -sin_y, 0.0,
							sin_x * sin_y * cos_z - cos_x * sin_z, sin_x * sin_y * sin_z + cos_x * cos_z, sin_x * cos_y, 0.0,
							cos_x * sin_y * cos_z + sin_x * sin_z, cos_x * sin_y * sin_z - sin_x * cos_z, cos_x * cos_y, 0.0,
							0.0, 0.0, 0.0, 1.0
						);
					}
					break;

				}

			}

			template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
			Matrix4x4t<T> RotationAll( const Math::Vector::Vector3t<T>& degreeRotateXYZ, Math::RotationOrder eulerOrder ) {
				return RotationAll( degreeRotateXYZ.x, degreeRotateXYZ.y, degreeRotateXYZ.z, eulerOrder );
			}

			template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
			Matrix4x4t<T> ArbitraryRotationMatrix( const Math::Vector::Vector3t<T>& rotVec, T degreeRotate ) {

				auto cos_theta = Math::Cos( Math::ToRadian<T>( degreeRotate ) );
				auto sin_theta = Math::Sin( Math::ToRadian<T>( degreeRotate ) );

				return Matrix4x4t<T>(
					rotVec.x * rotVec.x * ( 1 - cos_theta ) + cos_theta, rotVec.x * rotVec.y * ( 1 - cos_theta ) + rotVec.z * sin_theta, rotVec.x * rotVec.z * ( 1 - cos_theta ) - rotVec.y * sin_theta, 0,
					rotVec.x * rotVec.y * ( 1 - cos_theta ) - rotVec.z * sin_theta, rotVec.y * rotVec.y * ( 1 - cos_theta ) + cos_theta, rotVec.y * rotVec.z * ( 1 - cos_theta ) + rotVec.x * sin_theta, 0,
					rotVec.x * rotVec.z * ( 1 - cos_theta ) + rotVec.y * sin_theta, rotVec.y * rotVec.z * ( 1 - cos_theta ) - rotVec.x * sin_theta, rotVec.z * rotVec.z * ( 1 - cos_theta ) + cos_theta, 0,
					0.0, 0.0, 0.0, 1.0
				);

			}

			template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
			Matrix4x4t<T> Transform( T posX, T posY, T posZ ) {
				return Matrix4x4t<T>(
					1.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					posX, posY, posZ, 1.0
				);
			}

			template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
			Matrix4x4t<T> Transform( const Math::Vector::Vector3t<T>& position ) {
				return Matrix4x4t<T>(
					1.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					position.x, position.y, position.z, 1.0
				);
			}

			template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
			Matrix4x4t<T> Scaling( T scaleX, T scaleY, T scaleZ ) {
				return Matrix4x4t<T>(
					scaleX, 0.0, 0.0, 0.0,
					0.0, scaleY, 0.0, 0.0,
					0.0, 0.0, scaleZ, 0.0,
					0.0, 0.0, 0.0, 1.0
				);
			}

			template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
			Matrix4x4t<T> Scaling( const Math::Vector::Vector3t<T>& scale ) {
				return Matrix4x4t<T>(
					scale.x, 0.0, 0.0, 0.0,
					0.0, scale.y, 0.0, 0.0,
					0.0, 0.0, scale.z, 0.0,
					0.0, 0.0, 0.0, 1.0
				);
			}

			template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
			Matrix4x4t<T> Skew( T xyX, T xyY, T xzX, T xzZ, T yzY, T yzZ ) {
				return Matrix4x4t<T>(
					1.0, xyY, xzZ, 0.0,
					xyX, 1.0, yzZ, 0.0,
					xzX, yzY, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0
				);
			}

			template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
			Matrix4x4t<T> Skew( const Math::Vector::Vector2t<T>& xy, const Math::Vector::Vector2t<T>& xz, const Math::Vector::Vector2t<T>& yz ) {
				return Matrix4x4t<T>(
					1.0, xy.y, xz.y, 0.0,
					xy.x, 1.0, yz.y, 0.0,
					xz.x, yz.x, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0
				);
			}

		}

		// ZXY回転の回転行列からの変換
		template<class T, class MatType, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
		Math::Vector::Vector3t<T>RotationMatrixToEuler( const Math::Matrix::Matrix3x3t<MatType>& rotMatrix ) {

			//return Math::Vector::Vector3t<T>(
			//	Math::ToAroundAngle( Math::Asin( rotMatrix.m32 ) * Math::Rad2Deg<T> ),
			//	Math::ToAroundAngle( Math::Atan2( -rotMatrix.m12, rotMatrix.m22 )* Math::Rad2Deg<T> ),
			//	Math::ToAroundAngle( Math::Atan2( -rotMatrix.m31, rotMatrix.m33 )* Math::Rad2Deg<T> )
			//	);


			if( Math::Equal<T>( rotMatrix.m32, 1.0, 0.0001 ) ) {
				return Math::Vector::Vector3t<T>(
					Math::ToAroundAngle(( Math::Pi<T> / 2 ) * Math::Rad2Deg<T>),
					0.0,
					Math::ToAroundAngle(Math::Atan2( rotMatrix.m11, rotMatrix.m21 ) * Math::Rad2Deg<T>)
					);
			}
			else if( Math::Equal<T>( rotMatrix.m32, -1.0, 0.0001 ) ) {
				return Math::Vector::Vector3t<T>(
					Math::ToAroundAngle( ( -Math::Pi<T> / 2 ) * Math::Rad2Deg<T> ),
					0.0,
					Math::ToAroundAngle( Math::Atan2( rotMatrix.m11, rotMatrix.m21 ) * Math::Rad2Deg<T> )
					);
			}
			else {
				return Math::Vector::Vector3t<T>(
					Math::ToAroundAngle( Math::Asin( rotMatrix.m32 ) * Math::Rad2Deg<T> ),
					Math::ToAroundAngle( Math::Atan2( -rotMatrix.m12, rotMatrix.m22 )* Math::Rad2Deg<T> ),
					Math::ToAroundAngle( Math::Atan2( -rotMatrix.m31, rotMatrix.m33 )* Math::Rad2Deg<T> )
					);
			}
			


			//return Math::Vector::Vector3t<T>(
			//	Math::ToAroundAngle(-Math::Atan2( rotMatrix.m32, rotMatrix.m33)* Math::Rad2Deg<T>),
			//	Math::ToAroundAngle(-Math::Asin(-rotMatrix.m31)* Math::Rad2Deg<T>),
			//	Math::ToAroundAngle(-Math::Atan2(rotMatrix.m21, rotMatrix.m11)* Math::Rad2Deg<T>)
			//	);


			//auto y = Math::Atan2( rotMatrix.m31, rotMatrix.m33 ) * Math::Rad2Deg<T>;
			//auto x = Math::Asin( -rotMatrix.m32 * 0.5 ) * Math::Rad2Deg<T>;
			//auto z = Math::Asin( rotMatrix.m12 / Math::Cos( x ) ) * Math::Rad2Deg<T>;
			//if( Math::Less<T>( rotMatrix.m11, 0.0 ) ) z = 180.0 - z;

			//return Math::Vector::Vector3t<T>(
			//	x,
			//	y,
			//	z
			//	);

			//auto z = Math::Atan2( -rotMatrix.m21, rotMatrix.m22 ) * Math::Rad2Deg<T>;
			//auto x = Math::Asin( rotMatrix.m23 ) * Math::Rad2Deg<T>;
			//auto y = Math::Asin( -rotMatrix.m13 / Math::Cos( x ) ) * Math::Rad2Deg<T>;
			//if( Math::Less<T>( rotMatrix.m33, 0.0 ) ) y = 180.0 - y;

			//return Math::Vector::Vector3t<T>(
			//	x,
			//	y,
			//	z
			//	);
		}

	//===================================================
	// float_t型とdouble_t型ででエイリアス宣言
	//===================================================

		using Matrix2x2 = Matrix2x2t<float_t>;
		using Matrix2x2d = Matrix2x2t<double_t>;
		using Matrix3x3 = Matrix3x3t<float_t>;
		using Matrix3x3d = Matrix3x3t<double_t>;
		using Matrix4x4 = Matrix4x4t<float_t>;
		using Matrix4x4d = Matrix4x4t<double_t>;

	}

#pragma endregion MatrixLib


	// クォータニオンライブラリ
#pragma region Quaternion

	namespace Quaternion {

		// Reference

		// 参考サイト
		// 一番優秀：http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/index.htm

		// Slerp
		// http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm

		template<class T>
		class tQuaternion {

		public:

			// x, y, z, w要素のデータ型
			using type = T;

		public:

#pragma region member

			union {
				struct {
					T x;
					T y;
					T z;
					T w;
				};
				struct {
					Math::Vector::Vector3t<T> imaginary;	// 虚部　※ベクトル部と同等
					T real;											// 実部　※スカラー部と同等
				};
				struct {
					Math::Vector::Vector3t<T> vector;		// ベクトル部　※虚部と同等
					T scalar;										// スカラー部　※実部と同等
				};
				struct {
					std::array<T, 4> vec;
				};
			};

#pragma endregion member

		public:

#pragma region constructor

			tQuaternion() : x( 0 ), y( 0 ), z( 0 ), w( 0 ) {};
			tQuaternion( const tQuaternion& qt ) {
				vec = qt.vec;
			}
			tQuaternion( tQuaternion&& qt ) {
				vec = qt.vec;
			}
			tQuaternion( T x, T y, T z, T w ) : x( x ), y( y ), z( z ), w( w ) {};
			tQuaternion( Math::Vector::Vector3t<T> imaginary, T real ) : imaginary( imaginary ), real( real ) {};

			// 型変換を行い要素を構築する（型変換機能付きコンストラクタ）
			template<class U>
			tQuaternion( tQuaternion<U> const& qt ) {
				x = static_cast< T >( qt.x );
				y = static_cast< T >( qt.y );
				z = static_cast< T >( qt.z );
				w = static_cast< T >( qt.w );
			}

#pragma endregion constructor

		public:

#pragma region operator

		//===========================
		// 代入演算子
		//===========================

		// 型変換を行い代入する
			template<class U>
			tQuaternion& operator=( tQuaternion<U> const& qt ) noexcept {
				x = static_cast< T >( qt.x );
				y = static_cast< T >( qt.y );
				z = static_cast< T >( qt.z );
				w = static_cast< T >( qt.w );
				return *this;
			}

			tQuaternion& operator=( const tQuaternion& qt ) noexcept {
				vec = qt.vec;
				return *this;
			}

			tQuaternion& operator=( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				z = scalar;
				w = scalar;
				return *this;
			}

			//===========================
			// ()演算子　※数値を設定する
			//===========================

			tQuaternion& operator()( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				z = scalar;
				w = scalar;
				return *this;
			}

			tQuaternion& operator()( T newX, T newY, T newZ, T newW ) noexcept {
				x = newX;
				y = newY;
				z = newZ;
				w = newW;
				return *this;
			}

			tQuaternion& operator()( Math::Vector::Vector3t<T> newImaginary, T newReal ) noexcept {
				imaginary = newImaginary;
				real = newReal;
				return *this;
			}

			tQuaternion& operator()( const tQuaternion& qt ) noexcept {
				x = qt.x;
				y = qt.y;
				z = qt.z;
				w = qt.w;
				return *this;
			}

			//===========================
			// []演算子　※[0]、[1]、[2]、[3] を使用して x や y や z や w 成分にアクセスします
			//===========================

			T& operator[]( int_t i ) {
				return vec[i];
			}

			//===========================
			// 単項演算子
			//===========================

			tQuaternion operator+() const noexcept {
				return *this;
			}

			tQuaternion operator-() const noexcept {
				return InverseQuaternion();
			}

			//===========================
			// 算術演算子
			//===========================

			tQuaternion operator+( const tQuaternion& qt ) const noexcept {
				return tQuaternion(
					x + qt.x,
					y + qt.y,
					z + qt.z,
					w + qt.w
				);
			}

			tQuaternion operator-( const tQuaternion& qt ) const noexcept {
				return tQuaternion(
					x - qt.x,
					y - qt.y,
					z - qt.z,
					w - qt.w
				);
			}

			tQuaternion operator*( const tQuaternion& qt ) const noexcept {
				// Crossメソッドと同等の処理
#if _IF_LH_COORD
				return tQuaternion(
					// 虚部
					( qt.imaginary * w ) + ( imaginary * qt.w ) + ( qt.imaginary.Cross( imaginary ) ),
					// 実部
					( qt.w * w ) - ( qt.imaginary.Dot( imaginary ) )
				);
#elif _IF_RH_COORD
				return tQuaternion(
					// 虚部
					( imaginary * qt.w ) + ( qt.imaginary * w ) + ( imaginary.Cross( qt.imaginary ) ),
					// 実部
					( w * qt.w ) - ( imaginary.Dot( qt.imaginary ) )
				);
#endif
			}

			tQuaternion operator*( T scalar ) const noexcept {
				return tQuaternion(
					x * scalar,
					y * scalar,
					z * scalar,
					w * scalar
				);
			}

			tQuaternion operator/( T scalar ) const noexcept {
				return tQuaternion(
					x / scalar,
					y / scalar,
					z / scalar,
					w / scalar
				);
			}

			//===========================
			// 複合代入演算子
			//===========================

			tQuaternion& operator+=( const tQuaternion& qt ) {
				*this = *this + qt;
				return *this;
			}

			tQuaternion& operator-=( const tQuaternion& qt ) {
				*this = *this - qt;
				return *this;
			}

			tQuaternion& operator*=( const tQuaternion& qt ) {
				*this = *this * qt;
				return *this;
			}

			tQuaternion& operator*=( T scalar ) {
				*this = *this * scalar;
				return *this;
			}

			tQuaternion& operator/=( T scalar ) {
				*this = *this / scalar;
				return *this;
			}

			//===========================
			// 比較演算子
			//===========================

			bool operator==( const tQuaternion& qt ) const noexcept {
				return Equal<T>(qt);
			}

			bool operator!=( const tQuaternion& qt ) const noexcept {
				return NotEqual<T>(qt);
			}

			//===========================
			// ストリーム出力
			//===========================

			friend std::ostream& operator<<( std::ostream& stream, const tQuaternion& qt ) {
				stream << qt.ToString();
				return stream;
			}

#pragma endregion operator

		public:

#pragma region static param

			// 恒等四元数を取得する
			static constexpr tQuaternion Identity() noexcept {
				return tQuaternion( 0.0, 0.0, 0.0, 1.0 );
			}

			// NaNクォータニオン　※主にエラー値として返す
			static constexpr tQuaternion NaN() noexcept {
				return tQuaternion( Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>() );
			}

#pragma endregion static param

		public:

#pragma region static method

			// クォータニオン同士の線形補間を行います
			// 戻り値：はa～bの間のクォータニオンに制限されます
			static tQuaternion Lerp( const tQuaternion& a, const tQuaternion& b, T t ) {
				t = Math::Clamp01<T>( t );
				return LerpUnclamped( a, b, t );
			}

			// クォータニオン同士の線形補間を行います
			// 戻り値：はa～bの間のクォータニオンに制限されません
			static tQuaternion LerpUnclamped( const tQuaternion& a, const tQuaternion& b, T t ) {
				return a * ( 1.0f - t ) + b * t;
			}

			// クォータニオン同士の球面線形補間を行います
			// 戻り値：はa～bの間のクォータニオンに制限されます
			static tQuaternion Slerp( const tQuaternion& a, const tQuaternion& b, T t ) {
				t = Math::Clamp01<T>( t );
				return SlerpUnclamped( a, b, t );
			}

			// クォータニオン同士の球面線形補間を行います
			// 戻り値：はa～bの間のクォータニオンに制限されません
			static tQuaternion SlerpUnclamped( const tQuaternion& a, const tQuaternion& b, T t ) {
				// Calculate angle between them.
				auto cosHalfTheta = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
				// if a=b or a=-b then theta = 0 and we can return a
				if( Math::Abs<T>( cosHalfTheta ) >= static_cast<T>(1.0) ) {
					return a;
				}
				// Calculate temporary values.
				auto halfTheta = Math::Acos<T>( cosHalfTheta );
				auto sinHalfTheta = Math::Sqrt<T>( static_cast<T>( 1.0 ) - cosHalfTheta * cosHalfTheta );
				// if theta = 180 degrees then result is not fully defined
				// we could rotate around any axis normal to a or b
				if( Math::Abs<T>( sinHalfTheta ) < 0.001 ) { // fabs is floating point absolute
					return tQuaternion(
						( a.x * static_cast<T>( 0.5 ) + b.x * static_cast<T>( 0.5 ) ),
						( a.y * static_cast<T>( 0.5 ) + b.y * static_cast<T>( 0.5 ) ),
						( a.z * static_cast<T>( 0.5 ) + b.z * static_cast<T>( 0.5 ) ),
						( a.w * static_cast<T>( 0.5 ) + b.w * static_cast<T>( 0.5 ) )
					);
				}
				else {
					auto ratioA = Math::Sin<T>( ( 1.0 - t ) * halfTheta ) / sinHalfTheta;
					auto ratioB = Math::Sin<T>( t * halfTheta ) / sinHalfTheta;
					//Debug::Log( "HalfTheta : ", halfTheta, ", ", "SinHalfTheta : ", sinHalfTheta, ", ", "CosHalfTheta : ", cosHalfTheta );
					//Debug::Log( "Sin : ", Math::Sin<T>( ( 1.0 - t ) * halfTheta ) );
					return tQuaternion(
						( a.x * ratioA + b.x * ratioB ),
						( a.y * ratioA + b.y * ratioB ),
						( a.z * ratioA + b.z * ratioB ),
						( a.w * ratioA + b.w * ratioB )
					);
				}
			}

			// 回転行列からクォータニオンを作成します
			template<class U, class MatType>
			static tQuaternion<U> FromRotationMatrix( const Math::Matrix::Matrix3x3t<MatType>& m ) noexcept {
				auto px =  m.matrix3x3[0][0] - m.matrix3x3[1][1] - m.matrix3x3[2][2] + static_cast<MatType>(1.0);
				auto py = -m.matrix3x3[0][0] + m.matrix3x3[1][1] - m.matrix3x3[2][2] + static_cast<MatType>(1.0);
				auto pz = -m.matrix3x3[0][0] - m.matrix3x3[1][1] + m.matrix3x3[2][2] + static_cast<MatType>(1.0);
				auto pw =  m.matrix3x3[0][0] + m.matrix3x3[1][1] + m.matrix3x3[2][2] + static_cast<MatType>(1.0);

				auto selected = 0;
				auto max = px;
				if( max < py ) {
					selected = 1;
					max = py;
				}
				if( max < pz ) {
					selected = 2;
					max = pz;
				}
				if( max < pw ) {
					selected = 3;
					max = pw;
				}

				switch( selected ) {
					case 0:
					{
						auto x = Math::Sqrt( px ) * static_cast< MatType >( 0.5 );
						auto d = static_cast<MatType>(1.0) / ( static_cast<MatType>(4.0) * x );
						return tQuaternion<U>(
							x,
							( m.matrix3x3[1][0] + m.matrix3x3[0][1] ) * d,
							( m.matrix3x3[0][2] + m.matrix3x3[2][0] ) * d,
							( m.matrix3x3[2][1] - m.matrix3x3[1][2] ) * d
						);
					}
					break;
					case 1:
					{
						auto y = Math::Sqrt( py ) * static_cast< MatType >( 0.5 );
						auto d = static_cast<MatType>(1.0) / ( static_cast<MatType>(4.0) * y );
						return tQuaternion<U>(
							( m.matrix3x3[1][0] + m.matrix3x3[0][1] ) * d,
							y,
							( m.matrix3x3[2][1] + m.matrix3x3[1][2] ) * d,
							( m.matrix3x3[0][2] - m.matrix3x3[2][0] ) * d
						);
					}
					break;
					case 2:
					{
						auto z = Math::Sqrt( pz ) * static_cast< MatType >( 0.5 );
						auto d = static_cast<MatType>(1.0) / ( static_cast<MatType>(4.0) * z );
						return tQuaternion<U>(
							( m.matrix3x3[0][2] + m.matrix3x3[2][0] ) * d,
							( m.matrix3x3[2][1] + m.matrix3x3[1][2] ) * d,
							z,
							( m.matrix3x3[1][0] - m.matrix3x3[0][1] ) * d
						);
					}
					case 3:
					{
						auto w = Math::Sqrt( pw ) * 0.5f;
						auto d = static_cast<MatType>(1.0) / ( static_cast<MatType>(4.0) * w );
						return tQuaternion<U>(
							( m.matrix3x3[2][1] - m.matrix3x3[1][2] ) * d,
							( m.matrix3x3[0][2] - m.matrix3x3[2][0] ) * d,
							( m.matrix3x3[1][0] - m.matrix3x3[0][1] ) * d,
							w
						);
					}
				}

				Debug::LogWarning( "conversion of rotation matrix to quaterion is failed." );
			}

			// クォータニオンを回転行列に変換します
			template<class U, class QtType>
			static Math::Matrix::Matrix3x3t<U> ToRotationMatrix( const tQuaternion<QtType>& qt ) {
				// TODO
				return Math::Matrix::Matrix3x3t<U>();
			}

			// オイラー角からクォータニオンを作成します
			template<class U, class VecType>
			static tQuaternion<U> FromEuler( const Math::Vector::Vector3t<VecType>& eulerAngle, Math::RotationOrder rotationOrder = Math::RotationOrder::ZXY ) {

				// 参考サイト：https://qiita.com/take4eng/items/ae487c82a6f7d60ceba8

				
				auto cx = Math::Cos( static_cast<VecType>(0.5) * eulerAngle.x );
				auto sx = Math::Sin( static_cast<VecType>(0.5) * eulerAngle.x );
				auto cy = Math::Cos( static_cast<VecType>(0.5) * eulerAngle.y );
				auto sy = Math::Sin( static_cast<VecType>(0.5) * eulerAngle.y );
				auto cz = Math::Cos( static_cast<VecType>(0.5) * eulerAngle.z );
				auto sz = Math::Sin( static_cast<VecType>(0.5) * eulerAngle.z );



				switch( rotationOrder ) {

					case RotationOrder::XYZ:
					{
						return tQuaternion<U>(
							cx * sy * sz + sx * cy * cz,
							-sx * cy * sz + cx * sy * cz,
							cx * cy * sz + sx * sy * cz,
							-sx * sy * sz + cx * cy * cz
						);
					}
					break;
					case RotationOrder::XZY:
					{
						return tQuaternion<U>(
							-cx * sy * sz + sx * cy * cz,
							cx * sy * cz - sx * cy * sz,
							sx * sy * cz + cx * cy * sz,
							sx * sy * sz + cx * cy * cz
						);
					}
					break;
					case RotationOrder::YXZ:
					{
						return tQuaternion<U>(
							cx * sy * sz + sx * cy * cz,
							-sx * cy * sz + cx * sy * cz,
							cx * cy * sz - sx * sy * cz,
							sx * sy * sz + cx * cy * cz
						);
					}
					break;
					case RotationOrder::YZX:
					{
						return tQuaternion<U>(
							sx * cy * cz + cx * sy * sz,
							sx * cy * sz + cx * sy * cz,
							-sx * sy * cz + cx * cy * sz,
							-sx * sy * sz + cx * cy * cz
						);
					}
					break;
					case RotationOrder::ZXY:
					{
						return tQuaternion<U>(
							-cx * sy * sz + sx * cy * cz,
							cx * sy * cz + sx * cy * sz,
							sx * sy * cz + cx * cy * sz,
							-sx * sy * sz + cx * cy * cz
						);
					}
					break;
					case RotationOrder::ZYX:
					{
						return tQuaternion<U>(
							sx * cy * cz - cx * sy * sz,
							sx * cy * sz + cx * sy * cz,
							-sx * sy * cz + cx * cy * sz,
							sx * sy * sz + cx * cy * cz
						);
					}
					break;

				}

				Debug::LogWarning( "conversion of euler angle to quaterion is failed." );
				

				//===================================================

				//tQuaternion<U> qtx( Math::Cos( eulerAngle.x * 0.5 ), Math::Sin( eulerAngle.x * 0.5 ), 0.0, 0.0 );
				//tQuaternion<U> qty( Math::Cos( eulerAngle.y * 0.5 ), 0.0, Math::Sin( eulerAngle.y * 0.5 ), 0.0 );
				//tQuaternion<U> qtz( Math::Cos( eulerAngle.z * 0.5 ), 0.0, 0.0, Math::Sin( eulerAngle.z * 0.5 ) );

				//tQuaternion<U> qtx( Math::Sin( eulerAngle.x * 0.5 ), 0.0, 0.0, Math::Cos( eulerAngle.x * 0.5 ) );
				//tQuaternion<U> qty( 0.0, Math::Sin( eulerAngle.y * 0.5 ), 0.0, Math::Cos( eulerAngle.y * 0.5 ) );
				//tQuaternion<U> qtz( 0.0, 0.0, Math::Sin( eulerAngle.z * 0.5 ), Math::Cos( eulerAngle.z * 0.5 ) );

				//switch( rotationOrder ) {

				//	case RotationOrder::XYZ:
				//	{
				//		return qtx * qty * qtz;
				//	}
				//	break;
				//	case RotationOrder::XZY:
				//	{
				//		return qtx * qtz * qty;
				//	}
				//	break;
				//	case RotationOrder::YXZ:
				//	{
				//		return qty * qtx * qtz;
				//	}
				//	break;
				//	case RotationOrder::YZX:
				//	{
				//		return qty * qtz * qtx;
				//	}
				//	break;
				//	case RotationOrder::ZXY:
				//	{
				//		return qtz * qtx * qty;
				//	}
				//	break;
				//	case RotationOrder::ZYX:
				//	{
				//		return qtz * qty * qtx;
				//	}
				//	break;

				//}


				//double cy = Math::Cos( eulerAngle.z * 0.5 );
				//double sy = Math::Sin( eulerAngle.z * 0.5 );
				//double cp = Math::Cos( eulerAngle.y * 0.5 );
				//double sp = Math::Sin( eulerAngle.y * 0.5 );
				//double cr = Math::Cos( eulerAngle.x * 0.5 );
				//double sr = Math::Sin( eulerAngle.x * 0.5 );

				//tQuaternion<U> q;
				//q.w = cr * cp * cy + sr * sp * sy;
				//q.x = sr * cp * cy - cr * sp * sy;
				//q.y = cr * sp * cy + sr * cp * sy;
				//q.z = cr * cp * sy - sr * sp * cy;

				//return q;
			}

			// クォータニオンをオイラー角に変換します
			template<class U, class QtType>
			static Math::Vector::Vector3t<U> ToEuler( const tQuaternion<QtType>& qt, Math::RotationOrder rotationOrder = Math::RotationOrder::ZXY ) {

				{
				//auto t1 = 2.0f * qt.x * qt.z + 2.0f * qt.y * qt.w;
				//auto t2 = 2.0f * qt.x * qt.y + 2.0f * qt.z * qt.w;
				//auto t3 = 2.0f * qt.y * qt.z - 2.0f * qt.x * qt.w;
				//auto t4 = 2.0f * qt.x * qt.y - 2.0f * qt.z * qt.w;
				//auto t5 = 2.0f * qt.w * qt.w + 2.0f * qt.x * qt.x;
				//auto t6 = 2.0f * qt.y * qt.z + 2.0f * qt.x * qt.w;
				//auto t7 = 2.0f * qt.w * qt.w + 2.0f * qt.z * qt.z;
				//auto t8 = 2.0f * qt.w * qt.w + 2.0f * qt.y * qt.y;
				//auto t9 = 2.0f * qt.x * qt.z - 2.0f * qt.y * qt.w;


				//switch( rotationOrder ) {

				//	case RotationOrder::XYZ:
				//	{
				//		auto sy = t1;
				//		bool unlocked = Math::Abs( sy ) < 0.99999f;
				//		return Math::Vector::Vector3t<U>(
				//			unlocked ? Math::Atan2( -( t3 ), t7 - 1.0f ) : Math::Atan2( t6, t8 - 1.0f ),
				//			Math::Asin( sy ),
				//			unlocked ? Math::Atan2( -( t4 ), t5 - 1.0f ) : 0
				//		);
				//	}
				//	break;
				//	case RotationOrder::XZY:
				//	{
				//		auto sz = -( t4 );
				//		bool unlocked = Math::Abs( sz ) < 0.99999f;
				//		return Math::Vector::Vector3t<U>(
				//			unlocked ? Math::Atan2( t6, t8 - 1.0f ) : Math::Atan2( -( t3 ), t7 - 1.0f ),
				//			unlocked ? Math::Atan2( t1, t5 - 1.0f ) : 0.0f,
				//			Math::Asin( sz )
				//		);
				//	}
				//	break;
				//	case RotationOrder::YXZ:
				//	{
				//		auto sx = -( t3 );
				//		bool unlocked = Math::Abs( sx ) < 0.99999f;
				//		return Math::Vector::Vector3t<U>(
				//			Math::Asin( sx ),
				//			unlocked ? Math::Atan2( t1, t7 - 1.0f ) : Math::Atan2( -( t9 ), t5 - 1.0f ),
				//			unlocked ? Math::Atan2( t2, t8 - 1.0f ) : 0
				//		);
				//	}
				//	break;
				//	case RotationOrder::YZX:
				//	{
				//		auto sz = t2;
				//		bool unlocked = Math::Abs( sz ) < 0.99999f;
				//		return Math::Vector::Vector3t<U>(
				//			unlocked ? atan2( -( t3 ), t8 - 1.0f ) : 0,
				//			unlocked ? atan2( -( t9 ), t5 - 1.0f ) : atan2( t1, t7 - 1.0f ),
				//			Math::Asin( sz )
				//		);
				//	}
				//	break;
				//	case RotationOrder::ZXY:
				//	{
				//		auto sx = t6;
				//		bool unlocked = Math::Abs( sx ) < 0.99999f;
				//		return Math::Vector::Vector3t<U>(
				//			Math::Asin( sx ),
				//			unlocked ? Math::Atan2( -( t9 ), t7 - 1.0f ) : 0.0f,
				//			unlocked ? Math::Atan2( -( t4 ), t8 - 1.0f )
				//			: Math::Atan2( t2, t5 - 1.0f )
				//		);
				//	}
				//	break;
				//	case RotationOrder::ZYX:
				//	{
				//		auto sy = -( t9 );
				//		bool unlocked = Math::Abs( sy ) < 0.99999f;
				//		return Math::Vector::Vector3t<U>(
				//			unlocked ? Math::Atan2( t6, t7 - 1.0f ) : 0.0f,
				//			Math::Asin( sy ),
				//			unlocked ? Math::Atan2( t2, t5 - 1.0f ) : Math::Atan2( -( t4 ), t8 - 1.0f )
				//		);
				//	}

				//}
				}


				// 回転順番がZXYの場合の変換
				{
					float x = qt.x;
					float y = qt.y;
					float z = qt.z;
					float w = qt.w;

					float x2 = x * x;
					float y2 = y * y;
					float z2 = z * z;

					float xy = x * y;
					float xz = x * z;
					float yz = y * z;
					float wx = w * x;
					float wy = w * y;
					float wz = w * z;

					// 1 - 2y^2 - 2z^2
					float m00 = 1.0f - ( 2.0f * y2 ) - ( 2.0f * z2 );

					// 2xy + 2wz
					float m01 = ( 2.0f * xy ) + ( 2.0f * wz );

					// 2xy - 2wz
					float m10 = ( 2.0f * xy ) - ( 2.0f * wz );

					// 1 - 2x^2 - 2z^2
					float m11 = 1.0f - ( 2.0f * x2 ) - ( 2.0f * z2 );

					// 2xz + 2wy
					float m20 = ( 2.0f * xz ) + ( 2.0f * wy );

					// 2yz+2wx
					float m21 = ( 2.0f * yz ) - ( 2.0f * wx );

					// 1 - 2x^2 - 2y^2
					float m22 = 1.0f - ( 2.0f * x2 ) - ( 2.0f * y2 );


					float tx, ty, tz;

					if( Math::Equal<float>( m21, 1.0f ) ) {
						tx = Math::Pi<float> / 2.0f;
						ty = 0;
						tz = Math::Atan2<float>( m10, m00 );
					}
					else if( Math::Equal<float>( m21, -1.0f ) ) {
						tx = -Math::Pi<float> / 2.0f;
						ty = 0;
						tz = Math::Atan2<float>( m10, m00 );
					}
					else {
						tx = Math::Asin<float>( -m21 );
						ty = Math::Atan2<float>( m20, m22 );
						tz = Math::Atan2<float>( m01, m11 );
					}

					tx *= Math::Rad2Deg<float>;
					ty *= Math::Rad2Deg<float>;
					tz *= Math::Rad2Deg<float>;

					// TODO
					return Math::Vector::Vector3t<U>( tx, ty, tz );
				}


				{

					// 参考サイト：https://www.generacodice.com/jp/articolo/98127/

					//// 2軸分の回転を行う際に使用する
					//auto twoaxisrot = []( U r11, U r12, U r21, U r31, U r32 )->Math::Vector::Vector3t<U> {
					//	return Math::Vector::Vector3t<U>(
					//		Math::Atan2( r11, r12 ),
					//		Math::Acos( r21 ),
					//		Math::Atan2( r31, r32 )
					//		);
					//};

					//// 3軸分の回転を行う際に使用する
					//auto threeaxisrot = []( U r11, U r12, U r21, U r31, U r32 )->Math::Vector::Vector3t<U> {
					//	return Math::Vector::Vector3t<U>(
					//		Math::Atan2( r31, r32 ),
					//		Math::Asin( r21 ),
					//		Math::Atan2( r11, r12 )
					//		);
					//};

					//switch( rotationOrder ) {
					//	case RotationOrder::ZYX:
					//	{
					//		return threeaxisrot( 
					//			2 * ( qt.x*qt.y + qt.w*qt.z ),
					//			qt.w*qt.w + qt.x*qt.x - qt.y*qt.y - qt.z*qt.z,
					//			-2 * ( qt.x*qt.z - qt.w*qt.y ),
					//			2 * ( qt.y*qt.z + qt.w*qt.x ),
					//			qt.w*qt.w - qt.x*qt.x - qt.y*qt.y + qt.z*qt.z
					//			);
					//	}
					//	break;
					//	case RotationOrder::ZXY:
					//	{
					//		return threeaxisrot(
					//			-2 * ( qt.x*qt.y - qt.w*qt.z ),
					//			qt.w*qt.w - qt.x*qt.x + qt.y*qt.y - qt.z*qt.z,
					//			2 * ( qt.y*qt.z + qt.w*qt.x ),
					//			-2 * ( qt.x*qt.z - qt.w*qt.y ),
					//			qt.w*qt.w - qt.x*qt.x - qt.y*qt.y + qt.z*qt.z
					//			);
					//	}
					//	break;
					//	case RotationOrder::YXZ:
					//	{
					//		return threeaxisrot( 
					//			2 * ( qt.x*qt.z + qt.w*qt.y ),
					//			qt.w*qt.w - qt.x*qt.x - qt.y*qt.y + qt.z*qt.z,
					//			-2 * ( qt.y*qt.z - qt.w*qt.x ),
					//			2 * ( qt.x*qt.y + qt.w*qt.z ),
					//			qt.w*qt.w - qt.x*qt.x + qt.y*qt.y - qt.z*qt.z
					//			);
					//	}
					//	break;
					//	case RotationOrder::YZX:
					//	{
					//		return threeaxisrot(
					//			-2 * ( qt.x*qt.z - qt.w*qt.y ),
					//			qt.w*qt.w + qt.x*qt.x - qt.y*qt.y - qt.z*qt.z,
					//			2 * ( qt.x*qt.y + qt.w*qt.z ),
					//			-2 * ( qt.y*qt.z - qt.w*qt.x ),
					//			qt.w*qt.w - qt.x*qt.x + qt.y*qt.y - qt.z*qt.z
					//			);
					//	}
					//	break;
					//	case RotationOrder::XYZ:
					//	{
					//		return threeaxisrot( 
					//			-2 * ( qt.y*qt.z - qt.w*qt.x ),
					//			qt.w*qt.w - qt.x*qt.x - qt.y*qt.y + qt.z*qt.z,
					//			2 * ( qt.x*qt.z + qt.w*qt.y ),
					//			-2 * ( qt.x*qt.y - qt.w*qt.z ),
					//			qt.w*qt.w + qt.x*qt.x - qt.y*qt.y - qt.z*qt.z
					//			);
					//	}
					//	break;
					//	case RotationOrder::XZY:
					//	{
					//		return threeaxisrot(
					//			2 * ( qt.y*qt.z + qt.w*qt.x ),
					//			qt.w*qt.w - qt.x*qt.x + qt.y*qt.y - qt.z*qt.z,
					//			-2 * ( qt.x*qt.y - qt.w*qt.z ),
					//			2 * ( qt.x*qt.z + qt.w*qt.y ),
					//			qt.w*qt.w + qt.x*qt.x - qt.y*qt.y - qt.z*qt.z
					//			);
					//	}
					//	break;
					//}

				}


			}

#pragma endregion static method

		public:

#pragma region method

		// 全ての成分に同一の値を設定
			tQuaternion& Set( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				z = scalar;
				w = scalar;
				return *this;
			}

			// 各軸の成分ごとに値を設定
			tQuaternion& Set( T newX, T newY, T newZ, T newW ) noexcept {
				x = newX;
				y = newY;
				z = newZ;
				w = newW;
				return *this;
			}

			// 虚部と実部ごとに値を設定
			tQuaternion& Set( Math::Vector::Vector3t<T> newImaginary, T newReal ) noexcept {
				imaginary = newImaginary;
				real = newReal;
				return *this;
			}

			// クォータニオンの値をコピー
			tQuaternion& Set( const tQuaternion& qt ) noexcept {
				x = qt.x;
				y = qt.y;
				z = qt.z;
				w = qt.w;
				return *this;
			}

			// このクォータニオンの絶対値を返す
			// ※記事によっては絶対値ではなく長さとしているものもあるため注意
			T Magnitude() const noexcept {
				return Math::FastSqrt( SqrMagnitude() );
			}

			// このクォータニオンの絶対値の2乗を返す
			// ※記事によっては絶対値ではなく長さとしているものもあるため注意
			T SqrMagnitude() const noexcept {
				// 自身の値同士のドット積
				return Dot( *this );
			}

			// クォータニオンの共役を取得する
			tQuaternion ConjugateQuaternion() const noexcept {
				// 共役クォータニオンは
				// 実部はそのままで、虚部がマイナスのものである
				return tQuaternion( -imaginary, real );
			}

			// このクォータニオンの逆クォータニオンを取得する
			tQuaternion InverseQuaternion() const noexcept {
				// 共役クォータニオン / 絶対値の2乗
				return ConjugateQuaternion() / SqrMagnitude();
			}

			// このクォータニオンを正規化する　※このクォータニオンが変化する
			tQuaternion& Normalize() noexcept {
				*this /= Magnitude();
				return *this;
			}

			// このクォータニオンを正規化した結果を返す　※このクォータニオン自体は変化しない
			tQuaternion NormalizedQuaternion() const noexcept {
				return *this / Magnitude();
			}

			// このクォータニオンとのドット積の結果を返す
			T Dot( const tQuaternion& qt ) const noexcept {
				return imaginary.Dot( qt.imaginary ) + real * qt.real;
			}

			// このクォータニオンとのクロス積
			// ※式の結果はクォータニオンの左右が違うと変化する
			// ※記事によっては乗算と記載しているものもあるため注意
			tQuaternion Cross( const tQuaternion& qt ) const noexcept {
#if _IF_LH_COORD
				return tQuaternion(
					// 虚部
					( qt.imaginary * w ) + ( imaginary * qt.w ) + ( qt.imaginary.Cross( imaginary ) ),
					// 実部
					( qt.w * w ) - ( qt.imaginary.Dot( imaginary ) )
				);
#elif _IF_RH_COORD
				return tQuaternion(
					// 虚部
					( imaginary * qt.w ) + ( qt.imaginary * w ) + ( imaginary.Cross( qt.imaginary ) ),
					// 実部
					( w * qt.w ) - ( imaginary.Dot( qt.imaginary ) )
				);
#endif
			}

			// データ型を変換したtQuaternion型を返す
			template<class U>
			tQuaternion<U> TypeConvert() const noexcept {
				return tQuaternion<U>( *this );
			}

			// 使用されているデータ型名を取得
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// ベクトルの値を文字列に変換
			std::string ToString() const noexcept {
				return std::string(
					"Quaternion( "
					+
					Utility::ToString( x )
					+
					", "
					+
					Utility::ToString( y )
					+
					", "
					+
					Utility::ToString( z )
					+
					", "
					+
					Utility::ToString( w )
					+
					" )"
				);
			}

			// 与えられたベクトルがこのベクトルと正確に等しい場合、trueを返します
			template<class U = T>
			bool Equal( const tQuaternion& qt, U epsilon = 0.001 ) const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if (Math::NotEqual<U>( vec[i], qt.vec[i], epsilon )) {
						return false;
					}
				}
				return true;
			}

			// 与えられたベクトルがこのベクトルと正確に等しくない場合、trueを返します
			template<class U = T>
			bool NotEqual(const tQuaternion& qt, U epsilon = 0.001 ) const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if ( Math::NotEqual<U>( vec[i], qt.vec[i], epsilon ) ) {
						return true;
					}
				}
				return false;
			}

			// 自身のデータ型と比較
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// このクラスのメモリサイズを返す
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

#pragma endregion method

		};


	//===================================================
	// float_t型とdouble_t型ででエイリアス宣言
	//===================================================

		using Quaternion = tQuaternion<float_t>;
		using QuaternionD = tQuaternion<double_t>;

	}


#pragma endregion Quaternion

}