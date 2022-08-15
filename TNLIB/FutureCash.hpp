#pragma once





#include<future>
#include<optional>
#include<type_traits>
#include<iostream>





#pragma region future_cash



namespace future_cash_status {

	// どのメンバ変数が結果データを保持しているのかを知るためのクラス
	enum class has_result_state {

		has_future,				// future型メンバ変数がデータを保持している・保持予定
		has_shared_future,	// shared_future型メンバ変数がデータを保持している・保持予定
		has_result,				// result型メンバ変数がデータを保持している・保持予定
		no_result,				// どのメンバ変数も結果データを保持していない（void型の特殊化時に使用している）
		deferred_async			// 遅延実行async関数の実行待ち状態、実行状態に移行した場合にはhas_futureに変化する

	};

	// 詳細な保持データ　※詳細なデータはget_stateメンバ関数の使用時にのみ使用される
	enum class has_result_detail_state {

		has_future_deferred,			// future型メンバがデータを保持予定だが、deferred関数を実行していない
		has_future_ready,				// future型メンバが結果データを保持している
		has_future_timeout,			// future型メンバがけっかデータを保持予定だが処理が終了していない
		has_shared_future_deferred,// shared_future型メンバがデータを保持しているが、deferred関数を実行していない
		has_shared_future_ready,	// shared_future型メンバが結果データを保持している
		has_shared_future_timeout,	// shared_future型メンバがけっかデータを保持予定だが処理が終了していない
		has_result,						// result型メンバ変数がデータを保持している・保持予定
		no_result,						// どのメンバ変数も結果データを保持していない（void型の特殊化時に使用している）
		deferred_async					// 遅延実行async関数の実行待ち状態、実行状態に移行した場合にはhas_futureに変化する

	};


}





// 結果データをキャッシュ（保持）しておくためのデータのまとまり
// 
template<class ResultType>
class future_cash {

private:


	// static_assertは条件判定式がfalseの場合に動作するためメタ関数の結果をnot演算している
	// std::optional型を使用する際にobject型の要件を満たす必要があるため
	// object型の要件
	// 1.関数ではない
	// 2.voidではない
	// 3.参照型ではない
	static_assert(
		std::is_object_v<ResultType>,
		"result_type is not object type."
		);
	static_assert(
		std::is_object_v<ResultType>,
		"If you use future_cash class, the return type must be object type."
		);
	static_assert(
		!std::is_function_v<ResultType>,
		"The result_type was function. std::optional<T = function_type> cannot be used."
		);
	static_assert(
		!std::is_void_v<ResultType>,
		"The result_type was void. std::optional<T = void> cannot be used."
		);
	static_assert(
		!std::is_reference_v<ResultType>,
		"The result_type was reference type. std::optional<T = reference_type> cannot be used."
		);

public:


	using result_type = ResultType;
	using future_type = std::future<result_type>;
	using shared_future_type = std::shared_future<result_type>;
	using optional_type = std::optional<result_type>;
	using deferred_async_future_type = std::future<future_type>;

private:


	// 関数の実行結果を保持しておくためのクラス
	future_type m_future;

	// このクラスの保持するfutureのshareを行った際にshare状態を保持するためのメンバ変数
	shared_future_type m_sharedFuture;

	// findなどでデータを検索する際に一度外部に持ってくる必要があったためにデータを保持しておくためのメンバ変数
	// ※m_future, m_sharedFutureはgetメンバ関数を実行して結果を一度取得すると再度getメンバ関数を実行することは不可能なので
	optional_type m_result = std::nullopt;

	// どのメンバ変数が結果データを保持しているのかを返す
	future_cash_status::has_result_state m_has_result = future_cash_status::has_result_state::no_result;



	// async_functionクラス, async_function_dequeクラスで実行される
	// deferred_async, deferred_threadsafe_asyncメンバ関数（遅延実行されるasync関数）で使用されるメンバ変数
	deferred_async_future_type m_deferredAsyncFuture;

public:


	future_cash() noexcept {};

	future_cash( future_type&& future )
		: m_future( std::move( future ) ), m_has_result( future_cash_status::has_result_state::has_future ) {};

	future_cash( deferred_async_future_type&& deferredAsyncFuture )
		: m_deferredAsyncFuture( std::move( deferredAsyncFuture ) ), m_has_result(future_cash_status::has_result_state::deferred_async) {};

public:


#pragma region get member data method



