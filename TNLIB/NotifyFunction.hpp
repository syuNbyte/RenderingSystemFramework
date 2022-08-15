#pragma once





#include<type_traits>
#include<iostream>

#include"Notify.hpp"
#include"AsyncFunction.hpp"





// まだ慣れていないから下手っぴ
//// 特定のメンバ変数を保持しているかを検索
//struct notify_function_member_impl {
//
//	struct enable_if_m_asyncFunc {
//
//		template<class T>
//		static std::true_type check( decltype( T::m_asyncFunc )* );
//
//		template<class T>
//		static std::false_type check(...);
//
//	};
//
//	struct enable_if_m_notifyer {
//
//		template<class T>
//		static std::true_type check( decltype( T::m_notifyer )* );
//
//		template<class T>
//		static std::false_type check( ... );
//
//	};
//
//	struct enable_if_m_notifyGetter {
//
//		template<class T>
//		static std::true_type check( decltype( T::m_notifyGetter )* );
//
//		template<class T>
//		static std::false_type check( ... );
//
//	};
//
//
//
//	template<class T>
//	struct has_m_asyncFunc : public decltype( enable_if_m_asyncFunc::check<T>){};
//
//	template<class T>
//	struct has_m_notifyer : public decltype( enable_if_m_notifyer::check<T> ){};
//
//	template<class T>
//	struct has_m_notifyGetter : public decltype( enable_if_m_notifyGetter::check<T>){};
//
//	template<class T>
//	struct has_all_member : public std::conjunction<has_m_asyncFunc<T>, has_m_notifyer<T>, has_m_notifyGetter<T>>::value {};
//
//};





//============================================================================
// 関数の実行時に通知を行う関数
//============================================================================
#pragma region notify funcition class



template<class Func>
class notify_function {};



template<class Result, class ...FuncArgs >
class notify_function<Result( FuncArgs... )> {

public:


	using this_type = notify_function<Result( FuncArgs... )>;		// 自身のデータ型
	using async_func_type = async_function<Result( FuncArgs... )>;	// 自身の保有するasync_function型
	using func_type = std::function<Result( FuncArgs... )>;			// 自身の保有する関数オブジェクト型
	using result_type = Result;												// 自身の保有する関数オブジェクトの戻り値型
	using args_type = std::tuple<FuncArgs...>;							// 自身の保有する関数オブジェクトの引数型をまとめたtuple型

private:


//===================================
// メンバ変数
//===================================


	// 関数の処理を実行するasync_function型
	async_func_type m_asyncFunc;

	// 通知を受け取る関数
	notifyer m_notifyer;
	notify_getter m_notifyGetter;

public:


//===================================
// コンストラクタ
//===================================

	notify_function() noexcept {}

	notify_function( func_type func ) {

		m_asyncFunc.set_func( func );

	}

	notify_function( const notifyer& notifyer, func_type func ) {

		m_notifyGetter = notifyer.get_notifyer();

		m_asyncFunc.set_func( func );

	}

	notify_function( notify_getter notify_getter, func_type func ) {

		m_notifyGetter = notify_getter;

		m_asyncFunc.set_func( func );

	}

public:


	this_type& operator=( const this_type& threadsafe_notify_func ) {

		m_asyncFunc = threadsafe_notify_func.m_asyncFunc;

		return *this;

	}

	this_type& operator=( this_type&& threadsafe_notify_func ) {

		m_asyncFunc = threadsafe_notify_func.m_asyncFunc;

		return *this;

	}


	this_type& operator=( func_type func ) {

		m_asyncFunc.set_func( func );

		return *this;

	}


	// 関数を実行する　※このメンバ関数を通して実行する関数はスレッドセーフではない
	result_type operator()( FuncArgs... args ) {

		return m_asyncFunc( args... );

	}

public:


//===================================
// メンバ関数
//===================================

	// 使用する関数オブジェクトを渡す
	void set_func( func_type func ) {

		m_asyncFunc.set_func( func );

	}


