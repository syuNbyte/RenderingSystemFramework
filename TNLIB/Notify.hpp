#pragma once





#include<future>
#include"AsyncFunction.hpp"




#define TRUE 1
#define FALSE 0




// notify系クラスとなっているが、signal系クラスでもいいかも　※これは名称に関してのことです

// std::shared_future<void>を使用してこのクラス群は設計されているが
// std::condition_variable型・std::condition_variable_any型を使用しての作成も可能なので最適解がstd::condition_variable型なのであれば
// そちらをベースとしたクラス設計に組み替えることも行う
#pragma region notify class



#pragma region notifyer class



// 通知を送るためだけのクラス
// 後から気づいたのだが、このクラスはC++20で実装されているSTL std::stop_sourceと似たような処理を行っているため
// このSTLに似たような設計のクラスを作る
class o_notifyer {

private:


	// 任意のタイミングで通知するメンバ関数notifyでこのstd::shared_futureへの参照を持つ別スレッド
	std::promise<void> m_notifyer;

public:


	// 任意のタイミングで通知を行い、自身が渡しているnotifyerが止めている他スレッドの処理を起こす
	void notify() {

		m_notifyer.set_value();

	}


	// 通知を行うメンバ関数notifyから通知が来た際に通知を受け取るオブジェクトを渡すメンバ関数
	std::shared_future<void> get_notifyer() {

		return m_notifyer.get_future();

	}

};



#pragma endregion notifyer class





#pragma region notify_getter class



// notifyerクラスが発する通知を受け取るためだけのクラス
// notifyerクラスと一緒で後から気づいたのだが、このクラスはC++20で実装されているSTL std::stop_tokenと似たような処理を行っているため
// このSTLに似たような設計のクラスを作る
class o_notify_getter {

private:


	// 他のasync_functionオブジェクトのnotifyを受け取るためのnotifyerを保持しておくためのメンバ
	std::shared_future<void> m_notifyer;

public:


	o_notify_getter() noexcept {};


	o_notify_getter( o_notifyer& notifyer ) {

		m_notifyer = notifyer.get_notifyer();

	}


	o_notify_getter( std::shared_future<void> notifyer ) {

		m_notifyer = notifyer;

	}

public:


	void set_notifyer( o_notifyer& notifyer ) {

		m_notifyer = notifyer.get_notifyer();

	}


	void set_notifyer( std::shared_future<void> notifyer ) {

		m_notifyer = notifyer;

	}


	const std::shared_future<void>& use_notifyer() {

		return m_notifyer;

	}


	// notify待ち
	void wait() {

		m_notifyer.wait();

	}


	// 指定した時間が経過したら現在の処理状況を返す
	template< class Rep, class Period >
	std::future_status wait_for( const std::chrono::duration<Rep, Period>& timeout_duration ) {

		/*============================================================================
		// 参考サイト：https://ja.cppreference.com/w/cpp/thread/future/wait_for
		//============================================================================
		// 説明
		//============================================================================
		// future_status::deferred	: 結果を計算するための関数はまだ開始されていません
		// future_status::ready		: 結果は準備完了しています
		// future_status::timeout	: タイムアウトが経過しました
		============================================================================*/

		return m_notifyer.wait_for( timeout_duration );

	}


	// 指定した時間になったら現在の処理状況を返す
	template< class Clock, class Duration >
	std::future_status wait_until( const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		/*============================================================================
		// 参考サイト：https://ja.cppreference.com/w/cpp/thread/future/wait_until
		//============================================================================
		// 説明
		//============================================================================
		// future_status::deferred	: 結果を計算するための関数はまだ開始されていません
		// future_status::ready		: 結果は準備完了しています
		// future_status::timeout	: タイムアウトが経過しました
		============================================================================*/

		return m_notifyer.wait_until( timeout_time );

	}


};



#pragma endregion notify_getter class





#pragma region notify_callback




// m_argsの一番最初の要素に自クラスの&threadsafe_func_typeを追加できたら実装できる
#if FALSE





template<class Func>
class notify_callback{};



