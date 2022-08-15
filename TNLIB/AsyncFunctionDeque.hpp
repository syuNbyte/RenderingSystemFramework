#pragma once





#include<deque>
#include<optional>
#include<cinttypes>

#include"AsyncFunctionInterface.h"
#include"FutureCash.hpp"





// async_functionクラスの拡張クラス
// async_functionではできなかった同じ関数を複数回実行した際に複数個の結果を保持できるクラスでdequeの方式を採用している

// C++20で実装されているSTL std::bind_frontの機能を使用すればasync_functionクラスを継承することで実装することができたが
// それが使えないので別クラスとして定義している
#pragma region async_function_deque



template<class Func>
class async_function_deque;



template<class Result, class ...FuncArgs>
class async_function_deque<Result( FuncArgs... )> : public async_function_interface<Result( FuncArgs... )> {

public:


	using this_type = async_function_deque<Result( FuncArgs... )>;										// 自身のデータ型
	using threadsafe_func_type = threadsafe_function<Result( FuncArgs... )>;						// 自身の保有するスレッドセーフ関数クラスのデータ型
	using func_type = std::function<Result( FuncArgs... )>;												// 自身の保有するスレッドセーフ関数クラスの関数オブジェクト型
	using result_type = Result;																					// 自身の保有するスレッドセーフ関数クラスの関数オブジェクトの戻り値型
	using args_type = std::tuple<FuncArgs...>;																// 自身の保有するスレッドセーフ関数クラスの関数オブジェクトの引数型をまとめたtuple型
	using future_cash_type = future_cash<result_type>;														// 結果データをキャッシュ（保持）しておくためのデータのまとまり
	using deque_type = std::deque<future_cash_type>;														// 自身の関数の実行結果を保持するdeque型
	using iterator = typename deque_type::iterator;															// 自身が保持するdeque型のイテレータ
	using const_iterator = typename deque_type::const_iterator;											// 自身が保持するdeque型の読み取り専用イテレータ
	using reverse_iterator = typename deque_type::reverse_iterator;									// 自身が保持するdeque型の逆イテレータ
	using const_reverse_iterator = typename deque_type::const_reverse_iterator;					// 自身が保持するdeque型の読み取り専用逆イテレータ

private:

	// スレッドセーフな関数オブジェクトを保有するメンバ
	threadsafe_func_type m_threadsafe_func;

	// async, deferredを実行した際の戻り値のfutureを保持するメンバ
	deque_type m_futureDeque;

public:


	async_function_deque() noexcept {}


	async_function_deque( func_type func ) {

		m_threadsafe_func = func;

	}


	this_type& operator=( const this_type& async_function_deque ) noexcept {

		m_threadsafe_func = async_function_deque.m_threadsafe_func;

		return *this;

	}


	this_type& operator=( this_type&& async_function_deque ) noexcept {

		m_threadsafe_func = async_function_deque.m_threadsafe_func;

		return *this;

	}

public:


//======================================
// operator
//======================================


	// 関数を実行する　※このメンバ関数を通して実行する関数はスレッドセーフではない
	result_type operator()( FuncArgs... args ) override {

		return m_threadsafe_func.func_exe( args... );

	}


	// 関数を指定したローンチで開始する
	void operator()( std::launch launch, FuncArgs... args ) override {

		this->operator()( threadsafe_flag::unsafe, launch, args... );

	}



