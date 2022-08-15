#pragma once

#include<type_traits>

// enabler : https://qiita.com/kazatsuyu/items/203584ef4cb8b9e52462
// member detecter：https://qiita.com/IgnorantCoder/items/a6cebba4de6cb5901335

// type_traitsを使用しやすいように拡張したライブラリ
namespace TypeTraitsEx {

	// C++17以前にstd::bool_constantはないため実装
	template<bool T>
	using BoolConstant = typename std::integral_constant<bool, T>;

	template<bool T>
	inline constexpr bool BoolConstantV = BoolConstant<T>::value;

	template<class T, class U>
	using CommonType = std::common_type_t<T, U>;

	template<class T>
	inline constexpr bool IsBool = BoolConstantV<
		std::is_same_v<
			std::remove_cv_t<
				std::remove_reference_t<T>
			>,
		bool
		>
	>;

	template<class T>
	using NonNullPtr = std::enable_if_t<
		std::is_null_pointer_v<T>,
		std::nullptr_t
	>;

	// Tが数値で表せる型である場合
	template<class T>
	using IsArithmetic = std::enable_if_t<
		std::is_arithmetic_v<T>,
		std::nullptr_t
	>;
	// 使い方：IsArithmetic<T> = nullptr

	template<class T, class U>
	using IsArithmetic2 = std::enable_if_t<
		BoolConstantV<
		std::is_arithmetic_v<T>
		&&
		std::is_arithmetic_v<U>
		>,
		std::nullptr_t
	>;

	// Tがbool以外の算術型だった場合コンパイル可能
	template<class T>
	using IsNonBoolArithmetic = std::enable_if_t<
		BoolConstantV<
		std::is_arithmetic_v<T>
		&&
		!TypeTraitsEx::IsBool<T>
		>,
		std::nullptr_t
	>;

	// T, Uのどちらもbool以外の算術型だった場合コンパイル可能
	template<class T, class U>
	using IsNonBoolArithmetic2 = std::enable_if_t<
		BoolConstantV<
		std::is_arithmetic_v<T>
		&&
		!TypeTraitsEx::IsBool<T>
		&&
		std::is_arithmetic_v<U>
		&&
		!TypeTraitsEx::IsBool<U>
		>,
		std::nullptr_t
	>;

	// Tが浮動小数点型ならコンパイル可能
	template<class T>
	using IsFloatingPoint = std::enable_if_t<
		std::is_floating_point_v<T>,
		std::nullptr_t
	>;
	// 使い方：IsFloatingPoint<T> = nullptr

	// どちらも浮動小数点型ならコンパイル可能
	template<class T, class U>
	using IsFloatingPoint2 = std::enable_if_t<
		BoolConstantV<
		std::is_floating_point_v<T>
		&&
		std::is_floating_point_v<U>
		>,
		std::nullptr_t
	>;

	// Tが整数型ならコンパイル可能
	template<class T>
	using IsIntegral = std::enable_if_t<
		std::is_integral_v<T>,
		std::nullptr_t
	>;
	// 使い方：IsIntegral<T> = nullptr

	// どちらも整数型ならコンパイル可能
	template<class T, class U>
	using IsIntegral2 = std::enable_if_t<
		BoolConstantV<
		std::is_integral_v<T>
		&&
		std::is_integral_v<U>
		>,
		std::nullptr_t
	>;

	// Tがbool型ではない整数型ならコンパイル可能
	template<class T>
	using IsNonBoolIntegral = std::enable_if_t<
		TypeTraitsEx::BoolConstantV<
		std::is_integral_v<T> && !TypeTraitsEx::IsBool<T>
		>,
		std::nullptr_t
	>;

	// どちらもbool型ではない整数型ならコンパイル可能
	template<class T, class U>
	using IsNonBoolIntegral2 = std::enable_if_t<
		BoolConstantV<
		std::is_integral_v<T> && !TypeTraitsEx::IsBool<T>
		&&
		std::is_integral_v<U> && !TypeTraitsEx::IsBool<U>
		>,
		std::nullptr_t
	>;

	template<class T>
	using IsSigned = std::enable_if_t<
		std::is_signed_v<T>,
		std::nullptr_t
	>;
	// 使い方：IsSigned<T> = nullptr

	template<class T, class U>
	using IsSigned2 = std::enable_if_t<
		BoolConstantV<
		std::is_signed_v<T>
		&&
		std::is_signed_v<U>
		>,
		std::nullptr_t
	>;

	template<class T>
	using IsUnSigned = std::enable_if_t<
		std::is_unsigned_v<T>,
		std::nullptr_t
	>;
	// 使い方：IsUnSigned<T> = nullptr

	template<class T, class U>
	using IsUnSigned2 = std::enable_if_t<
		BoolConstantV<
		std::is_unsigned_v<T>
		&&
		std::is_unsigned_v<U>
		>,
		std::nullptr_t
	>;

	// どちらかの方が浮動小数点型ならコンパイル可能
	template<class T, class U>
	using EitherIsFloatingPoint = std::enable_if_t<
		BoolConstantV<
		std::is_floating_point_v<T>
		||
		std::is_floating_point_v<U>
		>,
		std::nullptr_t
	>;

	// どちらかの方が整数型ならコンパイル可能
	template<class T, class U>
	using EitherIsIntegral = std::enable_if_t<
		BoolConstantV<
		std::is_integral_v<T>
		||
		std::is_integral_v<U>
		>,
		std::nullptr_t
	>;

	// T型がBase型を継承しているデータ型かどうか判定
	template<class T, class Base>
	using IsSameBase = std::enable_if_t<
		std::is_base_of_v<Base, T>,
		std::nullptr_t
	>;

	// このデータ型がbool型ならbool型のパラメータを返す
	template<class T, std::enable_if_t<TypeTraitsEx::IsBool<T>, std::nullptr_t> = nullptr>
	inline bool BoolValue( T value ) {
		return value;
	}

	// このデータ型はbool型ではないので固定値でfalseを返す
	template<class T, std::enable_if_t<!TypeTraitsEx::IsBool<T>, std::nullptr_t> = nullptr>
	inline bool BoolValue( T value ) {
		return 0;
	}

}