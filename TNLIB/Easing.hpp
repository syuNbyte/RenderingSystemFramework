#pragma once

#include"Math.hpp"

// イージング関数ライブラリ
namespace Easing {

//===================================================
// 参考サイト
//===================================================
	// イージング関数を使ったUI挙動の作り方	：https://note.com/syun77/n/nd796c574fa96
	// イージング関数チートシート					：https://easings.net/ja
	// Easing Equation								：http://gizma.com/easing/
	// イージング処理の計算式						：https://nakamura001.hatenablog.com/entry/20111117/1321539246
	// GitHub : jquery.easing/jquery.easing.js：https://github.com/gdsmith/jquery.easing/blob/master/jquery.easing.js

//===================================================
// 変数
//===================================================

#pragma region Variable

	template<class T>
	constexpr T piHalf = T( Math::pi<T> / static_cast< T >( 2 ) );

	template<class T>
	constexpr T _0 = T( 0 );
	template<class T>
	constexpr T _0_5 = T( 0.5 );
	template<class T>
	constexpr T _0_75 = T( 0.75 );
	template<class T>
	constexpr T _0_9375 = T( 0.9375 );
	template<class T>
	constexpr T _0_984375 = T( 0.984375 );

	template<class T>
	constexpr T _1 = T( 1 );
	template<class T>
	constexpr T _1_5 = T( 1.5 );
	template<class T>
	constexpr T _1_525 = T( 1.525 );
	template<class T>
	constexpr T _1_70158 = T( 1.70158 );

	template<class T>
	constexpr T _2 = T( 2 );
	template<class T>
	constexpr T _2_25 = T( 2.25 );
	template<class T>
	constexpr T _2_5 = T( 2.5 );
	template<class T>
	constexpr T _2_625 = T( 2.625 );
	template<class T>
	constexpr T _2_75 = T( 2.75 );

	template<class T>
	constexpr T _3 = T( 3 );

	template<class T>
	constexpr T _4 = T( 4 );

	template<class T>
	constexpr T _4_5 = T( 4.5 );

	template<class T>
	constexpr T _5 = T( 5 );

	template<class T>
	constexpr T _7_5625 = T( 7.5625 );

	template<class T>
	constexpr T _8 = T( 8 );

	template<class T>
	constexpr T _10 = T( 10 );

	template<class T>
	constexpr T _10_75 = T( 10.75 );

	template<class T>
	constexpr T _11_125 = T( 11.125 );

	template<class T>
	constexpr T _16 = T( 16 );

	template<class T>
	constexpr T _20 = T( 20 );

	template<class T>
	constexpr T _180 = T( 180 );

	template<class T>
	constexpr T _90 = T( 90 );


	template<class T>
	constexpr T c1 = T( _1_70158<T> );
	template<class T>
	constexpr T c2 = T( _1_70158<T> * _1_525<T> );
	template<class T>
	constexpr T c3 = T( _1_70158<T> +_1<T> );
	template<class T>
	constexpr T c4 = T( Math::pi2<T> / _3<T> );
	template<class T>
	constexpr T c5 = T( Math::pi2<T> / _4_5<T> );

#pragma endregion Variable

//===================================================
// メソッド
//===================================================

#pragma region Quad

	template<class T>
	inline T InQuad( T t ) {
		Math::Clamp01( t );
		return t * t;
	}

	template<class T>
	inline T OutQuad( T t ) {
		Math::Clamp01( t );
		return -t * ( t - _2<T> );
	}

	template<class T>
	inline T InOutQuad( T t ) {

		Math::Clamp01( t );

		t /= _0_5<T>;

		if( t < _1<T> ) {
			return _0_5<T> * t * t;
		}
		else {
			t -= _1<T>;
			return -_0_5<T> * ( t * ( t - _2<T> ) - _1<T> );
		}

	}

	template<class T>
	inline T InQuadAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InQuad<T>( t );
	}

	template<class T>
	inline T OutQuadAtoB( T a, T b, T t ) {
		return a + ( b - a ) * OutQuad( t );
	}

	template<class T>
	inline T InOutQuadAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InOutQuad( t );
	}