	// 実行した関数の戻り値を受け取る　※この関数はwaitと同等の内容を実行した後に関数の実行結果を取得します
	// この関数は取得したい関数の実行が終了していない場合
	result_type get_result() {

#ifdef _DEBUG


		try {

			switch( m_has_result ) {

				case future_cash_status::has_result_state::has_future:
				{

					if( m_future.valid() == true ) {

						m_result = m_future.get();

						m_has_result = future_cash_status::has_result_state::has_result;

						return m_result.value();

					}
					else {

						// 設計上のミスによって適切なタグを設定できていない
						throw std::logic_error(
							"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
						);

					}

				}break;
				case future_cash_status::has_result_state::has_shared_future:
				{

					if( m_future.valid() == true ) {

						m_result = m_sharedFuture.get();

						m_has_result = future_cash_status::has_result_state::has_result;

						return m_result.value();

					}
					else {

						// 設計上のミスによって適切なタグを設定できていない
						throw std::logic_error(
							"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
						);

					}

				}break;
				case future_cash_status::has_result_state::has_result:
				{

					return m_result.value();

				}break;
				case future_cash_status::has_result_state::deferred_async:
				{

					if( m_deferredAsyncFuture.valid() == true ) {

						deferred_async_exe();

						// 再帰（もう一度この関数を呼ぶ、次の実行時にはhas_futureのタグが設定されているため処理内容が変更される）
						return get_result();

					}
					else {

						// 設計上のミスによって適切なタグを設定できていない
						throw std::logic_error(
							"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
						);

					}

				}break;
				case future_cash_status::has_result_state::no_result:
				{

					throw std::runtime_error(
						"not execute function"
					);

				}

			}

		}
		catch( const std::logic_error& logicError ) {

			std::cout << "error : " << logicError.what() << std::endl;

		}

#else


		switch( m_has_result ) {

			case future_cash_status::has_result_state::has_future:
			{

				m_result = m_future.get();

				m_has_result = future_cash_status::has_result_state::has_result;

				return m_result.value();

			}break;
			case future_cash_status::has_result_state::has_shared_future:
			{

				m_result = m_sharedFuture.get();

				m_has_result = future_cash_status::has_result_state::has_result;

				return m_result.value();

			}break;
			case future_cash_status::has_result_state::has_result:
			{

				return m_result.value();

			}break;
			case future_cash_status::has_result_state::deferred_async:
			{

				if( m_deferredAsyncFuture.valid() == true ) {

					deferred_async_exe();

					// 再帰（もう一度この関数を呼ぶ、次の実行時にはhas_futureのタグが設定されているため処理内容が変更される）
					return get_result();

				}

			}break;

		}

#endif

	}