	void operator()( threadsafe_flag isThreadsafe, std::launch launch, FuncArgs... args ) override {

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
	void set_func( func_type func ) override {

		m_threadsafe_func = func;

	}


	// 自身の保有する関数オブジェクトを渡す　※所持していないならnullptrを返す
	func_type own_func() override {

		return m_threadsafe_func.own_func();

	}


	// 関数を実行する　※このメンバ関数を通して実行する関数はスレッドセーフではない
	result_type func_exe( FuncArgs... args ) override {

		return m_threadsafe_func.func_exe( args... );

	}


	// スレッドセーフに関数を実行する
	result_type threadsafe_func_exe( FuncArgs... args ) override {

		return m_threadsafe_func.threadsafe_func_exe( args... );

	}

//======================================
// 要素アクセス・要素の操作
//======================================
#pragma region access


	// 要素に参照アクセスを行う　※こちらの方が高速
	future_cash_type& operator[]( uint32_t index ) {

		return m_futureDeque[index];

	}


	// 要素に安全に参照アクセスを行う　※こちらの方が低速で範囲外アクセスを行った場合out_of_rangeエラーを送出する
	future_cash_type& at( uint32_t index ) {

		return m_futureDeque.at( index );

	}


	// 先頭要素へ参照アクセスを行う
	future_cash_type& front() {

		return m_futureDeque.front();

	}


	// 末尾要素へ参照アクセスを行う
	future_cash_type& back() {

		return m_futureDeque.back();

	}


	// 先頭要素を削除
	void pop_front() {

		m_futureDeque.pop_front();
		
	}


	// 末尾要素を削除
	void pop_back() {

		m_futureDeque.pop_back();

	}


	// 先頭要素を取得し削除
	future_cash_type& get_and_pop_front() {

		auto& front_data = m_futureDeque.front();

		m_futureDeque.pop_front();

		return front_data;

	}


	// 末尾要素を取得し削除
	future_cash_type& get_and_pop_back() {

		auto& back_data = m_futureDeque.back();

		m_futureDeque.pop_back();

		return back_data;

	}


	// 現在の要素数を取得
	size_t size() const noexcept {

		return m_futureDeque.size();

	}


	// 要素の有無を確認する
	bool empty() const noexcept {

		return m_futureDeque.empty();

	}



	// 指定した結果が格納されているかどうかを判定し結果が格納されているなら一番最初に見つかったデータを格納するstd::optional型を返す
	// もし結果が存在しない場合は無効値の格納されたstd::optional型（std::nullopt）を返す
	[[nodiscard]] std::optional<result_type> find( result_type find_value ) {

		// 一ナノ秒（std::chronoが使用できる最小の時間）を使用する
		return find_wait_for( find_value, std::chrono::nanoseconds( 1 ) );

	}


	// 指定した結果が格納されているかどうかを判定し結果が格納されているなら一番最初に見つかったそのデータを格納するstd::optional型返す
	// もし結果が存在しない場合は無効値の格納されたstd::optional型（std::nullopt）を返す
	// このメンバ関数は一度結果を取得するために以下の時間を要する
	// 計算時間：指定した時間 * 要素数分の処理　※その要素が処理を終了している場合には即時結果が取得されるため実行する関数オブジェクトの処理速度に依存する
	template< class Rep, class Period >
	[[nodiscard]] std::optional<result_type> find_wait_for( result_type find_value, const std::chrono::duration<Rep, Period>& timeout_duration ) {


		// 一番最初に見つかった検索値と同値の物のイテレータを返す
		auto iter = std::find_if(
			m_futureDeque.begin(),
			m_futureDeque.end(),
			[ & ]( future_cash_type& value ) {

				auto state = value.get_state();


				if( state == future_cash_status::has_result_state::has_future && value.get_detail_state_wait_for( timeout_duration ) == future_cash_status::has_result_detail_state::has_future_ready ) {

					return find_value == value.get_result();

				}
				else if( state == future_cash_status::has_result_state::has_shared_future && value.get_detail_state_wait_for( timeout_duration ) == future_cash_status::has_result_detail_state::has_shared_future_ready ) {

					return find_value == value.get_result();

				}
				else if( state == future_cash_status::has_result_state::has_result ) {

					return find_value == value.get_result();

				}
				else {

					return false;

				}

			}
		);


		// 処理結果が受け取れる状態の場合は
		// 結果を受け取り、その結果を保持していたfuture_cash型を削除する
		if( iter != m_futureDeque.end() ) {

			auto result = iter->get_result();

			m_futureDeque.erase( iter );

			return result;

		}
		else {

			return std::nullopt;

		}

	}



	template<class Clock, class Duration>
	[[nodiscard]] std::optional<result_type> find_wait_until( result_type find_value, const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		// 指定された時間になるまで現行のスレッドを停止して待つ
		std::this_thread::sleep_until( timeout_time );


		// 指定された時間を超えたら検索を開始する
		return find( find_value );

	}


#pragma region iterator


	// 自身の保持するdeque型の先頭要素を指すイテレータを返す
	iterator begin() {

		return m_futureDeque.begin();

	}


	// 自身の保持するdeque型の末尾要素を指すイテレータを返す
	iterator end() {

		return m_futureDeque.end();

	}


	// 自身の保持するdeque型の先頭要素を指す読み取りイテレータを返す
	const_iterator cbegin() const {

		return m_futureDeque.cbegin();

	}


	// 自身の保持するdeque型の末尾要素を指す読み取りイテレータを返す
	const_iterator cend() const {

		return m_futureDeque.cend();

	}


	// 自身の保持するdeque型の先頭要素を指すイテレータを返す
	reverse_iterator rbegin() {

		return m_futureDeque.rbegin();

	}


	// 自身の保持するdeque型の末尾要素を指すイテレータを返す
	reverse_iterator rend() {

		return m_futureDeque.rend();

	}


	// 自身の保持するdeque型の末尾要素を指す読み取りイテレータを返す
	const_reverse_iterator crbegin() const {

		return m_futureDeque.crbegin();

	}

	// 自身の保持するdeque型の先頭要素を指す読み取りイテレータを返す

	const_reverse_iterator crend() const {

		return m_futureDeque.crend();

	}



#pragma endregion iterator



#pragma endregion access


//======================================
// async系
//======================================
#pragma region async



	// 関数を非同期実行する
	void async( FuncArgs... args ) override {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::async,
				&threadsafe_func_type::func_exe,
				&m_threadsafe_func,
				args...
			)

		);

	}



	void threadsafe_async( FuncArgs... args ) {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::async,
				&threadsafe_func_type::threadsafe_func_exe,
				&m_threadsafe_func,
				args...
			)

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
// deferred_async系関数
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

		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&this_type::internal_async,
				this,
				args...
			)

		);

	}


	// threadsafe_asyncメンバ関数を遅延実行させる
	void deferred_threadsafe_async( FuncArgs... args ) {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&this_type::internal_threadsafe_async,
				this,
				args...
			)

		);

	}


	// deferred_asyncメンバ関数, deferred_threadsafe_asyncメンバ関数を使用した際に
	// 遅延させている非同期処理の実行を開始する関数
	void deferred_async_exe( uint32_t index ) {

		m_futureDeque[index].deferred_async_exe();

	}


	// deferred_asyncメンバ関数, deferred_threadsafe_asyncメンバ関数によって遅延実行され、まだ実行されていないものを全て実行状態に移行させる
	void deferred_async_exe_all() {


		for( auto&& _future_cash : m_futureDeque ) {

			// 実行待ちのfuture_cashを全て実行状態に移行する
			if( _future_cash.get_state() == future_cash_status::has_result_state::deferred_async ) {

				_future_cash.deferred_async_exe();

			}

		}


	}


