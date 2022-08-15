#pragma once
// ���ۂ�C++�̃��C�u�������g�p�����
// ���ʂ�include���������邵�A���������T���Ȃ��Ă͂Ȃ�Ȃ�
// ������������邽�߂̃��C�u����
// Unity��Mathf���C�u�������Q�l�ɍ쐬���Ă���
// Unity.Mathf : https://docs.unity3d.com/jp/current/ScriptReference/Mathf.html

// TODO
// �N���X�����\�b�h�ȊO�͑S�Ă̂̃��\�b�h��inline ��t���Ȃ���LINK2005�G���[�𑗏o���邽�ߕt���邱��



//======================================================================================================
// ���W�n�w��}�N��
//======================================================================================================
// �����ӁF�}�N�����`����ۂ͂��̃w�b�_�̃C���N���[�h�錾�����O�ɒ�`���Ȃ��ƈӖ����Ȃ��A
// �@�@�@�@�Ȃ̂Ŏg�p����t�@�C���̍ŏ㕔�Ő錾���鎖�𐄏�����
// 
// ������W�n�𖾎��I�Ɏg�p����ۂɓK�p����}�N���F#define _MATH_LIB_LH_COORD
// �E����W�n�𖾎��I�Ɏg�p����ۂɓK�p����}�N���F#define _MATH_LIB_RH_COORD
//======================================================================================================

// �����I�Ɏg�p������W�n�}�N������`����Ă��邩�𔻒肵��`����Ă��Ȃ���Έȉ��̃}�N�����`����
#if !defined(_MATH_LIB_LH_COORD) && !defined(_MATH_LIB_RH_COORD)
#define _UNDEFINED_MATH_LIB_COORD
#endif

// �����I�Ŏg�p����}�N�����`����
#if defined(_MATH_LIB_LH_COORD)
// ������W�n�}�N������`����Ă���Ȃ�����I�ȍ�����W�n�}�N�����`
#define _LH_COORD
#define MATH_LIB_COORD "Left Hand Coord"
#elif defined(_MATH_LIB_RH_COORD)
// �E����W�n�}�N������`����Ă���Ȃ�����I�ȉE����W�n�}�N�����`
#define _RH_COORD
#define MATH_LIB_COORD "Right Hand Coord"
#elif defined(_UNDEFINED_MATH_LIB_COORD)
// �ǂ���̍��W�n�����`����Ă��Ȃ��ꍇ�͋����I�ɍ�����W�n��K�p����
#define _LH_COORD
#define MATH_LIB_COORD "Left Hand Coord"
#endif

// ������W�n���𔻒肷������}�N�����`
#ifdef _LH_COORD
#define _IF_LH_COORD 1
#else
#define _IF_LH_COORD 0
#endif

// �E����W�n���𔻒肷������}�N�����`
#ifdef _RH_COORD
#define _IF_RH_COORD 1
#else
#define _IF_RH_COORD 0
#endif



#include<array> // PerlinNoise�Ŏg�p����
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

// ���w�v�Z���C�u����
namespace Math {

//===================================================
// �ϐ�
//===================================================

#pragma region Variable

	// ��
	template<class T>
	constexpr T Pi = T( 3.141592653589793238462643383279 );

	// 1/�΁i�΂̋t���j
	template<class T>
	constexpr T InvPi = T( 1.0 / 3.141592653589793238462643383279 );

	// ��/2
	template<class T>
	constexpr T PiDiv2 = T(3.141592653589793238462643383279 / 2.0);

	// ��/4
	template<class T>
	constexpr T PiDiv4 = T(3.141592653589793238462643383279 / 4.0);


	// �x���烉�W�A���ɕϊ�����
	template<class T>
	constexpr T Deg2Rad = T( 0.017453293 );

	// ���W�A������x�ɕϊ�����
	template<class T>
	constexpr T Rad2Deg = T( 57.29577951 );

	// ���̖�����
	template<class T>
	inline constexpr T Infinity() noexcept {
		return std::numeric_limits<T>::infinity();
	};

// ���̖�����
	template<class T>
	inline constexpr T negativeInfinity() noexcept {
		return -std::numeric_limits<T>::infinity();
	};

// �C�v�V����
	template<class T>
	inline constexpr T Epsilon() noexcept {
		return std::numeric_limits<T>::epsilon();
	};

// NaN�iNot a Number�j���l�ł͕\���ł��Ȃ��ꍇ�Ɏg�p����
	template<class T>
	inline constexpr T NaN() noexcept {
		return std::numeric_limits<T>::quiet_NaN();
	};

// �ŏ��̕\���\�Ȑ��l
	template<class T>
	inline constexpr T MinValue() noexcept {
		return std::numeric_limits<T>::min();
	};

// �ő�̕\���\�Ȑ��l
	template<class T>
	inline constexpr T MaxValue() noexcept {
		return std::numeric_limits<T>::max();
	};

#pragma endregion Variable

//===================================================
// ���\�b�h
//===================================================

	// sin�F���������߂�
#pragma region Sin

	// ���������W�A���ŕ\�������sin�֐�
	template<class T>
	inline T Sin( T radian ) {
		return std::sin( radian );
	}

	// ���������W�A���ŕ\�������sin�֐�
	template<>
	inline float_t Sin( float_t radian ) {
		return std::sin( radian );
	}

	// ���������W�A���ŕ\�������sin�֐�
	template<>
	inline double_t Sin( double_t radian ) {
		return std::sin( radian );
	}

	// ���������W�A���ŕ\�������sin�֐�
	template<>
	inline long_double_t Sin( long_double_t radian ) {
		return std::sinl( radian );
	}


	// �������x�ŕ\�������sin�֐�
	template<class T>
	inline T SinDeg( T degree ) {
		return std::sin( degree * Math::Rad2Deg<T> );
	}

	// �������x�ŕ\�������sin�֐�
	template<>
	inline float_t SinDeg( float_t degree ) {
		return std::sin( degree * Math::Rad2Deg<float_t> );
	}

	// �������x�ŕ\�������sin�֐�
	template<>
	inline double_t SinDeg( double_t degree ) {
		return std::sin( degree * Math::Rad2Deg<double_t> );
	}

	// �������x�ŕ\�������sin�֐�
	template<>
	inline long_double_t SinDeg( long_double_t degree ) {
		return std::sinl( degree * Math::Rad2Deg<long_double_t> );
	}

#pragma endregion Sin

	// cos�F�]�������߂�
#pragma region Cos

	// ���������W�A���ŕ\�������cos�֐�
	template<class T>
	inline T Cos( T value ) {
		return std::cos( value );
	}

	// ���������W�A���ŕ\�������cos�֐�
	template<>
	inline float_t Cos( float_t value ) {
		return std::cosf( value );
	}

	// ���������W�A���ŕ\�������cos�֐�
	template<>
	inline double_t Cos( double_t value ) {
		return std::cos( value );
	}

	// ���������W�A���ŕ\�������cos�֐�
	template<>
	inline long_double_t Cos( long_double_t value ) {
		return std::cosl( value );
	}


	// �������x�ŕ\�������sin�֐�
	template<class T>
	inline T CosDeg( T degree ) {
		return std::cos( degree * Math::Rad2Deg<T> );
	}

	// �������x�ŕ\�������cos�֐�
	template<>
	inline float_t CosDeg( float_t degree ) {
		return std::cos( degree * Math::Rad2Deg<float_t> );
	}

	// �������x�ŕ\�������cos�֐�
	template<>
	inline double_t CosDeg( double_t degree ) {
		return std::cos( degree * Math::Rad2Deg<double_t> );
	}

	// �������x�ŕ\�������cos�֐�
	template<>
	inline long_double_t CosDeg( long_double_t degree ) {
		return std::cosl( degree * Math::Rad2Deg<long_double_t> );
	}

#pragma endregion Cos

	// tan�F�X�������߂�
#pragma region Tan

	// ���������W�A���ŕ\�������tan�֐�
	template<class T>
	inline T Tan( T value ) {
		return std::tan( value );
	}

	// ���������W�A���ŕ\�������tan�֐�
	template<>
	inline float_t Tan( float_t value ) {
		return std::tanf( value );
	}

	// ���������W�A���ŕ\�������tan�֐�
	template<>
	inline double_t Tan( double_t value ) {
		return std::tan( value );
	}

	// ���������W�A���ŕ\�������tan�֐�
	template<>
	inline long_double_t Tan( long_double_t value ) {
		return std::tanl( value );
	}

	// �������x�ŕ\�������tan�֐�
	template<class T>
	inline T TanDeg( T degree ) {
		return std::tan( degree * Math::Rad2Deg<T> );
	}

	// �������x�ŕ\�������tan�֐�
	template<>
	inline float_t TanDeg( float_t degree ) {
		return std::tan( degree * Math::Rad2Deg<float_t> );
	}

	// �������x�ŕ\�������tan�֐�
	template<>
	inline double_t TanDeg( double_t degree ) {
		return std::tan( degree * Math::Rad2Deg<double_t> );
	}

	// �������x�ŕ\�������tan�֐�
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


	// 2�����x�N�g������p�x�����߂�
	template<class T>
	inline T Atan2( T y, T x ) {
		return std::atan2( y, x );
	}

	// 2�����x�N�g������p�x�����߂�
	template<>
	inline float_t Atan2( float_t y, float_t x ) {
		return std::atan2f( y, x );
	}

	// 2�����x�N�g������p�x�����߂�
	template<>
	inline double_t Atan2( double_t y, double_t x ) {
		return std::atan2( y, x );
	}

	// 2�����x�N�g������p�x�����߂�
	template<>
	inline long_double_t Atan2( long_double_t y, long_double_t x ) {
		return std::atan2l( y, x );
	}

#pragma endregion Atan

	// ���v�����߂�
#pragma region Sum

	// ���v�����߂�
	// �����̃��\�b�h�̓R���e�i�N���X�̃C�e���[�^���g�p���Ă��͈̔͂̏W�v���ʂ��擾����
	// beginIter : �W�v�J�n�ꏊ�̃C�e���[�^
	// lastIter : �W�v�I���ꏊ�̃C�e���[�^
	template<class T, class Init>
	inline T Sum( Init beginIter, Init lastIter ) {

		// �C�e���[�^���̗v�f���f�t�H���g�\�z�\�ł͂Ȃ��ꍇ�G���[
		static_assert( 
			std::is_default_constructible_v<typename Init::value_type>,
			"The type is not default constructible."
			);
		// T&�^��Init::value_type�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
		// ���C�e���[�^���̃f�[�^�^���߂�l�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
		static_assert(
			std::is_assignable_v<std::add_lvalue_reference_t<T>, typename Init::value_type>,
			"The data type in the iterator is not changeable to the data type of the return value."
			);

		typename Init::value_type ret = typename Init::value_type();
		for( auto iter = beginIter; iter != lastIter; iter++ ) ret = ret + *iter;
		return static_cast<T>(ret);
	}

	// ���v�����߂�
	// �����̃��\�b�h�̓R���e�i�N���X�̃C�e���[�^���g�p���Ă��͈̔͂̏W�v���ʂ��擾����
	// beginIter : �W�v�J�n�ꏊ�̃C�e���[�^
	// lastIter : �W�v�I���ꏊ�̃C�e���[�^
	// init : �W�v�f�[�^�̏����l
	template<class T, class Init>
	inline T Sum( Init beginIter, Init lastIter, typename Init::value_type init ) {

		// T&�^��Init::value_type�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
		// ���C�e���[�^���̃f�[�^�^���߂�l�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
		static_assert(
			std::is_assignable_v<std::add_lvalue_reference_t<T>, typename Init::value_type>,
			"The data type in the iterator is not changeable to the data type of the return value."
			);

		typename Init::value_type ret = init;
		for( auto iter = beginIter; iter != lastIter; iter++ ) ret = ret + *iter;
		return static_cast< T >( ret );
	}

	// �����ɍ��v����l�̍��v�����߂�
	// �����̃��\�b�h�̓R���e�i�N���X�̃C�e���[�^���g�p���Ă��͈̔͂̏W�v���ʂ��擾����
	// beginIter : �W�v�J�n�ꏊ�̃C�e���[�^
	// lastIter : �W�v�I���ꏊ�̃C�e���[�^
	// cond : �W�v����ۂɎg�p����������莮�F// ���̎����������邱�Ɓu[]( const decltype( v )::value_type& value ) {return true;}�v
	template<class T, class Init, class Cond>
	inline T SumIf( Init beginIter, Init lastIter, Cond cond ) {

		// �C�e���[�^���̗v�f���f�t�H���g�\�z�\�ł͂Ȃ��ꍇ�G���[
		static_assert(
			std::is_default_constructible_v<typename Init::value_type>,
			"The type is not default constructible."
			);
		// T&�^��Init::value_type�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
		// ���C�e���[�^���̃f�[�^�^���߂�l�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
		static_assert(
			std::is_assignable_v<std::add_lvalue_reference_t<T>, typename Init::value_type>,
			"The data type in the iterator is not changeable to the data type of the return value."
			);

		typename Init::value_type ret = typename Init::value_type();
		for( auto iter = beginIter; iter != lastIter; iter++ ) if( cond( *iter ) ) ret = ret + *iter;
		return static_cast< T >( ret );
	}

	// �����ɍ��v����l�̍��v�����߂�
	// �����̃��\�b�h�̓R���e�i�N���X�̃C�e���[�^���g�p���Ă��͈̔͂̏W�v���ʂ��擾����
	// beginIter : �W�v�J�n�ꏊ�̃C�e���[�^
	// lastIter : �W�v�I���ꏊ�̃C�e���[�^
	// init : �W�v�f�[�^�̏����l
	// cond : �W�v����ۂɎg�p����������莮�F// ���̎����������邱�Ɓu[]( const decltype( v )::value_type& value ) {return true;}�v
	template<class T, class Init, class Cond>
	inline T SumIf( Init beginIter, Init lastIter, typename Init::value_type init, Cond cond ) {

		// T&�^��Init::value_type�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
		// ���C�e���[�^���̃f�[�^�^���߂�l�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
		static_assert(
			std::is_assignable_v<std::add_lvalue_reference_t<T>, typename Init::value_type>,
			"The data type in the iterator is not changeable to the data type of the return value."
			);

		typename Init::value_type ret = init;
		for( auto iter = beginIter; iter != lastIter; iter++ ) if( cond( *iter ) ) ret = ret + *iter;
		return static_cast< T >( ret );
	}

#pragma endregion Sum

	// ���ς����߂�
#pragma region Average