#pragma endregion Quad

#pragma region Cubic

	template<class T>
	inline T InCubic( T t ) {
		Math::Clamp01( t );
		return t * t * t;
	}

	template<class T>
	inline T OutCubic( T t ) {
		Math::Clamp01( t );
		t = t - _1<T>;
		return ( t * t * t + _1<T> );
	}

	template<class T>
	inline T InOutCubic( T t ) {

		Math::Clamp01( t );

		t /= _0_5<T>;

		if( t < _1<T> ) {
			return _0_5<T> * t * t * t;
		}
		else {
			t -= _2<T>;
			return _0_5<T> * ( t * t * t + _2<T> );
		}

	}

	template<class T>
	inline T InCubicAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InCubic( t );
	}

	template<class T>
	inline T OutCubicAtoB( T a, T b, T t ) {
		return a + ( b - a ) * OutCubic( t );
	}

	template<class T>
	inline T InOutCubicAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InOutCubic( t );
	}

#pragma endregion Cubic

#pragma region Quart

	template<class T>
	inline T InQuart( T t ) {
		Math::Clamp01( t );
		return t * t * t * t;
	}

	template<class T>
	inline T OutQuart( T t ) {
		Math::Clamp01( t );
		t = t - _1<T>;
		return -( t * t * t * t - _1<T> );
	}

	template<class T>
	inline T InOutQuart( T t ) {

		Math::Clamp01( t );

		if( t < _0_5<T> ) {
			return _8<T> * t * t * t * t;
		}
		else {
			return _1<T> -Math::Pow<T>( -_2<T> * t + _2<T>, _4<T> ) / _2<T>;
		}

	}

	template<class T>
	inline T InQuartAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InQuart( t );
	}

	template<class T>
	inline T OutQuartAtoB( T a, T b, T t ) {
		return a + ( b - a ) * OutQuart( t );
	}

	template<class T>
	inline T InOutQuartAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InOutQuart( t );
	}

#pragma endregion Quart

#pragma region Quint

	template<class T>
	inline T InQuint( T t ) {
		Math::Clamp01( t );
		return t * t * t * t * t;
	}

	template<class T>
	inline T OutQuint( T t ) {
		Math::Clamp01( t );
		t = t - _1<T>;
		return ( t * t * t * t * t + _1<T> );
	}

	template<class T>
	inline T InOutQuint( T t ) {

		Math::Clamp01( t );

		if( t < _0_5<T> ) {
			return _16<T> * t * t * t * t * t;
		}
		else {
			return _1<T> -Math::Pow<T>( -_2<T> * t + _2<T>, _5<T> ) * _0_5<T>;
		}

	}

	template<class T>
	inline T InQuintAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InQuint( t );
	}

	template<class T>
	inline T OutQuintAtoB( T a, T b, T t ) {
		return a + ( b - a ) * OutQuint( t );
	}

	template<class T>
	inline T InOutQuintAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InOutQuint( t );
	}

#pragma endregion Quint

#pragma region Sine

	template<class T>
	inline T InSine( T t ) {
		Math::Clamp01( t );
		return -Math::Cos( t * Easing::piHalf<T> ) + _1<T>;
	}

	template<class T>
	inline T OutSine( T t ) {
		Math::Clamp01( t );
		return Math::Sin( t * Easing::piHalf<T> );
	}

	template<class T>
	inline T InOutSine( T t ) {
		Math::Clamp01( t );
		return -_0_5<T> * ( Math::Cos( t * Math::pi<T> ) - 1 );
	}

	template<class T>
	inline T InSineAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InSine( t );
	}

	template<class T>
	inline T OutSineAtoB( T a, T b, T t ) {
		return a + ( b - a ) * OutSine( t );
	}

	template<class T>
	inline T InOutSineAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InOutSine( t );
	}

#pragma endregion Sine