	// 自身の保有する関数オブジェクトを渡す　※所持していないならnullptrを返す
	func_type own_func() {

		return m_asyncFunc.own_func();

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

//===================================================
// 通知系クラス受け取り関数・配布関数
//===================================================


	// 通知を受け取るクラスを設定する
	void set_notify_getter( const notify_getter& notifyGetter ) {

		m_notifyGetter = notifyGetter;

	}


	// 通知を発信するクラスを取得する
	notify_getter& get_notifyer() {

		return m_notifyer.get_notifyer();

	}



//===================================================
// 通知受け取り後実行関数・実行後通知発信関数
//===================================================


	// 通知がくるまで待機した後に関数を実行する
	[[nodiscard]] result_type notify_wait_func_exe( FuncArgs... args ) {

		auto lock = m_notifyGetter.get_lock();

		m_notifyGetter.wait( lock );

		return m_asyncFunc.func_exe( args... );

	}


	// 通知がくるまで待機した後にスレッドセーフに関数を実行する
	[[nodiscard]] result_type notify_wait_threadsafe_func_exe( FuncArgs... args ) {

		auto lock = m_notifyGetter.get_lock();

		m_notifyGetter.wait( lock );

		return m_asyncFunc.threadsafe_func_exe( args... );

	}


	// 関数を実行した後に通知を行う
	[[nodiscard]] result_type func_exe_and_notify( FuncArgs... args ) {

		auto value = m_asyncFunc.func_exe( args... );

		m_notifyer.notify_all();

		return value;

	}


	// スレッドセーフに関数を実行した後に通知を行う
	[[nodiscard]] result_type threadsafe_func_exe_and_notify( FuncArgs... args ) {

		auto value = m_asyncFunc.threadsafe_func_exe( args... );

		m_notifyer.notify_all();

		return value;

	}


	// 通知がくるまで待機した後に関数を実行し、関数の実行終了後に通知を行う
	[[nodiscard]] result_type notify_wait_func_exe_and_notify( FuncArgs... args ) {

		auto lock = m_notifyGetter.get_lock();

		m_notifyGetter.wait( lock );

		auto value = m_asyncFunc.func_exe( args... );

		m_notifyer.notify_all();

		return value;

	}


	// 通知がくるまで待機した後にスレッドセーフに関数を実行し、関数の実行終了後に通知を行う
	[[nodiscard]] result_type notify_wait_threadsafe_func_exe_and_notify( FuncArgs... args ) {

		auto lock = m_notifyGetter.get_lock();

		m_notifyGetter.wait( lock );

		auto value = m_asyncFunc.threadsafe_func_exe( args... );

		m_notifyer.notify_all();

		return value;

	}

};



#pragma endregion notify funcition class





#pragma region notify_function void specialization



template<class ...FuncArgs>
class notify_function<void( FuncArgs... )> {

public:


	using this_type = notify_function<void( FuncArgs... )>;		// 自身のデータ型
	using async_func_type = async_function<void( FuncArgs... )>;	// 自身の保有するasync_function型
	using func_type = std::function<void( FuncArgs... )>;			// 自身の保有する関数オブジェクト型
	using result_type = void;												// 自身の保有する関数オブジェクトの戻り値型
	using args_type = std::tuple<FuncArgs...>;							// 自身の保有する関数オブジェクトの引数型をまとめたtuple型

private:


//===================================
// メンバ変数
//===================================


	// 関数の処理を実行するasync_function型
	async_func_type m_asyncFunc;

	// 通知を受け取る関数
	notifyer m_notifyer;
	notify_getter m_notifyGetter;

	// 通知受信クラスを保持しているかを保持するフラグ型
	bool m_ownNotifyGetter = false;

public:


//===================================
// コンストラクタ
//===================================

	notify_function() noexcept {}

	notify_function( func_type func ) {

		m_asyncFunc.set_func( func );

	}

	notify_function( const notifyer& notifyer, func_type func ) {

		m_notifyGetter = notifyer.get_notifyer();

		m_asyncFunc.set_func( func );

	}

	notify_function( notify_getter notify_getter, func_type func ) {

		m_notifyGetter = notify_getter;

		m_asyncFunc.set_func( func );

	}

public:


	this_type& operator=( const this_type& threadsafe_notify_func ) {

		m_asyncFunc = threadsafe_notify_func.m_asyncFunc;

		return *this;

	}