#pragma endregion async


//======================================
// deferred系
//======================================
#pragma region deferred



	// 関数を遅延実行する　※std::future<T>::waitメンバ関数・getメンバ関数が呼ばれるまで関数は実行されない
	// 複数回実行する際には前回のasync, deferredの際の結果をget_resultメソッド, share_resultメソッドを使用して取得しておく必要がある、取得していない場合前回の結果は破棄される
	void deferred( FuncArgs... args ) override {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&threadsafe_func_type::func_exe,
				&m_threadsafe_func,
				args...
			)

		);

	}


	void threadsafe_deferred( FuncArgs... args ) {


		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&threadsafe_func_type::threadsafe_func_exe,
				&m_threadsafe_func,
				args...
			)

		);

	}

#pragma endregion deferred


//======================================
// async, deferred共通で使用するメンバ関数
//======================================
#pragma region async, deferred common method



#pragma region discard method



	// 実行結果をdiscard（破棄）する
	void discard( uint32_t index ) {

		m_futureDeque.erase( std::next(m_futureDeque.begin(), index));

	}


	// 指定した範囲の実行結果をdiscard（破棄）する
	void discard_range( uint32_t beginIndex, uint32_t endIndex ) {

		m_futureDeque.erase(
			std::next( m_futureDeque.begin(), beginIndex ),
			std::next( m_futureDeque.begin(), endIndex )
		);

	}


	// 全ての実行結果をdiscard（破棄）する
	void discard_all() {

		m_futureDeque.clear();

	}


	// 指定した条件に合致する要素をdiscard（破棄）する
	// ※predicateとは判断する関数という意味
	void discard_if( std::function<bool(future_cash_type&)> predicate ) {

		// 削除範囲の先頭イテレータ
		// ※削除範囲が末尾に集約されるので、この先頭イテレータから末尾イテレータまでの範囲を削除するとよい
		auto discard_begin_iter = std::remove_if( 
			m_futureDeque.begin(),
			m_futureDeque.end(),
			predicate
		);

		// 削除した個数
		auto discard_num = std::distance( 
			discard_begin_iter,
			m_futureDeque.end()
		);

		//範囲を削除
		m_futureDeque.erase(
			discard_begin_iter,
			m_futureDeque.end()
		);


	}