// notifyerクラス・notify_getterクラスががSTL std::stop_source, std::stop_tokenに似たような処理を行っているため
// std::stop_callbackクラスに似たようなクラスをここに新しく作成する
template<class Result, class ...FuncArgs>
class notify_callback<Result( FuncArgs... )> {

private:


	typedef std::tuple<threadsafe_function<Result( FuncArgs... )>> threadsafe_tuple_type;

public:


	typedef notify_callback<Result(FuncArgs...)> this_type;					// 自身のデータ型
	typedef async_function<Result( this_type )> async_func_type;					// 非同期処理を実行する関数クラス型
	typedef threadsafe_function<Result( FuncArgs... )> threadsafe_func_type;	// 自身の保有するスレッドセーフ関数クラスのデータ型
	typedef std::function<Result( FuncArgs... )> func_type;							// 自身の保有するスレッドセーフ関数クラスの関数オブジェクト型
	typedef Result result_type;																// 自身の保有するスレッドセーフ関数クラスの関数オブジェクトの戻り値型
	typedef std::tuple<FuncArgs...> args_type;											// 自身の保有するスレッドセーフ関数クラスの関数オブジェクトの引数型をまとめたtuple型

private:


	async_func_type m_asyncFunction;
	threadsafe_func_type m_threadsafe_func;

	args_type m_args;

	notify_getter m_info_getter;

public:


	notify_callback() noexcept {};


	// RAII方式を採用するコンストラクタ
	notify_callback( notify_getter notify_getter, func_type function, FuncArgs... args ) {

		// コールバックする関数を設定　※コールバック関数：非同期で実行される関数
		set_func( function );

		// コールバック関数の実行時に使用する引数を設定
		set_args( args );

		// 通知があった際に受け取るオブジェクト
		m_info_getter = notify_getter;

	}


private:


	
	result_type callback_exe() {

		// 通知が車でここで待機
		m_info_getter.wait();

		// 待機処理が終了したら関数を実行する
		return std::apply( 
			&decltype(m_threadsafe_func)::this_type::func_exe,
			m_args
		);

	}


	result_type threadsafe_callback_exe() {

		// 通知が車でここで待機
		m_info_getter.wait();

		// 待機処理が終了したらスレッドセーフな関数を実行する
		return std::apply(
			&decltype( m_threadsafe_func )::this_type::threadsafe_func_exe,
			m_args
		);

	}

public:


	void set_func( func_type function ) {

		// 関数を設定
		m_threadsafe_func = function;
		m_asyncFunction.set_func( &this_type::callback_exe );

	}


	void set_args( FuncArgs... args ) {

		// std::apply関数を使用すればstd::tupleの中身を関数に展開して渡すことができるらしい：https://qiita.com/seriru13/items/5115e92fa2665d01d2e1#:~:text=std%3A%3Aapply%20%E3%81%AF%E5%AE%9F%E8%A1%8C%E3%81%99%E3%82%8B%E9%96%A2%E6%95%B0%E3%81%A8%E3%82%BF%E3%83%97%E3%83%AB%E3%82%92%E5%BC%95%E6%95%B0%E3%81%A8%E3%81%97%E3%81%A6%E5%8F%96%E3%82%8A%E3%80%81,std%3A%3A__apply_impl%20%E3%82%92%E5%91%BC%E3%81%B3%E5%87%BA%E3%81%97%E3%81%BE%E3%81%99%E3%80%82
		// これを使用して後から引数を関数に渡す：https://qiita.com/_EnumHack/items/677363eec054d70b298d#index-tuple-idiom-the-indices-trick
		
		
		m_args = std::make_tuple<threadsafe_func_type*, FuncArgs...>( &m_threadsafe_func, args,,, );

	}


	void set_notify_getter( notify_getter notify_getter ) {

		m_info_getter = notify_getter;

	}



	result_type get_result() {

		m_asyncFunction.get_result();

	}



};

#endif




// C++20で実装されているstd::stop_callbackと同等の機能を持つ（多分）
// notifyerクラスがnotifyメソッドを実行したタイミングでこのコールバック関数が呼ばれる
class o_notify_callback {

private:


	async_function<void(o_notify_callback*)> m_asyncFunction;

	threadsafe_function<void()> m_threadsafeFunction;

	o_notify_getter m_notifyGetter;

private:


	void callback_exe() {

		m_notifyGetter.wait();

		m_threadsafeFunction();

	}

public:


	o_notify_callback( o_notify_getter notify_getter, std::function<void()> func ) {

		m_notifyGetter = notify_getter;

		m_threadsafeFunction.set_func( func );

		m_asyncFunction.set_func( &o_notify_callback::callback_exe );

		m_asyncFunction.async( this );

	}


};





#pragma endregion notify_callback



#pragma endregion old_notify class










#pragma region new notifyer class




// 前方宣言
class notifyer;





class notify_getter {

private:


	using condition_variable_pointer = std::add_pointer_t<std::condition_variable>;
	using consistency_flag_pointer = std::add_pointer_t<bool>;
	using mutex_pointer = std::add_pointer_t<std::mutex>;

	using condition_variable_lvalue = std::add_lvalue_reference_t<std::condition_variable>;
	using consistency_flag_lvalue = std::add_lvalue_reference_t<bool>;
	using mutex_lvalue = std::add_lvalue_reference_t<std::mutex>;

private:


	condition_variable_pointer m_cvPointer;				// notifyerクラスの持つ条件変数へのポインター
	consistency_flag_pointer m_consistencyFlagPointer;	// notifyerクラスの持つspurious wakeup防止フラグのポインター
	mutex_pointer m_mutexPointer;								// notifyerクラスの持つmutex型へのポインター

public:


	notify_getter() = default;
	notify_getter( const notify_getter& notifyGetter ) = default;
	notify_getter( notify_getter&& notifyGetter ) = default;


	notify_getter( notifyer& notifyer );
	notify_getter( notifyer&& notifyer );


	notify_getter( condition_variable_pointer cvPointer, consistency_flag_pointer consistencyFlagPointer, mutex_pointer mutexPointer );
	notify_getter( condition_variable_lvalue cvPointer, consistency_flag_lvalue consistencyFlagPointer, mutex_lvalue mutexPointer );
	
public:


	notify_getter& operator=( const notify_getter& notifyGetter ) = default;
	notify_getter& operator=( notify_getter&& notifyGetter ) = default;

	notify_getter& operator=( notifyer& notifyer);
	notify_getter& operator=( notifyer&& notifyer);

public:


