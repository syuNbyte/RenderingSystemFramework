#pragma once

#include<iostream>
#include<sstream>
#include<utility>
#include<vector>
#include<string>
#include<fstream>
#include<cassert>
#include<crtdbg.h>	// 出力ウィンドウに表示するマクロ用
#include<type_traits>

#include"Math.hpp"
#include"Utility.hpp"
#include"FileUtility.hpp"
#include"System.hpp"
#include"Semaphore.hpp"
#include"AsyncFunction.hpp"


//===================================================
// 出力ウィンドウに表示
//===================================================
#define DBG_MSG(msg) _RPT0(_CRT_WARN, msg)

//======================================================================================================
// <cassert>内のassertマクロだとfalseの指定がめんどくさいし、表示するエラーの内容が指定できないのでこのマクロが必要
//======================================================================================================
#define Assert(message)																							\
            (_wassert(_CRT_WIDE(#message), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0)	\



// デバッグ関連の情報を提供する
class Debug {
	
private:

	enum class LogType : char_t {
		Log,
		Warning,
		Assertion,
		Error,
		Exception
	};

private:

	std::vector<std::pair<std::pair<LogType, std::string>, std::string>> m_log;		// ログの保存先<<ログタイプ, ログのタグ>, 実際の文字列>
	std::string m_buf;																					// 一時的に使用されるバッファ
	ulonglong_t m_logIndex;																				// ログの保存先インデックス番号
	LogType m_logType;																					// 現在設定されているログのタイプ
	std::string m_logTag;																				// 現在設定されているログのタグ
	std::string m_bracketLogTag;																		// タグ付き波かっこ範囲のタグ情報を管理するためのテンポラリ
	ulonglong_t m_onceLogIndex;																		// 同一ログの出力を一度のみに制限するメソッド
	std::vector<std::pair<std::pair<LogType, std::string>, std::string>> m_onceLog;	// 同一ログの出力を一度のみに制限するメソッド使用時のログを保存しあとから参照するためのバッファ
	std::string m_prefix;																				// 出力する文字列全てに付与される接頭辞
	std::string m_suffix;																				// 出力する文字列全てに付与される接尾辞
	std::string m_tab;																					// 出力する文字列全てに付与されるタブ文字
	std::string m_bracketTab;																			// BracketBegin, BracketEndメソッドの範囲内にの文字列全てに付与されるタブ文字
	std::string m_header;																				// ファイルにパースする際のヘッダー
	std::string m_footer;																				// ファイルにパースする際のフッター

	// ログの処理を全てスキップする
	bool m_logProcessSkip;

	// ログの書き始めかどうかを判定する内部的なフラグ
	bool m_logInit;

	// ログのセーブを行うかどうかのフラグ
	bool m_logSave;

	// ログをセーブする際にタイムスタンプを押すかどうかのフラグ
	bool m_timeStamp;

	// ログ出力時にbool型パラメータをtrue, falseで出力させるかどうかのフラグ
	bool m_boolalpha;

	// 浮動小数点数をログ出力する際のパラメータ
	std::chars_format m_floatViewType;	// 浮動小数点型の文字列への変換形式
	int_t m_floatPrecision;					// 浮動小数点型を文字列へ変換する際の変換精度

	// 危険度が高いエラーの場合に停止するかどうかのフラグ
	// ※危険度が高いエラー：Assertion, Error, Exception
	bool m_stopIfHighDangerousness;

	// リリースモード時にコンソールウィンドウへの表示をするかどうかのフラグ
	// ※表示はされなくてもログとして保存はされます
	bool m_dispOnReleaseMode;

	// ログのコンソールウィンドウへの表示を行うかどうかのフラグ
	bool m_dispOnConsoleWindow;

	// ログの発行時にVisualStudioの出力ウィンドウにログを表示するかどうかのフラグ
	// ※リリース時には表示されません（これはVisualStudio側の仕様です）
	bool m_dispOnOutputWindow;

	//==================================================
	// パース処理をマルチスレッド化するためのパラメータ
	//==================================================
	// マルチスレッド処理を行うかを判定するためのフラグ
	bool m_multiThreadParse;
	// バイナリセマフォ
	binary_semaphore m_semaphore;
	// パース処理を行う関数
	async_function<void(std::string, FileUtility::_OpenMode, std::function<bool(LogType)>)> m_parseFunc;
	// タグ別でパース処理を行う関数
	async_function<void(std::string, FileUtility::_OpenMode, std::function<bool(LogType)>, std::function<bool(std::string)>)> m_parseForTagFunc;

private:

	Debug() {
		m_log.resize( 100 );
		m_logIndex = 0;
		m_onceLog.resize( 100 );
		m_onceLogIndex = 0;
		m_logType = LogType::Log;
		m_logTag = "Default";
		m_logProcessSkip = false;
		m_logInit = true;
		m_logSave = true;
		m_timeStamp = true;
		m_boolalpha = true;
		m_floatViewType = std::chars_format::general;
		m_floatPrecision = 5;
		m_stopIfHighDangerousness = true;
		m_dispOnReleaseMode = false;
		m_dispOnConsoleWindow = true;
		m_dispOnOutputWindow = false;
		m_multiThreadParse = true;
		m_semaphore = true;
		m_parseFunc.set_func(Debug::_Internal_ParseForLogTypeIf);
		m_parseForTagFunc.set_func(Debug::_Internal_ParseForLogTypeIfAndTagNameIf);
	}

	~Debug() {
		Debug::Parse::AllLog();
	}

	static Debug& GetInstance() {
		static Debug debug;
		return debug;
	}

	static void _Intenal_SetLogType( LogType logType ) {
		auto& instance = GetInstance();
		instance.m_logType = logType;
	}

	static bool _Intenal_StopProcess() {
		auto& instance = GetInstance();
		return instance.m_stopIfHighDangerousness;
	}

	static void _Intenal_ClearTemporaryBuffer() {
		auto& instance = GetInstance();
		instance.m_buf.clear();
	}

	// タグ付き波かっこ範囲を使用する際に作成される内部的な管理用のタグを作成する
	static void _Internal_CreateBracketLogTag(std::string tagName) {
		auto& instance = GetInstance();
		if (instance.m_bracketLogTag.empty()) {
			instance.m_bracketLogTag = tagName;
		}
		else {
			instance.m_bracketLogTag += "_" + tagName;
		}
	}

	// タグ付きの波かっこ範囲を使用する際に
	// 波かっこ範囲ごとのタグを管理しておき自動で適切なタグに切り替えるための処理
	static void _Internal_BracketLogTag() {
		auto& instance = GetInstance();

		auto splitPos = instance.m_bracketLogTag.find_last_of("_");

		if (splitPos != std::string::npos) {
			auto tagName = instance.m_bracketLogTag.substr(splitPos + 1, instance.m_bracketLogTag.size() - splitPos);
			instance.m_bracketLogTag.erase(splitPos, instance.m_bracketLogTag.size());
			Debug::SetLogTag(instance.m_bracketLogTag);
		}
		else {
			instance.m_bracketLogTag.clear();
			Debug::SetDefaultLogTag();
		}
	}

	//===================================================
	// ログの出力
	//===================================================

	// ログに使用する文字列を作成を終了する
	static void _Internal_CreateLogString() {
		auto& instance = GetInstance();

		// ログのフラグを出力フラグ初期化
		instance.m_logInit = true;

		// 改行文字列の前の部分にタブ文字を追加する
		auto count = 0;
		auto size = instance.m_buf.size();

		// 改行文字列の数をカウント
		for (int_t i = 0; i < size; i++) {
			if (instance.m_buf[i] == '\n') ++count;
		}

		// 改行文字数 * 現在のタブ文字数 + 元の文字列数 = 新しいバッファ内の文字列数
		auto tabCount = instance.m_tab.size() + instance.m_bracketTab.size();	// タブ文字の数
		auto appeaNewLine = 0;																	// ループ内の改行文字の出現回数
		auto tabSize = count * tabCount;														// 追加されるタブ文字列の数
		auto preBufSize = instance.m_buf.size();											// 元のバッファサイズ
		std::string tmp;																			// テンポラリ
		tmp.resize(preBufSize + tabSize);													// テンポラリの文字数を新しいバッファサイズに設定


		for (int_t i = 0; i < preBufSize; i++) {
			if (instance.m_buf[i] != '\n') {
				tmp[i + (tabCount * appeaNewLine)] = instance.m_buf[i];
			}
			else {
				tmp[i + (tabCount * appeaNewLine)] = '\n';
				for (int_t t = 1; t <= tabCount; t++) tmp[i + t + (tabCount * appeaNewLine)] = '\t';
				++appeaNewLine;
			}
		}

		// 再構成した文字列をインスタンスのバッファに設定
		instance.m_buf = std::move(tmp);
	}

	// ログに使用する文字列を作成する
	template<class Head, class ...Tail>
	static void _Internal_CreateLogString(Head&& head, Tail&&... tail) {
		auto& instance = GetInstance();

		if (instance.m_multiThreadParse) {
			// マルチスレッド対応用のバイナリセマフォ
			instance.m_semaphore.wait_true();
		}

		// 文字列ストリームに出力
		std::stringstream ss;
		std::string str;

		if (Math::template IsFloat<Head>(head)) {
			ss << Utility::ToStringFloat(head, instance.m_floatViewType, instance.m_floatPrecision);
			str = ss.str();
		}
		else if (instance.m_boolalpha && TypeTraitsEx::IsBool<Head>) {
			// データ型がboolの場合の処理
			if (TypeTraitsEx::template BoolValue<Head>(head)) {
				str = "true";
			}
			else {
				str = "false";
			}
		}
		else {
			ss << head;
			str = ss.str();
		}

		// 一度のログ出力中に最初だけ通る処理
		if (instance.m_logInit) {
			instance.m_logInit = false;
			// 文字列の先頭に接頭辞を追加
			str = instance.m_bracketTab + instance.m_tab + instance.m_prefix + str;
		}

		if (sizeof...(tail) == 0) {
			// 文字列の末尾に接尾辞を追加
			str = str + instance.m_suffix;
		}

		// 文字列をログ文字列のバッファの末尾に追加
		instance.m_buf.append(str);

		// パラメータパックtailをさらにheadとtailに分割する
		Debug::_Internal_CreateLogString(std::forward<Tail>(tail)...);
	}

	// デバッグのログの表示を終了する
	static void _Internal_Log() {

		auto& instance = GetInstance();

#if defined(_DEBUG)
		// 最後に改行して終了
		if( instance.m_dispOnConsoleWindow )std::cout << instance.m_buf << std::endl;
		if( instance.m_dispOnOutputWindow )DBG_MSG(instance.m_buf.c_str());
		if( instance.m_dispOnOutputWindow )DBG_MSG("\n");

#elif !defined(_DEBUG)
		// リリース時にログの表示を行うか
		if( instance.m_dispOnReleaseMode ) {
			std::cout << instance.m_buf << std::endl;
		}
#endif

		if (instance.m_logSave) {
			// ログ内容を保存する
			instance.m_log[instance.m_logIndex].first.first = instance.m_logType;	// ログのタイプ
			instance.m_log[instance.m_logIndex].first.second = instance.m_logTag;	// ログのタグ
			instance.m_log[instance.m_logIndex].second = instance.m_buf;				// ログの文字列

			// ログの保存先インデックスを前に
			instance.m_logIndex++;
		}

		// ログのフラグを出力フラグ初期化
		instance.m_logInit = true;
		

		// ログの保存先の容量がなければ追加
		if( instance.m_log.size() == instance.m_logIndex ) {
			instance.m_log.resize( instance.m_log.size() + 100 );
		}

	}

	// デバッグのログを表示する
	template<class ...Args>
	static void _Internal_Log( Args&&... args ) {
		Debug::_Internal_CreateLogString(args...);
		Debug::_Internal_Log();
		Debug::_Intenal_ClearTemporaryBuffer();
	}

public:

	// デバッグのログを表示する
	template<class ...Args>
	static void Log( Args... args ) {
		auto& instance = GetInstance();
		if( !instance.m_logProcessSkip ) {
			Debug::_Internal_Log( args... );
		}
	}

	// 空行をn回挿入する
	static void Space(int_t n = 1) {
		std::string space;
		for( int_t i = 1; i < n; i++ ) {
			space.append( "\n" );
		}
		Debug::Log( space );
	}

	// 分離文字列をn個挿入する
	// n：回数
	// separateChar：n回挿入される文字列
	// prefix：分割文字列の一番前に挿入される文字列
	// suffix：分割文字列の一番後に挿入される文字列
	// center：分割文字列の中心に挿入される文字列
	static void Separater( int_t n = 10, std::string separateChar = "=", std::string prefix = "", std::string suffix = "", std::string center = "" ) {
		std::string separater;
		int_t centerPos = n / 2 - 1;
		separater.append( prefix );
		for( int_t i = 0; i < n; i++ ) {
			separater.append( separateChar );
			if( i == centerPos ) separater.append( center );
		}
		separater.append( suffix );
		Debug::Log( separater );
	}

	// 指定文字列strを垂直方向にn回ループ挿入する
	static void VerticalLoop( int_t n, std::string str ) {
		for( int_t i = 0; i < n; i++ ) {
			Debug::Log( str );
		}
	}

	// 指定文字列strを水平方向にn回ループ挿入する
	static void HorizontalLoop( int_t n, std::string str ) {
		std::string tmp;
		for( int_t i = 0; i < n; i++ ) {
			tmp.append( str );
		}
		Debug::Log( tmp );
	}

	// コンソールウィンドウにClearコマンドを送る
	// ログ自体はClearされない
	static void ClearConsole() {
		std::system( "cls" );
	}

	//===================================================
	// エラー時のログの出力
	//===================================================

	// デバッグログを出力し弱警告を行う
	// ※この際「DebugLog.txt」ファイルに自動的に今までのログが保存される
	template <class ...Args>
	static void LogWarning( Args... args ) {
		Debug::_Intenal_SetLogType( LogType::Warning );
		Debug::BracketBegin("WarningLog");
		Debug::Log(args...);
		Debug::BracketEnd();
		Debug::_Intenal_SetLogType( LogType::Log );
		Debug::Parse::AllLog();
	}

	// デバッグログを出力し強警告を行う
	// ※この際「DebugLog.txt」ファイルに自動的に今までのログが保存される
	template <class ...Args>
	static void LogAssertion( Args... args ) {
		Debug::_Intenal_SetLogType( LogType::Assertion );
		Debug::BracketBegin("AssertionLog");
		Debug::Log(args...);
		Debug::BracketEnd();
		Debug::_Intenal_SetLogType( LogType::Log );
		Debug::Parse::AllLog();

#if _DEBUG
		// _USE_WINAPIを定義した場合のみ
#if defined(WINAPI) && defined(_USE_WINAPI)
		if( Debug::StopProcess() ) {
			auto& instance = GetInstance();
			MessageBox( nullptr, instance.m_log[instance.m_logIndex].c_str(), "Assertion", MB_OK );
			Assert( "Assertion" );
		}
#else
		if( Debug::_Intenal_StopProcess() ) {
			Assert( "Assertion" );
		}
#endif
#endif

	}

	// デバッグログを出力しエラー出力を行う
	// ※この際「DebugLog.txt」ファイルに自動的に今までのログが保存される
	template <class ...Args>
	static void LogError( Args... args ) {
		Debug::_Intenal_SetLogType( LogType::Error );
		Debug::BracketBegin("ErrorLog");
		Debug::Log(args...);
		Debug::BracketEnd();
		Debug::_Intenal_SetLogType( LogType::Log );
		Debug::Parse::AllLog();

#if _DEBUG
		// _USE_WINAPIを定義した場合のみ
#if defined(WINAPI) && defined(_USE_WINAPI)
		if( Debug::StopProcess() ) {
			auto& instance = GetInstance();
			MessageBox( nullptr, instance.m_log[instance.m_logIndex].c_str(), "Error", MB_OK );
			Assert( "Error" );
		}
#else
		if( Debug::_Intenal_StopProcess() ) {
			Assert( "Error" );
		}
#endif
#endif

	}

	// デバッグログを出力し例外出力を行う
	// ※この際「DebugLog.txt」ファイルに自動的に今までのログが保存される
	template <class ...Args>
	static void LogException( Args... args ) {
		Debug::_Intenal_SetLogType( LogType::Exception );
		Debug::BracketBegin("ExceptionLog");
		Debug::Log(args...);
		Debug::BracketEnd();
		Debug::_Intenal_SetLogType( LogType::Log );
		Debug::Parse::AllLog();

#if _DEBUG
		// _USE_WINAPIを定義した場合のみ
#if defined(WINAPI) && defined(_USE_WINAPI)
		if( Debug::StopProcess() ) {
			auto& instance = GetInstance();
			MessageBox( nullptr, instance.m_log[instance.m_logIndex].c_str(), "Exception", MB_OK );
			Assert( "Exception" );
		}
#else
		if( Debug::_Intenal_StopProcess() ) {
			Assert( "Exception" );
		}
#endif
#endif

	}

public:

	// ログの処理を行うかどうかを設定する
	static void SetLogSkipFlag( bool flag ) {
		GetInstance().m_logProcessSkip = flag;
	}

	// ログの保存をを行うかを設定する
	static void SetLogSaveFlag( bool flag ) {
		GetInstance().m_logSave = flag;
	}

	// ログの保存時にタイムスタンプを押すかどうかを設定する
	static void SetTimeStampFlag( bool flag ) {
		GetInstance().m_timeStamp = flag;
	}

	// ログ出力時にbool型パラメータをtrue, falseで出力させるかどうかのフラグ
	static void SetBoolAlphaFlag( bool flag ) {
		GetInstance().m_boolalpha = flag;
	}

	// ログに浮動小数点数を表示する際の変換フラグを設定する
	static void SetFloatViewType( std::chars_format floatViewType, int_t floatPrecision ) {
		auto& instance = GetInstance();
		instance.m_floatViewType = floatViewType;
		instance.m_floatPrecision = floatPrecision;
	}

	// この関数実行以降に発行される全てのログにデフォルトのタグを設定
	static void SetDefaultLogTag() {
		GetInstance().m_logTag = "Default";
	}

	// この関数実行以降に発行される全てのログにタグを設定
	static void SetLogTag( std::string tagName ) {
		GetInstance().m_logTag = tagName;
	}

	// 現在設定されている接頭辞を削除する
	static void ResetPrefix() {
		GetInstance().m_prefix = "";
	}

	// この関数実行以降に発行される全てのログに接頭辞を設定する
	static void SetPrefix( std::string prefixName ) {
		GetInstance().m_prefix = prefixName;
	}

	// 現在設定されている接尾辞を削除する
	static void ResetSuffix() {
		GetInstance().m_suffix = "";
	}

	// この関数実行以降に発行される全てのログに接尾辞を設定する
	static void SetSuffix( std::string suffixName ) {
		GetInstance().m_suffix = suffixName;
	}

	// 現在設定されているタブを削除する
	static void ResetTab() {
		GetInstance().m_tab = "";
	}

	// この関数実行以降に発行される全てのログにタブ文字列を設定する
	static void SetTab( int_t n = 1 ) {
		std::string tab;
		for( int_t i = 0; i < n; i++ ) tab.append( "\t" );
		GetInstance().m_tab = tab;
	}

	// 現在設定されているタブ文字列にタブ文字を追加する
	static void AddTab( int_t n = 1 ) {
		auto& instance = GetInstance();
		for( int_t i = 0; i < n; i++ ) instance.m_tab.append( "\t" );
	}

	// 現在設定されているタブ文字列からタブ文字を削除する
	static void RemoveTab( int_t n = 1 ) {
		auto& instance = GetInstance();
		for( int_t i = 0; i < n; i++ ) instance.m_tab.erase(instance.m_tab.begin());
	}

	// 波かっこの範囲を開始する
	static void BracketBegin() {
		auto& instance = GetInstance();
		Debug::Log( "{" );
		instance.m_bracketTab.append( "\t" );
	}

	// 波かっこの先頭にヘッダーを付けてから波かっこの範囲を開始する
	static void BracketBegin(std::string header) {
		auto& instance = GetInstance();
		Debug::SetPrefix(header);
		Debug::Log("{");
		Debug::ResetPrefix();
		instance.m_bracketTab.append("\t");
	}

	// 波かっこの範囲を開始し、範囲内にタグを設定する
	static void TagBracketBegin(std::string tagName) {
		Debug::SetLogTag(tagName);
		Debug::_Internal_CreateBracketLogTag(tagName);
		Debug::BracketBegin();
	}

	// 波かっこの先頭にヘッダーを付けてから波かっこの範囲を開始し、範囲内にタグを設定する
	static void TagBracketBegin(std::string tagName, std::string header) {
		Debug::SetLogTag(tagName);
		Debug::_Internal_CreateBracketLogTag(tagName);
		Debug::BracketBegin(header);
	}

	// 波かっこの範囲を終了する
	static void BracketEnd() {
		auto& instance = GetInstance();
		auto empty = instance.m_bracketTab.empty();
		instance.m_bracketTab.erase(instance.m_bracketTab.begin());
		if(!empty) Debug::Log( "}" );
	}

	// 波かっこの範囲を終了し、波かっこの末尾にフッターを付ける
	static void BracketEnd(std::string footer) {
		auto& instance = GetInstance();
		auto empty = instance.m_bracketTab.empty();
		instance.m_bracketTab.erase(instance.m_bracketTab.begin());
		if (!empty) {
			Debug::SetSuffix(footer);
			Debug::Log("}");
			Debug::ResetSuffix();
		}
	}

	// タグを設定された波かっこの範囲を終了する
	static void TagBracketEnd() {
		Debug::BracketEnd();
		Debug::_Internal_BracketLogTag();
	}

	// タグを設定された波かっこの範囲を終了し、波かっこの末尾にフッターを付ける
	static void TagBracketEnd(std::string footer) {
		Debug::BracketEnd(footer);
		Debug::_Internal_BracketLogTag();
	}

	static void ResetHeader() {
		GetInstance().m_header = "";
	}

	static void ResetFooter() {
		GetInstance().m_footer = "";
	}

	static void SetHeader(std::string header) {
		GetInstance().m_header = header;
	}

	static void SetFooter(std::string footer) {
		GetInstance().m_footer = footer;
	}

	// 危険度が高いエラーの場合に停止するかどうかを設定する
	// ※危険度が高いエラー：Assertion, Error, Exception
	static void SetDengerLogTimeStopFlag( bool flag ) {
		GetInstance().m_stopIfHighDangerousness = flag;
	}

	// リリースモード時にログを表示するかどうかを設定する
	// ※表示はされなくてもログとして保存はされます
	static void SetDispOnRelease( bool flag ) {
		GetInstance().m_dispOnReleaseMode = flag;
	}

	// ログの発行時にコンソールウィンドウに表示するかどうかを設定する
	static void SetDispOnConsoleWindow(bool flag) {
		GetInstance().m_dispOnConsoleWindow = flag;
	}

	// ログの発行時にVisualStudioの出力ウィンドウに表示するかどうかを設定する
	// ※リリース時には表示されません
	static void SetDispOnOutputWindow( bool flag ) {
		GetInstance().m_dispOnOutputWindow = flag;
	}

	// パース処理時にマルチスレッドにてパース処理を行うかどうかを設定する
	static void SetMultiThreadParse(bool flag) {
		GetInstance().m_multiThreadParse = flag;
	}

	//===================================================
	// 特殊なログの出力
	//===================================================
	// これらのメソッドはC++にデフォルトで実装されるマクロを使用するもので
	// これらのマクロはファイルごと・関数ごと・行ごとにマクロの内容が異なるため引数として渡させる必要がある

	// この関数は引数に「__FILE__」マクロを渡すこと！
	static void LogFilePath(std::string path) {
		Debug::Log("FilePath : " + path);
	}

	// この関数は引数に「__func__」マクロを渡すこと！　※__func__は実際にはマクロではない
	static void LogFunction(std::string funcName) {
		Debug::Log("Function : " + funcName);
	}

	// この関数は引数に「__LINE__」マクロを渡すこと！
	static void LogFileLineNum(int_t line) {
		Debug::Log("Line : ", line);
	}

#pragma region StackTrace

private:

	static std::vector<std::shared_ptr<std::string>> _Internal_GetStrPtrLogTypeIf(
		std::function<bool( LogType )> logTypeCond = []( LogType logType ) { return true; }
	) {
		auto& instance = GetInstance();
		std::vector<std::shared_ptr<std::string>> ret;

		for( auto i = 0; i < instance.m_logIndex; i++ ) {
			if( logTypeCond( instance.m_log[i].first.first ) ) {
				ret.emplace_back(std::make_shared<std::string>(instance.m_log[i].second));
			}
		}

		return ret;
	}

	static std::vector<std::shared_ptr<std::string>> _Internal_GetStrPtrLogTypeIfAndTagNameIf(
		std::function<bool( LogType )> logTypeCond = []( LogType logType ) { return true; },
		std::function<bool( std::string )> tagNameCond = []( std::string tagName ) { return true; }
		) {
		auto& instance = GetInstance();
		std::vector<std::shared_ptr<std::string>> ret;

		for( auto i = 0; i < instance.m_logIndex; i++ ) {
			if( logTypeCond( instance.m_log[i].first.first ) && tagNameCond( instance.m_log[i].first.second ) ) {
				ret.emplace_back( std::make_shared<std::string>( instance.m_log[i].second ) );
			}
		}

		return ret;
	}

public:

	struct StackTrace {

		// 全てのログを取得する
		static std::vector<std::shared_ptr<std::string>> AllLog() {
			return Debug::_Internal_GetStrPtrLogTypeIf();
		}

		// 通常のログを取得する
		static std::vector<std::shared_ptr<std::string>> NormalLog() {
			return Debug::_Internal_GetStrPtrLogTypeIf( []( LogType logType ) { return logType == LogType::Log; } );
		}

		// 危険なログを取得する
		static std::vector<std::shared_ptr<std::string>> DengerLog() {
			return Debug::_Internal_GetStrPtrLogTypeIf( []( LogType logType ) { return logType != LogType::Log; } );
		}

		// 弱警告のログを取得する
		static std::vector<std::shared_ptr<std::string>> WarningLog() {
			return Debug::_Internal_GetStrPtrLogTypeIf( []( LogType logType ) { return logType == LogType::Warning; } );
		}

		// 強警告のログを取得する
		static std::vector<std::shared_ptr<std::string>> AssertionLog() {
			return Debug::_Internal_GetStrPtrLogTypeIf( []( LogType logType ) { return logType == LogType::Assertion; } );
		}

		// エラーのログを取得する
		static std::vector<std::shared_ptr<std::string>> ErrorLog() {
			return Debug::_Internal_GetStrPtrLogTypeIf( []( LogType logType ) { return logType == LogType::Error; } );
		}

		// 例外のログを取得する
		static std::vector<std::shared_ptr<std::string>> ExceptionLog() {
			return Debug::_Internal_GetStrPtrLogTypeIf( []( LogType logType ) { return logType == LogType::Exception; } );
		}

	};

	struct StackTraceTag {

		// 同一タグの全てのログを取得する
		static std::vector<std::shared_ptr<std::string>> AllLog( std::string tagName ) {
			return Debug::_Internal_GetStrPtrLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return true; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、通常のログを取得する
		static std::vector<std::shared_ptr<std::string>> NormalLog( std::string tagName ) {
			return Debug::_Internal_GetStrPtrLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType == LogType::Log; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、危険なログを取得する
		static std::vector<std::shared_ptr<std::string>> DengerLog( std::string tagName ) {
			return Debug::_Internal_GetStrPtrLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType != LogType::Log; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、弱警告のログを取得する
		static std::vector<std::shared_ptr<std::string>> WarningLog( std::string tagName ) {
			return Debug::_Internal_GetStrPtrLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType == LogType::Warning; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、強警告のログを取得する
		static std::vector<std::shared_ptr<std::string>> AssertionLog( std::string tagName ) {
			return Debug::_Internal_GetStrPtrLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType == LogType::Assertion; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、エラーのログを取得する
		static std::vector<std::shared_ptr<std::string>> ErrorLog( std::string tagName ) {
			return Debug::_Internal_GetStrPtrLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType == LogType::Error; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、例外のログを取得する
		static std::vector<std::shared_ptr<std::string>> ExceptionLog( std::string tagName ) {
			return Debug::_Internal_GetStrPtrLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType == LogType::Exception; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

	};

#pragma endregion StackTrace

#pragma region Count

private:

	static ulonglong_t _Internal_CountLogTypeIf(
		std::function<bool( LogType )> logTypeCond = []( LogType logType ) { return true; }
	) {
		auto& instance = GetInstance();
		ulonglong_t count = 0;

		// 要素がなくなるまで出力
		for( auto i = 0; i < instance.m_logIndex; i++ ) {
			if( logTypeCond( instance.m_log[i].first.first ) ) {
				++count;
			}
		}

		return count;
	}

	static ulonglong_t _Internal_CountLogTypeIfAndTagNameIf(
		std::function<bool( LogType )> logTypeCond = []( LogType logType ) { return true; },
		std::function<bool( std::string )> tagNameCond = []( std::string tagName ) { return true; }
	) {
		auto& instance = GetInstance();
		ulonglong_t count = 0;

		// 要素がなくなるまで出力
		for( auto i = 0; i < instance.m_logIndex; i++ ) {
			if( logTypeCond( instance.m_log[i].first.first ) && tagNameCond( instance.m_log[i].first.second ) ) {
				++count;
			}
		}

		return count;
	}

public:

	struct Count {

		// 全てのログの数を取得する
		static ulonglong_t AllLog() {
			return Debug::GetInstance().m_log.size();
		}

		// 通常のログの数を取得する
		static ulonglong_t NormalLog() {
			return Debug::_Internal_CountLogTypeIf( []( LogType logType ) { return logType == LogType::Log; } );
		}

		// 危険なログの数を取得する
		static ulonglong_t DengerLog() {
			return Debug::_Internal_CountLogTypeIf( []( LogType logType ) { return logType != LogType::Log; } );
		}

		// 弱警告のログの数を取得する
		static ulonglong_t WarningLog() {
			return Debug::_Internal_CountLogTypeIf( []( LogType logType ) { return logType == LogType::Warning; } );
		}

		// 強警告のログの数を取得する
		static ulonglong_t AssertionLog() {
			return Debug::_Internal_CountLogTypeIf( []( LogType logType ) { return logType == LogType::Assertion; } );
		}

		// エラーのログの数を取得する
		static ulonglong_t ErrorLog() {
			return Debug::_Internal_CountLogTypeIf( []( LogType logType ) { return logType == LogType::Error; } );
		}

		// 例外のログの数を取得する
		static ulonglong_t ExceptionLog() {
			return Debug::_Internal_CountLogTypeIf( []( LogType logType ) { return logType == LogType::Exception; } );
		}

	};

	struct CountForTag {

		// 同一タグかつ、全てのログの数を取得する
		static ulonglong_t AllLog( std::string tagName ) {
			return Debug::_Internal_CountLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return true; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、通常のログの数を取得する
		static ulonglong_t NormalLog( std::string tagName ) {
			return Debug::_Internal_CountLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType == LogType::Log; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、危険なログの数を取得する
		static ulonglong_t DengerLog( std::string tagName ) {
			return Debug::_Internal_CountLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType != LogType::Log; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、弱警告のログの数を取得する
		static ulonglong_t WarningLog( std::string tagName ) {
			return Debug::_Internal_CountLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Warning; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、強警告のログの数を取得する
		static ulonglong_t AssertionLog( std::string tagName ) {
			return Debug::_Internal_CountLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Assertion; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、エラーのログの数を取得する
		static ulonglong_t ErrorLog( std::string tagName ) {
			return Debug::_Internal_CountLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Error; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、例外のログの数を取得する
		static ulonglong_t ExceptionLog( std::string tagName ) {
			return Debug::_Internal_CountLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType == LogType::Exception; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

	};

#pragma endregion Count

#pragma region Parse

private:

	static void _Internal_ParseForLogTypeIf(
		std::string path = "DebugLog.txt",
		FileUtility::_OpenMode openMode = FileUtility::_ZeroBit,
		std::function<bool(LogType)> logTypeCond = [](LogType logType) { return true; }
	) {
		auto& instance = GetInstance();

		if (instance.m_multiThreadParse) {
			// セマフォのフラグがfalseならtrueになるまで待つ
			if(instance.m_semaphore == false) instance.m_semaphore.wait_true();

			// セマフォをfalseに変更
			instance.m_semaphore.false_flag();
		}

		// 書き込みファイルの読み込み
		std::ofstream ofs( "Log/" + path, std::ios::out | openMode );

		// ディレクトリが存在しないならディレクトリを作成する
		if( !ofs ) {
			FileUtility::Utility::CreateFolder( "Log" );
			ofs.open( "Log/" + path, std::ios::out | openMode );
		}

		// ファイルが開けたら処理を行う
		if( ofs ) {
			if( instance.m_timeStamp ) {
				// 日時を出力
				ofs << "========Start========\n";
				ofs << ( System::Date::YYYY_MM_SS() + " " + System::Date::HH_MM_SS() + "\n" );
				ofs << "=====================\n";
			}
			if(!instance.m_header.empty()) {
				// ヘッダーを出力
				ofs << (instance.m_header + "\n");
			}
			// 要素がなくなるまで出力
			for( auto i = 0; i < instance.m_logIndex; i++ ) {
				if(logTypeCond( instance.m_log[i].first.first ) ) {
					ofs << instance.m_log[i].second << std::endl;
				}
			}
			if(!instance.m_footer.empty()) {
				// フッターを出力
				ofs << instance.m_footer + "\n";
			}
			if( instance.m_timeStamp ) {
				// 日時を出力
				ofs << "=========End=========\n";
				ofs << ( System::Date::YYYY_MM_SS() + " " + System::Date::HH_MM_SS() + "\n" );
				ofs << "=====================\n";
			}
		}

		if (instance.m_multiThreadParse) {
			// セマフォをtrueに変更
			instance.m_semaphore.true_flag();
		}
	}

	static void _Internal_ParseForLogTypeIfAndTagNameIf(
		std::string path = "DebugLog.txt",
		FileUtility::_OpenMode openMode = FileUtility::_ZeroBit,
		std::function<bool( LogType )> logTypeCond = []( LogType logType ) { return true; },
		std::function<bool( std::string )> tagNameCond = []( std::string tagName ) { return true; }
	) {
		auto& instance = GetInstance();
	
		if (instance.m_multiThreadParse) {
			// セマフォのフラグがfalseならtrueになるまで待つ
			if (instance.m_semaphore == false) instance.m_semaphore.wait_true();

			// セマフォをfalseに変更
			instance.m_semaphore.false_flag();
		}

		// 書き込みファイルの読み込み
		std::ofstream ofs( "Log/" + path, std::ios::out | openMode );

		// ディレクトリが存在しないならディレクトリを作成する
		if( !ofs ) {
			FileUtility::Utility::CreateFolder( "Log" );
			ofs.open( "Log/" + path, std::ios::out | openMode );
		}

		// ファイルが開けたら処理を行う
		if( ofs ) {
			if( instance.m_timeStamp ) {
				// 日時を出力
				ofs << "========Start========\n";
				ofs << ( System::Date::YYYY_MM_SS() + " " + System::Date::HH_MM_SS() + "\n" );
				ofs << "=====================\n";
			}
			if (!instance.m_header.empty()) {
				// ヘッダーを出力
				ofs << (instance.m_header + "\n");
			}
			// 要素がなくなるまで出力
			for( auto i = 0; i < instance.m_logIndex; i++ ) {
				if( logTypeCond( instance.m_log[i].first.first ) && tagNameCond( instance.m_log[i].first.second ) ) {
					ofs << instance.m_log[i].second << std::endl;
				}
			}
			if (!instance.m_footer.empty()) {
				// フッターを出力
				ofs << instance.m_footer + "\n";
			}
			if( instance.m_timeStamp ) {
				// 日時を出力
				ofs << "=========End=========\n";
				ofs << ( System::Date::YYYY_MM_SS() + " " + System::Date::HH_MM_SS() + "\n" );
				ofs << "=====================\n";
			}
		}

		if (instance.m_multiThreadParse) {
			// セマフォをtrueに変更
			instance.m_semaphore.true_flag();
		}
	}

public:

	//===================================================
	// ログをテキストファイルに出力する
	//===================================================
	struct Parse {

		// 全てのログをテキストファイルに出力
		static void AllLog( std::string path = "DebugLog.txt", FileUtility::_OpenMode openMode = FileUtility::_ZeroBit ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseFunc.threadsafe_async(
					path,
					openMode,
					[](LogType logType) { return true; }
				);
			}
			else {
				Debug::_Internal_ParseForLogTypeIf(
					path,
					openMode
				);
			}
		}

		// 全てのログをテキストファイルの末尾に追記
		static void AppendAllLog( std::string path = "DebugLog.txt" ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseFunc.threadsafe_async(
					path,
					std::ios::app,
					[](LogType logType) { return true; }
				);
			}
			else {
				Debug::Parse::AllLog(
					path,
					std::ios::app
				);
			}
		}

		// 通常のログのみをテキストファイルに出力
		static void NormalLog( std::string path = "DengerLog.txt", FileUtility::_OpenMode openMode = FileUtility::_ZeroBit ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseFunc.threadsafe_async(
					path,
					openMode,
					[](LogType logType) { return logType == LogType::Log; }
				);
			}
			else {
				Debug::_Internal_ParseForLogTypeIf( 
					path,
					openMode,
					[]( LogType logType ) { return logType == LogType::Log; }
				);
			}
		}

		// 危険なログのみをテキストファイルに出力
		static void DengerLog( std::string path = "DengerLog.txt", FileUtility::_OpenMode openMode = FileUtility::_ZeroBit ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseFunc.threadsafe_async(
					path,
					openMode,
					[](LogType logType) { return logType != LogType::Log; }
				);
			}
			else {
				Debug::_Internal_ParseForLogTypeIf(
					path,
					openMode,
					[](LogType logType) { return logType != LogType::Log; }
				);
			}
		}

		// 弱警告のみをテキストファイルに出力
		static void WarningLog( std::string path = "WarningLog.txt", FileUtility::_OpenMode openMode = FileUtility::_ZeroBit ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseFunc.threadsafe_async(
					path,
					openMode,
					[](LogType logType) { return logType == LogType::Warning; }
				);
			}
			else {
				Debug::_Internal_ParseForLogTypeIf(
					path,
					openMode,
					[](LogType logType) { return logType == LogType::Warning; }
				);
			}
		}

		// 強警告のみをテキストファイルに出力
		static void AssertionLog( std::string path = "AssertionLog.txt", FileUtility::_OpenMode openMode = FileUtility::_ZeroBit ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseFunc.threadsafe_async(
					path,
					openMode,
					[](LogType logType) { return logType == LogType::Assertion; }
				);
			}
			else {
				Debug::_Internal_ParseForLogTypeIf(
					path,
					openMode,
					[](LogType logType) { return logType == LogType::Assertion; }
				);
			}
		}

		// エラーのみをテキストファイルに出力
		static void ErrorLog( std::string path = "ErrorLog.txt", FileUtility::_OpenMode openMode = FileUtility::_ZeroBit ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseFunc.threadsafe_async(
					path,
					openMode,
					[](LogType logType) { return logType == LogType::Error; }
				);
			}
			else {
				Debug::_Internal_ParseForLogTypeIf(
					path,
					openMode,
					[](LogType logType) { return logType == LogType::Error; }
				);
			}
		}

		// 例外のみをテキストファイルに出力
		static void ExceptionLog( std::string path = "ExceptionLog.txt", FileUtility::_OpenMode openMode = FileUtility::_ZeroBit ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseFunc.threadsafe_async(
					path,
					openMode,
					[](LogType logType) { return logType == LogType::Exception; }
				);
			}
			else {
				Debug::_Internal_ParseForLogTypeIf(
					path,
					openMode,
					[](LogType logType) { return logType == LogType::Exception; }
				);
			}
		}

	};

	//===================================================
	// 同一タグのログのみをテキストファイルに出力する
	//===================================================
	struct ParseForTag {

	private:

		// 内部的なパスの加工 ※ParseForTag内の関数でのみ使用する
		static std::string _Internal_ParseToFilePathCreater( const std::string& defaultPath, const std::string& tagName, const std::string path ) {
			std::string ret;

			if( path == defaultPath ) {
				// パスを自身で入力していない場合はタグ名と連結したパスに変更する
				ret = tagName + "_" + path;
			}
			else {
				// パスを自身で入力している場合は自己責任なので入力されたパスを使用する
				ret = path;
			};

			return ret;
		}

	public:

		// 同一タグの全てのログをテキストファイルに出力
		static void AllLog( std::string tagName, std::string path = "DebugLog.txt", FileUtility::_OpenMode openMode = FileUtility::_ZeroBit ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseForTagFunc.threadsafe_async(
					_Internal_ParseToFilePathCreater("DebugLog.txt", tagName, path),
					openMode,
					[](Debug::LogType logType) { return true; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
			else {
				Debug::_Internal_ParseForLogTypeIfAndTagNameIf(
					_Internal_ParseToFilePathCreater("DebugLog.txt", tagName, path),
					openMode,
					[](Debug::LogType logType) { return true; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
		}

		// 同一タグの全てのログをテキストファイルの末尾に追記
		static void AppendAllLog( std::string tagName, std::string path = "DebugLog.txt" ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseForTagFunc.threadsafe_async(
					_Internal_ParseToFilePathCreater("DebugLog.txt", tagName, path),
					std::ios::app,
					[](Debug::LogType logType) { return true; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
			else {
				Debug::ParseForTag::AllLog(
					tagName,
					path,
					std::ios::app
				);
			}
		}

		// 同一タグかつ、通常のログのみをテキストファイルに出力
		static void NormalLog( std::string tagName, std::string path = "NormalLog.txt", FileUtility::_OpenMode openMode = FileUtility::_ZeroBit ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseForTagFunc.threadsafe_async(
					_Internal_ParseToFilePathCreater("NormalLog.txt", tagName, path),
					openMode,
					[](Debug::LogType logType) { return logType == LogType::Log; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
			else {
				Debug::_Internal_ParseForLogTypeIfAndTagNameIf(
					_Internal_ParseToFilePathCreater("NormalLog.txt", tagName, path),
					openMode,
					[](LogType logType) { return logType == LogType::Log; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
		}

		// 同一タグかつ、危険なログのみをテキストファイルに出力
		static void DengerLog( std::string tagName, std::string path = "DengerLog.txt", FileUtility::_OpenMode openMode = FileUtility::_ZeroBit ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseForTagFunc.threadsafe_async(
					_Internal_ParseToFilePathCreater("DengerLog.txt", tagName, path),
					openMode,
					[](Debug::LogType logType) { return logType != LogType::Log; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
			else {
				Debug::_Internal_ParseForLogTypeIfAndTagNameIf(
					_Internal_ParseToFilePathCreater("DengerLog.txt", tagName, path),
					openMode,
					[](LogType logType) { return logType != LogType::Log; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
		}

		// 同一タグかつ、弱警告のみをテキストファイルに出力
		static void WarningLog( std::string tagName, std::string path = "WarningLog.txt", FileUtility::_OpenMode openMode = FileUtility::_ZeroBit ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseForTagFunc.threadsafe_async(
					_Internal_ParseToFilePathCreater("WarningLog.txt", tagName, path),
					openMode,
					[](Debug::LogType logType) { return logType == LogType::Warning; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
			else {
				Debug::_Internal_ParseForLogTypeIfAndTagNameIf(
					_Internal_ParseToFilePathCreater("WarningLog.txt", tagName, path),
					openMode,
					[](LogType logType) { return logType == LogType::Warning; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
		}

		// 同一タグかつ、強警告のみをテキストファイルに出力
		static void AssertionLog( std::string tagName, std::string path = "AssertionLog.txt", FileUtility::_OpenMode openMode = FileUtility::_ZeroBit ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseForTagFunc.threadsafe_async(
					_Internal_ParseToFilePathCreater("AssertionLog.txt", tagName, path),
					openMode,
					[](Debug::LogType logType) { return logType == LogType::Assertion; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
			else {
				Debug::_Internal_ParseForLogTypeIfAndTagNameIf(
					_Internal_ParseToFilePathCreater("AssertionLog.txt", tagName, path),
					openMode,
					[](LogType logType) { return logType == LogType::Assertion; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
		}

		// 同一タグかつ、エラーのみをテキストファイルに出力
		static void ErrorLog( std::string tagName, std::string path = "ErrorLog.txt", FileUtility::_OpenMode openMode = FileUtility::_ZeroBit ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseForTagFunc.threadsafe_async(
					_Internal_ParseToFilePathCreater("ErrorLog.txt", tagName, path),
					openMode,
					[](Debug::LogType logType) { return logType == LogType::Error; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
			else {
				Debug::_Internal_ParseForLogTypeIfAndTagNameIf(
					_Internal_ParseToFilePathCreater("ErrorLog.txt", tagName, path),
					openMode,
					[](LogType logType) { return logType == LogType::Error; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
		}

		// 同一タグかつ、例外のみをテキストファイルに出力
		static void ExceptionLog( std::string tagName, std::string path = "ExceptionLog.txt", FileUtility::_OpenMode openMode = FileUtility::_ZeroBit ) {
			auto& instance = Debug::GetInstance();
			if (instance.m_multiThreadParse) {
				instance.m_parseForTagFunc.threadsafe_async(
					_Internal_ParseToFilePathCreater("ExceptionLog.txt", tagName, path),
					openMode,
					[](Debug::LogType logType) { return logType == LogType::Exception; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
			else {
				Debug::_Internal_ParseForLogTypeIfAndTagNameIf(
					_Internal_ParseToFilePathCreater("ExceptionLog.txt", tagName, path),
					openMode,
					[](LogType logType) { return logType == LogType::Exception; },
					[&](std::string _tagName) { return _tagName == tagName; }
				);
			}
		}

	};

#pragma endregion Parse

#pragma region LogDestroy

private:

	static void _Internal_LogDestroyLogTypeIf(
		std::function<bool( LogType )> logTypeCond = []( LogType logType ) { return true; }
	) {
		auto& instance = GetInstance();

		for( auto i = 0; i < instance.m_logIndex; i++ ) {
			if( logTypeCond( instance.m_log[i].first.first ) ) {
				instance.m_log.erase( std::next( instance.m_log.begin(), i ) );
				--instance.m_logIndex;
				--i;
			}
		}
	}

	static void _Internal_LogDestroyLogTypeIfAndTagNameIf(
		std::function<bool( LogType )> logTypeCond = []( LogType logType ) { return true; },
		std::function<bool( std::string )> tagNameCond = []( std::string tagName ) { return true; }
	) {
		auto& instance = GetInstance();

		for( auto i = 0; i < instance.m_logIndex; i++ ) {
			if( logTypeCond( instance.m_log[i].first.first ) && tagNameCond( instance.m_log[i].first.second ) ) {
				instance.m_log.erase( std::next( instance.m_log.begin(), i ) );
				--instance.m_logIndex;
				--i;
			}
		}
	}

public:


	struct LogDestroy {


		// 全てのログを削除
		static void AllLog() {
			Debug::_Internal_LogDestroyLogTypeIf();
		}

		// 通常のログのみを削除
		static void NormalLog() {
			Debug::_Internal_LogDestroyLogTypeIf( []( LogType logType ) { return logType == LogType::Log; } );
		}

		// 危険なログのみを削除
		static void DengerLog() {
			Debug::_Internal_LogDestroyLogTypeIf( []( LogType logType ) { return logType != LogType::Log; } );
		}

		// 弱警告のみを削除
		static void WarningLog() {
			Debug::_Internal_LogDestroyLogTypeIf( []( LogType logType ) { return logType == LogType::Warning; } );
		}

		// 強警告のみを削除
		static void AssertionLog() {
			Debug::_Internal_LogDestroyLogTypeIf( []( LogType logType ) { return logType == LogType::Assertion; } );
		}

		// エラーのみを削除
		static void ErrorLog() {
			Debug::_Internal_LogDestroyLogTypeIf( []( LogType logType ) { return logType == LogType::Error; } );
		}

		// 例外のみを削除
		static void ExceptionLog() {
			Debug::_Internal_LogDestroyLogTypeIf( []( LogType logType ) { return logType == LogType::Exception; } );
		}


	};

	struct LogDestroyForTag {

		// 同一タグの全てのログを削除
		static void AllLog( std::string tagName) {
			Debug::_Internal_LogDestroyLogTypeIfAndTagNameIf(
				[]( Debug::LogType logType ) { return true; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、通常のログのみを削除
		static void NormalLog( std::string tagName) {
			Debug::_Internal_LogDestroyLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Log; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、危険なログのみを削除
		static void DengerLog( std::string tagName) {
			Debug::_Internal_LogDestroyLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType != LogType::Log; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; } 
			);
		}

		// 同一タグかつ、弱警告のみを削除
		static void WarningLog( std::string tagName) {
			Debug::_Internal_LogDestroyLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Warning; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、強警告のみを削除
		static void AssertionLog( std::string tagName) {
			Debug::_Internal_LogDestroyLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Assertion; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、エラーのみを削除
		static void ErrorLog( std::string tagName) {
			Debug::_Internal_LogDestroyLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Error; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// 同一タグかつ、例外のみを削除
		static void ExceptionLog( std::string tagName) {
			Debug::_Internal_LogDestroyLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Exception; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

	};

#pragma endregion LogDestroy

#pragma region OnceLog

private:

	static bool IsOnce() {
		auto& instance = GetInstance();

		for (ulonglong_t i = 0; i < instance.m_onceLogIndex; i++) {
			if (instance.m_onceLog[i].first.first == instance.m_logType) {
				if (instance.m_onceLog[i].first.second == instance.m_logTag) {
					if (instance.m_onceLog[i].second == instance.m_buf) {
						return true;
					}
				}
			}
		}

		return false;
	}

public:

	struct Once {

		template<class ...Args>
		static void Log(Args&&... args) {
			auto& instance = GetInstance();

			// ログ文字列を生成
			Debug::_Internal_CreateLogString(args...);

			// 同一のログが存在するかどうか判定する
			auto isOnce = Debug::IsOnce();

			if (!instance.m_logProcessSkip && !isOnce) {
				instance.m_onceLog[instance.m_onceLogIndex].first.first = instance.m_logType;
				instance.m_onceLog[instance.m_onceLogIndex].first.second = instance.m_logTag;
				instance.m_onceLog[instance.m_onceLogIndex].second = instance.m_buf;
				Debug::_Internal_Log();
				instance.m_onceLogIndex++;
			}

			if (instance.m_onceLog.size() == instance.m_onceLogIndex) {
				instance.m_onceLog.resize(instance.m_onceLog.size() + 100);
			}

			Debug::_Intenal_ClearTemporaryBuffer();
		}

		template<class ...Args>
		static void LogWarning(Args... args) {
			Debug::_Intenal_SetLogType(LogType::Warning);
			Debug::Once::Log(
				"===S_Warning===\n",
				"Warning : ", args...,
				"\n===E_Warning==="
			);
			Debug::_Intenal_SetLogType(LogType::Log);
			Debug::Parse::AllLog();
		}

		template<class ...Args>
		static void LogAssertion(Args... args) {
			Debug::_Intenal_SetLogType(LogType::Assertion);
			Debug::Once::Log(
				"===S_Assertion===\n",
				"Assertion : ", args...,
				"\n===E_Assertion==="
			);
			Debug::_Intenal_SetLogType(LogType::Log);
			Debug::Parse::AllLog();

#if _DEBUG
		// _USE_WINAPIを定義した場合のみ
#if defined(WINAPI) && defined(_USE_WINAPI)
			if (Debug::StopProcess()) {
				auto& instance = GetInstance();
				MessageBox(nullptr, instance.m_log[instance.m_logIndex].c_str(), "Assertion", MB_OK);
				Assert("Assertion");
			}
#else
			if (Debug::_Intenal_StopProcess()) {
				Assert("Assertion");
			}
#endif
#endif
		}

		template<class ...Args>
		static void LogError(Args... args) {
			Debug::_Intenal_SetLogType(LogType::Error);
			Debug::Once::Log(
				"===S_Error===\n",
				"Error : ", args...,
				"\n===E_Error==="
			);
			Debug::_Intenal_SetLogType(LogType::Log);
			Debug::Parse::AllLog();

#if _DEBUG
		// _USE_WINAPIを定義した場合のみ
#if defined(WINAPI) && defined(_USE_WINAPI)
			if (Debug::StopProcess()) {
				auto& instance = GetInstance();
				MessageBox(nullptr, instance.m_log[instance.m_logIndex].c_str(), "Error", MB_OK);
				Assert("Error");
			}
#else
			if (Debug::_Intenal_StopProcess()) {
				Assert("Error");
			}
#endif
#endif
		}

		template<class ...Args>
		static void LogException(Args... args) {
			Debug::_Intenal_SetLogType(LogType::Exception);
			Debug::Once::Log(
				"===S_Exception===\n",
				"Exception : ", args...,
				"\n===E_Exception==="
			);
			Debug::_Intenal_SetLogType(LogType::Log);
			Debug::Parse::AllLog();

#if _DEBUG
		// _USE_WINAPIを定義した場合のみ
#if defined(WINAPI) && defined(_USE_WINAPI)
			if (Debug::StopProcess()) {
				auto& instance = GetInstance();
				MessageBox(nullptr, instance.m_log[instance.m_logIndex].c_str(), "Exception", MB_OK);
				Assert("Exception");
			}
#else
			if (Debug::_Intenal_StopProcess()) {
				Assert("Exception");
			}
#endif
#endif
		}

	};

#pragma endregion OnceLog

};



// ログにchar型を使用した場合、数値が表示される



