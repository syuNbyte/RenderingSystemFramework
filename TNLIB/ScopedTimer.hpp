#pragma once

#include<chrono>
#include<vector>
#include<numeric>
#include<type_traits>

#include"SafetyDataType.h"
#include"Math.hpp"

using namespace std::chrono_literals;

class ScopedTimer {

private:

	// �W�v�Ɏg�p���鎞�ԃf�[�^
	std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_end;

	// �T���v�����O�Ɏg�p����f�[�^
	std::vector<longlong_t> m_sample;	// �T���v�����O���Ɠ������̒��߂̏������ʁ@���T���v���͑S�čō����𐫔\�̃i�m�b�œ����Ă���
	ulonglong_t m_samplingRate;			// �T���v�����O��

	// �v���Ɏg�p����t���O
	bool m_measureStart;
	bool m_measureEnd;

	// �͈͂̌v�������邩�ǂ����̃t���O
	bool m_measureFlag;

	// �͈͂̌v�����ʂ��T���v�����O�����邩�ǂ����̃t���O
	bool m_stopSampling;

public:

	ScopedTimer() {
		m_measureStart = false;
		m_measureEnd = false;
		m_measureFlag = true;
		m_stopSampling = false;
	};
	explicit ScopedTimer( ulonglong_t samplingRate ){
		m_measureStart = false;
		m_measureEnd = false;
		m_measureFlag = true;
		m_stopSampling = false;

		// �T���v�����O���̐ݒ�
		m_samplingRate = samplingRate;
	}

public:

#pragma region GetSampleData

	// �T���v�����O�f�[�^�̈ꕔ���i�m�b�Ŏ擾
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T GetSampleDataNanoSec( std::size_t index ) {
		if( index < m_sample.size() ) {
			return static_cast<T>(m_sample[index]);
		}
		else {
			Debug::LogWarning( "ScopedTimer::GetSampleDataNanoSec : index out of range." );
			return static_cast< T >( 0.0 );
		}
	}

	// �T���v�����O�f�[�^�̈ꕔ���}�C�N���b�Ŏ擾
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T GetSampleDataMicroSec() {
		auto nanoSec = GetSampleDataNanoSec<T>();
		if( !Math::IsZero( nanoSec ) ) {
			return nanoSec / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
			return nanoSec;
		}
	}

	// �T���v�����O�f�[�^�̈ꕔ���~���b�Ŏ擾
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T GetSampleDataMilliSec() {
		auto microSec = GetSampleDataMicroSec<T>();
		if( !Math::IsZero( microSec ) ) {
			return microSec / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
			return microSec;
		}
	}

	// �T���v�����O�f�[�^�̈ꕔ��b�Ŏ擾
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T GetSampleDataSecond() {
		auto milliSec = GetSampleDataMilliSec<T>();
		if( !Math::IsZero( milliSec ) ) {
			return milliSec / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
			milliSec;
		}
	}

	// �T���v�����O�f�[�^�̈ꕔ�𕪂Ŏ擾
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T GetSampleDataMinute() {
		auto second = GetSampleDataSecond<T>();
		if( !Math::IsZero( second ) ) {
			return second / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
			return second;
		}
	}

	// �T���v�����O�f�[�^�̈ꕔ�����Ŏ擾
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T GetSampleDataHour() {
		auto minute = GetSampleDataMinute<T>();
		if( !Math::IsZero( minute ) ) {
			return minute / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
			minute;
		}
	}

#pragma endregion GetSampleData

public:

	// ���݂̃T���v�����O���x���擾
	ulonglong_t GetSamplingRate() {
		return m_samplingRate;
	}

	// �T���v�����O���x�̐ݒ�
	void SetSamplingRate( ulonglong_t samplingRate ) {
		m_samplingRate = samplingRate;
		m_sample.resize( samplingRate );
	}

	// ���݂ۗ̕L�T���v����
	std::size_t GetSampleNum() {
		return m_sample.size();
	}

