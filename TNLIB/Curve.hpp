#pragma once

#include"Math.hpp"

// 曲線ライブラリ
namespace Curve {

	// 参考サイト
	// https://qiita.com/edo_m18/items/643512f27c2b083b47ac

	// 曲線の種類
	// 2次ベジエ
	// 3次ベジエ
	// n次ベジエ
	// 有理ベジエ
	// Bスプライン
	// n次Bスプライン

	template<class T>
	using Vector2t = Math::Vector::Vector2t<T>;

	// ベジェ曲線関数
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline Vector2t<T> Bezier( T t ) {
		return Vector2t<T>();
	}

   // バーンスタイン基底関数
	template<class T = double_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline float Bernstein(int_t n, int_t i, T t)
   {
       return Binomial<T>(n, i) * Math::Pow<T>(t, i) * Math::Pow<T>(1 - t, n - i);
   }

	// 二項係数を計算する
	template<class T = double_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T Binomial( int_t n, int_t k ) {
		return Math::Factorial<T>( n ) / ( Math::Factorial<T>( k ) * Math::Factorial<T>( n - k ) );
	}

}