	// ���ς����߂�
	// �����̃��\�b�h�̓R���e�i�N���X�̃C�e���[�^���g�p���Ă��͈̔͂̏W�v���ʂ��擾����
	// �����̃��\�b�h�͐��l�^�p�̓��ꉻver
	// beginIter : �W�v�J�n�ꏊ�̃C�e���[�^
	// lastIter : �W�v�I���ꏊ�̃C�e���[�^
	template<class T, class Init>
	inline T Average( Init beginIter, Init lastIter ) {

		// �C�e���[�^���̗v�f���f�t�H���g�\�z�\�ł͂Ȃ��ꍇ�G���[
		static_assert(
			std::is_default_constructible_v<typename Init::value_type>,
			"The type is not default constructible."
			);
		// T&�^��Init::value_type�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
		// ���C�e���[�^���̃f�[�^�^���߂�l�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
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

	// ���ς����߂�
	// �����̃��\�b�h�̓R���e�i�N���X�̃C�e���[�^���g�p���Ă��͈̔͂̏W�v���ʂ��擾����
	// �����̃��\�b�h�͐��l�^�p�̓��ꉻver
	// beginIter : �W�v�J�n�ꏊ�̃C�e���[�^
	// lastIter : �W�v�I���ꏊ�̃C�e���[�^
	// init : �W�v�f�[�^�̏����l
	template<class T, class Init>
	inline T Average( Init beginIter, Init lastIter, typename Init::value_type init ) {

		// T&�^��Init::value_type�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
		// ���C�e���[�^���̃f�[�^�^���߂�l�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
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

	// �����ɍ��v����l�̕��ς����߂�
	// �����̃��\�b�h�̓R���e�i�N���X�̃C�e���[�^���g�p���Ă��͈̔͂̏W�v���ʂ��擾����
	// �����̃��\�b�h�͐��l�^�p�̓��ꉻver
	// beginIter : �W�v�J�n�ꏊ�̃C�e���[�^
	// lastIter : �W�v�I���ꏊ�̃C�e���[�^
	// cond : �W�v����ۂɎg�p����������莮�F// ���̎����������邱�Ɓu[]( const decltype( v )::value_type& value ) {return true;}�v
	template<class T, class Init, class Cond>
	inline T AverageIf( Init beginIter, Init lastIter, Cond cond ) {

		// �C�e���[�^���̗v�f���f�t�H���g�\�z�\�ł͂Ȃ��ꍇ�G���[
		static_assert(
			std::is_default_constructible_v<typename Init::value_type>,
			"The type is not default constructible."
			);
		// T&�^��Init::value_type�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
		// ���C�e���[�^���̃f�[�^�^���߂�l�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
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

	// �����ɍ��v����l�̕��ς����߂�
	// �����̃��\�b�h�̓R���e�i�N���X�̃C�e���[�^���g�p���Ă��͈̔͂̏W�v���ʂ��擾����
	// �����̃��\�b�h�͐��l�^�p�̓��ꉻver
	// beginIter : �W�v�J�n�ꏊ�̃C�e���[�^
	// lastIter : �W�v�I���ꏊ�̃C�e���[�^
	// init : �W�v�f�[�^�̏����l
	// cond : �W�v����ۂɎg�p����������莮�F// ���̎����������邱�Ɓu[]( const decltype( v )::value_type& value ) {return true;}�v
	template<class T, class Init, class Cond>
	inline T AverageIf( Init beginIter, Init lastIter, typename Init::value_type init, Cond cond ) {

		// T&�^��Init::value_type�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
		// ���C�e���[�^���̃f�[�^�^���߂�l�^�ɑ���\�ł͂Ȃ��ꍇ�G���[
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

	// �ݏ�
#pragma region Pow

	// �l��ݏ悵�����ʂ����߂�
	// value : �ݏ悳����l
	// n : �ݏ悳�����
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T Pow( T value, T n ) {
		return static_cast< T >( std::powl( value, n ) );
	}

	// �l��2�悵�����ʂ����߂�
	// value : �ݏ悳����l
	template<class T>
	inline T Pow2( T value ) {
		return value * value;
	}

#pragma endregion Pow

	// �K��
#pragma region Factorial

	// n��K�悵�����ʂ����߂�
	template<class T = double_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T Factorial( int_t n ) {
		T tmp = 1;
		for( T i = 2; i < n; i++ ) tmp *= i;

		return tmp;
	}

#pragma endregion Factorial

	// ���{����ΖړI�̐��l�ɂȂ邩�����߂�
#pragma region Target multiply value

	// ���{����ΖړI�̐��l�ɂȂ邩�����߂�
	template<class T = double_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T TargetMultiply( T value, T targetValue ) {
		return targetValue / value;
	}

#pragma endregion Target multiply value

	// �]��
#pragma region Mod

	// �]����v�Z����
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline T Mod( T value, T modBase ) {
		return value % modBase;
	}

	// �]����v�Z����
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline T Mod( T value, T modBase ) {
		return value - ( Math::Floor( value / modBase ) * modBase );
	}

	// ���l���]������߂鐔���傫���ꍇ�A���̐��̗]���Ԃ�
	// �]������߂鐔�����������ꍇ�A0��Ԃ�
	// value�F���l
	// modBase�F�]������߂鐔
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T GreaterMod( T value, T modBase ) {
		return ( Math::GreaterEqual( value, modBase ) ) ? Math::Mod<T>( value, modBase ) : static_cast< T >( 0.0 );
	}

#pragma endregion Mod

	// ����
#pragma region Fraction

	// �����̌v�Z���s��
	// �߂�l�F�����̌v�Z����
	// molecule : ���q
	// denominator : ����
	template<class T>
	inline T Fraction( T molecule, T denominator ) {
		return static_cast<T>(1.0) / denominator * molecule;
	}

	// value�𕪐��{�������ʂ�Ԃ�
	// �߂�l�F�����{��������
	// value : ���Ƃ̒l
	// molecule : ���q
	// denominator : ����
	template<class T>
	inline T FractionValue( T value, T molecule, T denominator ) {
		return static_cast< T >( 1.0 ) / denominator * molecule * value;
	}

#pragma endregion Fraction

	// ����
#pragma region Difference

	// �������v�Z����
	template<class T>
	inline T Difference( T a, T b ) {
		return Math::Abs( a - b );
	}

#pragma endregion Difference

	// �������̌v�Z�i��j
#pragma region Sqrt

	// �������̌v�Z���s���܂��i��j
	template<class T>
	inline T Sqrt( T value ) {
		return static_cast< T >( std::sqrt( value ) );
	}

	// �����ȕ������̌v�Z���s���܂��i��j
	// ���ʏ�̕������̌v�Z���������ł������x��������4�ʒ��x�܂ł����ۏ؂���Ă��܂���
	template<class T>
	inline T FastSqrt( T value ) {

		//��������( ���[�g )�v�Z�Fhttp://takashiijiri.com/study/miscs/fastsqrt.html

		float_t xHalf = 0.5f * value;
		int_t   tmp = 0x5F3759DF - ( *( int_t* )&value >> 1 ); //initial guess
		float_t xRes = *( float* )&tmp;

		xRes *= ( 1.5f - ( xHalf * xRes * xRes ) );
		xRes *= ( 1.5f - ( xHalf * xRes * xRes ) );//�R�����g�A�E�g���O���Ɛ��x���オ��

		return xRes * value;

	}

#pragma endregion Sqrt

	// ��Βl
#pragma region Abs

	// ��Βl�����߂�
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr, std::enable_if_t<std::is_signed_v<T>>* = nullptr>
	inline T Abs( T value ) {
		return std::abs( value );
	}

	// ��Βl�����߂�
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr, std::enable_if_t<std::is_unsigned_v<T>>* = nullptr>
	inline T Abs( T value ) {
		// �����Ȃ������^�͂���������Βl�Ȃ̂ł��̂܂ܕԂ�
		return value;
	}

	// ��Βl�����߂�
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline T Abs( T value ) {
		return static_cast< T >( std::abs( value ) );
	}

	// ��Βl�����߂�
	template<>
	inline float_t Abs( float_t value ) {
		return std::fabsf( value );
	}

#pragma endregion Abs

	// �傫�����r���ĕԂ�
#pragma region Min, Max

	// ���l�̑傫�����r���A���������̐��l���擾
	template<class T>
	inline T Min( T a, T b ) {
		return ( Math::LessEqual<T>( a, b ) ) ? a : b;
	}

	// ���l�̑傫�����r���A�傫�����̐��l���擾
	template<class T>
	inline T Max( T a, T b ) {
		return ( Math::GreaterEqual<T>( a, b ) ) ? a : b;
	}

#pragma endregion Min, Max

	// �ő���񐔁E�ŏ����{��
#pragma region Gcd, Lcm

	// ��̐��̍ő���񐔂����߂�
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

	// ��̐��̍ŏ����{�������߂�
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

	// ��̐��̍ŏ����{�������߂�
	template<
		class T,
		class U,
		TypeTraitsEx::IsNonBoolArithmetic2<T, U> = nullptr,
		TypeTraitsEx::EitherIsFloatingPoint<T, U> = nullptr
	>
	inline long_double_t Lcm( T a, U b ) {

		long_double_t ad = Math::Abs(a);
		long_double_t bd = Math::Abs(b);

		// ���������_�ȉ��̌������������̌������擾
		auto underDigit = Math::Max(
			Utility::UnderFloatDigit( ad ),
			Utility::UnderFloatDigit( bd )
		);

		// ���������_�ȉ��̌������A10�̗ݏ���s��
		auto pow10 = Math::Pow(
			10,
			underDigit
		);

		// 10�̗ݏ�{����
		ad *= pow10;
		bd *= pow10;

		return Math::Lcm( 
			Math::FloorToInt<ulonglong_t>(ad), Math::FloorToInt<ulonglong_t>(bd) 
		) / static_cast< longlong_t >( pow10 );


		// �ꉞ��ver�̏������o�b�N�A�b�v
		{
			//long_double_t ad = Math::Abs( a );
			//long_double_t bd = Math::Abs( b );

			//// ���������_�ȉ��̌������������̌������擾
			//auto underDigit = Math::Max(
			//	Utility::UnderFloatDigit( ad ),
			//	Utility::UnderFloatDigit( bd )
			//);

			//// ���������_�ȉ��̌������A10�̗ݏ���s��
			//auto pow10 = Math::Pow(
			//	10,
			//	underDigit
			//);

			//// 10�̗ݏ�{����
			//ad *= pow10;
			//bd *= pow10;

			//// �f�t�H���g�̕��������_�ۂߕ������擾
			//auto defType = std::fegetround();
			//std::fesetround( FE_TONEAREST );

			//// ���݂̊ۂߕ����Ő����^�ɕϊ�
			//ad = std::nearbyintl( ad );
			//bd = std::nearbyintl( bd );

			//// ���̕��������_�ۂߕ����ɖ߂�
			//std::fesetround( defType );
			//return Math::Lcm( static_cast< ulonglong_t >( ad ), static_cast< ulonglong_t >( bd ) ) / static_cast< ulonglong_t >( pow10 );
		}

	}

#pragma endregion Gcd, Lcm

	// �p�x�̕ϊ�
#pragma region ConvertAngle

	// �x���烉�W�A���ɕϊ�����
	template<class T>
	inline T ToRadian( T degree ) {
		return degree * Math::Deg2Rad<T>;
	}

	// ���W�A������x�ɕϊ�����
	template<class T>
	inline T ToDegree( T radian ) {
		return radian * Math::Rad2Deg<T>;
	}

	// +180�`-180�ŕ\������Ă���p�x��360�x�ŕ\������
	template<class T>
	inline T ToAroundAngle( T degree ) {
		return ( Math::GreaterEqual<T>( degree, 0.0 ) ) ? degree : 360.0 + static_cast< double_t >( degree );
	}

	// 360�x�ŕ\������Ă���p�x��+180�`-180�ŕ\������
	template<class T>
	inline T ToHalfAngle( T degree ) {
		return ( Math::LessEqual<T>( degree, 180.0 ) ) ? degree : static_cast< double_t >( degree ) - 360.0;
	}

	// �p�x���}0�`+360�x�ȓ��ɐ�������
	template<class T>
	inline T ClampAngle( T degree ) {
		if( Math::GreaterEqual<T>( degree, 0.0 ) ) {
			return ( Math::LessEqual( degree, 360.0 ) ) ? degree : static_cast< double_t >( degree ) - 360.0;
		}
		else {
			return 360.0 - static_cast< double_t >( degree );
		}
	}

	// x, y����p�x�����߂�
	template<class T>
	inline T Atan2Angle( T x, T y ) {
		return Math::ToAroundAngle( Math::Atan2( y, x ) * Math::Rad2Deg<T> );
	}

#pragma endregion ConvertAngle

	// ����������s���A+1�E-1��Ԃ��֐�
#pragma region Sign

	// ����������s���A0�ȏ�Ȃ�+1���A0�����Ȃ�-1��Ԃ�
	template<class T>
	inline T Sign( T value ) {
		return Math::GreaterEqual<T>( value, static_cast<T>(0.0) ) ? +1 : -1;
	}

	// ����������s���A0�ȏ�Ȃ�-1���A0�����Ȃ�+1��Ԃ�
	template<class T>
	inline T InverseSign( T value ) {
		return Math::GreaterEqual<T>( value, static_cast< T >( 0.0 ) ) ? -1 : +1;
	}

#pragma endregion Sign

	// �[�����ǂ������肵�[���Ȃ�[����Ԃ��֐�
#pragma region ZeroIfZero

	// �[�����ǂ������肵�[���Ȃ�[����Ԃ�
	// �[���ł͂Ȃ��Ȃ猳�̒l��Ԃ�
	template<class T>
	inline T ZeroIfZero( T value ) {
		return Math::IsZero<T>( value ) ? static_cast< T >( 0.0 ) : value;
	}

#pragma endregion ZeroIfZero

	// ���`��Ԋ֘A�ilinear interpolation)
#pragma region Lerp

	// a�`b�Ԃ̐��`��Ԃ��s���܂��B
	// �߂�l�Fa�`b�̊Ԃ̐��l
	template<class T>
	inline T Lerp01( T a, T b, T t ) {
		return a + Math::Clamp01( t ) * ( b - a );
	}

	// a�`b�Ԃ̐��`��Ԃ��s���܂��B
	// �f�t�H���g�߂�l�Fa�`b�̊Ԃ̐��l�@��Lerp01�Ɠ�������
	// min max ��ݒ肵���ꍇ�̖߂�l�Fa�`b�̍��� * Math::Clamp(t, min, max);
	template<class T>
	inline T Lerp( T a, T b, T t, T min = 0.0, T max = 1.0 ) {
		return a + Math::Clamp(t, min, max) * ( b - a );
	}

	// a�`b�Ԃ̐��`��Ԃ��s���܂�
	// �߂�l�Fa�`b�̍��� * t
	template<class T>
	inline T LerpUnclamped( T a, T b, T t ) {
		return a + t * ( b - a );
	}

	// a�`b�Ԃ�0�`1�̊Ԃɐ��`��Ԃ��s���܂��B
	// �߂�l : 0�`1�̊Ԃ̐��l
	template<class T>
	inline T InverseLerp01( T a, T b, T value ) {
		return Math::Clamp01(( value - a ) / ( b - a ));
	}

	// a�`b�Ԃ�0�`1�̊Ԃɐ��`��Ԃ��s���܂��B
	// �f�t�H���g�߂�l�F0�`1�̊Ԃ̐��l
	// �߂�l : min max�̊Ԃ̐��l
	template<class T>
	inline T InverseLerp( T a, T b, T value, T min = 0.0, T max = 1.0 ) {
		return Math::Clamp( ( value - a ) / ( b - a ), min, max );
	}

	// a�`b�Ԃ�0�`1�̊Ԃɐ��`��Ԃ��s���܂��B
	// t�ɐ����͂���܂���
	// �߂�l : 0�`1�̊Ԃ̐��l
	template<class T>
	inline T InverseLerpUnclamped( T a, T b, T value ) {
		return ( value - a ) / ( b - a );
	}

#pragma endregion Lerp

	// �X���[�Y�ȕ�Ԃ��s��
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

	// ���̐��l�ȏ�ōŋߖT�̐����l�ŕ\���\�Ȑ��l���擾����
#pragma region Ceil

	// ���̐��l�ȏ�ōŋߖT�̐����l�ŕ\���\�ȕ��������_���l���擾����
	template<class T, TypeTraitsEx::IsFloatingPoint<T> = nullptr>
	inline T Ceil( T value ) {
		return std::ceil( static_cast< T >( value ) );
	}

	template<class T, TypeTraitsEx::IsNonBoolIntegral<T> = nullptr>
	inline T Ceil( T value ) {
		return std::ceil( static_cast< T >( value ) );
	}

	// ���̐��l�ȏ�ōŋߖT�̐����l���擾����
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
		// ���̂܂ܕԂ�
		return value;
	}

#pragma endregion Ceil

	// ���̐��l�ȉ��ōŋߖT�̐����l�ŕ\���\�Ȑ��l���擾����
#pragma region Floor

	// ���̐��l�ȉ��ōŋߖT�̐����l�ŕ\���\�ȕ��������_���n���擾����
	template<class T, TypeTraitsEx::IsFloatingPoint<T> = nullptr>
	inline T Floor( T value ) {
		return std::floor( static_cast< T >( value ) );
	}

	// ���̐��l�ȉ��ōŋߖT�̐����l�ŕ\���\�ȕ��������_���n���擾����
	template<class T, TypeTraitsEx::IsNonBoolIntegral<T> = nullptr>
	inline T Floor( T value ) {
		// ���̂܂ܕԂ�
		return value;
	}

	// ���̐��l�ȉ��ōŋߖT�̐����l���擾����
	template<
		class T = longlong_t,
		class U,
		TypeTraitsEx::IsNonBoolIntegral<T> = nullptr,
		TypeTraitsEx::IsFloatingPoint<U> = nullptr
	>
	inline T FloorToInt( U value ) {
		return std::floor( static_cast< U >( value ) );
	}

	// ���̐��l�ȉ��ōŋߖT�̐����l���擾����
	template<class T, TypeTraitsEx::IsNonBoolIntegral<T> = nullptr>
	inline T FloorToInt( T value ) {
		// ���̂܂ܕԂ�
		return value;
	}

#pragma endregion Floor

	// ���l��͈͓��ɐ�������
#pragma region Clamp

	// std::clamp��Clmap01�̍ۂɗ�O�𑗏o�����̂�
	// ��O���e�͕]���s�\�Ȑ��l���n���ꂽ���Ƃɂ����̂Ƃ̂��Ƃł����A
	// �]���s�\�Ȑ��l�ł͂Ȃ��̂�
	// ���O�ŗ����Ƃ��Ď������܂���

	// min�`max�͈̔͂ɊԂɒl�𐧌����܂�
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T Clamp( T value, T min, T max ) {
		if( value <= min ) return min;
		if( value >= max ) return max;
		return value;
	}

	// 0.0�`1.0�͈̔͂ɊԂɒl�𐧌����܂��@���O�a�֐�
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T Clamp01( T value ) {
		return Math::Clamp( value, static_cast< T >( 0.0 ), static_cast< T >( 1.0 ) );
	}

	// 0�ȏ�ɒl�𐧌����܂�
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T ClampToPlus( T value ) {
		return Math::Clamp( value, static_cast< T >( 0.0 ), Math::MaxValue<T>() );
	}

	// 0�����ɒl�𐧌����܂�
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline T ClampToMinus( T value ) {
		return Math::Clamp( value, Math::MinValue<T>(), -Math::Epsilon<T>() );
	}

	// 0�����ɒl�𐧌����܂�
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline T ClampToMinus( T value ) {
		return Math::Clamp( value, Math::MinValue<T>(), -1 );
	}

	// min�ȏ�ɒl�𐧌����܂�
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T ClampToMin( T value, T min ) {
		return Math::Clamp( value, min, Math::MaxValue<T>() );
	}

	// max�ȉ��ɒl�𐧌����܂�
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T ClampToMax( T value, T max ) {
		return Math::Clamp( value, Math::MinValue<T>(), max );
	}

#pragma endregion Clamp

		// �l�̌ܓ��A�؂�グ�A�؂�̂�
#pragma region Round, RoundUp, RoundDown

	// �S�Ă�Round�n���\�b�h�œ����A���S���Y�����g�p����
	// �ꕔ�g�p����֐��ɂ���ē��e���قȂ邽�߂��̕����Ɏg�p����֐��͐ݒ�ł���悤�ɂ��Ă���
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline double _Internal_Round_Method( T value, int_t digit, std::function<double_t( double_t )> _roundFunc ) {

		if( Math::Sign( digit ) > 0 && digit != 0 ) {
			// �I���������������_�ȏ�̏ꍇ
			// 10��-n��
			auto mul = std::pow( 10, -digit );
			return _roundFunc( value * mul ) / mul;
		}
		else if( Math::Sign( digit ) < 0 && digit != 0 ) {

#if _DEBUG

			try {
				// ���������_�^�̏ꍇ�̂݌v�Z
				if( Math::IsFloat( value ) ) {
					// �I���������������_�ȉ��̏ꍇ
					// 10��+n��
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
				// ���̂܂ܕԂ�
				return value;
			}

#else

			// ���������_�^�̏ꍇ�̂݌v�Z
			if( Math::IsFloat( value ) ) {
				// �I���������������_�ȉ��̏ꍇ
				// 10��+n��
				auto mul = std::pow( 10, -digit );
				return _roundFunc( value * mul ) / mul;
			}
			else {
				// ���̂܂ܕԂ�
				return value;
			}

#endif
		}
		else {
			// �I�������O�̏ꍇ
			return _roundFunc( value );
		}

	}

	// �I�����ꂽ���Ŏl�̌ܓ����s��
	// value : ���l�f�[�^
	// digit : �擾���錅�@���O�͂P�̈ʂ̌����w���A-�P�͕��������_�ȉ����ʂ��w��
	// ���w�肪�Ȃ��ꍇ�̓f�t�H���g��1�̈ʂ��l�̌ܓ�����
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T Round( T value, int_t digit = 0 ) {
		return Math::_Internal_Round_Method( value, digit, []( double_t v ) {return std::round( v ); } );
	}

	// �I�����ꂽ���Ő؂�グ���s��
	// value : ���l�f�[�^
	// digit : �擾���錅�@���O�͂P�̈ʂ̌����w���A-�P�͕��������_�ȉ����ʂ��w��
	// ���w�肪�Ȃ��ꍇ�̓f�t�H���g��1�̈ʂ�؂�グ��
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T RoundUp( T value, int_t digit = 0 ) {
		return Math::_Internal_Round_Method( value, digit, []( double_t v ) {return std::ceil( v ); } );
	}

	// �I�����ꂽ���Ő؂�̂Ă��s��
	// value : ���l�f�[�^
	// digit : �擾���錅�@���O�͂P�̈ʂ̌����w���A-�P�͕��������_�ȉ����ʂ��w��
	// ���w�肪�Ȃ��ꍇ�̓f�t�H���g��1�̈ʂ�؂�̂Ă�
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T RoundDown( T value, int_t digit = 0 ) {
		return static_cast< T >( Math::_Internal_Round_Method( value, digit, []( double_t v ) {return std::floor( v ); } ) );
	}


#pragma endregion Round, RoundUp, RoundDown

	// �����𐶐�����
#pragma region Random

	// �����𐶐�����
	// min : �����̍ŏ����z�͈�
	// max : �����̍ő啪�z�͈�
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline T Random( T min, T max ) {

		// cppref:uniform_int_distribution�Fhttps://cpprefjp.github.io/reference/random/uniform_int_distribution.html

		// 64bit
#if _WIN64
		std::random_device random_device;						// ��Ƀ����_���ȃV�[�h�l���쐬����f�o�C�X
		std::mt19937_64 random_engine( random_device() );	// 64�r�b�g�Ń����Z���k�c�C�X�^�[
		std::uniform_int_distribution<> dist( min, max );	// �����^�̎w��͈͂ň�l���z���s��
#else
		std::random_device random_device;						// ��Ƀ����_���ȃV�[�h�l���쐬����f�o�C�X
		std::mt19937 random_engine( random_device() );		// 32�r�b�g�Ń����Z���k�c�C�X�^�[
		std::uniform_int_distribution<> dist( min, max );	// �����^�̎w��͈͂ň�l���z���s��
#endif

		return dist( random_engine );
	}

	// �����𐶐�����
	// min : �����̍ŏ����z�͈�
	// max : �����̍ő啪�z�͈�
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline T Random( T min, T max ) {

		// cppref:uniform_real_distribution�Fhttps://cpprefjp.github.io/reference/random/uniform_real_distribution.html

#if _WIN64
		std::random_device random_device;						// ��Ƀ����_���ȃV�[�h�l���쐬����f�o�C�X
		std::mt19937_64 random_engine( random_device() );	// 64�r�b�g�Ń����Z���k�c�C�X�^�[
		std::uniform_real_distribution<> dist( min, max );	// ���������_�^�̎w��͈͂ň�l���z���s��
#else
		std::random_device random_device;						// ��Ƀ����_���ȃV�[�h�l���쐬����f�o�C�X
		std::mt19937 random_engine( random_device() );		// 32�r�b�g�Ń����Z���k�c�C�X�^�[
		std::uniform_real_distribution<> dist( min, max );	// ���������_�^�̎w��͈͂ň�l���z���s��
#endif

		return dist( random_engine );
	}


	// xorshift128�̃A���S���Y�����g�p��������������
	inline ulong_t RandomXorShift128() {

		// �ȉ��̃T�C�g�ɂ�xorshift128�ɂ��Ẳ�����ڂ��Ă���
		// �Œ�V�[�h�l�̎Q�l�T�C�g�Fhttps://tech.ckme.co.jp/cpp/cpp_xorshift.shtml
		// �@�σV�[�h�l�ɂ���ꍇ�Fhttp://hexadrive.sblo.jp/article/63660775.html

		// �Œ�V�[�h�l
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

	// xorshift128�̃A���S���Y�����g�p��0.0�`1.0�͈̔͂ŗ������擾����
	inline double_t Random01XorShift128() {
		return static_cast< double_t >( Math::RandomXorShift128() ) / static_cast< double_t >( Math::MaxValue<ulong_t>() );
	}

	// 0.0�`1.0�͈̔͂ŗ������擾����
	// template param : <T = ���������_�^�̃f�[�^�^, Resolution = ����\>
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

	// �p�[�����m�C�Y�𐶐�����
#pragma region PerlinNoise

	template<class T = double_t>
	class _PerlinNoiseClass {

	private:

		//�����o�ϐ�
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

		//SEED�l��ݒ肷��
		void setSeed( const std::uint_fast32_t seed_ ) {
			for( std::size_t i {}; i < 256; ++i ) {
				this->p[i] = static_cast< Pint >( i );
			}

			//p[0�`255]�͈̔͂��V���b�t��
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


		//�I�N�^�[�u�����m�C�Y���擾����
		template <typename... Args>
		T noise( const Args... args_ ) const noexcept {
			return this->setNoise( args_... ) * 0.5 + 0.5;
		}

		//�I�N�^�[�u�L��m�C�Y���擾����
		template <typename... Args>
		T octaveNoise( const std::size_t octaves_, const Args... args_ ) const noexcept {
			return this->setOctaveNoise( octaves_, args_... ) * 0.5 + 0.5;
		}

	};

	// �p�[�����m�C�Y���g�p����@���͈͂�0.0�`1.0�͈̔͂Ŏw�肷��
	inline double_t PerlinNoise( double_t x = 0.0, double_t y = 0.0, double_t z = 0.0 ) {
		static const _PerlinNoiseClass<double_t> perlin( Math::Random<uint_t>( 0, 10000 ) ); // �V�[�h�l�������_���ɐݒ�
		return perlin.noise(
			Math::Clamp01( x ),
			Math::Clamp01( y ),
			Math::Clamp01( z )
		);
	}

	inline double_t PerlinNoiseOctave( const std::size_t octaves, double_t x = 0.0, double_t y = 0.0, double_t z = 0.0 ) {
		static const _PerlinNoiseClass<double_t> perlin( Math::Random<uint_t>( 0, 10000 ) ); // �V�[�h�l�������_���ɐݒ�
		return perlin.octaveNoise(
			octaves,
			Math::Clamp01( x ),
			Math::Clamp01( y ),
			Math::Clamp01( z )
		);
	}

#pragma endregion PerlinNoise

//===================================================
// ���胁�\�b�h
//===================================================

#pragma region JudghMethod

	// ���̐��l�����̖�����E���̖����傩�𔻒肷��
	// �������ǂ��炩�̖�����Ȃ�true
	template<class T>
	inline bool IsInfinity( T value ) {
		return std::isinf( value );
	}

	// ���̐��l��NaN�iNot a Number�j���𔻒肷��
	// ��NaN�Ȃ�true
	template<class T>
	inline bool IsNaN( T value ) {
		return std::isnan( value );
	}

	// ���̐��l��0�ɋߎ����Ă��邩���肷��
	// �[���Ȃ�true
	template<class T>
	inline bool IsZero( T value, T epsilon = Math::Epsilon<T>() ) {
		return Math::Equal<T>( value, static_cast< T >( 0.0 ), epsilon );
	}

	// ���̐��l���L���Ȑ��l���𔻒肷��
	// �������ǂ���̖�����ł��Ȃ��ANaN�iNot a Number�j�ł��Ȃ��ꍇtrue
	template<class T>
	inline bool IsFinite( T value ) {
		return std::isfinite( value );
	}

	// ���̐��l���������𔻒肷��
	// �����Ȃ�true
	// ��0�������̂���true�ƂȂ�F�ڂ����́u0�̋��v�Ō���
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline bool IsEven( T value ) {
		return ( value % 2 == 0 );
	}

	// ���̐��l������𔻒肷��
	// ��Ȃ�true
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline bool IsOddnum( T value ) {
		return ( value % 2 == 1 );
	}

	// ���l�^���ǂ����𔻒肷��
	// ���l�^�Ȃ�true
	template<class T>
	inline bool IsNumber( T value ) {
		return ( std::is_arithmetic_v<T> ) ? true : false;
	}

	// ���̐��l�������^���ǂ����𔻒肷��
	// �����^�Ȃ�true
	template<class T>
	inline bool IsInt( T value ) {
		return ( std::is_integral_v<T> ) ? true : false;
	}

	// ���̐��l�����������_�^���ǂ����𔻒肷��
	// ���������_�^�Ȃ�true
	template<class T>
	inline bool IsFloat( T value ) {
		return ( std::is_floating_point_v<std::remove_reference_t<T>> ) ? true : false;
	}

	// ���̐��l�����鐔�̔{�����ǂ����𔻒肷��
	// value��mulBase�̔{���Ȃ�true
	template<class T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	inline bool IsMultiply( T value, T mulBase ) {
		return ( mulBase % value == 0 ) ? true : false;
	}

	// ���̐��l�����鐔�̔{�����ǂ����𔻒肷��
	// value��mulBase�̔{���Ȃ�true
	template<class T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	inline bool IsMultiply( T value, T mulBase ) {
		return ( Math::Mod( value, mulBase ) == 0 ) ? true : false;
	}

	// ���̐��l�����̐����ǂ����𔻒肷��@��0�����̐��ɓ���
	// ���l�����̐��Ȃ�true
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool IsPlus( T value ) {
		return ( Math::Sign( value ) > 0 );
	}

	// ���̐��l�����̐����ǂ����𔻒肷��@��0�̏ꍇ�͐��̐��ɓ��邽��false�ƂȂ�
	// ���l�����̐��Ȃ�true
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool IsMinus( T value ) {
		return ( Math::Sign( value ) < 0 );
	}


#pragma endregion JudghMethod

//===================================================
// ���l��r
//===================================================

	// ��r���s��
#pragma region Compare

	// �덷���l���������l����i=�j
	// a��b�̌덷��epsilon�ȉ��Ȃ瓯��
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool Equal( T a, T b, T epsilon = Math::Epsilon<T>() ) {
		return ( Math::Abs( a - b ) <= epsilon );
	}

	// �덷���l�������񓙒l����i!=�j
	// a��b�̌덷��epsilon�ȉ��Ȃ瓯��
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool NotEqual( T a, T b, T epsilon = Math::Epsilon<T>() ) {
		return !( Math::Abs( a - b ) <= epsilon );
	}

	// �덷���l��������Ȃ蔻��i>�j
	// a��b�̌덷��epsilon�ȏォ�Aa�̕����傫���Ȃ�true
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool Greater( T a, T b, T epsilon = Math::Epsilon<T>() ) {
		return ( a > b && Math::NotEqual( a, b, epsilon ) );
	}

	// �덷���l��������Ȃ�C�R�[������i>=�j
	// a��b�̌덷��epsilon�ȉ����Aa�̕����傫���E���l�Ȃ�true
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool GreaterEqual( T a, T b, T epsilon = Math::Epsilon<T>() ) {
		return ( a >= b || Math::Equal( a, b, epsilon ) );
	}

	// �덷���l���������Ȃ蔻��i<�j
	// a��b�̌덷��epsilon�ȏォ�Aa�̕����������Ȃ�true
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool Less( T a, T b, T epsilon = Math::Epsilon<T>() ) {
		return ( a < b && Math::NotEqual( a, b, epsilon ) );
	}

	// �덷���l���������Ȃ�C�R�[������i<=�j
	// a��b�̌덷��epsilon�ȉ����Aa�̕����������E���l�Ȃ�true
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool LessEqual( T a, T b, T epsilon = Math::Epsilon<T>() ) {
		return ( a <= b || Math::Equal( a, b, epsilon ) );
	}

	// �덷���l��������Ȃ菬�Ȃ蔻��
	// self��min���傫��max��菬�������𔻒肷��
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool GreaterLess(T min, T self, T max, T epsilon = Math::Epsilon<T>()) {
		return (Math::Less(min, self, epsilon) && Math::Less(self, max, epsilon));
	}

	// �덷���l��������Ȃ�C�R�[�����Ȃ�C�R�[������
	// self��min�ȏ��max�ȉ����𔻒肷��
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline bool GreaterLessEqual(T min, T self, T max, T epsilon = Math::Epsilon<T>()) {
		return (Math::LessEqual(min, self, epsilon) && Math::LessEqual(self, max, epsilon));
	}

#pragma endregion Compare

//===================================================
// ��{�I�Ȑ��l�v�Z
//===================================================

	// ��{�I�Ȑ��l�v�Z
#pragma region Basic Numerical Calculations

	// ���Z
	// �߂�l�Fa + b
	template<class T>
	inline T Add( T a, T b ) {
		return a + b;
	}

	// ���Z
	// �߂�l�Fa - b
	template<class T>
	inline T Sub( T a, T b ) {
		return a - b;
	}

	// ��Z
	// �߂�l�Fa * b
	template<class T>
	inline T Mul( T a, T b ) {
		return a * b;
	}

	// ���Z
	// �߂�l�Fa / b
	template<class T>
	inline T Div( T a, T b ) {
		return a / b;
	}

#pragma endregion Basic Numerical Calculations

	// �x�N�g���E�s��ȂǂŎg�p�����ėp�I�ȃN���X�E���\�b�h
#pragma region VecMathUtil

	// ��]���Ԃ��`����񋓒萔
	enum class RotationOrder {
		XYZ,
		XZY,
		YXZ,
		YZX,
		ZXY,
		ZYX
	};

#pragma endregion VecMathUtil


	// �x�N�g�����C�u����
#pragma region VectorLib

	// Reference
	// �Q�l�T�C�g
	// https://zenn.dev/ryutorion/articles/game-math-vector

	// �x�N�g�����C�u����
	namespace Vector {

		// �O���錾
		template<class T>
		class Vector1t;
		template<class T>
		class Vector2t;
		template<class T>
		class Vector3t;
		template<class T>
		class Vector4t;

		// �ꎟ���̃x�N�g����\���f�[�^�^
		template<class T>
		class Vector1t {

		public:

			// x�v�f�̃f�[�^�^
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

			// �^�ϊ����s���v�f���\�z����i�^�ϊ��@�\�t���R���X�g���N�^�j
			template<class U>
			Vector1t( Vector1t<U> const& vec1 ) {
				x = static_cast< T >( vec1.x );
			}

#pragma endregion constructor

		public:

#pragma region operator

		//===========================
		// ������Z�q
		//===========================

			// �^�ϊ����s���������
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
		// ()���Z�q�@�����l��ݒ肷��
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
		// �P�����Z�q
		//===========================

			Vector1t operator+() const noexcept {
				return *this;
			}

			Vector1t operator-() const noexcept {
				return Vector1t( -x );
			}

		//===========================
		// �Z�p���Z�q
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
		// ����������Z�q
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
		// ��r���Z�q
		//===========================

			bool operator==( const Vector1t& vec1 ) const noexcept {
				return Equal<T>(vec1);
			}

			bool operator!=( const Vector1t& vec1 ) const noexcept {
				return NotEqual<T>(vec1);
			}

			//===========================
			// �X�g���[���o��
			//===========================

			friend std::ostream& operator<<( std::ostream& stream, const Vector1t& vec1 ) {
				stream << vec1.ToString();
				return stream;
			}

#pragma endregion operator

		public:

#pragma region method

			// �S�Ă̐����ɓ���̒l��ݒ�
			Vector1t& Set( T scalar ) noexcept {
				x = scalar;
				return *this;
			}

			// �S�Ă̐����ɓ���̒l�����Z
			Vector1t& SetRelative(T diff) noexcept {
				x += diff;
				return *this;
			}

			// �t�x�N�g�����擾
			Vector1t& Inverse() noexcept {
				*this = -*this;
				return *this;
			}

			// ���̃x�N�g������toVector�܂ł̃x�N�g�������߂�
			Vector1t GoToVector( const Vector1t& toVector ) const noexcept {
				return toVector - *this;
			}

			// �f�[�^�^��ϊ�����Vector1t�^��Ԃ�
			template<class U>
			Vector1t<U> TypeConvert() const noexcept {
				return Vector1t<U>( *this );
			}

			// �g�p����Ă���f�[�^�^�����擾
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// �x�N�g���̒l�𕶎���ɕϊ�
			std::string ToString() const noexcept {
				return std::string(
					"Vector1( "
					+
					Utility::ToString( x )
					+
					" )"
				);
			}

			// ���̃x�N�g���̐�������NaN�������Ă��邩�𒲂ׂ�
			bool HasNaN() const noexcept {
				return Math::IsNaN(x);
			}

			// ���̃x�N�g���̐������ɐ��̖�����/���̖����傪�����Ă��邩�𒲂ׂ�
			bool HasInfinity() const noexcept {
				return Math::IsInfinity(x);
			}

			// �^����ꂽ�x�N�g�������̃x�N�g���Ɛ��m�ɓ������ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool Equal( const Vector1t& vec1, U epsilon = 0.001 ) const noexcept {
				return Math::Equal<U>(x, vec1.x, epsilon);
			}

			// �^����ꂽ�x�N�g�������̃x�N�g���Ɛ��m�ɓ������Ȃ��ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool NotEqual(const Vector1t& vec1, U epsilon = 0.001) const noexcept {
				return Math::NotEqual<U>(x, vec1.x, epsilon);
			}

			// ���g�̃f�[�^�^�Ɣ�r
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// ���̃N���X�̃������T�C�Y��Ԃ�
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

			// �V���A���C�Y���s��
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

		// �񎟌��x�N�g����\���f�[�^�^
		template<class T>
		class Vector2t {

		public:

			// x, y�v�f�̃f�[�^�^
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

			// �^�ϊ����s���v�f���\�z����i�^�ϊ��@�\�t���R���X�g���N�^�j
			template<class U>
			Vector2t( Vector2t<U> const& vec2 ) {
				x = static_cast< T >( vec2.x );
				y = static_cast< T >( vec2.y );
			}

#pragma endregion constructor

		public:

#pragma region operator

		//===========================
		// ������Z�q
		//===========================

			// �^�ϊ����s���������
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
		// ()���Z�q�@�����l��ݒ肷��
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
		// []���Z�q�@��[0]�A[1] ���g�p���� x �� y �����ɃA�N�Z�X���܂�
		//===========================

			T& operator[]( int_t i ) {
				return vec[i];
			}

		//===========================
		// �P�����Z�q
		//===========================

			Vector2t operator+() const noexcept {
				return *this;
			}

			Vector2t operator-() const noexcept {
				return Vector2t( -x, -y );
			}

		//===========================
		// �Z�p���Z�q
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
		// ����������Z�q
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
		// ��r���Z�q
		//===========================

			bool operator==( const Vector2t& vec2 ) const noexcept {
				return Equal<T>(vec2);
			}

			bool operator!=( const Vector2t& vec2 ) const noexcept {
				return NotEqual<T>(vec2);
			}

			//===========================
			// �X�g���[���o��
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
			// NaN�x�N�g���@����ɃG���[�l�Ƃ��ĕԂ�
			static constexpr Vector2t NaN() noexcept {
				return Vector2t( Math::NaN<T>(), Math::NaN<T>() );
			}

#pragma endregion static param

		public:

#pragma region static method

			//static T Length( Vector2t vec2 ) {

			//}

			static Vector2t Lerp( const Vector2t& a, const Vector2t& b, T t ) noexcept {
				// a ���� b �܂ł̃x�N�g���̃X�J���[�{�������̂�a�x�N�g���ɉ��Z����
				return a + ( a.GoToVector( b ) * Math::Clamp01<T>( t ) );
			}

			static Vector2t LerpUnclamped( const Vector2t& a, const Vector2t& b, T t ) noexcept {
				// a ���� b �܂ł̃x�N�g���̃X�J���[�{�������̂�a�x�N�g���ɉ��Z����
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

			// �S�Ă̐����ɓ���̒l��ݒ�
			Vector2t& Set( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				return *this;
			}

			// �e���̐������Ƃɒl��ݒ�
			Vector2t& Set( T newX, T newY ) noexcept {
				x = newX;
				y = newY;
				return *this;
			}

			// �x�N�g���̒l���R�s�[
			Vector2t& Set( const Vector2t& vec2 ) noexcept {
				x = vec2.x;
				y = vec2.y;
				return *this;
			}

			// �S�Ă̐����ɓ���̒l�����Z
			Vector2t& SetRelative( T diff ) noexcept {
				x += diff;
				y += diff;
				return *this;
			}

			// �e���̐������Ƃɒl�����Z
			Vector2t& SetRelative( T diffX, T diffY ) noexcept {
				x += diffX;
				y += diffY;
				return *this;
			}

			// �x�N�g���̒l�����Z
			Vector2t& SetRelative( const Vector2t& vec2 ) noexcept {
				x += vec2.x;
				y += vec2.y;
				return *this;
			}

			// �x�N�g���̒������擾
			T Length() const noexcept {
				return Math::FastSqrt( SqrLength() );
			}

			// �x�N�g���̒�����2����擾
			T SqrLength() const noexcept {
				return ( x * x + y * y );
			}

			// �x�N�g�����m�̋������擾
			T Distance( const Vector2t& vec2 ) const noexcept {
				return GoToVector( vec2 ).Length();
			}

			// �x�N�g�����m�̋�����2����擾
			T SqrDistance( const Vector2t& vec2 ) const noexcept {
				return GoToVector( vec2 ).SqrLength();
			}

			// �t�x�N�g�����擾
			Vector2t& Inverse() noexcept {
				*this = -*this;
				return *this;
			}

			Vector2t InverseVector() const noexcept {
				return -*this;
			}

			// ���̃x�N�g���𐳋K������@�����̃x�N�g�����ω�����
			Vector2t& Normalize() noexcept {
				*this /= Length();
				return *this;
			}

			// ���̃x�N�g���𐳋K���������ʂ�Ԃ��@�����̃x�N�g�����͕̂ω����Ȃ�
			Vector2t NormalizedVector() const noexcept {
				return *this / Length();
			}

			// ���̃x�N�g���Ƃ̃h�b�g�ς̌��ʂ�Ԃ�
			T Dot( const Vector2t& vec2 ) const noexcept {
				return ( x * vec2.x + y * vec2.y );
			}

			// ���̃x�N�g���Ƃ̃N���X�ς̌��ʂ�Ԃ�
			T Cross( const Vector2t& vec2 ) const noexcept {
				return x * vec2.y - y * vec2.x;
			}

			// ���̃x�N�g���Ƃ̊Ԃ̊p�x�����߂�
			T Angle( const Vector2t& vec2 ) {
				return Math::Acos( Dot( vec2 ) );
			}

			// ���̃x�N�g������toVector�܂ł̃x�N�g�������߂�
			Vector2t GoToVector( const Vector2t& toVector ) const noexcept {
				return toVector - *this;
			}

			// Vector3t<U>�^�ɕϊ�����
			template<class U = T>
			Math::Vector::Vector3t<U> ToVector3() const noexcept {
				return Math::Vector::Vector3t<U>(
					x,
					y,
					0
					);
			}

			// �f�[�^�^��ϊ�����Vector2t�^��Ԃ�
			template<class U>
			Vector2t<U> TypeConvert() const noexcept {
				return Vector2t<U>( *this );
			}

			// �g�p����Ă���f�[�^�^�����擾
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// �x�N�g���̒l�𕶎���ɕϊ�
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

			// ���̃x�N�g���̐�������NaN�������Ă��邩�𒲂ׂ�
			bool HasNaN() const noexcept {
				for (int_t i = 0; i < 2; i++) {
					if (Math::IsNaN(vec[i])) return true;
				}
				return false;
			}

			// ���̃x�N�g���̐������ɐ��̖�����/���̖����傪�����Ă��邩�𒲂ׂ�
			bool HasInfinity() const noexcept {
				for (int_t i = 0; i < 2; i++) {
					if (Math::IsInfinity(vec[i])) return true;
				}
				return false;
			}

			// �^����ꂽ�x�N�g�������̃x�N�g���Ɛ��m�ɓ������ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool Equal( const Vector2t& vec2, U epsilon = 0.001) const noexcept {
				for (int_t i = 0; i < 2; i++) {
					if (Math::NotEqual<U>(vec[i], vec2.vec[i], epsilon)) {
						return false;
					}
				}
				return true;
			}

			// �^����ꂽ�x�N�g�������̃x�N�g���Ɛ��m�ɓ������Ȃ��ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool NotEqual(const Vector2t& vec2, U epsilon = 0.001) const noexcept {
				for (int_t i = 0; i < 2; i++) {
					if (Math::NotEqual<U>(vec[i], vec2.vec[i], epsilon)) {
						return true;
					}
				}
				return false;
			}

			// ���g�̃f�[�^�^�Ɣ�r
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// ���̃N���X�̃������T�C�Y��Ԃ�
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

			// �V���A���C�Y���s��
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

		// �O�����x�N�g����\���f�[�^�^
		template<class T>
		class Vector3t {

		public:

			// x, y, z�v�f�̃f�[�^�^
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

			// �^�ϊ����s���v�f���\�z����i�^�ϊ��@�\�t���R���X�g���N�^�j
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
		// ������Z�q
		//===========================

			// �^�ϊ����s���������
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
		// ()���Z�q�@�����l��ݒ肷��
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
		// []���Z�q�@��[0]�A[1]�A[2] ���g�p���� x �� y �� z �����ɃA�N�Z�X���܂�
		//===========================

			T& operator[]( int_t i ) {
				return vec[i];
			}

		//===========================
		// �P�����Z�q
		//===========================

			Vector3t operator+() const noexcept {
				return *this;
			}

			Vector3t operator-() const noexcept {
				return Vector3t( -x, -y, -z );
			}

		//===========================
		// �Z�p���Z�q
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
		// ����������Z�q
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
		// ��r���Z�q
		//===========================

			bool operator==( const Vector3t& vec3 ) const noexcept {
				return Equal<T>(vec3);
			}

			bool operator!=( const Vector3t& vec3 ) const noexcept {
				return NotEqual<T>(vec3);
			}

		//===========================
		// �X�g���[���o��
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
			// NaN�x�N�g���@����ɃG���[�l�Ƃ��ĕԂ�
			static constexpr Vector3t NaN() noexcept {
				return Vector3t( Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>() );
			}

#pragma endregion static param

		public:

#pragma region static method

			// 3�����x�N�g���Ԃ̐��`���
			static Vector3t Lerp( const Vector3t& a, const Vector3t& b, T t ) noexcept {
				// a ���� b �܂ł̃x�N�g���̃X�J���[�{�������̂�a�x�N�g���ɉ��Z����
				return a + ( a.GoToVector( b ) * Math::Clamp01<T>(t) );
			}

			// 3�����x�N�g���Ԃ̐��`���
			static Vector3t LerpUnclamped( const Vector3t& a, const Vector3t& b, T t ) noexcept {
				// a ���� b �܂ł̃x�N�g���̃X�J���[�{�������̂�a�x�N�g���ɉ��Z����
				return a + ( a.GoToVector( b ) * t );
			}

#pragma endregion static method

		public:

#pragma region method

			// �S�Ă̐����ɓ���̒l��ݒ�
			Vector3t& Set( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				z = scalar;
				return *this;
			}

			// �e���̐������Ƃɒl��ݒ�
			Vector3t& Set( T newX, T newY, T newZ ) noexcept {
				x = newX;
				y = newY;
				z = newZ;
				return *this;
			}

			// �x�N�g���̒l���R�s�[
			Vector3t& Set( const Vector3t& vec3 ) noexcept {
				x = vec3.x;
				y = vec3.y;
				z = vec3.z;
				return *this;
			}

			// �S�Ă̐����ɓ���̒l�����Z
			Vector3t& SetRelative(T diff) noexcept {
				x += diff;
				y += diff;
				z += diff;
				return *this;
			}

			// �e���̐������Ƃɒl�����Z
			Vector3t& SetRelative(T diffX, T diffY, T diffZ) noexcept {
				x += diffX;
				y += diffY;
				z += diffZ;
				return *this;
			}

			// �x�N�g���̒l�����Z
			Vector3t& SetRelative(const Vector3t& vec3) noexcept {
				x += vec3.x;
				y += vec3.y;
				z += vec3.z;
				return *this;
			}

			// �x�N�g���̒������擾
			T Length() const noexcept {
				return Math::FastSqrt( SqrLength() );
			}

			// �x�N�g���̒�����2����擾
			T SqrLength() const noexcept {
				return ( x * x + y * y + z * z );
			}

			// �x�N�g�����m�̋������擾
			T Distance( const Vector3t& vec3 ) const noexcept {
				return GoToVector( vec3 ).Length();
			}

			// �x�N�g�����m�̋�����2����擾
			T SqrDistance( const Vector3t& vec3 ) const noexcept {
				return GoToVector( vec3 ).SqrLength();
			}

			// ���̃x�N�g�����t�x�N�g���ɂ���@�����̃x�N�g�����ω�����
			Vector3t& Inverse() noexcept {
				*this = -*this;
				return *this;
			}

			// ���̃x�N�g�����t�x�N�g���ɂ������ʂ�Ԃ��@�����̃x�N�g�����͕̂ω����Ȃ�
			Vector3t InverseVector() const noexcept {
				return -*this;
			}

			// ���̃x�N�g���𐳋K������@�����̃x�N�g�����ω�����
			Vector3t& Normalize() noexcept {
				*this /= Length();
				return *this;
			}

			// ���̃x�N�g���𐳋K���������ʂ�Ԃ��@�����̃x�N�g�����͕̂ω����Ȃ�
			Vector3t NormalizedVector() const noexcept {
				return *this / Length();
			}

			// ���̃x�N�g���Ƃ̃h�b�g�ς̌��ʂ�Ԃ�
			T Dot( const Vector3t& vec3 ) const noexcept {
				return (x * vec3.x + y * vec3.y + z * vec3.z);
			}

			// ���̃x�N�g���Ƃ̃N���X�ς̌��ʂ�Ԃ�
			Vector3t Cross( const Vector3t& vec3 ) const noexcept {
				return Vector3t(
					y * vec3.z - z * vec3.y,
					z * vec3.x - x * vec3.z,
					x * vec3.y - y * vec3.x
				);
			}

			// ���̃x�N�g���Ƃ̊Ԃ̊p�x�����߂�
			T Angle( const Vector3t& vec3 ) {
				return Math::Acos( Dot( vec3 ) );
			}

			// ���̃x�N�g������toVector�܂ł̃x�N�g�������߂�
			Vector3t GoToVector( const Vector3t& toVector ) const noexcept {
				return toVector - *this;
			}

			// Vector2t<U>�^�ɕϊ�����
			template<class U = T>
			Math::Vector::Vector2t<U> ToVector2() const noexcept {
				return Math::Vector::Vector2t<U>(
					x,
					y
					);
			}

			// Vector4t<U>�^�ɕϊ�����
			template<class U = T>
			Math::Vector::Vector4t<U> ToVector4() const noexcept {
				return Math::Vector::Vector4t<U>(
					x,
					y,
					z,
					0
					);
			}

			// �f�[�^�^��ϊ�����Vector3t�^��Ԃ�
			template<class U>
			Vector3t<U> TypeConvert() const noexcept {
				return Vector3t<U>( *this );
			}

			// �g�p����Ă���f�[�^�^�����擾
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// �x�N�g���̒l�𕶎���ɕϊ�
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

			// ���̃x�N�g���̐�������NaN�������Ă��邩�𒲂ׂ�
			bool HasNaN() const noexcept {
				for (int_t i = 0; i < 3; i++) {
					if (Math::IsNaN(vec[i])) return true;
				}
				return false;
			}

			// ���̃x�N�g���̐������ɐ��̖�����/���̖����傪�����Ă��邩�𒲂ׂ�
			bool HasInfinity() const noexcept {
				for (int_t i = 0; i < 3; i++) {
					if (Math::IsInfinity(vec[i])) return true;
				}
				return false;
			}

			// �^����ꂽ�x�N�g�������̃x�N�g���Ɛ��m�ɓ������ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool Equal( const Vector3t& vec3, U epsilon = 0.001) const noexcept {
				for (int_t i = 0; i < 3; i++) {
					if (Math::NotEqual<U>(vec[i], vec3.vec[i], epsilon)) {
						return false;
					}
				}
				return true;
			}

			// �^����ꂽ�x�N�g�������̃x�N�g���Ɛ��m�ɓ������Ȃ��ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool NotEqual(const Vector3t& vec3, U epsilon = 0.001) const noexcept {
				for (int_t i = 0; i < 3; i++) {
					if (Math::NotEqual<U>(vec[i], vec3.vec[i], epsilon)) {
						return true;
					}
				}
				return false;
			}

			// ���g�̃f�[�^�^�Ɣ�r
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// ���̃N���X�̃������T�C�Y��Ԃ�
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

			// �V���A���C�Y���s��
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

		// �l�����x�N�g����\���f�[�^�^
		template<class T>
		class Vector4t {

		public:

			// x, y, z, w�v�f�̃f�[�^�^
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

			// �^�ϊ����s���v�f���\�z����i�^�ϊ��@�\�t���R���X�g���N�^�j
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
		// ������Z�q
		//===========================

			// �^�ϊ����s���������
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
		// ()���Z�q�@�����l��ݒ肷��
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
		// []���Z�q�@��[0]�A[1]�A[2]�A[3] ���g�p���� x �� y �� z �� w �����ɃA�N�Z�X���܂�
		//===========================

			T& operator[]( int_t i ) {
				return vec[i];
			}

		//===========================
		// ��r���Z�q
		//===========================

			bool operator==( const Vector4t& vec4 ) const noexcept {
				return Equal<T>(vec4);
			}

			bool operator!=( const Vector4t& vec4 ) const noexcept {
				return NotEqual<T>(vec4);
			}

			//===========================
			// �X�g���[���o��
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
			// NaN�x�N�g���@����ɃG���[�l�Ƃ��ĕԂ�
			static constexpr Vector4t NaN() noexcept {
				return Vector4t( Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>() );
			}

#pragma endregion static param

		public:

#pragma region method

			// �S�Ă̐����ɓ���̒l��ݒ�
			Vector4t& Set( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				z = scalar;
				w = scalar;
				return *this;
			}

			// �e���̐������Ƃɒl��ݒ�
			Vector4t& Set( T newX, T newY, T newZ, T newW ) noexcept {
				x = newX;
				y = newY;
				z = newZ;
				w = newW;
				return *this;
			}

			// �x�N�g���̒l���R�s�[
			Vector4t& Set( const Vector4t& vec4 ) noexcept {
				x = vec4.x;
				y = vec4.y;
				z = vec4.z;
				w = vec4.w;
				return *this;
			}

			// �S�Ă̐����ɓ���̒l�����Z
			Vector4t& SetRelative(T diff) noexcept {
				x += diff;
				y += diff;
				z += diff;
				w += diff;
				return *this;
			}

			// �e���̐������Ƃɒl�����Z
			Vector4t& SetRelative(T diffX, T diffY, T diffZ, T diffW) noexcept {
				x += diffX;
				y += diffY;
				z += diffZ;
				w += diffW;
				return *this;
			}

			// �x�N�g���̒l�����Z
			Vector4t& SetRelative(const Vector4t& vec4) noexcept {
				x += vec4.x;
				y += vec4.y;
				z += vec4.z;
				w += vec4.w;
				return *this;
			}

			// Vector3t<U>�^�ɕϊ�����
			template<class U = T>
			Math::Vector::Vector3t<U> ToVector3() const noexcept {
				return Math::Vector::Vector3t<U>(
					x,
					y,
					z
					);
			}

			// �f�[�^�^��ϊ�����Vector4t�^��Ԃ�
			template<class U>
			Vector4t<U> TypeConvert() const noexcept {
				return Vector4t<U>( *this );
			}

			// �g�p����Ă���f�[�^�^�����擾
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// �x�N�g���̒l�𕶎���ɕϊ�
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

			// ���̃x�N�g���̐�������NaN�������Ă��邩�𒲂ׂ�
			bool HasNaN() const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if (Math::IsNaN(vec[i])) return true;
				}
				return false;
			}

			// ���̃x�N�g���̐������ɐ��̖�����/���̖����傪�����Ă��邩�𒲂ׂ�
			bool HasInfinity() const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if (Math::IsInfinity(vec[i])) return true;
				}
				return false;
			}

			// �^����ꂽ�x�N�g�������̃x�N�g���Ɛ��m�ɓ������ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool Equal( const Vector4t& vec4, U epsilon = 0.001) const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if (Math::NotEqual<U>(vec[i], vec4.vec[i], epsilon)) {
						return false;
					}
				}
				return true;
			}

			// �^����ꂽ�x�N�g�������̃x�N�g���Ɛ��m�ɓ������Ȃ��ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool NotEqual(const Vector4t& vec4, U epsilon = 0.001) const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if (Math::NotEqual<U>(vec[i], vec4.vec[i], epsilon)) {
						return true;
					}
				}
				return false;
			}

			// ���g�̃f�[�^�^�Ɣ�r
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// ���̃N���X�̃������T�C�Y��Ԃ�
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

			// �V���A���C�Y���s��
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
		// float_t�^��int_t�^�ŃG�C���A�X�錾
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


	// �s�񃉃C�u����
