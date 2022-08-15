#pragma once





#include<atomic>
#include<condition_variable>





class counting_semaphore {

private:


	std::atomic<unsigned int> m_counter;
	const unsigned int m_max;

public:


	explicit counting_semaphore( unsigned int counter_init, unsigned int counter_max )
		: m_counter( counter_init ), m_max(counter_max) {};

public:


//======================================
// operator
//======================================


#pragma region operator



	void operator++(int) {

		count_increment();

	}

	void operator--(int) {

		count_decrement();

	}


	void operator+=( unsigned int up_value ) {

		count_up( up_value );

	}


	void operator-=( unsigned int down_value ) {

		count_down( down_value );

	}



	friend std::ostream& operator<<( std::ostream& stream, counting_semaphore& semaphore ) {

		stream << semaphore.m_counter.load();

		return stream;

	}



#pragma endregion operator

public:


//===================================
// メンバ関数
//===================================


#pragma region counter control method


	// 自身の保持するカウンタを１加算する
	bool count_increment() {

		if( m_counter < m_max ) {

			m_counter++;

			return true;

		}
		else {

			return false;

		}

	}


	// 自身の保持するカウンタを１減算する
	bool count_decrement() {

		if( m_counter > 0 ) {

			m_counter--;

			return true;

		}
		else {

			return false;

		}

	}


	// 自身の保持するカウンタに任意の数だけ加算する
	bool count_up( unsigned int up_value ) {

		if( m_counter + up_value <= m_max ) {

			m_counter += up_value;

			return true;

		}
		else {

			m_counter = m_max;

			return false;

		}

	}


	// 自身の保持するカウンタに任意の数だけ減算する
	bool count_down( unsigned int down_value ) {

		if( m_counter - down_value >= 0 ) {

			m_counter -= down_value;

			return true;

		}
		else {

			m_counter = 0;

			return false;

		}

	}



#pragma endregion counter control method



#pragma region wait method


	// 自身の保持するカウンタが０になるまで待つ　※ビジーウェイトによる待機処理を行う
	void wait_count_zero() {

		while( m_counter != 0 )
			std::this_thread::yield();

	}


	// 自身の保持するカウンタが１以上になるまで待つ　※ビジーウェイトによる待機処理を行う
	void wait_count_above_zero() {

		while( m_counter == 0 )
			std::this_thread::yield();

	}


	// 自身の保持するカウンタが設定された最大値になるまで待つ　※ビジーウェイトによる待機処理を行う
	void wait_count_max() {

		while( m_counter < m_max )
			std::this_thread::yield();

	}


	// 自身が保持するカウンタの値と同等になるまで待つ　※ビジーウェイトによる待機処理を行う
	void wait_count_match( unsigned int count ) {

		while( m_counter != count )
			std::this_thread::yield();

	}


	// カウントが上がってくるのを待つ　※ビジーウェイトによる待機処理を行う
	void wait_count_plus( unsigned int count ) {

		while( m_counter < count )
			std::this_thread::yield();

	}


	// カウントが下がってくるのを待つ　※ビジーウェイトによる待機処理を行う
	void wait_count_minus( unsigned int count ) {
	
		while( m_counter > count )
			std::this_thread::yield();

	}


#pragma endregion wait method

};





class binary_semaphore {

private:


	std::atomic<bool> m_flag;

public:

	binary_semaphore() : m_flag(true) {};
	explicit binary_semaphore( bool flag_init ) : m_flag( flag_init ) {};

public:


	void operator=( bool value ) {

		m_flag = value;

	}

	operator bool() {
		return m_flag;
	}

public:

	bool get_flag() {
		return m_flag;
	}

	// 自身が保持するフラグをtrueにする
	// 保持するフラグがfalseの場合はtrueを返し、falseの場合はfalseを返す
	bool true_flag() {

		if( m_flag == false ) {

			m_flag = true;

			return true;

		}
		else {

			return false;

		}

	}


	// 自身が保持するフラグをfalseにする
	// 保持するフラグがtrueの場合はtrueを返し、falseの場合はfalseを返す
	bool false_flag() {

		if( m_flag == true ) {

			m_flag = false;

			return true;

		}
		else {

			return false;

		}

	}

	
	// 保持するフラグがtrueになるまで待つ　※ビジーウェイトによる待機処理を行う
	void wait_true() {

		while( m_flag != true )
			std::this_thread::yield();

	}


	// 保持するフラグがfalseになるまで待つ　※ビジーウェイトによる待機処理を行う
	void wait_false() {

		while( m_flag != false )
			std::this_thread::yield();

	}

};