#pragma region Exponential

	template<class T>
	inline T InExpo( T t ) {

		Math::Clamp01( t );

		if( Math::Equal<T>( t, _0<T> ) ) {
			return _0<T>;
		}
		else {
			return Math::Pow<T>( _2 < T >, _10<T> * t - _10<T> );
		}
	}

	template<class T>
	inline T OutExpo( T t ) {

		Math::Clamp01( t );

		if( Math::Equal<T>( t, _1<T> ) ) {
			return _1<T>;
		}
		else {
			return _1<T> -Math::Pow<T>( _2<T>, -_10<T> * t );
		}
	}

	template<class T>
	inline T InOutExpo( T t ) {

		Math::Clamp01( t );

		if( Math::Equal<T>( t, _0<T> ) ) {
			return _0<T>;
		}
		else if( Math::Equal<T>( t, _1<T> ) ) {
			return _1<T>;
		}
		else if( t < _0_5<T> ) {
			return Math::Pow<T>( _2<T>, _20<T> * t - _10<T> ) / _2<T>;
		}
		else {
			return ( _2<T> -Math::Pow<T>( _2<T>, -_20<T> * t + _10<T> ) ) / _2<T>;
		}

	}

	template<class T>
	inline T InExpoAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InExpo( t );
	}

	template<class T>
	inline T OutExpoAtoB( T a, T b, T t ) {
		return a + ( b - a ) * OutExpo( t );
	}

	template<class T>
	inline T InOutExpoAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InOutExpo( t );
	}

#pragma endregion Exponential

#pragma region Circular

	template<class T>
	inline T InCirc( T t ) {
		Math::Clamp01( t );
		t = Math::Max( Math::Min( t, _1<T> ), -_1<T> );
		return -( Math::Sqrt( _1<T> -t * t ) - _1<T> );
	}

	template<class T>
	inline T OutCirc( T t ) {
		Math::Clamp01( t );
		t -= _1<T>;
		return Math::Sqrt( _1<T> -t * t );
	}

	template<class T>
	inline T InOutCirc( T t ) {

		Math::Clamp01( t );
		t /= _0_5<T>;

		if( t < _1<T> ) {
			return -_0_5<T> * ( Math::Sqrt( _1<T> -t * t ) - _1<T> );
		}
		else {
			t -= _2<T>;
			return _0_5<T> * ( Math::Sqrt( _1<T> -t * t ) + _1<T> );
		}
	}

	template<class T>
	inline T InCircAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InCirc( t );
	}

	template<class T>
	inline T OutCircAtoB( T a, T b, T t ) {
		return a + ( b - a ) * OutCirc( t );
	}

	template<class T>
	inline T InOutCircAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InOutCirc( t );
	}

#pragma endregion Circular

#pragma region Back

	template<class T>
	inline T InBack( T t ) {
		Math::Clamp01( t );
		return t * t * ( ( _1_70158<T> +_1<T> ) * t - _1_70158<T> );
	}

	template<class T>
	inline T OutBack( T t ) {
		Math::Clamp01( t );
		return _1<T> +Easing::c3<T> * Math::Pow<T>( t - 1, 3 ) + Easing::c1<T> * Math::Pow<T>( t - 1, _2<T> );
	}

	template<class T>
	inline T InOutBack( T t ) {
		if( t < _0_5<T> ) {
			return ( Math::Pow<T>( 2 * t, _2<T> ) * ( ( Easing::c2<T> +_1<T> ) * _2<T> * t - Easing::c2<T> ) ) / _2<T>;
		}
		else {
			return ( Math::Pow<T>( 2 * t - 2, _2<T> ) * ( ( Easing::c2<T> +1 ) * ( t * _2<T> -_2<T> ) + Easing::c2<T> ) + _2<T> ) / _2<T>;
		}
	}

	template<class T>
	inline T InBackAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InBack( t );
	}

	template<class T>
	inline T OutBackAtoB( T a, T b, T t ) {
		return a + ( b - a ) * OutBack( t );
	}

	template<class T>
	inline T InOutBackAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InOutBack( t );
	}

#pragma endregion Back