	this_type& operator=( this_type&& threadsafe_notify_func ) {

		m_asyncFunc = threadsafe_notify_func.m_asyncFunc;

		return *this;

	}


	this_type& operator=( func_type func ) {

		m_asyncFunc.set_func( func );

		return *this;

	}


	// 関数を実行する　※このメンバ関数を通して実行する関数はスレッドセーフではない
	result_type operator()( FuncArgs... args ) {

		return m_asyncFunc( args... );

	}

public:


//===================================
// メンバ関数
//===================================

	// 使用する関数オブジェクトを渡す
	void set_func( func_type func ) {

		m_asyncFunc.set_func( func );

	}


	// 自身の保有する関数オブジェクトを渡す　※所持していないならnullptrを返す
	func_type own_func() {

		return m_asyncFunc.own_func();

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

//===================================================
// 通知系クラス受け取り関数・配布関数
//===================================================


	// 通知を受け取るクラスを設定する
	void set_notify_getter( const notify_getter& notifyGetter ) {

		m_notifyGetter = notifyGetter;

	}


	void set_notify_getter( const notifyer& notifyer ) {

		m_notifyGetter = notifyer.get_notifyer();

	}


	// 通知を発信するクラスを取得する
	notify_getter&& get_notifyer() {

		return m_notifyer.get_notifyer();

	}



//===================================================
// 通知受け取り後実行関数・実行後通知発信関数
//===================================================


	// 通知がくるまで待った後に関数を実行する
	result_type notify_wait_func_exe( FuncArgs... args ) {

		wait_object wait_obj( m_notifyGetter );

		m_asyncFunc.func_exe( args... );

	}


	// 通知がくるまで待った後にスレッドセーフに関数を実行する
	result_type notify_wait_threadsafe_func_exe( FuncArgs... args ) {

		wait_object wait_obj( m_notifyGetter );

		m_asyncFunc.threadsafe_func_exe( args... );

	}


	// 関数を実行した後に通知を行う
	[[noreturn]] result_type func_exe_and_notify( FuncArgs... args ) {

		m_asyncFunc.func_exe( args... );

		m_notifyer.notify_all();

	}


	// スレッドセーフに関数を実行した後に通知を行う
	[[noreturn]] result_type threadsafe_func_exe_and_notify( FuncArgs... args ) {

		m_asyncFunc.threadsafe_func_exe( args... );

		m_notifyer.notify_all();

	}


	// 通知がくるまで待機した後に関数を実行し、関数の実行終了後に通知を行う
	[[noreturn]] result_type notify_wait_func_exe_and_notify( FuncArgs... args ) {

		auto lock = m_notifyGetter.get_lock();

		m_notifyGetter.wait( lock );

		m_asyncFunc.func_exe( args... );

		m_notifyer.notify_all();

	}


	// 通知がくるまで待機した後にスレッドセーフに関数を実行し、関数の実行終了後に通知を行う
	[[noreturn]] result_type notify_wait_threadsafe_func_exe_and_notify( FuncArgs... args ) {

		auto lock = m_notifyGetter.get_lock();

		m_notifyGetter.wait( lock );

		m_asyncFunc.threadsafe_func_exe( args... );

		m_notifyer.notify_all();

	}


};



#pragma endregion notify_function void specialization





#pragma region helper function



// 通知を待ってから関数の実行を行う　※この関数は即時実行する
template<class Func, class Result = std::invoke_result_t<Func>, class ...Args >
std::invoke_result_t<Func> notify_wait_func_exe( notifyer& notifyer, Func func, Args... args ) {

	wait_object wait_obj( notifyer );

	return func(args...);

}



//// 通知を待ってから関数の実行を行う
//template<class Func, class Result = std::invoke_result_t<Func>, class ...Args >
//future_cash<Result>&& notify_wait_async_func_exe( notifyer& notifyer, Func func, Args... args ) {
//
//	async_function<Result(void)>async_func([ & ] {
//
//		wait_object wait_obj( notifyer );
//
//		return func(args...);
//
//	} );
//
//	async_func.async();
//
//	return std::move(async_func;
//
//}



#pragma endregion helper function










