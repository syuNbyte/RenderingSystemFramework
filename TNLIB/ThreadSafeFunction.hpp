#pragma once





#include<mutex>
#include<functional>





//============================================================================
// スレッドセーフな関数オブジェクトを保持するクラス
//============================================================================
#pragma region MultiThread Function class



// スレッドセーフに関数を実行する際に使用するタグ型　※タグ型は実際に使用することはない
enum class threadsafe_flag {

	safe,		// スレッドセーフな状態で実行する
	unsafe,	// 非スレッドセーフな状態で実行する

};





template<class Func>
class threadsafe_function {};



template<class Result, class ...FuncArgs >
class threadsafe_function<Result( FuncArgs... )> {

public:


	using this_type = threadsafe_function<Result( FuncArgs... )>;	// 自身のデータ型
	using func_type = std::function<Result( FuncArgs... )>;			// 自身の保有する関数オブジェクト型
	using result_type = Result;												// 自身の保有する関数オブジェクトの戻り値型
	using args_type = std::tuple<FuncArgs...>;							// 自身の保有する関数オブジェクトの引数型をまとめたtuple型

private:


	func_type m_func = nullptr;
	std::mutex m_mutex;

public:


//===================================
// コンストラクタ
//===================================

	threadsafe_function() noexcept {}

	threadsafe_function( func_type func ) {

		m_func = func;

	}

public:


	this_type& operator=( const this_type& threadsafe_func ) {

		m_func = threadsafe_func.m_func;

		return *this;

	}

	this_type& operator=( this_type&& threadsafe_func ) {

		m_func = threadsafe_func.m_func;

		return *this;

	}


	this_type& operator=( func_type func ) {

		m_func = func;

		return *this;

	}


	// 関数を実行する　※このメンバ関数を通して実行する関数はスレッドセーフではない
	result_type operator()( FuncArgs... args ) {

		return m_func( args... );

	}


	// スレッドセーフに関数を実行する
	result_type operator()( threadsafe_flag isThreadsafe, FuncArgs... args ) {

		if( isThreadsafe == threadsafe_flag::unsafe ) {

			return func_exe( args... );

		}
		else {

			return threadsafe_func_exe( args... );

		}

	}

public:


//===================================
// メンバ関数
//===================================

	// 使用する関数オブジェクトを渡す
	void set_func( func_type func ) {

		m_func = func;

	}


	// 自身の保有する関数オブジェクトを渡す　※所持していないならnullptrを返す
	func_type own_func() {

		return m_func;

	}


	// 関数を実行する　※このメンバ関数を通して実行する関数はスレッドセーフではない
	result_type func_exe( FuncArgs... args ) {


#ifdef _DEBUG

		try {

			return m_func( args... );

		}
		catch( const std::bad_function_call& error ) {

			std::cout << "no set function : " << error.what() << std::endl;

		}

#else

		return m_func( args... );

#endif

	}


	// スレッドセーフな状態で関数を実行する
	result_type threadsafe_func_exe( FuncArgs... args ) {

		// RAIIクラスでデストラクタでunlockを実行するクラスのためスコープを抜けると自動的にロック解除される
		std::lock_guard<std::mutex> lock( m_mutex );

		return func_exe( args... );

	}

};



#pragma endregion MultiThread Function class