#pragma region MatrixLib

	// Reference

	// �Q�l�T�C�g
	// https://light11.hatenadiary.com/entry/2019/01/07/213408
	// https://zenn.dev/ryutorion/articles/game-math-matrix
	// https://nogson2.hatenablog.com/entry/2017/05/22/222937?amp=1
	// https://imagingsolution.net/math/

	// �s��
	// http://www.slis.tsukuba.ac.jp/~fujisawa.makoto.fu/cgi-bin/wiki/index.php?4x4%A4%DE%A4%C7%A4%CE%B5%D5%B9%D4%CE%F3%A4%CE%C4%BE%C0%DC%B2%F2%CB%A1
	// 2x2�Fhttps://risalc.info/src/determinant-two-by-two.html
	// 3x3�Fhttps://risalc.info/src/determinant-three-by-three.html

	// �t�s��
	// http://www.slis.tsukuba.ac.jp/~fujisawa.makoto.fu/cgi-bin/wiki/index.php?4x4%A4%DE%A4%C7%A4%CE%B5%D5%B9%D4%CE%F3%A4%CE%C4%BE%C0%DC%B2%F2%CB%A1
	// https://takun-physics.net/12554/
	// http://cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/tech23.html

	// ���Z
	// https://matrixcalc.org/ja/

	// �s�񃉃C�u����
	namespace Matrix {

		// �O���錾
		template<class T>
		class Matrix2x2t;
		template<class T>
		class Matrix3x3t;
		template<class T>
		class Matrix4x4t;

		// 2x2�s��
		template<class T>
		class Matrix2x2t {

		public:

			// �s��v�f�̃f�[�^�^
			using type = T;

			// 2x2�s��̈�s���̐���
			using Row = std::array<T, 2>;

			// 2x2�s��̈�񕪂̐���
			using Column = std::array<T, 2>;

			// �z��Ńf�[�^��ێ�����2�s2��̃f�[�^�^�@��1�����ڂ��s�A2�����ڂ���̃f�[�^
			using Matrix2x2Array = std::array<Matrix2x2t::Row, 2>;

		public:

#pragma region member

			// R : Row(�s), C : Column(��)
			union {
				struct {
					// �e�������ʂɈ����ꍇ�Ɏg�p
					T m11, m12;			// 1�s�ڂ̊e����
					T m21, m22;			// 2�s�ڂ̊e����
				};
				struct {
					// �s���Ƃ̐������O�����x�N�g���Ƃ��Ĉ����ꍇ�Ɏg�p
					Math::Vector::Vector2t<T> rowVector1;	// 1�s�ڂ̐����̃x�N�g��
					Math::Vector::Vector2t<T> rowVector2;	// 2�s�ڂ̐����̃x�N�g��
				};
				struct {
					// �s���Ƃ̐�����z��Ƃ��Ĉ����ꍇ�Ɏg�p
					Matrix2x2t::Row row1;	// 1�s�ڂ̐����̔z��
					Matrix2x2t::Row row2;	// 2�s�ڂ̐����̔z��
				};
				struct {
					// �����S�̂ɑ΂���1�����z��
					std::array<T, 4> matrix;
				};
				struct {
					// �����S�̂ɑ΂���2�����z��@��1�����ڂ��s�A2�����ڂ���̃f�[�^
					Matrix2x2t::Matrix2x2Array matrix2x2;
				};
			};

#pragma endregion member

		public:

#pragma region constructor

			// �f�t�H���g�ŒP�ʍs��
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

			// �^�ϊ����s���v�f���\�z����i�^�ϊ��@�\�t���R���X�g���N�^�j
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
		// ������Z�q
		//===========================

			// �^�ϊ����s���������
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
		// �Z�p���Z�q
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

				// �s�v�f�Ɨ�v�f�̏�Z
				auto mul = []( const Matrix2x2t::Row& row, const Matrix2x2t::Column& column ) {
					T ret = 0;
					for( int i = 0; i < 2; i++ ) {
						ret += row[i] * column[i];
					}
					return ret;
				};

				// �ꎞ�ϐ�
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

				// ��v�f��2�����x�N�g���̏�Z
				auto mul = []( const Matrix2x2t::Column& column, const Math::Vector::Vector2t<T>& _vec2 ) {
					T ret = 0;
					for( int i = 0; i < 2; i++ ) {
						ret += column[i] * _vec2.vec[i];
					}
					return ret;
				};

				// �ꎞ�ϐ�
				auto c1 = GetRow( 0 );
				auto c2 = GetRow( 1 );

				return Math::Vector::Vector2t<T>(
					mul( c1, vec2 ),
					mul( c2, vec2 )
					);

			}

		//===========================
		// ����������Z�q
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
		// ��r���Z�q
		//===========================

			bool operator==( const Matrix2x2t& matrix2x2 ) const noexcept {
				return Equal<T>(matrix2x2);
			}

			bool operator!=( const Matrix2x2t& matrix2x2 ) const noexcept {
				return NotEqual<T>(matrix2x2);
			}

			//===========================
			// �X�g���[���o��
			//===========================

			friend std::ostream& operator<<( std::ostream& stream, const Matrix2x2t& matrix2x2 ) {
				stream << matrix2x2.ToString();
				return stream;
			}

#pragma endregion operator

		public:

#pragma region static param

			// �P�ʍs��
			static constexpr Matrix2x2t Identity() noexcept {
				return Matrix2x2t(
					1.0, 0.0,
					0.0, 1.0
				);
			}

			// �[���s��
			static constexpr Matrix2x2t Zero() noexcept {
				return Matrix2x2t(
					0.0, 0.0,
					0.0, 0.0
				);
			}

			// NaN�s��@����ɃG���[�l�Ƃ��ĕԂ�
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

			// ���g�̍s���]�u����
			Matrix2x2t& Transpose() noexcept {
				*this = TransposeMatrix();
				return *this;
			}

			// ���g�̍s���]�u�������ʂ�Ԃ�
			Matrix2x2t TransposeMatrix() const noexcept {
				return Matrix2x2t(
					m11, m21,
					m12, m22
					);
			}

			// ���g�̍s��̋t�s���Ԃ�
			Matrix2x2t InverseMatrix() const noexcept {
				// �s�񎮂̌��ʒl
				auto det = Determinant();

				// �s�񎮂̌��ʒl��0�Ȃ�t�s��͑��݂��Ȃ�
				if( Math::Equal<T>( det, 0.0, 0.001 ) ) {
#if _DEBUG
					try {
						throw std::runtime_error( "Matrix2x2::InverseMatrix : Unable to compute inverse matrix." );
					}
					catch( const std::runtime_error& error ) {
						// ��x�����s��
						Debug::LogWarning( error.what() );
						// NaN�s���Ԃ�
						return Matrix2x2t::NaN();
					}
#else
					// ��x�����s��
					Debug::LogWarning( "Matrix2x2::InverseMatrix : Unable to compute inverse matrix." );
					// NaN�s���Ԃ�
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

			// ���g�̍s�񎮂̌��ʂ�Ԃ�
			T Determinant() const noexcept {
				return T(
					m11 * m22 - m12 * m21
				);
			}

			// �w�肵���s�̗v�f���擾����
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
							// �v�f�O�A�N�Z�X
							throw std::runtime_error( "Matrix3x3::GetRowMethod : index out of range." );
						}
						catch( const std::runtime_error& error ) {
							Debug::LogError( error.what() );
						}
					}
					break;
				}
			}

			// �w�肵����̗v�f���擾����
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
							// �v�f�O�A�N�Z�X
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

			// ��̃f�[�^�𒼐���ɕ��ׂ����̂��擾����
			std::array<T, 4> ColumnMajorOrder() const noexcept {
				return {
					// 1���
					m11, m21,
					// 2���
					m12, m22
				};
			}

			// �s�̃f�[�^�𒼐���ɕ��ׂ����̂��擾����
			std::array<T, 4> RowMajorOrder() const noexcept {
				return {
					// 1�s��
					m11, m12,
					// 2�s��
					m21, m22
				};
			}

			// Matrix3x3t<U>�^�ɕϊ�����
			template<class U = T>
			Matrix3x3t<U> ToMatrix3x3() const noexcept {
				return Matrix3x3t<U>(
					// 1�s��
					m11, m12, 0,
					// 2�s��
					m21, m22, 0,
					// 3�s��
					0, 0, 1
					);
			}

			// �f�[�^�^��ϊ�����Matrix2x2t�^��Ԃ�
			template<class U>
			Matrix2x2t<U> TypeConvert() const noexcept {
				return Matrix2x2t<U>( *this );
			}

			// �g�p����Ă���f�[�^�^�����擾
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// �s��̒l�𕶎���ɕϊ�
			std::string ToString() const noexcept {
				// std::string("1.(")�Ƃ��Ă���̂͂ЂƂO�̍s��" )\n"�ƕ����񓯎m���������邽�߂�
				// �������Ȃ��ƁAconst char*���m�̉��Z�ƂȂ��Ă��܂��̂ŃR���p�C���G���[�ƂȂ�
				return std::string(
					"Matrix2x2 {\n" +
					std::string( " 1.( " ) + Utility::ToString( m11 ) + ", " + Utility::ToString( m12 ) + " )\n" +
					std::string( " 2.( " ) + Utility::ToString( m21 ) + ", " + Utility::ToString( m22 ) + " )\n" +
					std::string("}")
				);
			}

			// ���̍s��̐�������NaN�������Ă��邩�𒲂ׂ�
			bool HasNaN() const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if (Math::IsNaN(matrix[i])) return true;
				}
				return false;
			}

			// ���̍s��̐������ɐ��̖�����/���̖����傪�����Ă��邩�𒲂ׂ�
			bool HasInfinity() const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if (Math::IsInfinity(matrix[i])) return true;
				}
				return false;
			}

			// �^����ꂽ�s�񂪂��̍s��Ɛ��m�ɓ������ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool Equal( const Matrix2x2t& matrix2x2, U epsilon = 0.001) const noexcept {
				for (int i = 0; i < 4; i++) {
					if (Math::NotEqual<U>(matrix[i], matrix2x2.matrix[i], epsilon)) {
						return false;
					}
				}
				return true;
			}

			// �^����ꂽ�s�񂪂��̍s��Ɛ��m�ɓ������Ȃ��ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool NotEqual(const Matrix2x2t& matrix2x2, U epsilon = 0.001) const noexcept {
				for (int i = 0; i < 4; i++) {
					if (Math::NotEqual<U>(matrix[i], matrix2x2.matrix[i], epsilon)) {
						return true;
					}
				}
				return false;
			}

			// ���g�̃f�[�^�^�Ɣ�r
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// ���̃N���X�̃������T�C�Y��Ԃ�
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

			// �V���A���C�Y���s��
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

		// 3x3�s��
		template<class T>
		class Matrix3x3t {

		public:

			// �s��v�f�̃f�[�^�^
			using type = T;

			// 3x3�s��̈�s���̐���
			using Row = std::array<T, 3>;

			// 3x3�s��̈�񕪂̐���
			using Column = std::array<T, 3>;

			// �z��Ńf�[�^��ێ�����3�s3��̃f�[�^�^�@��1�����ڂ��s�A2�����ڂ���̃f�[�^
			using Matrix3x3Array = std::array<Matrix3x3t::Row, 3>;

		public:

#pragma region member

			// R : Row(�s), C : Column(��)
			union {
				struct {
					// �e�������ʂɈ����ꍇ�Ɏg�p
					T m11, m12, m13;			// 1�s�ڂ̊e����
					T m21, m22, m23;			// 2�s�ڂ̊e����
					T m31, m32, m33;			// 3�s�ڂ̊e����
				};
				struct {
					// �s���Ƃ̐������O�����x�N�g���Ƃ��Ĉ����ꍇ�Ɏg�p
					Math::Vector::Vector3t<T> rowVector1;	// 1�s�ڂ̐����̃x�N�g��
					Math::Vector::Vector3t<T> rowVector2;	// 2�s�ڂ̐����̃x�N�g��
					Math::Vector::Vector3t<T> rowVector3;	// 3�s�ڂ̐����̃x�N�g��
				};
				struct {
					// �s���Ƃ̐�����z��Ƃ��Ĉ����ꍇ�Ɏg�p
					Matrix3x3t::Row row1;	// 1�s�ڂ̐����̔z��
					Matrix3x3t::Row row2;	// 2�s�ڂ̐����̔z��
					Matrix3x3t::Row row3;	// 3�s�ڂ̐����̔z��
				};
				struct {
					// �����S�̂ɑ΂���1�����z��
					std::array<T, 9> matrix;
				};
				struct {
					// �����S�̂ɑ΂���2�����z��@��1�����ڂ��s�A2�����ڂ���̃f�[�^
					Matrix3x3t::Matrix3x3Array matrix3x3;
				};
			};

#pragma endregion member

		public:

#pragma region constructor

			// �f�t�H���g�ŒP�ʍs��
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

			// �^�ϊ����s���v�f���\�z����i�^�ϊ��@�\�t���R���X�g���N�^�j
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
		// ������Z�q
		//===========================

			// �^�ϊ����s���������
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
		// �Z�p���Z�q
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

				// �s�v�f�Ɨ�v�f�̏�Z
				auto mul = []( const Matrix3x3t::Row& row, const Matrix3x3t::Column& column ) {
					T ret = 0;
					for( int i = 0; i < 3; i++ ) {
						ret += row[i] * column[i];
					}
					return ret;
				};

				// �ꎞ�ϐ�
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

				// ��v�f��3�����x�N�g���̏�Z
				auto mul = []( const Matrix3x3t::Column& column, const Math::Vector::Vector3t<T>& _vec3 ) {
					T ret = 0;
					for( int i = 0; i < 3; i++ ) {
						ret += column[i] * _vec3.vec[i];
					}
					return ret;
				};

				// �ꎞ�ϐ�
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
		// ����������Z�q
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
		// ��r���Z�q
		//===========================

			bool operator==( const Matrix3x3t& matrix3x3 ) const noexcept {
				return Equal<T>(matrix3x3);
			}

			bool operator!=( const Matrix3x3t& matrix3x3 ) const noexcept {
				return NotEqual<T>(matrix3x3);
			}

			//===========================
			// �X�g���[���o��
			//===========================

			friend std::ostream& operator<<( std::ostream& stream, const Matrix3x3t& matrix3x3 ) {
				stream << matrix3x3.ToString();
				return stream;
			}

#pragma endregion operator

		public:

#pragma region static param

			// �P�ʍs��
			static constexpr Matrix3x3t Identity() noexcept {
				return Matrix3x3t(
					1.0, 0.0, 0.0,
					0.0, 1.0, 0.0,
					0.0, 0.0, 1.0
				);
			}

			// �[���s��
			static constexpr Matrix3x3t Zero() noexcept {
				return Matrix3x3t(
					0.0, 0.0, 0.0,
					0.0, 0.0, 0.0,
					0.0, 0.0, 0.0
				);
			}

			// NaN�s��@����ɃG���[�l�Ƃ��ĕԂ�
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

			// ���g�̍s���]�u����
			Matrix3x3t& Transpose() noexcept {
				*this = TransposeMatrix();
				return *this;
			}

			// ���g�̍s���]�u�������ʂ�Ԃ�
			Matrix3x3t TransposeMatrix() const noexcept {
				return Matrix3x3t(
					m11, m21, m31,
					m12, m22, m32,
					m13, m23, m33
				);
			}

			// ���g�̍s��̋t�s����擾����
			Matrix3x3t InverseMatrix() const noexcept {
				// �s�񎮂̌��ʒl
				auto det = Determinant();

				// �s�񎮂̌��ʒl��0�Ȃ�t�s��͑��݂��Ȃ�
				if( Math::Equal<T>( det, 0.0, 0.001 ) ) {
#if _DEBUG
					try {
						throw std::runtime_error( "Matrix3x3::InverseMatrix : Unable to compute inverse matrix." );
					}
					catch( const std::runtime_error& error ) {
						// ��x�����s��
						Debug::LogWarning( error.what() );
						// NaN�s���Ԃ�
						return Matrix3x3t::NaN();
					}
#else
					// ��x�����s��
					Debug::LogWarning( "Matrix3x3::InverseMatrix : Unable to compute inverse matrix." );
					// NaN�s���Ԃ�
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
			
			// ���g�̍s�񎮂̌��ʂ�Ԃ�
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

			// �w�肵���s�̗v�f���擾����
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
							// �v�f�O�A�N�Z�X
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

			// �w�肵����̗v�f���擾����
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
							// �v�f�O�A�N�Z�X
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

			// ��̃f�[�^�𒼐���ɕ��ׂ����̂��擾����
			std::array<T, 9> ColumnMajorOrder() const noexcept {
				return {
					// 1���
					m11, m21, m31,
					// 2���
					m12, m22, m32,
					// 3���
					m13, m23, m33
				};
			}

			// �s�̃f�[�^�𒼐���ɕ��ׂ����̂��擾����
			std::array<T, 9> RowMajorOrder() const noexcept {
				return {
					// 1�s��
					m11, m12, m13,
					// 2�s��
					m21, m22, m23,
					// 3�s��
					m31, m32, m33
				};
			}

			// Matrix2x2t<U>�^�ɕϊ�����
			template<class U = T>
			Matrix2x2t<U> ToMatrix2x2() const noexcept {
				return Matrix2x2t<U>(
					m11, m12,
					m21, m22
					);
			}

			// Matrix4x4t<U>�^�ɕϊ�����
			template<class U = T>
			Matrix4x4t<U> ToMatrix4x4() const noexcept {
				return Matrix4x4t<U>(
					// 1�s��
					m11, m12, m13, 0,
					// 2�s��
					m21, m22, m23, 0,
					// 3�s��
					m31, m32, m33, 0,
					// 4�s��
					0, 0, 0, 1
					);
			}

			// �f�[�^�^��ϊ�����Matrix3x3t�^��Ԃ�
			template<class U>
			Matrix3x3t<U> TypeConvert() const noexcept {
				return Matrix3x3t<U>( *this );
			}

			// �g�p����Ă���f�[�^�^�����擾
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// �s��̒l�𕶎���ɕϊ�
			std::string ToString() const noexcept {
				// std::string("1.(")�Ƃ��Ă���̂͂ЂƂO�̍s��" )\n"�ƕ����񓯎m���������邽�߂�
				// �������Ȃ��ƁAconst char*���m�̉��Z�ƂȂ��Ă��܂��̂ŃR���p�C���G���[�ƂȂ�
				return std::string(
					"Matrix3x3 {\n" +
					std::string(" 1.( ") + Utility::ToString( m11 ) + ", " + Utility::ToString( m12 ) + ", " + Utility::ToString( m13 ) + " )\n" +
					std::string(" 2.( ") + Utility::ToString( m21 ) + ", " + Utility::ToString( m22 ) + ", " + Utility::ToString( m23 ) + " )\n" +
					std::string(" 3.( ") + Utility::ToString( m31 ) + ", " + Utility::ToString( m32 ) + ", " + Utility::ToString( m33 ) + " )\n" +
					std::string("}")
				);
			}

			// ���̍s��̐�������NaN�������Ă��邩�𒲂ׂ�
			bool HasNaN() const noexcept {
				for (int_t i = 0; i < 9; i++) {
					if (Math::IsNaN(matrix[i])) return true;
				}
				return false;
			}

			// ���̍s��̐������ɐ��̖�����/���̖����傪�����Ă��邩�𒲂ׂ�
			bool HasInfinity() const noexcept {
				for (int_t i = 0; i < 9; i++) {
					if (Math::IsInfinity(matrix[i])) return true;
				}
				return false;
			}

			// �^����ꂽ�s�񂪂��̍s��Ɛ��m�ɓ������ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool Equal( const Matrix3x3t& matrix3x3, U epsilon = 0.001 ) const noexcept {
				for (int i = 0; i < 9; i++) {
					if (Math::NotEqual<U>(matrix[i], matrix3x3.matrix[i], epsilon)) {
						return false;
					}
				}
				return true;
			}

			// �^����ꂽ�s�񂪂��̍s��Ɛ��m�ɓ������Ȃ��ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool NotEqual(const Matrix3x3t& matrix3x3, U epsilon = 0.001) const noexcept {
				for (int i = 0; i < 9; i++) {
					if (Math::NotEqual<U>(matrix[i], matrix3x3.matrix[i], epsilon)) {
						return true;
					}
				}
				return false;
			}

			// ���g�̃f�[�^�^�Ɣ�r
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// ���̃N���X�̃������T�C�Y��Ԃ�
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

			// �V���A���C�Y���s��
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

		// 4x4�s��
		template<class T>
		class Matrix4x4t {

		public:

			// �s��v�f�̃f�[�^�^
			using type = T;

			// 4x4�s��̈�s���̐���
			using Row = std::array<T, 4>;

			// 4x4�s��̈�񕪂̐���
			using Column = std::array<T, 4>;

			// �z��Ńf�[�^��ێ�����3�s3��̃f�[�^�^�@��1�����ڂ��s�A2�����ڂ���̃f�[�^
			using Matrix4x4Array = std::array<Matrix4x4t::Row, 4>;

		public:

#pragma region member

			// R : Row(�s), C : Column(��)
			union {
				struct {
					// �e�������ʂɈ����ꍇ�Ɏg�p
					T m11, m12, m13, m14;			// 1�s�ڂ̊e����
					T m21, m22, m23, m24;			// 2�s�ڂ̊e����
					T m31, m32, m33, m34;			// 3�s�ڂ̊e����
					T m41, m42, m43, m44;			// 4�s�ڂ̊e����
				};
				struct {
					// �s���Ƃ̐������l�����x�N�g���Ƃ��Ĉ����ꍇ�Ɏg�p
					Math::Vector::Vector4t<T> rowVector1;	// 1�s�ڂ̐����̃x�N�g��
					Math::Vector::Vector4t<T> rowVector2;	// 2�s�ڂ̐����̃x�N�g��
					Math::Vector::Vector4t<T> rowVector3;	// 3�s�ڂ̐����̃x�N�g��
					Math::Vector::Vector4t<T> rowVector4;	// 4�s�ڂ̐����̃x�N�g��
				};
				struct {
					// �s���Ƃ̐�����z��Ƃ��Ĉ����ꍇ�Ɏg�p
					Matrix4x4t::Row row1;	// 1�s�ڂ̐����̔z��
					Matrix4x4t::Row row2;	// 2�s�ڂ̐����̔z��
					Matrix4x4t::Row row3;	// 3�s�ڂ̐����̔z��
					Matrix4x4t::Row row4;	// 4�s�ڂ̐����̔z��
				};
				struct {
					// �����S�̂ɑ΂���1�����z��
					std::array<T, 16> matrix;
				};
				struct {
					// �����S�̂ɑ΂���2�����z��@��1�����ڂ��s�A2�����ڂ���̃f�[�^
					Matrix4x4t::Matrix4x4Array matrix4x4;
				};
			};

#pragma endregion member

		public:

#pragma region constructor

			// �f�t�H���g�ŒP�ʍs��
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

			// �^�ϊ����s���v�f���\�z����i�^�ϊ��@�\�t���R���X�g���N�^�j
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
		// ������Z�q
		//===========================

			// �^�ϊ����s���������
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
		// �Z�p���Z�q
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

				// �s�v�f�Ɨ�v�f�̏�Z
				auto mul = []( const Matrix4x4t::Row& row, const Matrix4x4t::Column& column ) {
					T ret = 0;
					for( int i = 0; i < 4; i++ ) {
						ret += row[i] * column[i];
					}
					return ret;
				};

				// �ꎞ�ϐ�
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

				// ��v�f��4�����x�N�g���̏�Z
				auto mul = []( const Matrix4x4t::Column& column, const Math::Vector::Vector4t<T>& _vec4 ) {
					T ret = 0;
					for( int i = 0; i < 4; i++ ) {
						ret += column[i] * _vec4.vec[i];
					}
					return ret;
				};

				// �ꎞ�ϐ�
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
		// ����������Z�q
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
		// ��r���Z�q
		//===========================

			bool operator==( const Matrix4x4t& matrix4x4 ) const noexcept {
				return Equal<T>(matrix4x4);
			}

			bool operator!=( const Matrix4x4t& matrix4x4 ) const noexcept {
				return NotEqual<T>(matrix4x4);
			}

			//===========================
			// �X�g���[���o��
			//===========================

			friend std::ostream& operator<<( std::ostream& stream, const Matrix4x4t& matrix4x4 ) {
				stream << matrix4x4.ToString();
				return stream;
			}

#pragma endregion operator

		public:

#pragma region static param

			// �P�ʍs��
			static constexpr Matrix4x4t Identity() noexcept {
				return Matrix4x4t(
					1.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0
				);
			}

			// �[���s��
			static constexpr Matrix4x4t Zero() noexcept {
				return Matrix4x4t(
					0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 0.0, 0.0,
					0.0, 0.0, 0.0, 0.0
				);
			}

			// NaN�s��@����ɃG���[�l�Ƃ��ĕԂ�
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

			// ���g�̍s���]�u����
			Matrix4x4t& Transpose() noexcept {
				*this = TransposeMatrix();
				return *this;
			}

			// ���g�̍s���]�u�������ʂ�Ԃ�
			Matrix4x4t TransposeMatrix() const noexcept {
				return Matrix4x4t(
					m11, m21, m31, m41,
					m12, m22, m32, m42,
					m13, m23, m33, m43,
					m14, m24, m34, m44
				);
			}

			// ���g�̍s��̋t�s����擾����
			Matrix4x4t InverseMatrix() const {
				// �s�񎮂̌��ʒl
				auto det = Determinant();

				// �s�񎮂̌��ʒl��0�Ȃ�t�s��͑��݂��Ȃ�
				if( Math::Equal<T>( det, 0.0, 0.001 ) ) {
#if _DEBUG
					try {
						throw std::runtime_error( "Matrix4x4::InverseMatrix : Unable to compute inverse matrix." );
					}
					catch( const std::runtime_error& error ) {
						// ��x�����s��
						Debug::LogWarning( error.what() );
						// NaN�s���Ԃ�
						return Matrix4x4t::NaN();
					}
#else
					// ��x�����s��
					Debug::LogWarning( "Matrix4x4::InverseMatrix : Unable to compute inverse matrix." );
					// NaN�s���Ԃ�
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

			// ���g�̍s�񎮂̌��ʂ�Ԃ�
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

			// �w�肵���s�̗v�f���擾����
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
							// �v�f�O�A�N�Z�X
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

			// �w�肵����̗v�f���擾����
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
							// �v�f�O�A�N�Z�X
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

			// ��̃f�[�^�𒼐���ɕ��ׂ����̂��擾����
			std::array<T, 12> ColumnMajorOrder() const noexcept {
				return {
					// 1���
					m11, m21, m31, m41,
					// 2���
					m12, m22, m32, m42,
					// 3���
					m13, m23, m33, m43,
					// 4���
					m14, m24, m34, m44
				};
			}

			// �s�̃f�[�^�𒼐���ɕ��ׂ����̂��擾����
			std::array<T, 12> RowMajorOrder() const noexcept {
				return {
					// 1�s��
					m11, m12, m13, m14,
					// 2�s��
					m21, m22, m23, m24,
					// 3�s��
					m31, m32, m33, m34,
					// 3�s��
					m41, m42, m43, m44,
				};
			}

			// Matrix3x3t<U>�^�ɕϊ�����
			template<class U = T>
			Matrix3x3t<T> ToMatrix3x3() const noexcept {
				return Matrix3x3t<T>(
					// 1�s��
					m11, m12, m13,
					// 2�s��
					m21, m22, m23,
					// 3�s��
					m31, m32, m33
					);
			}

			// �f�[�^�^��ϊ�����Matrix4x4t�^��Ԃ�
			template<class U>
			Matrix4x4t<U> TypeConvert() const noexcept {
				return Matrix4x4t<U>( *this );
			}

			// �g�p����Ă���f�[�^�^�����擾
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// �s��̒l�𕶎���ɕϊ�
			std::string ToString() const noexcept {
				// std::string("1.(")�Ƃ��Ă���̂͂ЂƂO�̍s��" )\n"�ƕ����񓯎m���������邽�߂�
				// �������Ȃ��ƁAconst char*���m�̉��Z�ƂȂ��Ă��܂��̂ŃR���p�C���G���[�ƂȂ�
				return std::string(
					"Matrix4x4 {\n" +
					std::string( " 1.( " ) + Utility::ToString( m11 ) + ", " + Utility::ToString( m12 ) + ", " + Utility::ToString( m13 ) + ", " + Utility::ToString( m14 ) + " )\n" +
					std::string( " 2.( " ) + Utility::ToString( m21 ) + ", " + Utility::ToString( m22 ) + ", " + Utility::ToString( m23 ) + ", " + Utility::ToString( m24 ) + " )\n" +
					std::string( " 3.( " ) + Utility::ToString( m31 ) + ", " + Utility::ToString( m32 ) + ", " + Utility::ToString( m33 ) + ", " + Utility::ToString( m34 ) + " )\n" +
					std::string( " 4.( " ) + Utility::ToString( m41 ) + ", " + Utility::ToString( m42 ) + ", " + Utility::ToString( m43 ) + ", " + Utility::ToString( m44 ) + " )\n" +
					std::string( "}" )
				);
			}

			// ���̍s��̐�������NaN�������Ă��邩�𒲂ׂ�
			bool HasNaN() const noexcept {
				for (int_t i = 0; i < 16; i++) {
					if (Math::IsNaN(matrix[i])) return true;
				}
				return false;
			}

			// ���̍s��̐������ɐ��̖�����/���̖����傪�����Ă��邩�𒲂ׂ�
			bool HasInfinity() const noexcept {
				for (int_t i = 0; i < 16; i++) {
					if (Math::IsInfinity(matrix[i])) return true;
				}
				return false;
			}

			// �^����ꂽ�s�񂪂��̍s��Ɛ��m�ɓ������ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool Equal( const Matrix4x4t& matrix4x4, U epsilon = 0.001 ) const noexcept {
				for (int i = 0; i < 16; i++) {
					if (Math::NotEqual<U>( matrix[i], matrix4x4.matrix[i], epsilon) ) {
						return false;
					}
				}
				return true;
			}

			// �^����ꂽ�s�񂪂��̍s��Ɛ��m�ɓ������Ȃ��ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool NotEqual(const Matrix4x4t& matrix4x4, U epsilon = 0.001) const noexcept {
				for (int i = 0; i < 16; i++) {
					if (Math::NotEqual<U>(matrix[i], matrix4x4.matrix[i], epsilon) ) {
						return true;
					}
				}
				return false;
			}

			// ���g�̃f�[�^�^�Ɣ�r
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// ���̃N���X�̃������T�C�Y��Ԃ�
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

			// �V���A���C�Y���s��
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

		// �A�t�B���ϊ�
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

				// �w�肳�ꂽ�v�Z���ԂŌv�Z����
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

		// ZXY��]�̉�]�s�񂩂�̕ϊ�
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
	// float_t�^��double_t�^�łŃG�C���A�X�錾
	//===================================================

		using Matrix2x2 = Matrix2x2t<float_t>;
		using Matrix2x2d = Matrix2x2t<double_t>;
		using Matrix3x3 = Matrix3x3t<float_t>;
		using Matrix3x3d = Matrix3x3t<double_t>;
		using Matrix4x4 = Matrix4x4t<float_t>;
		using Matrix4x4d = Matrix4x4t<double_t>;

	}

#pragma endregion MatrixLib


	// �N�H�[�^�j�I�����C�u����
#pragma region Quaternion

	namespace Quaternion {

		// Reference

		// �Q�l�T�C�g
		// ��ԗD�G�Fhttp://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/index.htm

		// Slerp
		// http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm

		template<class T>
		class tQuaternion {

		public:

			// x, y, z, w�v�f�̃f�[�^�^
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
					Math::Vector::Vector3t<T> imaginary;	// �����@���x�N�g�����Ɠ���
					T real;											// �����@���X�J���[���Ɠ���
				};
				struct {
					Math::Vector::Vector3t<T> vector;		// �x�N�g�����@�������Ɠ���
					T scalar;										// �X�J���[���@�������Ɠ���
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

			// �^�ϊ����s���v�f���\�z����i�^�ϊ��@�\�t���R���X�g���N�^�j
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
		// ������Z�q
		//===========================

		// �^�ϊ����s���������
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
			// ()���Z�q�@�����l��ݒ肷��
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
			// []���Z�q�@��[0]�A[1]�A[2]�A[3] ���g�p���� x �� y �� z �� w �����ɃA�N�Z�X���܂�
			//===========================

			T& operator[]( int_t i ) {
				return vec[i];
			}

			//===========================
			// �P�����Z�q
			//===========================

			tQuaternion operator+() const noexcept {
				return *this;
			}

			tQuaternion operator-() const noexcept {
				return InverseQuaternion();
			}

			//===========================
			// �Z�p���Z�q
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
				// Cross���\�b�h�Ɠ����̏���
#if _IF_LH_COORD
				return tQuaternion(
					// ����
					( qt.imaginary * w ) + ( imaginary * qt.w ) + ( qt.imaginary.Cross( imaginary ) ),
					// ����
					( qt.w * w ) - ( qt.imaginary.Dot( imaginary ) )
				);
#elif _IF_RH_COORD
				return tQuaternion(
					// ����
					( imaginary * qt.w ) + ( qt.imaginary * w ) + ( imaginary.Cross( qt.imaginary ) ),
					// ����
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
			// ����������Z�q
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
			// ��r���Z�q
			//===========================

			bool operator==( const tQuaternion& qt ) const noexcept {
				return Equal<T>(qt);
			}

			bool operator!=( const tQuaternion& qt ) const noexcept {
				return NotEqual<T>(qt);
			}

			//===========================
			// �X�g���[���o��
			//===========================

			friend std::ostream& operator<<( std::ostream& stream, const tQuaternion& qt ) {
				stream << qt.ToString();
				return stream;
			}

#pragma endregion operator

		public:

#pragma region static param

			// �P���l�������擾����
			static constexpr tQuaternion Identity() noexcept {
				return tQuaternion( 0.0, 0.0, 0.0, 1.0 );
			}

			// NaN�N�H�[�^�j�I���@����ɃG���[�l�Ƃ��ĕԂ�
			static constexpr tQuaternion NaN() noexcept {
				return tQuaternion( Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>(), Math::NaN<T>() );
			}

#pragma endregion static param

		public:

#pragma region static method

			// �N�H�[�^�j�I�����m�̐��`��Ԃ��s���܂�
			// �߂�l�F��a�`b�̊Ԃ̃N�H�[�^�j�I���ɐ�������܂�
			static tQuaternion Lerp( const tQuaternion& a, const tQuaternion& b, T t ) {
				t = Math::Clamp01<T>( t );
				return LerpUnclamped( a, b, t );
			}

			// �N�H�[�^�j�I�����m�̐��`��Ԃ��s���܂�
			// �߂�l�F��a�`b�̊Ԃ̃N�H�[�^�j�I���ɐ�������܂���
			static tQuaternion LerpUnclamped( const tQuaternion& a, const tQuaternion& b, T t ) {
				return a * ( 1.0f - t ) + b * t;
			}

			// �N�H�[�^�j�I�����m�̋��ʐ��`��Ԃ��s���܂�
			// �߂�l�F��a�`b�̊Ԃ̃N�H�[�^�j�I���ɐ�������܂�
			static tQuaternion Slerp( const tQuaternion& a, const tQuaternion& b, T t ) {
				t = Math::Clamp01<T>( t );
				return SlerpUnclamped( a, b, t );
			}

			// �N�H�[�^�j�I�����m�̋��ʐ��`��Ԃ��s���܂�
			// �߂�l�F��a�`b�̊Ԃ̃N�H�[�^�j�I���ɐ�������܂���
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

			// ��]�s�񂩂�N�H�[�^�j�I�����쐬���܂�
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

			// �N�H�[�^�j�I������]�s��ɕϊ����܂�
			template<class U, class QtType>
			static Math::Matrix::Matrix3x3t<U> ToRotationMatrix( const tQuaternion<QtType>& qt ) {
				// TODO
				return Math::Matrix::Matrix3x3t<U>();
			}

			// �I�C���[�p����N�H�[�^�j�I�����쐬���܂�
			template<class U, class VecType>
			static tQuaternion<U> FromEuler( const Math::Vector::Vector3t<VecType>& eulerAngle, Math::RotationOrder rotationOrder = Math::RotationOrder::ZXY ) {

				// �Q�l�T�C�g�Fhttps://qiita.com/take4eng/items/ae487c82a6f7d60ceba8

				
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

			// �N�H�[�^�j�I�����I�C���[�p�ɕϊ����܂�
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


				// ��]���Ԃ�ZXY�̏ꍇ�̕ϊ�
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

					// �Q�l�T�C�g�Fhttps://www.generacodice.com/jp/articolo/98127/

					//// 2�����̉�]���s���ۂɎg�p����
					//auto twoaxisrot = []( U r11, U r12, U r21, U r31, U r32 )->Math::Vector::Vector3t<U> {
					//	return Math::Vector::Vector3t<U>(
					//		Math::Atan2( r11, r12 ),
					//		Math::Acos( r21 ),
					//		Math::Atan2( r31, r32 )
					//		);
					//};

					//// 3�����̉�]���s���ۂɎg�p����
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

		// �S�Ă̐����ɓ���̒l��ݒ�
			tQuaternion& Set( T scalar ) noexcept {
				x = scalar;
				y = scalar;
				z = scalar;
				w = scalar;
				return *this;
			}

			// �e���̐������Ƃɒl��ݒ�
			tQuaternion& Set( T newX, T newY, T newZ, T newW ) noexcept {
				x = newX;
				y = newY;
				z = newZ;
				w = newW;
				return *this;
			}

			// �����Ǝ������Ƃɒl��ݒ�
			tQuaternion& Set( Math::Vector::Vector3t<T> newImaginary, T newReal ) noexcept {
				imaginary = newImaginary;
				real = newReal;
				return *this;
			}

			// �N�H�[�^�j�I���̒l���R�s�[
			tQuaternion& Set( const tQuaternion& qt ) noexcept {
				x = qt.x;
				y = qt.y;
				z = qt.z;
				w = qt.w;
				return *this;
			}

			// ���̃N�H�[�^�j�I���̐�Βl��Ԃ�
			// ���L���ɂ���Ă͐�Βl�ł͂Ȃ������Ƃ��Ă�����̂����邽�ߒ���
			T Magnitude() const noexcept {
				return Math::FastSqrt( SqrMagnitude() );
			}

			// ���̃N�H�[�^�j�I���̐�Βl��2���Ԃ�
			// ���L���ɂ���Ă͐�Βl�ł͂Ȃ������Ƃ��Ă�����̂����邽�ߒ���
			T SqrMagnitude() const noexcept {
				// ���g�̒l���m�̃h�b�g��
				return Dot( *this );
			}

			// �N�H�[�^�j�I���̋������擾����
			tQuaternion ConjugateQuaternion() const noexcept {
				// �����N�H�[�^�j�I����
				// �����͂��̂܂܂ŁA�������}�C�i�X�̂��̂ł���
				return tQuaternion( -imaginary, real );
			}

			// ���̃N�H�[�^�j�I���̋t�N�H�[�^�j�I�����擾����
			tQuaternion InverseQuaternion() const noexcept {
				// �����N�H�[�^�j�I�� / ��Βl��2��
				return ConjugateQuaternion() / SqrMagnitude();
			}

			// ���̃N�H�[�^�j�I���𐳋K������@�����̃N�H�[�^�j�I�����ω�����
			tQuaternion& Normalize() noexcept {
				*this /= Magnitude();
				return *this;
			}

			// ���̃N�H�[�^�j�I���𐳋K���������ʂ�Ԃ��@�����̃N�H�[�^�j�I�����͕̂ω����Ȃ�
			tQuaternion NormalizedQuaternion() const noexcept {
				return *this / Magnitude();
			}

			// ���̃N�H�[�^�j�I���Ƃ̃h�b�g�ς̌��ʂ�Ԃ�
			T Dot( const tQuaternion& qt ) const noexcept {
				return imaginary.Dot( qt.imaginary ) + real * qt.real;
			}

			// ���̃N�H�[�^�j�I���Ƃ̃N���X��
			// �����̌��ʂ̓N�H�[�^�j�I���̍��E���Ⴄ�ƕω�����
			// ���L���ɂ���Ă͏�Z�ƋL�ڂ��Ă�����̂����邽�ߒ���
			tQuaternion Cross( const tQuaternion& qt ) const noexcept {
#if _IF_LH_COORD
				return tQuaternion(
					// ����
					( qt.imaginary * w ) + ( imaginary * qt.w ) + ( qt.imaginary.Cross( imaginary ) ),
					// ����
					( qt.w * w ) - ( qt.imaginary.Dot( imaginary ) )
				);
#elif _IF_RH_COORD
				return tQuaternion(
					// ����
					( imaginary * qt.w ) + ( qt.imaginary * w ) + ( imaginary.Cross( qt.imaginary ) ),
					// ����
					( w * qt.w ) - ( imaginary.Dot( qt.imaginary ) )
				);
#endif
			}

			// �f�[�^�^��ϊ�����tQuaternion�^��Ԃ�
			template<class U>
			tQuaternion<U> TypeConvert() const noexcept {
				return tQuaternion<U>( *this );
			}

			// �g�p����Ă���f�[�^�^�����擾
			std::string TypeName() const noexcept {
				return typeid( type ).name();
			}

			// �x�N�g���̒l�𕶎���ɕϊ�
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

			// �^����ꂽ�x�N�g�������̃x�N�g���Ɛ��m�ɓ������ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool Equal( const tQuaternion& qt, U epsilon = 0.001 ) const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if (Math::NotEqual<U>( vec[i], qt.vec[i], epsilon )) {
						return false;
					}
				}
				return true;
			}

			// �^����ꂽ�x�N�g�������̃x�N�g���Ɛ��m�ɓ������Ȃ��ꍇ�Atrue��Ԃ��܂�
			template<class U = T>
			bool NotEqual(const tQuaternion& qt, U epsilon = 0.001 ) const noexcept {
				for (int_t i = 0; i < 4; i++) {
					if ( Math::NotEqual<U>( vec[i], qt.vec[i], epsilon ) ) {
						return true;
					}
				}
				return false;
			}

			// ���g�̃f�[�^�^�Ɣ�r
			template<class U>
			bool CompareType( U value = U() ) const noexcept {
				return ( typeid( U ) == typeid( *this ) );
			}

			// ���̃N���X�̃������T�C�Y��Ԃ�
			std::size_t MemSize() const noexcept {
				return sizeof( *this );
			}

#pragma endregion method

		};


	//===================================================
	// float_t�^��double_t�^�łŃG�C���A�X�錾
	//===================================================

		using Quaternion = tQuaternion<float_t>;
		using QuaternionD = tQuaternion<double_t>;

	}


#pragma endregion Quaternion

}