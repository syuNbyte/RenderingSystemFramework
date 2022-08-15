#pragma once

#include"Math.hpp"

// �Ȑ����C�u����
namespace Curve {

	// �Q�l�T�C�g
	// https://qiita.com/edo_m18/items/643512f27c2b083b47ac

	// �Ȑ��̎��
	// 2���x�W�G
	// 3���x�W�G
	// n���x�W�G
	// �L���x�W�G
	// B�X�v���C��
	// n��B�X�v���C��

	template<class T>
	using Vector2t = Math::Vector::Vector2t<T>;

	// �x�W�F�Ȑ��֐�
	template<class T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline Vector2t<T> Bezier( T t ) {
		return Vector2t<T>();
	}

   // �o�[���X�^�C�����֐�
	template<class T = double_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline float Bernstein(int_t n, int_t i, T t)
   {
       return Binomial<T>(n, i) * Math::Pow<T>(t, i) * Math::Pow<T>(1 - t, n - i);
   }

	// �񍀌W�����v�Z����
	template<class T = double_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	inline T Binomial( int_t n, int_t k ) {
		return Math::Factorial<T>( n ) / ( Math::Factorial<T>( k ) * Math::Factorial<T>( n - k ) );
	}

}