#pragma endregion discard method



#pragma region get member data method



	// 実行した関数の戻り値を受け取る　※この関数はwaitと同等の内容を実行した後に関数の実行結果を取得します
	// この関数は取得したい関数の実行が終了していない場合
	[[nodiscard]] result_type get_result( uint32_t index ) {

		auto future_result = m_futureDeque[index].get_result();

		return future_result;

	}


	// 実行した関数の戻り値を受け取り、この関数の実行結果のデータを削除する
	[[nodiscard]] result_type get_result_and_discard( uint32_t index ) {

		auto future_result = m_futureDeque[index].get_result();

		discard( index );

		return future_result;

	}


	// 結果を保持するfuutre_cash型を返す
	[[nodiscard]] future_cash_type&& get_future_cash( uint32_t index ) {

		return std::move(m_futureDeque[index]);

	}


	// 結果を保持するstd::futureを返す
	[[nodiscard]] std::future<result_type> get_future( uint32_t index ) {

		auto future_result = std::move(m_futureDeque[index].get_future());

		discard( index );

		return future_result;

	}


	// get_resultの結果を共有する為のstd::shared_future<result_type>オブジェクトとを取得する
	[[nodiscard]] std::shared_future<result_type> get_shared_future( uint32_t index ) {

		return m_futureDeque[index].get_shared_future();

	}



#pragma endregion get member data method



#pragma region wait method



	// 実行した関数の処理か終了し、関数の結果が取得できるようになるまで処理を待ちます
	void wait( uint32_t index ) {

		m_futureDeque[index].wait();

	}


	// 現在実行されているすべての関数の処理が終了するまで処理を待ちます
	void all_wait() {

		for( auto&& _future_cash : m_futureDeque ) {

			_future_cash.wait();

		}

	}



	// 指定した時間が経過したら現在の処理状況を返す
	template< class Rep, class Period >
	[[nodiscard]] std::future_status wait_for( uint32_t index, const std::chrono::duration<Rep, Period>& timeout_duration ) {

		/*============================================================================
		// 参考サイト：https://ja.cppreference.com/w/cpp/thread/future/wait_for
		//============================================================================
		// 説明
		//============================================================================
		// future_status::deferred	: 結果を計算するための関数はまだ開始されていません
		// future_status::ready		: 結果は準備完了しています
		// future_status::timeout	: タイムアウトが経過しました
		============================================================================*/

		return m_futureDeque[index].wait_for( timeout_duration );

	}


	// 指定した時間になったら現在の処理状況を返す
	template< class Clock, class Duration >
	[[nodiscard]] std::future_status wait_until( uint32_t index, const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		/*============================================================================
		// 参考サイト：https://ja.cppreference.com/w/cpp/thread/future/wait_until
		//============================================================================
		// 説明
		//============================================================================
		// future_status::deferred	: 結果を計算するための関数はまだ開始されていません
		// future_status::ready		: 結果は準備完了しています
		// future_status::timeout	: タイムアウトが経過しました
		============================================================================*/

		return m_futureDeque[index].wait_until( timeout_time );

	}



#pragma endregion wait method



#pragma endregion async, deferred common method

};



#pragma endregion async_function_deque





#pragma region async_function_deque void specialization



template<class ...FuncArgs>
class async_function_deque<void( FuncArgs... )> : public async_function_interface<void( FuncArgs... )> {

public:


	using this_type = async_function_deque<void( FuncArgs... )>;										// 自身のデータ型
	using threadsafe_func_type = threadsafe_function<void( FuncArgs... )>;							// 自身の保有するスレッドセーフ関数クラスのデータ型
	using func_type = std::function<void( FuncArgs... )>;													// 自身の保有するスレッドセーフ関数クラスの関数オブジェクト型
	using result_type = void;																						// 自身の保有するスレッドセーフ関数クラスの関数オブジェクトの戻り値型
	using args_type = std::tuple<FuncArgs...>;																// 自身の保有するスレッドセーフ関数クラスの関数オブジェクトの引数型をまとめたtuple型
	using future_cash_type = future_cash<result_type>;														// 結果データをキャッシュ（保持）しておくためのデータのまとまり
	using deque_type = std::deque<future_cash_type>;														// 自身の関数の実行結果を保持するdeque型
	using iterator = typename deque_type::iterator;															// 自身が保持するdeque型のイテレータ
	using const_iterator = typename deque_type::const_iterator;											// 自身が保持するdeque型の読み取り専用イテレータ
	using reverse_iterator = typename deque_type::reverse_iterator;									// 自身が保持するdeque型の逆イテレータ
	using const_reverse_iterator = typename deque_type::const_reverse_iterator;					// 自身が保持するdeque型の読み取り専用逆イテレータ

private:

	// スレッドセーフな関数オブジェクトを保有するメンバ
	threadsafe_func_type m_threadsafe_func;

	// async, deferredを実行した際の戻り値のfutureを保持するメンバ
	deque_type m_futureDeque;

public:


	async_function_deque() noexcept {}


	async_function_deque( func_type func ) {

		m_threadsafe_func = func;

	}


	this_type& operator=( const this_type& async_function_deque ) noexcept {

		m_threadsafe_func = async_function_deque.m_threadsafe_func;

		return *this;

	}


	this_type& operator=( this_type&& async_function_deque ) noexcept {

		m_threadsafe_func = async_function_deque.m_threadsafe_func;

		return *this;

	}

public:


//======================================
// operator
//======================================


	// 関数を実行する　※このメンバ関数を通して実行する関数はスレッドセーフではない
	result_type operator()( FuncArgs... args ) override {

		return m_threadsafe_func.func_exe( args... );

	}


	// 関数を指定したローンチで開始する
	void operator()( std::launch launch, FuncArgs... args ) override {

		this->operator()( threadsafe_flag::unsafe, launch, args... );

	}



