#pragma once






#include<thread>
#include<future>
#include<chrono>
#include<type_traits>
#include<iostream>

#include"ThreadSafeFunction.hpp"





template<class Func>
class async_function_interface;


template<class Result, class ...FuncArgs>
class async_function_interface<Result(FuncArgs...)> {

public:


	using threadsafe_func_type = threadsafe_function<Result( FuncArgs... )>;	// 自身の保有するスレッドセーフ関数クラスのデータ型
	using func_type = std::function<Result( FuncArgs... )>;							// 自身の保有するスレッドセーフ関数クラスの関数オブジェクト型
	using result_type = Result;																// 自身の保有するスレッドセーフ関数クラスの関数オブジェクトの戻り値型
	using args_type = std::tuple<FuncArgs...>;											// 自身の保有するスレッドセーフ関数クラスの関数オブジェクトの引数型をまとめたtuple型

public:


	// 関数を実行する　※このメンバ関数を通して実行する関数はスレッドセーフではない
	virtual result_type operator()( FuncArgs... args ) = 0;

	// 関数を指定したローンチで開始する
	virtual void operator()( std::launch launch, FuncArgs... args ) = 0;

	// スレッドセーフにするかどうかを選択し、関数を指定したローンチで開始する
	virtual void operator()( threadsafe_flag isThreadsafe, std::launch launch, FuncArgs... args ) = 0;

public:


	// 使用する関数オブジェクトを設定する
	virtual void set_func( func_type func ) = 0;


	// 自身の保有する関数オブジェクトを渡す　※所持していないならnullptrを返す
	virtual func_type own_func() = 0;


	// 関数を実行する　※このメンバ関数を通して実行する関数はスレッドセーフではない
	virtual result_type func_exe( FuncArgs... args ) = 0;


	// スレッドセーフに関数を実行する
	virtual result_type threadsafe_func_exe( FuncArgs... args ) = 0;

public:


	// 関数を非同期実行する
	virtual void async( FuncArgs... args ) = 0;


	// 関数をスレッドセーフに非同期実行する
	virtual void deferred( FuncArgs... args ) = 0;

};



// このインターフェイスクラスをasync_functionクラスで継承しているとエラーが出る
// async_function_dequeクラスの場合はエラーが出ない