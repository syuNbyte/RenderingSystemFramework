#pragma once





#include<windows.h>
#include<system_error>





// マクロの定義
#pragma region



// DirectXLibraryのデフォルトで作成されている色々なものを使用する際に使用する
#define DefaultDXL 0

// 配列を使用しない場合のスタートスロットのデフォルト番号
#define NotArrayStartSlot 0

// 16バイトにアラインメントする　
// ※定数バッファにデータを渡す際に16バイトごとにデータの境界が区切られているためデータを境界に合わせて16バイトアラインメントを行っている
#define ALIGN16 _declspec(align(16))

// 戻り値の受け取りを強制するnodiscard属性をマクロ化させたもの
#define NoDiscard [[nodiscard]]

// 非推奨のプログラムであることをコンパイラに伝える
#define Deprecated [[deprecated]]	// 参考サイト：https://cpprefjp.github.io/lang/cpp14/deprecated_attr.html

// 非推奨のプログラムであることをコンパイラに伝え非推奨コメントを書き加える
#define DeprecatedComment( comment ) [[deprecated(comment)]] 

// 使用非推奨のオブジェクトにつける属性構文
#define MaybeUnused [[maybe_unused]]

//使用しないプログラムに使用するifndef用マクロ
#define UNUSED_PROGRAM

// 指定した関数にパブリックなアクセス指定子を付与する
#define PublicFunc public:

// 指定した関数にプライベートなアクセス指定子を付与する
#define PrivateFunc private:

// 指定した関数にプロテクトなアクセス指定子を付与する
#define ProtectedFunc protected:

// 指定した関数のこれ以上のオーバーライドを禁止する
#define OverrideFinal override final;

// 指定したクラスのこれ以上の継承を禁止する
#define FinalClass( className ) class className final



#pragma endregion Define Macro

// MessageBoxの表示をなくすため
#define NO_MESSAGE_BOX

// 関数マクロの定義
#pragma region



// HRESULT型に入っているエラー文をstd::string型に置き換え
#define GetHRESULTMessage( hr ) std::system_category().message(hr)

// HRESULT型の戻り値が失敗だった場合に入るif文
#define IF_HR(hr) if(FAILED(hr))

// エラーメッセージを出力
#if !defined(NO_MESSAGE_BOX)
#ifdef _DEBUG
#define ErrorMessage(error_message) MessageBox(nullptr, error_message, "error", MB_OK)
#else
#define ErrorMessage(error_message)
#endif
#else
#define ErrorMessage(error_message)
#endif

// HRESULT型に入っているエラーメッセージをメッセージボックスに表示
#define ErrorMessageHR( hr ) { \
	std::string error_message = GetHRESULTMessage(hr);\
	ErrorMessage( error_message.c_str() );\
}

// どの部分でエラーが起きたのかを加味したエラーメッセージ
#define RegionErrorMessage( region_str, error_message ) ErrorMessage(region_str##" : "##error_message)

// HRESULT型の戻り値が失敗だった場合にエラーメッセージを出力する
#define IF_ErrorMessage( hr, error_message ) if( FAILED(hr) ){ ErrorMessage(error_message); }

// ErrorMessageStorageクラスにエラーメッセージを追加
#define AddErrorMessageLogger( class_name, error_type, error_message ){ \
\
	ErrorMessageLogger::AddErrorMessage(\
		class_name,\
		error_type,\
		file_name_macro,\
		std::to_string(line_num_macro),\
		function_name_macro,\
		error_message\
	);\
\
}

//簡易的に回数ループを行うためのマクロ
#define ForNum( loop_num ) for( int i = 0; i < loop_num; i++ )

// 安全にComPtrを解放するためのマクロ
#define SafeReleaseCom( com_object ) {\
	if( com_object ){\
		com_object->Release(); \
		com_object.Detach(); \
	}\
}

// 安全にデータを解放するためのマクロ
#define SafeRelease( object ) {\
 if( object != nullptr ){\
	object->Release();\
	object = nullptr;\
 }\
}


// 三項演算子を実行するマクロ
#define TernaryOperator( _if_, _true_, _false_ ) _if_ ? _true_ : _false_

// 生の配列の要素数を取得する
#define ArrayCount( _array_ ) _countof(_array_)



#pragma endregion Define Function Macro