	// ���ݕێ�����T���v����j������
	void RemoveSample() {
		m_sample.clear();
	}

	// �͈͂̌v���J�n
	void Begin() {
		if( m_measureFlag ) {
			// �v������Ȃ�ȉ��̑���������s��
			m_measureStart = true;
			m_measureEnd = false;
			m_start = std::chrono::high_resolution_clock::now();
		}
	}

	// �͈͂̌v���I��
	void End() {
		if( m_measureFlag ) {
			// �v������Ȃ�ȉ��̑���������s��
			m_measureEnd = true;
			m_end = std::chrono::high_resolution_clock::now();
		}
		if( !m_stopSampling ) {
			// �T���v�����O���𒴂��Ă���Ȃ�A��ԌÂ��T���v���̍폜
			if( m_sample.size() >= m_samplingRate && !m_sample.empty() )	m_sample.erase( m_sample.begin() );

			// �T���v���̕ۑ�
			m_sample.push_back( NanoSecI() );
		}
	}

public:

#pragma region Flag

	// �͈͂̌v�����s�����ǂ����̃t���O��ݒ肷��
	void SetMeasureFlag( bool flag ) {
		m_measureFlag = flag;
	}

	// �T���v�����O���~���邩�ǂ����̃t���O��ݒ肷��
	void SetStopSamplingFlag( bool flag ) {
		m_stopSampling = flag;
	}

	// �͈͂̌v�����s���Ă��邩�ǂ�����Ԃ�
	// �͈͂̌v�����s���Ă���Ȃ�true��Ԃ�
	bool GetMeasureFlag() {
		return m_measureFlag = true;
	}

	// ���݃T���v�����O���s���Ă��邩�ǂ�����Ԃ�
	// �T���v�����O���Ă���Ȃ�true��Ԃ�
	bool GetSamplingState() {
		return ( !m_stopSampling );
	}

	// �͈͂̌v�����J�n���Ă��邩�ǂ�����Ԃ�
	bool GetMeasureStart() {
		return m_measureStart;
	}

	// �͈͂̌v�����I�����Ă��邩�ǂ�����Ԃ�
	bool GetMeasureEnd() {
		return m_measureEnd;
	}

	// �͈͂̌v�������ǂ�����Ԃ�
	bool GetMeasureProcess() {
		return m_measureStart;
	}

#pragma endregion Flag

public:

//===================================================
// �����Ŏ擾
//===================================================

	longlong_t NanoSecI() {
		return std::chrono::duration_cast< std::chrono::nanoseconds >( m_end - m_start ).count();
	}

	longlong_t MicroSecI() {
		return std::chrono::duration_cast< std::chrono::microseconds >( m_end - m_start ).count();
	}

	longlong_t MilliSecI() {
		return std::chrono::duration_cast< std::chrono::milliseconds >( m_end - m_start ).count();
	}

	longlong_t SecondI() {
		return std::chrono::duration_cast< std::chrono::seconds >( m_end - m_start ).count();
	}

	longlong_t MinuteI() {
		return std::chrono::duration_cast< std::chrono::minutes >( m_end - m_start ).count();
	}

	longlong_t HourI() {
		return std::chrono::duration_cast< std::chrono::hours >( m_end - m_start ).count();
	}

//===================================================
// �v�����ꂽ���l�̎擾
//===================================================