	void operator()( threadsafe_flag isThreadsafe, std::launch launch, FuncArgs... args ) override {

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
	void set_func( func_type func ) override {

		m_threadsafe_func = func;

	}


	// 自身の保有する関数オブジェクトを渡す　※所持していないならnullptrを返す
	func_type own_func() override {

		return m_threadsafe_func.own_func();

	}


	// 関数を実行する　※このメンバ関数を通して実行する関数はスレッドセーフではない
	result_type func_exe( FuncArgs... args ) override {

		return m_threadsafe_func.func_exe( args... );

	}


	// スレッドセーフに関数を実行する
	result_type threadsafe_func_exe( FuncArgs... args ) override {

		return m_threadsafe_func.threadsafe_func_exe( args... );

	}

//======================================
// 要素アクセス・要素の操作
//======================================
#pragma region access


	// 要素に参照アクセスを行う　※こちらの方が高速
	future_cash_type& operator[]( uint32_t index ) {

		return m_futureDeque[index];

	}


	// 要素に安全に参照アクセスを行う　※こちらの方が低速で範囲外アクセスを行った場合out_of_rangeエラーを送出する
	future_cash_type& at( uint32_t index ) {

		return m_futureDeque.at( index );

	}


	// 先頭要素へ参照アクセスを行う
	future_cash_type& front() {

		return m_futureDeque.front();

	}


	// 末尾要素へ参照アクセスを行う
	future_cash_type& back() {

		return m_futureDeque.back();

	}


	// 先頭要素を削除
	void pop_front() {

		m_futureDeque.pop_front();

	}


	// 末尾要素を削除
	void pop_back() {

		m_futureDeque.pop_back();

	}


	// 先頭要素を取得し削除
	future_cash_type& get_and_pop_front() {

		auto& front_data = m_futureDeque.front();

		m_futureDeque.pop_front();

		return front_data;

	}


	// 末尾要素を取得し削除
	future_cash_type& get_and_pop_back() {

		auto& back_data = m_futureDeque.back();

		m_futureDeque.pop_back();

		return back_data;

	}


	// 現在の要素数を取得
	size_t size() const noexcept {

		return m_futureDeque.size();

	}


	// 要素の有無を確認する
	bool empty() const noexcept {

		return m_futureDeque.empty();

	}



#pragma region iterator


	// 自身の保持するdeque型の先頭要素を指すイテレータを返す
	iterator begin() {

		return m_futureDeque.begin();

	}


	// 自身の保持するdeque型の末尾要素を指すイテレータを返す
	iterator end() {

		return m_futureDeque.end();

	}


	// 自身の保持するdeque型の先頭要素を指す読み取りイテレータを返す
	const_iterator cbegin() const {

		return m_futureDeque.cbegin();

	}


	// 自身の保持するdeque型の末尾要素を指す読み取りイテレータを返す
	const_iterator cend() const {

		return m_futureDeque.cend();

	}


	// 自身の保持するdeque型の先頭要素を指すイテレータを返す
	reverse_iterator rbegin() {

		return m_futureDeque.rbegin();

	}


	// 自身の保持するdeque型の末尾要素を指すイテレータを返す
	reverse_iterator rend() {

		return m_futureDeque.rend();

	}


	// 自身の保持するdeque型の末尾要素を指す読み取りイテレータを返す
	const_reverse_iterator crbegin() const {

		return m_futureDeque.crbegin();

	}

	// 自身の保持するdeque型の先頭要素を指す読み取りイテレータを返す

	const_reverse_iterator crend() const {

		return m_futureDeque.crend();

	}



#pragma endregion iterator



#pragma endregion access


//======================================
// async系
//======================================
#pragma region async



	// 関数を非同期実行する
	void async( FuncArgs... args ) override {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::async,
				&threadsafe_func_type::func_exe,
				&m_threadsafe_func,
				args...
			)

		);

	}



	void threadsafe_async( FuncArgs... args ) {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::async,
				&threadsafe_func_type::threadsafe_func_exe,
				&m_threadsafe_func,
				args...
			)

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
// deferred_async系関数
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

		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&this_type::internal_async,
				this,
				args...
			)

		);

	}


	// threadsafe_asyncメンバ関数を遅延実行させる
	void deferred_threadsafe_async( FuncArgs... args ) {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&this_type::internal_threadsafe_async,
				this,
				args...
			)

		);

	}


	// deferred_asyncメンバ関数, deferred_threadsafe_asyncメンバ関数を使用した際に
	// 遅延させている非同期処理の実行を開始する関数
	void deferred_async_exe( uint32_t index ) {

		m_futureDeque[index].deferred_async_exe();

	}


	// deferred_asyncメンバ関数, deferred_threadsafe_asyncメンバ関数によって遅延実行され、まだ実行されていないものを全て実行状態に移行させる
	void deferred_async_exe_all() {


		for( auto&& _future_cash : m_futureDeque ) {

			// 実行待ちのfuture_cashを全て実行状態に移行する
			if( _future_cash.get_state() == future_cash_status::has_result_state::deferred_async ) {

				_future_cash.deferred_async_exe();

			}

		}


	}


#pragma endregion async


//======================================
// deferred系
//======================================
#pragma region deferred



	// 関数を遅延実行する　※std::future<T>::waitメンバ関数・getメンバ関数が呼ばれるまで関数は実行されない
	// 複数回実行する際には前回のasync, deferredの際の結果をget_resultメソッド, share_resultメソッドを使用して取得しておく必要がある、取得していない場合前回の結果は破棄される
	void deferred( FuncArgs... args ) {

		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&threadsafe_func_type::func_exe,
				&m_threadsafe_func,
				args...
			)

		);

	}


	void threadsafe_deferred( FuncArgs... args ) {


		m_futureDeque.emplace_back(

			std::async(
				std::launch::deferred,
				&threadsafe_func_type::threadsafe_func_exe,
				&m_threadsafe_func,
				args...
			)

		);

	}

#pragma endregion deferred


//======================================
// async, deferred共通で使用するメンバ関数
//======================================
#pragma region async, deferred common method