	// この通知受信関数が有効かどうか（有効なnotifyerのnotify_getterかどうかという意味で、falseなら通知関数を受け取れていないという意味）
	bool enable() {

#ifdef _DEBUG

		try {

			if( m_mutexPointer != nullptr && m_cvPointer != nullptr && m_consistencyFlagPointer != nullptr ) {

				return true;

			}
			else {

				throw std::runtime_error( "is invalid notify_getter." );

				return false;

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}
		
#else

		return ( m_mutexPointer != nullptr && m_cvPointer != nullptr && m_consistencyFlagPointer != nullptr );

#endif

	}


	// 現在保持している通知を受け取り先のデータを削除する
	void discard() {

		m_mutexPointer = nullptr;
		m_cvPointer = nullptr;
		m_consistencyFlagPointer = nullptr;

	}

public:

//===================================================
// ロック取得関数
//===================================================


	// 自身の保持するstd::mutexへの参照を返す
	std::mutex& get_mutex() {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return *m_mutexPointer;

			}
			else {

				throw std::runtime_error( "mutex is nullptr." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return *m_mutexPointer;

#endif
	}


	// 自身の保持するstd::mutexを使用してstd::unique_lockを作成
	std::unique_lock<std::mutex> get_lock() {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return std::unique_lock<std::mutex>( *m_mutexPointer );

			}
			else {

				throw std::runtime_error( "Failed to acquire the lock." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return std::unique_lock<std::mutex> ( *m_mutexPointer );

#endif

	}


	// 自身の保持するstd::mutexを使用して遅延ロックのstd::unique_lockを作成　
	// ※遅延ロックのため自身でロック関数を実行する必要がある
	std::unique_lock<std::mutex> get_defferd_lock() {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return std::unique_lock<std::mutex>( *m_mutexPointer, std::defer_lock );

			}
			else {

				throw std::runtime_error( "Failed to acquire the deferred lock." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return std::unique_lock<std::mutex>( *m_mutexPointer, std::defer_lock );

#endif

	}


	// 自身の保持するstd::mutexを使用してのstd::unique_lockを作成
	// ※ロックが取得できればtrueを返し、取得できなければfalseを返す
	std::unique_lock<std::mutex> get_try_lock() {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return std::unique_lock<std::mutex>( *m_mutexPointer, std::try_to_lock );

			}
			else {

				throw std::runtime_error( "Failed to acquire the try lock." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return std::unique_lock<std::mutex>( *m_mutexPointer, std::try_to_lock );

#endif

	}


	// 自身の保持するstd::mutexを使用してこの関数実行以前にロック取得済みのstd::mutexのロックをこのstd::unique_lockに移譲するのstd::unique_lockを作成　
	// ※基本的には使用しないロック取得方法だが機能としての提供のみを行っている状態で危険性が高いロック取得方法になる（通知元の存在するスレッドのロックをはずしてしまうことに相当するため危険でなロック取得方法）
	std::unique_lock<std::mutex> get_adopt_lock() {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return std::unique_lock<std::mutex>( *m_mutexPointer, std::adopt_lock );

			}
			else {

				throw std::runtime_error( "Failed to acquire the adopt lock." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return std::unique_lock<std::mutex>( *m_mutexPointer, std::adopt_lock );

#endif

	}


	// 自身の保持する整合性フラグの結果を取得する　※整合性フラグ：このクラスの持つ通知元のnotifyerクラスに到達しているかどうかを示すフラグ
	bool get_consistency_flag() {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return *m_consistencyFlagPointer;

			}
			else {

				throw std::runtime_error( "condition variable is nullptr." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return *m_consistencyFlagPointer;

#endif

	}

public:


#pragma region wait 


	// 通知元に処理が到達するまでこのスレッドの処理を待機する
	void wait( std::unique_lock<std::mutex>& lock ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				m_cvPointer->wait( lock, [ this ] { return get_consistency_flag(); } );

				//discard(); // 一度waitを使用したら再使用不可にする

			}
			else {

				throw std::runtime_error( "Failed to execute the wait method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		m_cvPointer->wait( lock, [this] { return get_consistency_flag(); } );

#endif

	}


	// 通知元に処理が到達し、predicate関数の実行結果がtrueを返すまでこのスレッドの処理を待機する
	template<class Predicate>
	void wait( std::unique_lock<std::mutex>& lock, Predicate predicate ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				m_cvPointer->wait( lock, [ & ] { return predicate(); } );

			}
			else {

				throw std::runtime_error( "Failed to execute the wait method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		m_cvPointer->wait( lock, [&] { return predicate(); } );

#endif

	}



#pragma endregion wait 



#pragma region wait_for 


	// 指定した時間経過後に待機処理が終了できるなら終了し終了できないなら終了しない
	// 待機処理が終了できた場合はstd::cv_status::no_timeoutを返し、終了できなかった場合はstd::cv_status::timeoutを返す
	template< class Rep, class Period >
	std::cv_status wait_for( std::unique_lock<std::mutex>& lock, const std::chrono::duration<Rep, Period>& timeoutDuration ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return m_cvPointer->wait_for( lock, timeoutDuration );

			}
			else {

				throw std::runtime_error( "Failed to execute the wait_for method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return m_cvPointer->wait_for( lock, timeoutDuration );

#endif

	}



	// 指定した時間経過後に待機処理が終了できるなら終了し終了できないなら終了しない
	// 待機処理が終了できており述語(predicate)がtrueの場合はtrueを返し、それ以外の場合はfalseを返す
	template< class Rep, class Period, class Predicate >
	bool wait_for_pred( std::unique_lock<std::mutex>& lock, const std::chrono::duration<Rep, Period>& timeoutDuration, Predicate predicate ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return m_cvPointer->wait_for( lock, timeoutDuration, [ & ] { predicate(); } );

			}
			else {

				throw std::runtime_error( "Failed to execute the wait_for_pred method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return m_cvPointer->wait_for( lock, timeoutDuration, [&] { predicate(); } );

#endif

	}


	// 指定した時間経過後に待機処理が終了できるなら終了し終了できないなら終了しない
	// 待機処理が終了できた場合はtrueを返し、終了できなかった場合はfalseを返す
	template< class Rep, class Period >
	bool wait_for_pred( std::unique_lock<std::mutex>& lock, const std::chrono::duration<Rep, Period>& timeoutDuration ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return m_cvPointer->wait_for( lock, timeoutDuration, [ this ] { return get_consistency_flag(); } );

			}
			else {

				throw std::runtime_error( "Failed to execute the wait_for_pred method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return m_cvPointer->wait_for( lock, timeoutDuration, [ this ] { return get_consistency_flag(); } );

#endif

	}



#pragma endregion wait_for



#pragma region wait_until


	// 指定した時間までに待機処理が終了できるなら終了し終了できないなら終了しない
	// 待機処理が終了できた場合はstd::cv_status::no_timeoutを返し、終了できなかった場合はstd::cv_status::timeoutを返す
	template< class Clock, class Duration >
	std::cv_status wait_until( std::unique_lock<std::mutex>& lock, const std::chrono::time_point<Clock, Duration>& timeoutTime ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return m_cvPointer->wait_until( lock, timeoutTime );

			}
			else {

				throw std::runtime_error( "Failed to execute the wait_until method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return m_cvPointer->wait_until( lock, timeoutTime );

#endif

	}


	// 指定した時間までに待機処理が終了できるなら終了し終了できないなら終了しない
	// 待機処理が終了できており述語(predicate)がtrueの場合はtrueを返し、それ以外の場合はfalseを返す
	template< class Clock, class Duration, class Predicate >
	bool wait_until_pred( std::unique_lock<std::mutex>& lock, const std::chrono::time_point<Clock, Duration>& timeoutTime, Predicate predicate ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return m_cvPointer->wait_until( lock, timeoutTime, predicate );

			}
			else {

				throw std::runtime_error( "Failed to execute the wait_until_pred method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return m_cvPointer->wait_until( lock, timeoutTime, predicate );

#endif

	}


	// 指定した時間までに待機処理が終了できるなら終了し終了できないなら終了しない
	// 待機処理が終了できた場合はtrueを返し、終了できなかった場合はfalseを返す
	template< class Clock, class Duration >
	bool wait_until_pred( std::unique_lock<std::mutex>& lock, const std::chrono::time_point<Clock, Duration>& timeoutTime ) {

#ifdef _DEBUG

		try {

			if( enable() ) {

				return m_cvPointer->wait_until( lock, timeoutTime, [ this ] { return get_consistency_flag(); } );

			}
			else {

				throw std::runtime_error( "Failed to execute the wait_until_pred method." );

			}

		}
		catch( std::runtime_error& runtime_error ) {

			std::cout << runtime_error.what() << std::endl;

		}

#else

		return m_cvPointer->wait_until( lock, timeoutTime, [ this ] { return get_consistency_flag(); } );

#endif

	}



#pragma endregion wait_until



};





class notifyer {

private:


	std::condition_variable m_cv;		// 保持する条件変数
	bool m_consistencyFlag = false;	// notify系関数が呼ばれたことを保証するための整合性フラグ
	std::mutex m_mutex;					// 保持するmutex

public:


	notify_getter&& get_notifyer() {

		return notify_getter(
			&m_cv,
			&m_consistencyFlag,
			&m_mutex
		);

	}



	std::mutex& get_mutex() {

		return m_mutex;

	}



	std::unique_lock<std::mutex> get_lock() {

		return std::unique_lock<std::mutex>( m_mutex );

	}



	std::unique_lock<std::mutex> get_defferd_lock() {

		return std::unique_lock<std::mutex>( m_mutex, std::defer_lock );

	}



	std::unique_lock<std::mutex> get_try_lock() {

		return std::unique_lock<std::mutex>( m_mutex, std::try_to_lock );

	}



	std::unique_lock<std::mutex> get_adopt_lock() {

		return std::unique_lock<std::mutex>( m_mutex, std::adopt_lock );

	}

public:


	//======================================================================================================
	// このクラスインスタンスのget_notifyerメンバ関数を使用して作成された
	// notify_getterクラスインスタンスでのwait系関数を実行中のスレッドをどれか一つのみ待機処理を終了させる通知を送る
	//======================================================================================================o
	// ※このメンバ関数を使用する際は以下の順番を守る必要がある。　これに順守しない場合は未定義の動作となる
	// ①このクラスインスタンスからget_notifyerメンバ関数を用いて作成された
	// 　notify_getterクラスインスタンスを使用したwait系関数の実行処理を行うスレッドを先に作成する
	// ②このメンバ関数を実行するスレッドを作成する
	//======================================================================================================
	// 以上の順番を遵守しない場合は以下のようなバグを招く可能性が高い
	// 　notify_oneを一度しか実行していないにも関わらず、複数のwait系関数が待機処理を終了する（notify_allのような挙動をする）
	void notify_one() {

		m_consistencyFlag = true;

		m_cv.notify_one();

	}


	// このクラスインスタンスのget_notifyerメンバ関数を使用して作成された
	// notify_getterクラスインスタンスでのwait系関数を実行中のスレッドを全て待機処理を終了させる通知を送る
	void notify_all() {

		m_consistencyFlag = true;

		m_cv.notify_all();

	}


	// このメンバ関数はstd::thread内で実行された場合のみ実行可能、それ以外の場合は未定義の動作を起こす
	// std::async関数の中で実行した場合は実行されない
	void notify_all_at_thread_exit( std::unique_lock<std::mutex>& lock ) {

		m_consistencyFlag = true;

		std::notify_all_at_thread_exit( m_cv, std::move(lock) );

	}

};



#pragma endregion new notifyer class





#pragma region wait object class



// RAIIクラスで単純に渡されたnotifyerから通知がくるまで待つだけのクラス
// 使い方はstd::lcok_guardのように使用する
class wait_object {

public:


	wait_object() = delete;
	wait_object( const wait_object& ) = delete;
	wait_object( wait_object&& ) = delete;



	explicit wait_object( notifyer& notifyer ) {

		auto notify_getter = notifyer.get_notifyer();

		auto lock = notify_getter.get_lock();

		notify_getter.wait( lock );

	}


	explicit wait_object( notify_getter& notify_getter ) {

		auto lock = notify_getter.get_lock();

		notify_getter.wait( lock );

	}

};



#pragma endregion wait object class









// クラス内にインライン展開するとnotifyerクラスの定義が見つからずエラーを吐くので、やむなくファイル最下部にインライン展開を行っている
#pragma region notify_getter constructor



inline notify_getter::notify_getter( notifyer& notifyer ) {
	*this = notifyer.get_notifyer();
};



inline notify_getter::notify_getter( notifyer&& notifyer ) {
	*this = notifyer.get_notifyer();
};



inline notify_getter::notify_getter( condition_variable_pointer cvPointer, consistency_flag_pointer consistencyFlagPointer, mutex_pointer mutexPointer )
	: m_cvPointer( cvPointer ), m_consistencyFlagPointer( consistencyFlagPointer ), m_mutexPointer( mutexPointer ) {};



inline notify_getter::notify_getter( condition_variable_lvalue cvPointer, consistency_flag_lvalue consistencyFlagPointer, mutex_lvalue mutexPointer )
	: m_cvPointer( &cvPointer ), m_consistencyFlagPointer( &consistencyFlagPointer ), m_mutexPointer( &mutexPointer ) {};



#pragma endregion notify_getter constructor



#pragma region notify_getter operator=



inline notify_getter& notify_getter::operator=( notifyer& notifyer ) {
	*this = notifyer.get_notifyer();
};



inline notify_getter& notify_getter::operator=( notifyer&& notifyer ) {
	*this = notifyer.get_notifyer();
};



#pragma endregion notify_getter operator=