	// ���O�͈̔͌v�����ʂ�
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T NanoSec() {
		if( m_measureFlag && ( m_measureStart && m_measureEnd )) {
			// �v������Ă���Ȃ炻�̐��l��Ԃ�
			return static_cast< T >( std::chrono::duration_cast< std::chrono::nanoseconds >( m_end - m_start ).count() );
		}
		else if( m_measureFlag && ( m_measureStart && !m_measureEnd ) ) {
			// �v���I���������ɓ��B���Ă��Ȃ��Ȃ�v���J�n���猻�݂܂ł̐��l��Ԃ�
			return static_cast< T >( std::chrono::duration_cast< std::chrono::nanoseconds >( std::chrono::high_resolution_clock::now() - m_start ).count() );
		}
		else {
			// �v������Ă��Ȃ��Ȃ�[����Ԃ�
			return static_cast< T >( 0.0 );
		}
	}

	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T MicroSec() {
		auto nanoSec = NanoSec<T>();
		if( !Math::IsZero( nanoSec ) ) {
			return nanoSec / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
		}
	}

	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T MilliSec() {
		auto microSec = MicroSec<T>();
		if( !Math::IsZero( microSec ) ) {
			return microSec / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
		}
	}

	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T Second() {
		auto milliSec = MilliSec<T>();
		if( !Math::IsZero( milliSec ) ) {
			return milliSec / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
		}
	}

	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T Minute() {
		auto second = Second<T>();
		if( !Math::IsZero( second ) ) {
			return second / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
		}
	}

	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T Hour() {
		auto minute = Minute<T>();
		if( !Math::IsZero( minute ) ) {
			return minute / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
		}
	}


//===================================================
// �T���v�����O���ʂ̕���
//===================================================

	// �T���v�����O���ʂ̕��ϒl���i�m�b�ŕԂ�
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T AverageNanoSec() {
		// �v�f����ł͂Ȃ��Ȃ�v�Z����
		if( !m_sample.empty() ) {
			// �Ō�̈�����0LL�Ŗ߂�l�̐��l�����܂�̂ŕύX���Ă͂����Ȃ�
			return static_cast< T >( std::accumulate( m_sample.begin(), m_sample.end(), 0LL ) / m_sample.size() );
		}
		else {
			return static_cast< T >( 0.0 );
		}
	}

	// �T���v�����O���ʂ̕��ϒl���}�C�N���b�ŕԂ�
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T AverageMicroSec() {
		auto average = AverageNanoSec<T>();
		if( !Math::IsZero( average ) ) {
			return average / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
		}
	}

	// �T���v�����O���ʂ̕��ϒl���~���b�ŕԂ�
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T AverageMilliSec() {
		auto average = AverageMicroSec<T>();
		if( !Math::IsZero( average ) ) {
			return average / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
		}
	}

	// �T���v�����O���ʂ̕��ϒl��b�ŕԂ�
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T AverageSecond() {
		auto average = AverageMilliSec<T>();
		if( !Math::IsZero( average ) ) {
			return average / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
		}
	}

	// �T���v�����O���ʂ̕��ϒl�𕪂ŕԂ�
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T AverageMinute() {
		auto average = AverageSecond<T>();
		if( !Math::IsZero( average ) ) {
			return average / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
		}
	}

	// �T���v�����O���ʂ̕��ϒl�����ŕԂ�
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T AverageHour() {
		auto average = AverageMinute<T>();
		if( !Math::IsZero( average ) ) {
			return average / static_cast< T >( 1000 );
		}
		else {
			return static_cast< T >( 0.0 );
		}
	}

};



class Time {

private:

	// �W�v�Ɏg�p���鎞�ԃf�[�^
	std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_end;
	longlong_t m_deltaTime;

private:

	Time() {
		m_deltaTime = 0;
	}

private:

	static Time& GetInstance() {
		static Time time;
		return time;
	}

public:

	static void Begin() {
		GetInstance().m_start = std::chrono::high_resolution_clock::now();
	}

	static void End() {
		auto& instance = GetInstance();
		instance.m_end = std::chrono::high_resolution_clock::now();
		instance.m_deltaTime = std::chrono::duration_cast< std::chrono::milliseconds >( instance.m_end - instance.m_start ).count();;
	}

	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	static T DeltaTime() {
		auto& instance = GetInstance();
		if( !Math::IsZero( instance.m_deltaTime ) ) {
			return static_cast< T >( instance.m_deltaTime / static_cast< T >( 1000 ) );
		}
		else {
			return static_cast< T >( 0.0 );
		}
	}



};