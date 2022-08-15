#pragma once





#include"AsyncFunctionInterface.h"
#include"FutureCash.hpp"





// このクラスを作った時には気付かなかったけど
// std::packed_taskクラスと似たような動きをするがこちらの方がメンバ関数のメッセージ性が高いのでお勧めする
#pragma region async_function class



template<class Func>
class async_function {};



template<class Result, class ...FuncArgs >
class async_function<Result( FuncArgs... )> {

public:


	using this_type = async_function<Result( FuncArgs... )>;								// 自身のデータ型
	using threadsafe_func_type = threadsafe_function<Result( FuncArgs... )>;		// 自身の保有するスレッドセーフ関数クラスのデータ型
	using func_type = std::function<Result( FuncArgs... )>;								// 自身の保有するスレッドセーフ関数クラスの関数オブジェクト型
	using result_type = Result;																	// 自身の保有するスレッドセーフ関数クラスの関数オブジェクトの戻り値型
	using args_type = std::tuple<FuncArgs...>;												// 自身の保有するスレッドセーフ関数クラスの関数オブジェクトの引数型をまとめたtuple型
	using future_cash_type = future_cash<result_type>;										// 結果データをキャッシュ（保持）しておくためのデータのまとまり

private:


//===================================
// メンバ変数
//===================================


	// スレッドセーフな関数オブジェクトを保有するメンバ
	threadsafe_func_type m_threadsafe_func;

	future_cash_type m_futureCash;

public:


//===================================
// コンストラクタ
//===================================


	async_function() noexcept {}


	async_function( func_type func ) {

		m_threadsafe_func = func;

	}


	this_type& operator=( const this_type& async_function ) noexcept {

		m_threadsafe_func = async_function.m_threadsafe_func;

		return *this;

	}


	this_type& operator=( this_type&& async_function ) noexcept {

		m_threadsafe_func = async_function.m_threadsafe_func;

		return *this;

	}


public:


//======================================
// operator
//======================================


	// 関数を実行する　※このメンバ関数を通して実行する関数はスレッドセーフではない
	result_type operator()( FuncArgs... args ) {

		return m_threadsafe_func.func_exe( args... );

	}


	// 関数を指定したローンチで開始する
	void operator()( std::launch launch, FuncArgs... args ) {

		this->operator()( threadsafe_flag::unsafe, launch, args... );

	}



	void operator()( threadsafe_flag isThreadsafe, std::launch launch, FuncArgs... args ) {

		if( isThreadsafe == threadsafe_flag::unsafe ) {

			// ローンチがasyncなら保持する関数の非同期実行を行い、deferredなら保持する関数の遅延実行を行う
			if( launch == std::launch::async ) {

				async( args... );

			}
			else if( launch == std::launch::deferred ) {

				deferred( args... );

			}
			else if( launch == ( std::launch::async | std::launch::deferred ) ) {

				deferred_async( args... );

			}

		}
		else {

			// ローンチがasyncなら保持する関数の非同期実行を行い、deferredなら保持する関数の遅延実行を行う
			if( launch == std::launch::async ) {

				threadsafe_async( args... );

			}
			else if( launch == std::launch::deferred ) {

				threadsafe_deferred( args... );

			}
			else if( launch == ( std::launch::async | std::launch::deferred ) ) {

				deferred_threadsafe_async( args... );

			}

		}

	}

public:


//===================================
// メンバ関数
//===================================


	// 使用する関数オブジェクトを設定する
	void set_func( func_type func ) {

		m_threadsafe_func = func;

	}


	// 自身の保有する関数オブジェクトを渡す　※所持していないならnullptrを返す
	func_type own_func() {

		return m_threadsafe_func.own_func();

	}


	// 関数を実行する　※このメンバ関数を通して実行する関数はスレッドセーフではない
	result_type func_exe( FuncArgs... args ) {

		return m_threadsafe_func.func_exe( args... );

	}


	// スレッドセーフに関数を実行する
	result_type threadsafe_func_exe( FuncArgs... args ) {

		return m_threadsafe_func.threadsafe_func_exe( args... );

	}

//======================================
// async系
//======================================
#pragma region async



	// 関数を非同期実行する
	// 複数回実行する際には前回のasync, deferredの際の結果をget_resultメソッド, get_futureメソッド, get_shared_futureメソッドを使用して取得しておく必要がある、取得していない場合前回の結果は破棄される
	void async( FuncArgs... args ) {


		m_futureCash = std::async(
			std::launch::async,
			&threadsafe_func_type::func_exe,
			&m_threadsafe_func,
			args...
		);

	}



	// 関数をスレッドセーフに非同期実行する
	// ※このメソッドを介して全てを行う限りはスレッドセーフに動作するが、このクラスに渡した関数オブジェクトと同じ関数を外部で実行した場合は未定義の動作となる
	void threadsafe_async( FuncArgs... args ) {


		m_futureCash = std::async(
			std::launch::async,
			&threadsafe_func_type::threadsafe_func_exe,
			&m_threadsafe_func,
			args...
		);

	}

private:


	// deferred_asyncメンバ関数でのみ呼ばれる関数
	std::future<result_type> internal_async( FuncArgs... args ) {

		return std::async(
			std::launch::async,
			&threadsafe_func_type::func_exe,
			&m_threadsafe_func,
			args...
		);

	}