	// 結果を保持するstd::futureを返す
	[[nodiscard]] future_type get_future() {

#ifdef _DEBUG


		try {

			// async, deferredの結果を持つm_futureメンバがまだなにからも情報を取得されていない状態の場合
			// ※std::future::getメソッド, std::future::shareメソッドが一度も呼ばれていない状態のときはtrue
			if( m_future.valid() == true ) {

				m_has_result = future_cash_status::has_result_state::no_result;

				return std::move( m_future );

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				m_has_result = future_cash_status::has_result_state::no_result;

				return std::move( m_deferredAsyncFuture.get() );

			}
			else {

				// 保持するfuture型がemptyだった
				throw std::runtime_error( "The future type was empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

			// emptyだが戻り値は返す
			return std::move( m_future );

		}

#else


		if( m_future.valid() == true ) {

			m_has_result = future_cash_status::has_result_state::no_result;

			return std::move( m_future );

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			m_has_result = future_cash_status::has_result_state::no_result;

			return std::move( m_deferredAsyncFuture.get() );

		}

#endif

	}


	// get_resultの結果を共有する為のstd::shared_future<result_type>オブジェクトとを取得する
	[[nodiscard]] shared_future_type get_shared_future() {


		if( m_has_result == future_cash_status::has_result_state::has_future ) {

			m_sharedFuture = m_future.share();

			m_has_result = future_cash_status::has_result_state::has_shared_future;

		}
		else if( m_has_result == future_cash_status::has_result_state::deferred_async ) {

			m_sharedFuture = m_deferredAsyncFuture.get().share();

			m_has_result = future_cash_status::has_result_state::has_shared_future;

		}


#ifdef _DEBUG


		try {

			// async, deferredの結果を持つm_futureメンバがまだなにからも情報を取得されていない状態の場合
			// ※std::future::getメソッド, std::future::shareメソッドが一度も呼ばれていない状態のときはtrue
			if( m_sharedFuture.valid() == true ) {

				return m_sharedFuture;

			}
			else {

				// 保持するfuture型・shared_future型がemptyだった
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

			// emptyだが戻り値は返す
			return m_sharedFuture;

		}

#else

		if( m_sharedFuture.valid() == true ) {

			return m_sharedFuture;

		}

#endif


	}


	// 遅延実行を行う
	void deferred_async_exe() {

		if( m_deferredAsyncFuture.valid() == true ) {

			m_future = std::move( m_deferredAsyncFuture.get() );

			m_has_result = future_cash_status::has_result_state::has_future;

		}


	}


	// 実行結果をdiscard（破棄）する
	[[noreturn]] void discard() const {

		m_future = std::future<result_type>();
		m_sharedFuture = std::shared_future<result_type>();
		m_result = std::nullopt;
		m_has_result = future_cash_status::has_result_state::no_result;
		m_deferredAsyncFuture = std::future<std::future<result_type>>();

	}


#pragma endregion get member data method



#pragma region future, shared_future valid method



// 保持するfuture型メンバ変数が有効かどうかを返す
	bool valid_future() {

		return m_future.valid();

	}


	// 保持するshared_future型メンバ変数が有効かどうかを返す
	bool valid_shared_future() {

		return m_sharedFuture.valid();

	}


	// 保持するdeferred_async用のfuture型メンバ変数が有効かどうかを返す
	bool valid_deferred_async() {

		return m_deferredAsyncFuture.valid();

	}


#pragma endregion future, shared_future valid method



#pragma region wait method



	// 実行した関数の処理か終了し、関数の結果が取得できるようになるまで待ちます
	void wait() {

#ifdef _DEBUG

		try {

			// async, deferredの結果を持つm_futureメンバがまだなにからも情報を取得されていない状態の場合
			// ※std::future::getメソッド, std::future::shareメソッドが一度も呼ばれていない状態のときはtrue
			if( m_future.valid() == true ) {

				m_future.wait();

			}
			else if( m_sharedFuture.valid() == true ) {

				m_sharedFuture.wait();

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				deferred_async_exe();

				// 再帰
				wait();

			}
			else {

				// 保持するfuture型・shared_future型がemptyだった
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else

		if( m_future.valid() == true ) {

			m_future.wait();

		}
		else if( m_sharedFuture.valid() == true ) {

			m_sharedFuture.wait();

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			deferred_async_exe();

			wait();

		}

#endif

	}


	// 指定した時間が経過したら現在の処理状況を返す
	template< class Rep, class Period >
	[[nodiscard]] std::future_status wait_for( const std::chrono::duration<Rep, Period>& timeout_duration ) {

		/*============================================================================
		// 参考サイト：https://ja.cppreference.com/w/cpp/thread/future/wait_for
		//============================================================================
		// 説明
		//============================================================================
		// future_status::deferred	: 結果を計算するための関数はまだ開始されていません
		// future_status::ready		: 結果は準備完了しています
		// future_status::timeout	: タイムアウトが経過しました
		============================================================================*/

#ifdef _DEBUG

		try {

			if( m_future.valid() == true ) {

				return m_future.wait_for( timeout_duration );

			}
			else if( m_sharedFuture.valid() == true ) {

				return m_sharedFuture.wait_for( timeout_duration );

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				deferred_async_exe();

				// 再帰
				return wait_for( timeout_duration );

			}
			else {

				// 保持するfuture型・shared_future型がemptyだった
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else

		if( m_future.valid() == true ) {

			return m_future.wait_for( timeout_duration );

		}
		else if( m_sharedFuture.valid() == true ) {

			return m_sharedFuture.wait_for( timeout_duration );

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			deferred_async_exe();

			// 再帰
			return wait_for( timeout_duration );

		}

#endif

	}


	// 指定した時間になったら現在の処理状況を返す
	template< class Clock, class Duration >
	[[nodiscard]] std::future_status wait_until( const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		/*============================================================================
		// 参考サイト：https://ja.cppreference.com/w/cpp/thread/future/wait_for
		//============================================================================
		// 説明
		//============================================================================
		// future_status::deferred	: 結果を計算するための関数はまだ開始されていません
		// future_status::ready		: 結果は準備完了しています
		// future_status::timeout	: タイムアウトが経過しました
		============================================================================*/

#ifdef _DEBUG

		try {

			if( m_future.valid() == true ) {

				return m_future.wait_until( timeout_time );

			}
			else if( m_sharedFuture.valid() == true ) {

				return m_sharedFuture.wait_until( timeout_time );

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				deferred_async_exe();

				// 再帰
				return wait_until( timeout_time );

			}
			else {

				// 保持するfuture型・shared_future型がemptyだった
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else

		if( m_future.valid() == true ) {

			return m_future.wait_until( timeout_time );

		}
		else if( m_sharedFuture.valid() == true ) {

			return m_sharedFuture.wait_until( timeout_time );

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			deferred_async_exe();

			// 再帰
			return wait_until( timeout_time );

		}

#endif

	}



#pragma endregion wait method



#pragma region get state method



	// 処理結果の保持状態を取得する
	future_cash_status::has_result_state get_state() {

		return m_has_result;

	}


	// 処理結果の詳細保持状態を取得する
	future_cash_status::has_result_detail_state get_detail_state() {

		return get_detail_state_wait_for( std::chrono::nanoseconds( 1 ) );

	}


	// 処理結果の詳細保持状態を一定時間経過後に取得する
	template< class Rep, class Period >
	future_cash_status::has_result_detail_state get_detail_state_wait_for( const std::chrono::duration<Rep, Period>& timeout_duration ) {


#ifdef _DEBUG

		// 一ナノ秒（std::chronoが使用できる最小の時間）を使用する
		if( m_has_result == future_cash_status::has_result_state::has_future ) {

			if( m_future.valid() == true ) {


				switch( m_future.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_future_timeout;
					}break;

				}


			}
			else {

				// 設計上のミスによって適切なタグを設定できていない
				throw std::logic_error(
					"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
				);

			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_shared_future ) {

			if( m_sharedFuture.valid() == true ) {


				switch( m_sharedFuture.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_timeout;
					}break;

				}

			}
			else {

				// 設計上のミスによって適切なタグを設定できていない
				throw std::logic_error(
					"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
				);

			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_result ) {

			return future_cash_status::has_result_detail_state::has_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::no_result ) {

			return future_cash_status::has_result_detail_state::no_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::deferred_async ) {

			return future_cash_status::has_result_detail_state::deferred_async;

		}

#else


				// 一ナノ秒（std::chronoが使用できる最小の時間）を使用する
		if( m_has_result == future_cash_status::has_result_state::has_future ) {

			if( m_future.valid() == true ) {


				switch( m_future.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_future_timeout;
					}break;

				}


			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_shared_future ) {

			if( m_sharedFuture.valid() == true ) {


				switch( m_sharedFuture.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_timeout;
					}break;

				}


			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_result ) {

			return future_cash_status::has_result_detail_state::has_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::no_result ) {

			return future_cash_status::has_result_detail_state::no_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::deferred_async ) {

			return future_cash_status::has_result_detail_state::deferred_async;

		}

#endif


	}


	// 処理結果の詳細保持状態を指定した時間になったら取得する
	template< class Clock, class Duration >
	future_cash_status::has_result_detail_state get_detail_state_wait_until( const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		std::this_thread::sleep_until( timeout_time );

		return get_detail_state();

	}



#pragma endregion get state method


};



#pragma endregion future_cash





#pragma region future_cash void specialization



template<>
class future_cash<void> {

public:


	using result_type = void;
	using future_type = std::future<result_type>;
	using shared_future_type = std::shared_future<result_type>;
	using deferred_async_future_type = std::future<future_type>;

private:


	// 関数の実行結果を保持しておくためのクラス
	future_type m_future;

	// このクラスの保持するfutureのshareを行った際にshare状態を保持するためのメンバ変数
	shared_future_type m_sharedFuture;

	// どのメンバ変数が結果データを保持しているのかを返す
	future_cash_status::has_result_state m_has_result = future_cash_status::has_result_state::has_future;



	// async_functionクラス, async_function_dequeクラスで実行される
	// deferred_async, deferred_threadsafe_asyncメンバ関数（遅延実行されるasync関数）で使用されるメンバ変数
	deferred_async_future_type m_deferredAsyncFuture;

public:



	future_cash() noexcept {};

	future_cash( future_type&& future )
		: m_future( std::move( future ) ), m_has_result( future_cash_status::has_result_state::has_future ) {};

	future_cash( deferred_async_future_type&& deferredAsyncFuture )
		: m_deferredAsyncFuture( std::move( deferredAsyncFuture ) ), m_has_result( future_cash_status::has_result_state::deferred_async ) {};



public:


#pragma region get member data method



	// 実行した関数の戻り値を受け取る　※この関数はwaitと同等の内容を実行した後に関数の実行結果を取得します
	// この関数は取得したい関数の実行が終了していない場合
	result_type get_result() {

#ifdef _DEBUG


		try {

			switch( m_has_result ) {

				case future_cash_status::has_result_state::has_future:
				{

					if( m_future.valid() == true ) {

						m_future.get();

						m_has_result = future_cash_status::has_result_state::no_result;

					}
					else {

						// 設計上のミスによって適切なタグを設定できていない
						throw std::logic_error(
							"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
						);

					}

				}break;
				case future_cash_status::has_result_state::has_shared_future:
				{

					if( m_sharedFuture.valid() == true ) {

						m_sharedFuture.get();

						m_has_result = future_cash_status::has_result_state::no_result;

					}
					else {

						// 設計上のミスによって適切なタグを設定できていない
						throw std::logic_error(
							"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
						);

					}

				}break;
				case future_cash_status::has_result_state::deferred_async:
				{

					if( m_deferredAsyncFuture.valid() == true ) {

						deferred_async_exe();

						// 再帰（もう一度この関数を呼ぶ、次の実行時にはhas_futureのタグが設定されているため処理内容が変更される）
						return get_result();

					}
					else {

						// 設計上のミスによって適切なタグを設定できていない
						throw std::logic_error(
							"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
						);

					}

				}break;
				case future_cash_status::has_result_state::no_result:
				{

					throw std::runtime_error(
						"not execute function"
					);

				}


			}

		}
		catch( const std::logic_error& logicError ) {

			std::cout << "error : " << logicError.what() << std::endl;

		}

#else


		switch( m_has_result ) {

			case future_cash_status::has_result_state::has_future:
			{

				if( m_future.valid() == true ) {

					m_future.get();

					m_has_result = future_cash_status::has_result_state::no_result;

				}

			}break;
			case future_cash_status::has_result_state::has_shared_future:
			{

				if( m_sharedFuture.valid() == true ) {

					m_sharedFuture.get();

					m_has_result = future_cash_status::has_result_state::no_result;

				}

			}break;
			case future_cash_status::has_result_state::deferred_async:
			{

				if( m_deferredAsyncFuture.valid() == true ) {

					deferred_async_exe();

					// 再帰（もう一度この関数を呼ぶ、次の実行時にはhas_futureのタグが設定されているため処理内容が変更される）
					return get_result();

				}

			}break;

		}

#endif

	}


	// 結果を保持するstd::futureを返す
	[[nodiscard]] future_type get_future() {

#ifdef _DEBUG


		try {

			// async, deferredの結果を持つm_futureメンバがまだなにからも情報を取得されていない状態の場合
			// ※std::future::getメソッド, std::future::shareメソッドが一度も呼ばれていない状態のときはtrue
			if( m_future.valid() == true ) {

				return std::move( m_future );

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				return std::move( m_deferredAsyncFuture.get() );

			}
			else {

				// 保持するfuture型がemptyだった
				throw std::runtime_error( "The future type was empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else


		if( m_future.valid() == true ) {

			return std::move( m_future );

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			return std::move( m_deferredAsyncFuture.get() );

		}

#endif

	}


	// get_resultの結果を共有する為のstd::shared_future<result_type>オブジェクトとを取得する
	[[nodiscard]] shared_future_type get_shared_future() {


		if( m_has_result == future_cash_status::has_result_state::has_future ) {

			m_sharedFuture = m_future.share();

			m_has_result = future_cash_status::has_result_state::has_shared_future;

		}
		else if( m_has_result == future_cash_status::has_result_state::deferred_async ) {

			m_sharedFuture = m_deferredAsyncFuture.get().share();

			m_has_result = future_cash_status::has_result_state::has_shared_future;

		}


#ifdef _DEBUG


		try {

			// async, deferredの結果を持つm_futureメンバがまだなにからも情報を取得されていない状態の場合
			// ※std::future::getメソッド, std::future::shareメソッドが一度も呼ばれていない状態のときはtrue
			if( m_sharedFuture.valid() == true ) {

				return m_sharedFuture;

			}
			else {

				// 保持するfuture型・shared_future型がemptyだった
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else

		if( m_sharedFuture.valid() == true ) {

			return m_sharedFuture;

		}

#endif


	}


	// 遅延実行を行う
	void deferred_async_exe() {

		m_future = std::move( m_deferredAsyncFuture.get() );

		m_has_result = future_cash_status::has_result_state::has_future;

	}


	// 実行結果をdiscard（破棄）する
	[[noreturn]] void discard() {

		m_future = std::future<result_type>();
		m_sharedFuture = std::shared_future<result_type>();
		m_has_result = future_cash_status::has_result_state::no_result;
		m_deferredAsyncFuture = std::future<std::future<result_type>>();

	}


#pragma endregion get member data method



#pragma region future, shared_future valid method



// 保持するfuture型メンバ変数が有効かどうかを返す
	bool valid_future() {

		return m_future.valid();

	}


	// 保持するshared_future型メンバ変数が有効かどうかを返す
	bool valid_shared_future() {

		return m_sharedFuture.valid();

	}


	// 保持するdeferred_async用のfuture型メンバ変数が有効かどうかを返す
	bool valid_deferred_async() {

		return m_deferredAsyncFuture.valid();

	}


#pragma endregion future, shared_future valid method



#pragma region wait method



	// 実行した関数の処理か終了し、関数の結果が取得できるようになるまで待ちます
	void wait() {

#ifdef _DEBUG

		try {

			// async, deferredの結果を持つm_futureメンバがまだなにからも情報を取得されていない状態の場合
			// ※std::future::getメソッド, std::future::shareメソッドが一度も呼ばれていない状態のときはtrue
			if( m_future.valid() == true ) {

				m_future.wait();

			}
			else if( m_sharedFuture.valid() == true ) {

				m_sharedFuture.wait();

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				deferred_async_exe();

				// 再帰
				wait();

			}
			else {

				// 保持するfuture型・shared_future型がemptyだった
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else

		if( m_future.valid() == true ) {

			m_future.wait();

		}
		else if( m_sharedFuture.valid() == true ) {

			m_sharedFuture.wait();

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			deferred_async_exe();

			// 再帰
			wait();

		}

#endif

	}


	// 指定した時間が経過したら現在の処理状況を返す
	template< class Rep, class Period >
	[[nodiscard]] std::future_status wait_for( const std::chrono::duration<Rep, Period>& timeout_duration ) {

		/*============================================================================
		// 参考サイト：https://ja.cppreference.com/w/cpp/thread/future/wait_for
		//============================================================================
		// 説明
		//============================================================================
		// future_status::deferred	: 結果を計算するための関数はまだ開始されていません
		// future_status::ready		: 結果は準備完了しています
		// future_status::timeout	: タイムアウトが経過しました
		============================================================================*/

#ifdef _DEBUG

		try {

			if( m_future.valid() == true ) {

				return m_future.wait_for( timeout_duration );

			}
			else if( m_sharedFuture.valid() == true ) {

				return m_sharedFuture.wait_for( timeout_duration );

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				deferred_async_exe();

				// 再帰
				return wait_for( timeout_duration );

			}
			else {

				// 保持するfuture型・shared_future型がemptyだった
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else

		if( m_future.valid() == true ) {

			return m_future.wait_for( timeout_duration );

		}
		else if( m_sharedFuture.valid() == true ) {

			return m_sharedFuture.wait_for( timeout_duration );

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			deferred_async_exe();

			// 再帰
			return wait_for( timeout_duration );

		}

#endif

	}


	// 指定した時間になったら現在の処理状況を返す
	template< class Clock, class Duration >
	[[nodiscard]] std::future_status wait_until( const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		/*============================================================================
		// 参考サイト：https://ja.cppreference.com/w/cpp/thread/future/wait_for
		//============================================================================
		// 説明
		//============================================================================
		// future_status::deferred	: 結果を計算するための関数はまだ開始されていません
		// future_status::ready		: 結果は準備完了しています
		// future_status::timeout	: タイムアウトが経過しました
		============================================================================*/

#ifdef _DEBUG

		try {

			if( m_future.valid() == true ) {

				return m_future.wait_until( timeout_time );

			}
			else if( m_sharedFuture.valid() == true ) {

				return m_sharedFuture.wait_until( timeout_time );

			}
			else if( m_deferredAsyncFuture.valid() == true ) {

				deferred_async_exe();

				// 再帰
				return wait_until( timeout_time );

			}
			else {

				// 保持するfuture型・shared_future型がemptyだった
				throw std::runtime_error( "Both future type and shared_future type were empty." );

			}

		}
		catch( const std::runtime_error& runtimeError ) {

			std::cout << "error : " << runtimeError.what() << std::endl;

		}

#else

		if( m_future.valid() == true ) {

			return m_future.wait_until( timeout_time );

		}
		else if( m_sharedFuture.valid() == true ) {

			return m_sharedFuture.wait_until( timeout_time );

		}
		else if( m_deferredAsyncFuture.valid() == true ) {

			deferred_async_exe();

			// 再帰
			return wait_until( timeout_time );

		}

#endif

	}



#pragma endregion wait method



#pragma region get state method



	// 処理結果の保持状態を取得する
	future_cash_status::has_result_state get_state() {

		return m_has_result;

	}


	// 処理結果の詳細保持状態を取得する
	future_cash_status::has_result_detail_state get_detail_state() {

		return get_detail_state_wait_for( std::chrono::nanoseconds( 1 ) );

	}


	// 処理結果の詳細保持状態を一定時間経過後に取得する
	template< class Rep, class Period >
	future_cash_status::has_result_detail_state get_detail_state_wait_for( const std::chrono::duration<Rep, Period>& timeout_duration ) {


#ifdef _DEBUG

		// 一ナノ秒（std::chronoが使用できる最小の時間）を使用する
		if( m_has_result == future_cash_status::has_result_state::has_future ) {

			if( m_future.valid() == true ) {


				switch( m_future.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_future_timeout;
					}break;

				}


			}
			else {

				// 設計上のミスによって適切なタグを設定できていない
				throw std::logic_error(
					"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
				);

			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_shared_future ) {

			if( m_sharedFuture.valid() == true ) {


				switch( m_sharedFuture.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_timeout;
					}break;

				}

			}
			else {

				// 設計上のミスによって適切なタグを設定できていない
				throw std::logic_error(
					"Set the appropriate future_cash::has_result_state.This is an error due to a design error."
				);

			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_result ) {

			return future_cash_status::has_result_detail_state::has_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::no_result ) {

			return future_cash_status::has_result_detail_state::no_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::deferred_async ) {

			return future_cash_status::has_result_detail_state::deferred_async;

		}

#else


				// 一ナノ秒（std::chronoが使用できる最小の時間）を使用する
		if( m_has_result == future_cash_status::has_result_state::has_future ) {

			if( m_future.valid() == true ) {


				switch( m_future.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_future_timeout;
					}break;

				}


			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_shared_future ) {

			if( m_sharedFuture.valid() == true ) {


				switch( m_sharedFuture.wait_for( timeout_duration ) ) {

					case std::future_status::deferred:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_deferred;
					}break;
					case std::future_status::ready:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_ready;
					}break;
					case std::future_status::timeout:
					{
						return future_cash_status::has_result_detail_state::has_shared_future_timeout;
					}break;

				}


			}

		}
		else if( m_has_result == future_cash_status::has_result_state::has_result ) {

			return future_cash_status::has_result_detail_state::has_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::no_result ) {

			return future_cash_status::has_result_detail_state::no_result;

		}
		else if( m_has_result == future_cash_status::has_result_state::deferred_async ) {

			return future_cash_status::has_result_detail_state::deferred_async;

		}

#endif


	}


	// 処理結果の詳細保持状態を指定した時間になったら取得する
	template< class Clock, class Duration >
	future_cash_status::has_result_detail_state get_detail_state_wait_until( const std::chrono::time_point<Clock, Duration>& timeout_time ) {

		std::this_thread::sleep_until( timeout_time );

		return get_detail_state();

	}



#pragma endregion get state method


};



#pragma endregion future_cash void specialization