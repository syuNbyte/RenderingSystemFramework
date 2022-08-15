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

	// 集計に使用する時間データ
	std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_end;

	// サンプリングに使用するデータ
	std::vector<longlong_t> m_sample;	// サンプリング数と同等数の直近の処理結果　※サンプルは全て最高分解性能のナノ秒で入っている
	ulonglong_t m_samplingRate;			// サンプリング数

	// 計測に使用するフラグ
	bool m_measureStart;
	bool m_measureEnd;

	// 範囲の計測をするかどうかのフラグ
	bool m_measureFlag;

	// 範囲の計測結果をサンプリングをするかどうかのフラグ
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

		// サンプリング数の設定
		m_samplingRate = samplingRate;
	}

public:

#pragma region GetSampleData

	// サンプリングデータの一部をナノ秒で取得
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

	// サンプリングデータの一部をマイクロ秒で取得
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

	// サンプリングデータの一部をミリ秒で取得
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

	// サンプリングデータの一部を秒で取得
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

	// サンプリングデータの一部を分で取得
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

	// サンプリングデータの一部を時で取得
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

	// 現在のサンプリング制度を取得
	ulonglong_t GetSamplingRate() {
		return m_samplingRate;
	}

	// サンプリング精度の設定
	void SetSamplingRate( ulonglong_t samplingRate ) {
		m_samplingRate = samplingRate;
		m_sample.resize( samplingRate );
	}

	// 現在の保有サンプル数
	std::size_t GetSampleNum() {
		return m_sample.size();
	}

	// 現在保持するサンプルを破棄する
	void RemoveSample() {
		m_sample.clear();
	}

	// 範囲の計測開始
	void Begin() {
		if( m_measureFlag ) {
			// 計測するなら以下の代入処理を行う
			m_measureStart = true;
			m_measureEnd = false;
			m_start = std::chrono::high_resolution_clock::now();
		}
	}

	// 範囲の計測終了
	void End() {
		if( m_measureFlag ) {
			// 計測するなら以下の代入処理を行う
			m_measureEnd = true;
			m_end = std::chrono::high_resolution_clock::now();
		}
		if( !m_stopSampling ) {
			// サンプリング数を超えているなら、一番古いサンプルの削除
			if( m_sample.size() >= m_samplingRate && !m_sample.empty() )	m_sample.erase( m_sample.begin() );

			// サンプルの保存
			m_sample.push_back( NanoSecI() );
		}
	}

public:

#pragma region Flag

	// 範囲の計測を行うかどうかのフラグを設定する
	void SetMeasureFlag( bool flag ) {
		m_measureFlag = flag;
	}

	// サンプリングを停止するかどうかのフラグを設定する
	void SetStopSamplingFlag( bool flag ) {
		m_stopSampling = flag;
	}

	// 範囲の計測を行っているかどうかを返す
	// 範囲の計測を行っているならtrueを返す
	bool GetMeasureFlag() {
		return m_measureFlag = true;
	}

	// 現在サンプリングを行っているかどうかを返す
	// サンプリングしているならtrueを返す
	bool GetSamplingState() {
		return ( !m_stopSampling );
	}

	// 範囲の計測が開始しているかどうかを返す
	bool GetMeasureStart() {
		return m_measureStart;
	}

	// 範囲の計測が終了しているかどうかを返す
	bool GetMeasureEnd() {
		return m_measureEnd;
	}

	// 範囲の計測中かどうかを返す
	bool GetMeasureProcess() {
		return m_measureStart;
	}

#pragma endregion Flag

public:

//===================================================
// 整数で取得
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
// 計測された数値の取得
//===================================================

	// 直前の範囲計測結果を
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T NanoSec() {
		if( m_measureFlag && ( m_measureStart && m_measureEnd )) {
			// 計測されているならその数値を返す
			return static_cast< T >( std::chrono::duration_cast< std::chrono::nanoseconds >( m_end - m_start ).count() );
		}
		else if( m_measureFlag && ( m_measureStart && !m_measureEnd ) ) {
			// 計測終了が部分に到達していないなら計測開始から現在までの数値を返す
			return static_cast< T >( std::chrono::duration_cast< std::chrono::nanoseconds >( std::chrono::high_resolution_clock::now() - m_start ).count() );
		}
		else {
			// 計測されていないならゼロを返す
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
// サンプリング結果の平均
//===================================================

	// サンプリング結果の平均値をナノ秒で返す
	template<class T = float_t, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
	T AverageNanoSec() {
		// 要素が空ではないなら計算する
		if( !m_sample.empty() ) {
			// 最後の引数の0LLで戻り値の数値が決まるので変更してはいけない
			return static_cast< T >( std::accumulate( m_sample.begin(), m_sample.end(), 0LL ) / m_sample.size() );
		}
		else {
			return static_cast< T >( 0.0 );
		}
	}

	// サンプリング結果の平均値をマイクロ秒で返す
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

	// サンプリング結果の平均値をミリ秒で返す
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

	// サンプリング結果の平均値を秒で返す
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

	// サンプリング結果の平均値を分で返す
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

	// サンプリング結果の平均値を時で返す
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

	// 集計に使用する時間データ
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