	// deferred_threadsafe_asyncメンバ関数内でのみ呼ばれる関数
	std::future<result_type> internal_threadsafe_async( FuncArgs... args ) {

		return std::async(
			std::launch::async,
			&threadsafe_func_type::threadsafe_func_exe,
			&m_threadsafe_func,
			args...
		);

	}

public:


//===================================================
// defferd_async系関数
//===================================================

//===================================================
// deferred_asyncメンバ関数, deferred_threadsafe_asyncメンバ関数の特徴と使い方
//===================================================
// 特徴
//---------------------------------------------------
// 1.この二つのメンバ関数の実行結果はasyncメンバ関数, threadsafe_asyncメンバ関数と同等の実行内容となる
// 2.この二つのメンバ関数の遅延実行はdeferred_async_exe関数を使用して行われる
// 3.この二つのメンバ関数でdeferred_async_exe関数を実行する前に以下のメンバ関数を実行した場合は自動的にdeferred_async_exeを実行した後に以下のメンバ関数の処理に移る
//   ※deferred_async系メンバ関数を使った意味がなくなる(これならdeferred系メンバ関数を実行しているのと内容が同じとなるかそれ以下（スレッド作成のオーバーヘッドが生じるため)）
// --------------------------------------------------
// get_result, get_future, get_shared_future, wait, wait_for, wait_until
// 以上のメンバ関数をdeferred_async_exeメンバ関数の実行前に呼ぶとdeferred_async_exeメンバ関数が自動的に呼ばれる
//===================================================

	// asyncメンバ関数を遅延実行させる
	void deferred_async( FuncArgs... args ) {

		m_futureCash = std::async(
			std::launch::deferred,
			&this_type::internal_async,
			this,
			args...
		);

	}


	// threadsafe_asyncメンバ関数を遅延実行させる
	void deferred_threadsafe_async( FuncArgs... args ) {

		m_futureCash = std::async(
			std::launch::deferred,
			&this_type::internal_threadsafe_async,
			this,
			args...
		);

	}


	// deferred_asyncメンバ関数, deferred_threadsafe_asyncメンバ関数を使用した際に
	// 遅延させている非同期処理の実行を開始する関数
	void deferred_async_exe() {

		m_futureCash.deferred_async_exe();

	}

#pragma endregion async


//======================================
// deferred系
//======================================
#pragma region deferred



	// 関数を遅延実行する　※std::future<T>::waitメンバ関数・getメンバ関数が呼ばれるまで関数は実行されない
	// 複数回実行する際には前回のasync, deferredの際の結果をget_resultメソッド, share_resultメソッドを使用して取得しておく必要がある、取得していない場合前回の結果は破棄される
	void deferred( FuncArgs... args ) {

		m_futureCash = std::async(
			std::launch::deferred,
			&threadsafe_func_type::func_exe,
			&m_threadsafe_func,
			args...
		);

	}


	void threadsafe_deferred( FuncArgs... args ) {

		m_futureCash = std::async(
			std::launch::deferred,
			&threadsafe_func_type::threadsafe_func_exe,
			&m_threadsafe_func,
			args...
		);

	}

#pragma endregion deferred


//======================================
// async, deferred共通で使用するメンバ関数
//======================================
#pragma region async, deferred common method


	// 実行結果をdiscard（破棄）する
	[[noreturn]] void discard() {

		m_futureCash.discard();

	}


	// 実行した関数の戻り値を受け取る　※この関数はwaitと同等の内容を実行した後に関数の実行結果を取得します
	[[nodiscard]] result_type get_result() {

		return m_futureCash.get_result();

	}


	// future_cash型を渡す
	// ムーブなのでこれ以降の管理を明け渡すことになる
	[[nodiscard]] future_cash_type&& get_future_cash() {

		return std::move(m_futureCash);

	}


	// 結果を保持するstd::futureを返す
	// この関数を呼び出した後に再度async, deferredを実行せずに
	[[nodiscard]] std::future<result_type>&& get_future() {

		return std::move(m_futureCash.get_future());

	}


	// get_resultの結果を共有する為のstd::shared_future<result_type>オブジェクトとを取得する
	[[nodiscard]] std::shared_future<result_type> get_shared_future() {

		return m_futureCash.get_shared_future();

	}



	// 実行した関数の処理か終了し、関数の結果が取得できるようになるまで待ちます
	void wait() {

		m_futureCash.wait();

	}


	// 指定した時間が経過したら現在の処理状況を返す
	template< class Rep, class Period >
	[[nodiscard]] std::future_status wait_for( const std::chrono::duration<Rep, Period>& timeout_duration ) {

		return m_futureCash.wait_for( timeout_duration );

	}


	// 指定した時間になったら現在の処理状況を返す
	template< class Clock, class Duration >
	[[nodiscard]] std::future_status wait_until( const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		return m_futureCash.wait_until( timeout_time );

	}



#pragma endregion async, deferred common method

};





// ヘルパー関数
template<class Result, class ...FuncArgs>
async_function<Result( FuncArgs... )>&& make_async_function( std::function<Result( FuncArgs... )>&& function ) {

	return async_function<Result( FuncArgs... )>( function );

}



#pragma endregion async_function class