#pragma region Bounce

	template<class T>
	inline T InBounce( T t ) {
		Math::Clamp01( t );
		return  _1<T> -Easing::OutBounce<T>( _1<T> -t );
	}

	template<class T>
	inline T OutBounce( T t ) {

		Math::Clamp01( t );

		if( t < _1<T> / _2_75<T> ) {
			return _7_5625<T> * t * t;
		}
		else if( t < _2<T> / _2_75<T> ) {
			return _7_5625<T> * ( t -= _1_5<T> / _2_75<T> ) * t + _0_75<T>;
		}
		else if( t < _2_5<T> / _2_75<T> ) {
			return _7_5625<T> * ( t -= _2_25<T> / _2_75<T> ) * t + _0_9375<T>;
		}
		else {
			return _7_5625<T> * ( t -= _2_625<T> / _2_75<T> ) * t + _0_984375<T>;
		}

	}

	template<class T>
	inline T InOutBounce( T t ) {

		Math::Clamp01( t );

		if( t < _0_5<T> ) {
			return ( _1<T> -Easing::OutBounce<T>( _1<T> -_2<T> * t ) ) / _2<T>;
		}
		else {
			return ( _1<T> +Easing::OutBounce<T>( _2<T> * t - _1<T> ) ) / _2<T>;
		}
	}

	template<class T>
	inline T InBounceAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InBounce( t );
	}

	template<class T>
	inline T OutBounceAtoB( T a, T b, T t ) {
		return a + ( b - a ) * OutBounce( t );
	}

	template<class T>
	inline T InOutBounceAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InOutBounce( t );
	}

#pragma endregion Bounce

#pragma region Elastic

	template<class T>
	inline T InElastic( T t ) {

		Math::Clamp01( t );

		if( Math::Equal<T>( t, _0<T> ) ) {
			return _0<T>;
		}
		else if( Math::Equal<T>( t, _1<T> ) ) {
			return _1<T>;
		}
		else {
			return -Math::Pow<T>( _2<T>, _10<T> * t - _10<T> ) * Math::Sin( ( t * _10<T> -_10_75<T> ) * Easing::c4<T> );
		}

	}

	template<class T>
	inline T OutElastic( T t ) {

		Math::Clamp01( t );

		if( Math::Equal<T>( t, _0<T> ) ) {
			return _0<T>;
		}
		else if( Math::Equal<T>( t, _1<T> ) ) {
			return _1<T>;
		}
		else {
			return Math::Pow<T>( _2<T>, -_10<T> * t ) * Math::Sin( ( t * _10<T> -_0_75<T> )  * Easing::c4<T> ) + _1<T>;
		}

	}

	template<class T>
	inline T InOutElastic( T t ) {

		Math::Clamp01( t );

		if( Math::Equal<T>( t, _0<T> ) ) {
			return _0<T>;
		}
		else if( Math::Equal<T>( t, _1<T> ) ) {
			return _1<T>;
		}
		else if( t < _0_5<T> ) {
			return -( Math::Pow<T>( 2, _20<T> * t - _10<T> ) * Math::Sin( ( _20<T> * t - _11_125<T> ) * Easing::c5<T> ) ) / _2<T>;
		}
		else {
			return ( Math::Pow<T>( 2, -_20<T> * t + _10<T> ) * Math::Sin( ( _20<T> * t - _11_125<T> ) * ( Math::pi2<T> / _4_5<T> ) ) ) / _2<T> +_1<T>;
		}

	}

	template<class T>
	inline T InElasticAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InElastic( t );
	}

	template<class T>
	inline T OutElasticAtoB( T a, T b, T t ) {
		return a + ( b - a ) * OutElastic( t );
	}

	template<class T>
	inline T InOutElasticAtoB( T a, T b, T t ) {
		return a + ( b - a ) * InOutElastic( t );
	}

#pragma endregion Elastic

#pragma region Linear

	template<class T>
	inline T Linear( T t ) {
		Math::Clamp01( t );
		return t;
	}

	template<class T>
	inline T LinearAtoB( T a, T b, T t ) {
		return a + ( b - a ) * Linear( t );
	}

#pragma endregion Linear

}


