#pragma once


#include<cinttypes>	// int~_t�^, uint~_t�^���g�p���邽�߂ɕK�v
#include<cmath>		// float_t�^, double_t�^���g�p���邽�߂ɕK�v

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

// float_t, double_t�͕ʃt�@�C���Œ�`����Ă��邽�ߒ�`���Ă��Ȃ�
using LongDouble = long double;
using long_double_t = long double;


#pragma endregion Default Datatype





// ������������₷���悤�Ɏg�����߂̃f�[�^�^
#pragma region



// �f�t�H���g�Œ�`����Ă���f�[�^�^���g���₷�����邽�߂̃G�C���A�X
using Pointer = void*;	// �ėp�|�C���^�^�i�ǂ̃|�C���^�^�ł�����\�j
using Size = size_t;		// size_t�^�𖼑O�ύX��������


// Util�n�N���X�Ŏg�p����Ǘ�ID
using ID = uint32_t;
using index = uint32_t;		// �C���f�b�N�X�^


using if_index = int32_t;	// ����������g����C���f�b�N�X
#define if_index_error -1	// if_index�^���̎g�p���Ɏg�p����G���[�^



#pragma endregion using Alias Easy to Understand DataType





// �p�[�Z���g�����񃊃e����
#pragma region



constexpr float operator""per( unsigned long long value ) {

	return static_cast< float >( value );

}


constexpr float operator""per( long double value ) {

	return static_cast< float >( value );

}



#pragma endregion percentage literal operator
