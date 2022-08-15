#pragma once

#include<type_traits>

// enabler : https://qiita.com/kazatsuyu/items/203584ef4cb8b9e52462
// member detecter�Fhttps://qiita.com/IgnorantCoder/items/a6cebba4de6cb5901335

// type_traits���g�p���₷���悤�Ɋg���������C�u����
namespace TypeTraitsEx {

	// C++17�ȑO��std::bool_constant�͂Ȃ����ߎ���
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

	// T�����l�ŕ\����^�ł���ꍇ
	template<class T>
	using IsArithmetic = std::enable_if_t<
		std::is_arithmetic_v<T>,
		std::nullptr_t
	>;
	// �g�����FIsArithmetic<T> = nullptr

	template<class T, class U>
	using IsArithmetic2 = std::enable_if_t<
		BoolConstantV<
		std::is_arithmetic_v<T>
		&&
		std::is_arithmetic_v<U>
		>,
		std::nullptr_t
	>;

	// T��bool�ȊO�̎Z�p�^�������ꍇ�R���p�C���\
	template<class T>
	using IsNonBoolArithmetic = std::enable_if_t<
		BoolConstantV<
		std::is_arithmetic_v<T>
		&&
		!TypeTraitsEx::IsBool<T>
		>,
		std::nullptr_t
	>;

	// T, U�̂ǂ����bool�ȊO�̎Z�p�^�������ꍇ�R���p�C���\
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

	// T�����������_�^�Ȃ�R���p�C���\
	template<class T>
	using IsFloatingPoint = std::enable_if_t<
		std::is_floating_point_v<T>,
		std::nullptr_t
	>;
	// �g�����FIsFloatingPoint<T> = nullptr

	// �ǂ�������������_�^�Ȃ�R���p�C���\
	template<class T, class U>
	using IsFloatingPoint2 = std::enable_if_t<
		BoolConstantV<
		std::is_floating_point_v<T>
		&&
		std::is_floating_point_v<U>
		>,
		std::nullptr_t
	>;

	// T�������^�Ȃ�R���p�C���\
	template<class T>
	using IsIntegral = std::enable_if_t<
		std::is_integral_v<T>,
		std::nullptr_t
	>;
	// �g�����FIsIntegral<T> = nullptr

	// �ǂ���������^�Ȃ�R���p�C���\
	template<class T, class U>
	using IsIntegral2 = std::enable_if_t<
		BoolConstantV<
		std::is_integral_v<T>
		&&
		std::is_integral_v<U>
		>,
		std::nullptr_t
	>;

	// T��bool�^�ł͂Ȃ������^�Ȃ�R���p�C���\
	template<class T>
	using IsNonBoolIntegral = std::enable_if_t<
		TypeTraitsEx::BoolConstantV<
		std::is_integral_v<T> && !TypeTraitsEx::IsBool<T>
		>,
		std::nullptr_t
	>;

	// �ǂ����bool�^�ł͂Ȃ������^�Ȃ�R���p�C���\
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
	// �g�����FIsSigned<T> = nullptr

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
	// �g�����FIsUnSigned<T> = nullptr

	template<class T, class U>
	using IsUnSigned2 = std::enable_if_t<
		BoolConstantV<
		std::is_unsigned_v<T>
		&&
		std::is_unsigned_v<U>
		>,
		std::nullptr_t
	>;

	// �ǂ��炩�̕������������_�^�Ȃ�R���p�C���\
	template<class T, class U>
	using EitherIsFloatingPoint = std::enable_if_t<
		BoolConstantV<
		std::is_floating_point_v<T>
		||
		std::is_floating_point_v<U>
		>,
		std::nullptr_t
	>;

	// �ǂ��炩�̕��������^�Ȃ�R���p�C���\
	template<class T, class U>
	using EitherIsIntegral = std::enable_if_t<
		BoolConstantV<
		std::is_integral_v<T>
		||
		std::is_integral_v<U>
		>,
		std::nullptr_t
	>;

	// T�^��Base�^���p�����Ă���f�[�^�^���ǂ�������
	template<class T, class Base>
	using IsSameBase = std::enable_if_t<
		std::is_base_of_v<Base, T>,
		std::nullptr_t
	>;

	// ���̃f�[�^�^��bool�^�Ȃ�bool�^�̃p�����[�^��Ԃ�
	template<class T, std::enable_if_t<TypeTraitsEx::IsBool<T>, std::nullptr_t> = nullptr>
	inline bool BoolValue( T value ) {
		return value;
	}

	// ���̃f�[�^�^��bool�^�ł͂Ȃ��̂ŌŒ�l��false��Ԃ�
	template<class T, std::enable_if_t<!TypeTraitsEx::IsBool<T>, std::nullptr_t> = nullptr>
	inline bool BoolValue( T value ) {
		return 0;
	}

}