#pragma region discard method



	// 実行結果をdiscard（破棄）する
	void discard( uint32_t index ) {

		m_futureDeque.erase( std::next( m_futureDeque.begin(), index ) );

	}


	// 指定した範囲の実行結果をdiscard（破棄）する
	void discard_range( uint32_t beginIndex, uint32_t endIndex ) {

		m_futureDeque.erase(
			std::next( m_futureDeque.begin(), beginIndex ),
			std::next( m_futureDeque.begin(), endIndex )
		);

	}


	// 全ての実行結果をdiscard（破棄）する
	void discard_all() {

		m_futureDeque.clear();

	}


	// 指定した条件に合致する要素をdiscard（破棄）する
	// ※predicateとは判断する関数という意味
	void discard_if( std::function<bool( future_cash_type& )> predicate ) {

		// 削除範囲の先頭イテレータ
		// ※削除範囲が末尾に集約されるので、この先頭イテレータから末尾イテレータまでの範囲を削除するとよい
		auto discard_begin_iter = std::remove_if(
			m_futureDeque.begin(),
			m_futureDeque.end(),
			predicate
		);

		// 削除した個数
		auto discard_num = std::distance(
			discard_begin_iter,
			m_futureDeque.end()
		);

		//範囲を削除
		m_futureDeque.erase(
			discard_begin_iter,
			m_futureDeque.end()
		);


	}



#pragma endregion discard method



#pragma region get member data method



	// 実行した関数の戻り値を受け取る　※この関数はwaitと同等の内容を実行した後に関数の実行結果を取得します
	// void型なので戻り値はないが関数の結果を取得するために使用する
	[[noreturn]] result_type get_result( uint32_t index ) {

		auto future_result = m_futureDeque[index].get_result();

	}


	// 実行した関数の戻り値を受け取り、この関数の実行結果のデータを削除する
	// void型なので戻り値はないが関数の結果を取得するために使用する
	[[noreturn]] result_type get_result_and_discard( uint32_t index ) {

		auto future_result = m_futureDeque[index].get_result();

		discard( index );

	}


	// 結果を保持するfuutre_cash型を返す
	[[nodiscard]] future_cash_type&& get_future_cash( uint32_t index ) {

		return std::move( m_futureDeque[index] );

	}


	// 結果を保持するstd::futureを返す
	[[nodiscard]] std::future<result_type> get_future( uint32_t index ) {

		auto future_result = std::move( m_futureDeque[index].get_future() );

		discard( index );

		return future_result;

	}


	// get_resultの結果を共有する為のstd::shared_future<result_type>オブジェクトとを取得する
	[[nodiscard]] std::shared_future<result_type> get_shared_future( uint32_t index ) {

		return m_futureDeque[index].get_shared_future();

	}



#pragma endregion get member data method



#pragma region wait method



	// 実行した関数の処理か終了し、関数の結果が取得できるようになるまで処理を待ちます
	void wait( uint32_t index ) {

		m_futureDeque[index].wait();

	}


	// 現在実行されているすべての関数の処理が終了するまで処理を待ちます
	void all_wait() {

		for( auto&& _future_cash : m_futureDeque ) {

			_future_cash.wait();

		}

	}



	// 指定した時間が経過したら現在の処理状況を返す
	template< class Rep, class Period >
	[[nodiscard]] std::future_status wait_for( uint32_t index, const std::chrono::duration<Rep, Period>& timeout_duration ) {

		/*============================================================================
		// 参考サイト：https://ja.cppreference.com/w/cpp/thread/future/wait_for
		//============================================================================
		// 説明
		//============================================================================
		// future_status::deferred	: 結果を計算するための関数はまだ開始されていません
		// future_status::ready		: 結果は準備完了しています
		// future_status::timeout	: タイムアウトが経過しました
		============================================================================*/

		return m_futureDeque[index].wait_for( timeout_duration );

	}


	// 指定した時間になったら現在の処理状況を返す
	template< class Clock, class Duration >
	[[nodiscard]] std::future_status wait_until( uint32_t index, const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		/*============================================================================
		// 参考サイト：https://ja.cppreference.com/w/cpp/thread/future/wait_until
		//============================================================================
		// 説明
		//============================================================================
		// future_status::deferred	: 結果を計算するための関数はまだ開始されていません
		// future_status::ready		: 結果は準備完了しています
		// future_status::timeout	: タイムアウトが経過しました
		============================================================================*/

		return m_futureDeque[index].wait_until( timeout_time );

	}



#pragma endregion wait method



#pragma endregion async, deferred common method

};




#pragma endregion async_function_deque void specialization





// ヘルパー関数
template<class Result, class ...FuncArgs>
async_function_deque<Result( FuncArgs... )>&& make_async_function_deque( std::function<Result( FuncArgs... )>&& function ) {

	return async_function_deque<Result( FuncArgs... )>( function );

}