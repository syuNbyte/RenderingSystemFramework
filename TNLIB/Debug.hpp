#pragma once

#include<iostream>
#include<sstream>
#include<utility>
#include<vector>
#include<string>
#include<fstream>
#include<cassert>
#include<crtdbg.h>	// �o�̓E�B���h�E�ɕ\������}�N���p
#include<type_traits>

#include"Math.hpp"
#include"Utility.hpp"
#include"FileUtility.hpp"
#include"System.hpp"
#include"Semaphore.hpp"
#include"AsyncFunction.hpp"


//===================================================
// �o�̓E�B���h�E�ɕ\��
//===================================================
#define DBG_MSG(msg) _RPT0(_CRT_WARN, msg)

//======================================================================================================
// <cassert>����assert�}�N������false�̎w�肪�߂�ǂ��������A�\������G���[�̓��e���w��ł��Ȃ��̂ł��̃}�N�����K�v
//======================================================================================================
#define Assert(message)																							\
            (_wassert(_CRT_WIDE(#message), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0)	\



// �f�o�b�O�֘A�̏���񋟂���
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

	std::vector<std::pair<std::pair<LogType, std::string>, std::string>> m_log;		// ���O�̕ۑ���<<���O�^�C�v, ���O�̃^�O>, ���ۂ̕�����>
	std::string m_buf;																					// �ꎞ�I�Ɏg�p�����o�b�t�@
	ulonglong_t m_logIndex;																				// ���O�̕ۑ���C���f�b�N�X�ԍ�
	LogType m_logType;																					// ���ݐݒ肳��Ă��郍�O�̃^�C�v
	std::string m_logTag;																				// ���ݐݒ肳��Ă��郍�O�̃^�O
	std::string m_bracketLogTag;																		// �^�O�t���g�������͈͂̃^�O�����Ǘ����邽�߂̃e���|����
	ulonglong_t m_onceLogIndex;																		// ���ꃍ�O�̏o�͂���x�݂̂ɐ������郁�\�b�h
	std::vector<std::pair<std::pair<LogType, std::string>, std::string>> m_onceLog;	// ���ꃍ�O�̏o�͂���x�݂̂ɐ������郁�\�b�h�g�p���̃��O��ۑ������Ƃ���Q�Ƃ��邽�߂̃o�b�t�@
	std::string m_prefix;																				// �o�͂��镶����S�Ăɕt�^�����ړ���
	std::string m_suffix;																				// �o�͂��镶����S�Ăɕt�^�����ڔ���
	std::string m_tab;																					// �o�͂��镶����S�Ăɕt�^�����^�u����
	std::string m_bracketTab;																			// BracketBegin, BracketEnd���\�b�h�͈͓̔��ɂ̕�����S�Ăɕt�^�����^�u����
	std::string m_header;																				// �t�@�C���Ƀp�[�X����ۂ̃w�b�_�[
	std::string m_footer;																				// �t�@�C���Ƀp�[�X����ۂ̃t�b�^�[

	// ���O�̏�����S�ăX�L�b�v����
	bool m_logProcessSkip;

	// ���O�̏����n�߂��ǂ����𔻒肷������I�ȃt���O
	bool m_logInit;

	// ���O�̃Z�[�u���s�����ǂ����̃t���O
	bool m_logSave;

	// ���O���Z�[�u����ۂɃ^�C���X�^���v���������ǂ����̃t���O
	bool m_timeStamp;

	// ���O�o�͎���bool�^�p�����[�^��true, false�ŏo�͂����邩�ǂ����̃t���O
	bool m_boolalpha;

	// ���������_�������O�o�͂���ۂ̃p�����[�^
	std::chars_format m_floatViewType;	// ���������_�^�̕�����ւ̕ϊ��`��
	int_t m_floatPrecision;					// ���������_�^�𕶎���֕ϊ�����ۂ̕ϊ����x

	// �댯�x�������G���[�̏ꍇ�ɒ�~���邩�ǂ����̃t���O
	// ���댯�x�������G���[�FAssertion, Error, Exception
	bool m_stopIfHighDangerousness;

	// �����[�X���[�h���ɃR���\�[���E�B���h�E�ւ̕\�������邩�ǂ����̃t���O
	// ���\���͂���Ȃ��Ă����O�Ƃ��ĕۑ��͂���܂�
	bool m_dispOnReleaseMode;

	// ���O�̃R���\�[���E�B���h�E�ւ̕\�����s�����ǂ����̃t���O
	bool m_dispOnConsoleWindow;

	// ���O�̔��s����VisualStudio�̏o�̓E�B���h�E�Ƀ��O��\�����邩�ǂ����̃t���O
	// �������[�X���ɂ͕\������܂���i�����VisualStudio���̎d�l�ł��j
	bool m_dispOnOutputWindow;

	//==================================================
	// �p�[�X�������}���`�X���b�h�����邽�߂̃p�����[�^
	//==================================================
	// �}���`�X���b�h�������s�����𔻒肷�邽�߂̃t���O
	bool m_multiThreadParse;
	// �o�C�i���Z�}�t�H
	binary_semaphore m_semaphore;
	// �p�[�X�������s���֐�
	async_function<void(std::string, FileUtility::_OpenMode, std::function<bool(LogType)>)> m_parseFunc;
	// �^�O�ʂŃp�[�X�������s���֐�
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

	// �^�O�t���g�������͈͂��g�p����ۂɍ쐬���������I�ȊǗ��p�̃^�O���쐬����
	static void _Internal_CreateBracketLogTag(std::string tagName) {
		auto& instance = GetInstance();
		if (instance.m_bracketLogTag.empty()) {
			instance.m_bracketLogTag = tagName;
		}
		else {
			instance.m_bracketLogTag += "_" + tagName;
		}
	}

	// �^�O�t���̔g�������͈͂��g�p����ۂ�
	// �g�������͈͂��Ƃ̃^�O���Ǘ����Ă��������œK�؂ȃ^�O�ɐ؂�ւ��邽�߂̏���
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
	// ���O�̏o��
	//===================================================

	// ���O�Ɏg�p���镶������쐬���I������
	static void _Internal_CreateLogString() {
		auto& instance = GetInstance();

		// ���O�̃t���O���o�̓t���O������
		instance.m_logInit = true;

		// ���s������̑O�̕����Ƀ^�u������ǉ�����
		auto count = 0;
		auto size = instance.m_buf.size();

		// ���s������̐����J�E���g
		for (int_t i = 0; i < size; i++) {
			if (instance.m_buf[i] == '\n') ++count;
		}

		// ���s������ * ���݂̃^�u������ + ���̕����� = �V�����o�b�t�@���̕�����
		auto tabCount = instance.m_tab.size() + instance.m_bracketTab.size();	// �^�u�����̐�
		auto appeaNewLine = 0;																	// ���[�v���̉��s�����̏o����
		auto tabSize = count * tabCount;														// �ǉ������^�u������̐�
		auto preBufSize = instance.m_buf.size();											// ���̃o�b�t�@�T�C�Y
		std::string tmp;																			// �e���|����
		tmp.resize(preBufSize + tabSize);													// �e���|�����̕�������V�����o�b�t�@�T�C�Y�ɐݒ�


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

		// �č\��������������C���X�^���X�̃o�b�t�@�ɐݒ�
		instance.m_buf = std::move(tmp);
	}

	// ���O�Ɏg�p���镶������쐬����
	template<class Head, class ...Tail>
	static void _Internal_CreateLogString(Head&& head, Tail&&... tail) {
		auto& instance = GetInstance();

		if (instance.m_multiThreadParse) {
			// �}���`�X���b�h�Ή��p�̃o�C�i���Z�}�t�H
			instance.m_semaphore.wait_true();
		}

		// ������X�g���[���ɏo��
		std::stringstream ss;
		std::string str;

		if (Math::template IsFloat<Head>(head)) {
			ss << Utility::ToStringFloat(head, instance.m_floatViewType, instance.m_floatPrecision);
			str = ss.str();
		}
		else if (instance.m_boolalpha && TypeTraitsEx::IsBool<Head>) {
			// �f�[�^�^��bool�̏ꍇ�̏���
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

		// ��x�̃��O�o�͒��ɍŏ������ʂ鏈��
		if (instance.m_logInit) {
			instance.m_logInit = false;
			// ������̐擪�ɐړ�����ǉ�
			str = instance.m_bracketTab + instance.m_tab + instance.m_prefix + str;
		}

		if (sizeof...(tail) == 0) {
			// ������̖����ɐڔ�����ǉ�
			str = str + instance.m_suffix;
		}

		// ����������O������̃o�b�t�@�̖����ɒǉ�
		instance.m_buf.append(str);

		// �p�����[�^�p�b�Ntail�������head��tail�ɕ�������
		Debug::_Internal_CreateLogString(std::forward<Tail>(tail)...);
	}

	// �f�o�b�O�̃��O�̕\�����I������
	static void _Internal_Log() {

		auto& instance = GetInstance();

#if defined(_DEBUG)
		// �Ō�ɉ��s���ďI��
		if( instance.m_dispOnConsoleWindow )std::cout << instance.m_buf << std::endl;
		if( instance.m_dispOnOutputWindow )DBG_MSG(instance.m_buf.c_str());
		if( instance.m_dispOnOutputWindow )DBG_MSG("\n");

#elif !defined(_DEBUG)
		// �����[�X���Ƀ��O�̕\�����s����
		if( instance.m_dispOnReleaseMode ) {
			std::cout << instance.m_buf << std::endl;
		}
#endif

		if (instance.m_logSave) {
			// ���O���e��ۑ�����
			instance.m_log[instance.m_logIndex].first.first = instance.m_logType;	// ���O�̃^�C�v
			instance.m_log[instance.m_logIndex].first.second = instance.m_logTag;	// ���O�̃^�O
			instance.m_log[instance.m_logIndex].second = instance.m_buf;				// ���O�̕�����

			// ���O�̕ۑ���C���f�b�N�X��O��
			instance.m_logIndex++;
		}

		// ���O�̃t���O���o�̓t���O������
		instance.m_logInit = true;
		

		// ���O�̕ۑ���̗e�ʂ��Ȃ���Βǉ�
		if( instance.m_log.size() == instance.m_logIndex ) {
			instance.m_log.resize( instance.m_log.size() + 100 );
		}

	}

	// �f�o�b�O�̃��O��\������
	template<class ...Args>
	static void _Internal_Log( Args&&... args ) {
		Debug::_Internal_CreateLogString(args...);
		Debug::_Internal_Log();
		Debug::_Intenal_ClearTemporaryBuffer();
	}

public:

	// �f�o�b�O�̃��O��\������
	template<class ...Args>
	static void Log( Args... args ) {
		auto& instance = GetInstance();
		if( !instance.m_logProcessSkip ) {
			Debug::_Internal_Log( args... );
		}
	}

	// ��s��n��}������
	static void Space(int_t n = 1) {
		std::string space;
		for( int_t i = 1; i < n; i++ ) {
			space.append( "\n" );
		}
		Debug::Log( space );
	}

	// �����������n�}������
	// n�F��
	// separateChar�Fn��}������镶����
	// prefix�F����������̈�ԑO�ɑ}������镶����
	// suffix�F����������̈�Ԍ�ɑ}������镶����
	// center�F����������̒��S�ɑ}������镶����
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

	// �w�蕶����str�𐂒�������n�񃋁[�v�}������
	static void VerticalLoop( int_t n, std::string str ) {
		for( int_t i = 0; i < n; i++ ) {
			Debug::Log( str );
		}
	}

	// �w�蕶����str�𐅕�������n�񃋁[�v�}������
	static void HorizontalLoop( int_t n, std::string str ) {
		std::string tmp;
		for( int_t i = 0; i < n; i++ ) {
			tmp.append( str );
		}
		Debug::Log( tmp );
	}

	// �R���\�[���E�B���h�E��Clear�R�}���h�𑗂�
	// ���O���̂�Clear����Ȃ�
	static void ClearConsole() {
		std::system( "cls" );
	}

	//===================================================
	// �G���[���̃��O�̏o��
	//===================================================

	// �f�o�b�O���O���o�͂���x�����s��
	// �����̍ہuDebugLog.txt�v�t�@�C���Ɏ����I�ɍ��܂ł̃��O���ۑ������
	template <class ...Args>
	static void LogWarning( Args... args ) {
		Debug::_Intenal_SetLogType( LogType::Warning );
		Debug::BracketBegin("WarningLog");
		Debug::Log(args...);
		Debug::BracketEnd();
		Debug::_Intenal_SetLogType( LogType::Log );
		Debug::Parse::AllLog();
	}

	// �f�o�b�O���O���o�͂����x�����s��
	// �����̍ہuDebugLog.txt�v�t�@�C���Ɏ����I�ɍ��܂ł̃��O���ۑ������
	template <class ...Args>
	static void LogAssertion( Args... args ) {
		Debug::_Intenal_SetLogType( LogType::Assertion );
		Debug::BracketBegin("AssertionLog");
		Debug::Log(args...);
		Debug::BracketEnd();
		Debug::_Intenal_SetLogType( LogType::Log );
		Debug::Parse::AllLog();

#if _DEBUG
		// _USE_WINAPI���`�����ꍇ�̂�
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

	// �f�o�b�O���O���o�͂��G���[�o�͂��s��
	// �����̍ہuDebugLog.txt�v�t�@�C���Ɏ����I�ɍ��܂ł̃��O���ۑ������
	template <class ...Args>
	static void LogError( Args... args ) {
		Debug::_Intenal_SetLogType( LogType::Error );
		Debug::BracketBegin("ErrorLog");
		Debug::Log(args...);
		Debug::BracketEnd();
		Debug::_Intenal_SetLogType( LogType::Log );
		Debug::Parse::AllLog();

#if _DEBUG
		// _USE_WINAPI���`�����ꍇ�̂�
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

	// �f�o�b�O���O���o�͂���O�o�͂��s��
	// �����̍ہuDebugLog.txt�v�t�@�C���Ɏ����I�ɍ��܂ł̃��O���ۑ������
	template <class ...Args>
	static void LogException( Args... args ) {
		Debug::_Intenal_SetLogType( LogType::Exception );
		Debug::BracketBegin("ExceptionLog");
		Debug::Log(args...);
		Debug::BracketEnd();
		Debug::_Intenal_SetLogType( LogType::Log );
		Debug::Parse::AllLog();

#if _DEBUG
		// _USE_WINAPI���`�����ꍇ�̂�
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

	// ���O�̏������s�����ǂ�����ݒ肷��
	static void SetLogSkipFlag( bool flag ) {
		GetInstance().m_logProcessSkip = flag;
	}

	// ���O�̕ۑ������s������ݒ肷��
	static void SetLogSaveFlag( bool flag ) {
		GetInstance().m_logSave = flag;
	}

	// ���O�̕ۑ����Ƀ^�C���X�^���v���������ǂ�����ݒ肷��
	static void SetTimeStampFlag( bool flag ) {
		GetInstance().m_timeStamp = flag;
	}

	// ���O�o�͎���bool�^�p�����[�^��true, false�ŏo�͂����邩�ǂ����̃t���O
	static void SetBoolAlphaFlag( bool flag ) {
		GetInstance().m_boolalpha = flag;
	}

	// ���O�ɕ��������_����\������ۂ̕ϊ��t���O��ݒ肷��
	static void SetFloatViewType( std::chars_format floatViewType, int_t floatPrecision ) {
		auto& instance = GetInstance();
		instance.m_floatViewType = floatViewType;
		instance.m_floatPrecision = floatPrecision;
	}

	// ���̊֐����s�ȍ~�ɔ��s�����S�Ẵ��O�Ƀf�t�H���g�̃^�O��ݒ�
	static void SetDefaultLogTag() {
		GetInstance().m_logTag = "Default";
	}

	// ���̊֐����s�ȍ~�ɔ��s�����S�Ẵ��O�Ƀ^�O��ݒ�
	static void SetLogTag( std::string tagName ) {
		GetInstance().m_logTag = tagName;
	}

	// ���ݐݒ肳��Ă���ړ������폜����
	static void ResetPrefix() {
		GetInstance().m_prefix = "";
	}

	// ���̊֐����s�ȍ~�ɔ��s�����S�Ẵ��O�ɐړ�����ݒ肷��
	static void SetPrefix( std::string prefixName ) {
		GetInstance().m_prefix = prefixName;
	}

	// ���ݐݒ肳��Ă���ڔ������폜����
	static void ResetSuffix() {
		GetInstance().m_suffix = "";
	}

	// ���̊֐����s�ȍ~�ɔ��s�����S�Ẵ��O�ɐڔ�����ݒ肷��
	static void SetSuffix( std::string suffixName ) {
		GetInstance().m_suffix = suffixName;
	}

	// ���ݐݒ肳��Ă���^�u���폜����
	static void ResetTab() {
		GetInstance().m_tab = "";
	}

	// ���̊֐����s�ȍ~�ɔ��s�����S�Ẵ��O�Ƀ^�u�������ݒ肷��
	static void SetTab( int_t n = 1 ) {
		std::string tab;
		for( int_t i = 0; i < n; i++ ) tab.append( "\t" );
		GetInstance().m_tab = tab;
	}

	// ���ݐݒ肳��Ă���^�u������Ƀ^�u������ǉ�����
	static void AddTab( int_t n = 1 ) {
		auto& instance = GetInstance();
		for( int_t i = 0; i < n; i++ ) instance.m_tab.append( "\t" );
	}

	// ���ݐݒ肳��Ă���^�u�����񂩂�^�u�������폜����
	static void RemoveTab( int_t n = 1 ) {
		auto& instance = GetInstance();
		for( int_t i = 0; i < n; i++ ) instance.m_tab.erase(instance.m_tab.begin());
	}

	// �g�������͈̔͂��J�n����
	static void BracketBegin() {
		auto& instance = GetInstance();
		Debug::Log( "{" );
		instance.m_bracketTab.append( "\t" );
	}

	// �g�������̐擪�Ƀw�b�_�[��t���Ă���g�������͈̔͂��J�n����
	static void BracketBegin(std::string header) {
		auto& instance = GetInstance();
		Debug::SetPrefix(header);
		Debug::Log("{");
		Debug::ResetPrefix();
		instance.m_bracketTab.append("\t");
	}

	// �g�������͈̔͂��J�n���A�͈͓��Ƀ^�O��ݒ肷��
	static void TagBracketBegin(std::string tagName) {
		Debug::SetLogTag(tagName);
		Debug::_Internal_CreateBracketLogTag(tagName);
		Debug::BracketBegin();
	}

	// �g�������̐擪�Ƀw�b�_�[��t���Ă���g�������͈̔͂��J�n���A�͈͓��Ƀ^�O��ݒ肷��
	static void TagBracketBegin(std::string tagName, std::string header) {
		Debug::SetLogTag(tagName);
		Debug::_Internal_CreateBracketLogTag(tagName);
		Debug::BracketBegin(header);
	}

	// �g�������͈̔͂��I������
	static void BracketEnd() {
		auto& instance = GetInstance();
		auto empty = instance.m_bracketTab.empty();
		instance.m_bracketTab.erase(instance.m_bracketTab.begin());
		if(!empty) Debug::Log( "}" );
	}

	// �g�������͈̔͂��I�����A�g�������̖����Ƀt�b�^�[��t����
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

	// �^�O��ݒ肳�ꂽ�g�������͈̔͂��I������
	static void TagBracketEnd() {
		Debug::BracketEnd();
		Debug::_Internal_BracketLogTag();
	}

	// �^�O��ݒ肳�ꂽ�g�������͈̔͂��I�����A�g�������̖����Ƀt�b�^�[��t����
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

	// �댯�x�������G���[�̏ꍇ�ɒ�~���邩�ǂ�����ݒ肷��
	// ���댯�x�������G���[�FAssertion, Error, Exception
	static void SetDengerLogTimeStopFlag( bool flag ) {
		GetInstance().m_stopIfHighDangerousness = flag;
	}

	// �����[�X���[�h���Ƀ��O��\�����邩�ǂ�����ݒ肷��
	// ���\���͂���Ȃ��Ă����O�Ƃ��ĕۑ��͂���܂�
	static void SetDispOnRelease( bool flag ) {
		GetInstance().m_dispOnReleaseMode = flag;
	}

	// ���O�̔��s���ɃR���\�[���E�B���h�E�ɕ\�����邩�ǂ�����ݒ肷��
	static void SetDispOnConsoleWindow(bool flag) {
		GetInstance().m_dispOnConsoleWindow = flag;
	}

	// ���O�̔��s����VisualStudio�̏o�̓E�B���h�E�ɕ\�����邩�ǂ�����ݒ肷��
	// �������[�X���ɂ͕\������܂���
	static void SetDispOnOutputWindow( bool flag ) {
		GetInstance().m_dispOnOutputWindow = flag;
	}

	// �p�[�X�������Ƀ}���`�X���b�h�ɂăp�[�X�������s�����ǂ�����ݒ肷��
	static void SetMultiThreadParse(bool flag) {
		GetInstance().m_multiThreadParse = flag;
	}

	//===================================================
	// ����ȃ��O�̏o��
	//===================================================
	// �����̃��\�b�h��C++�Ƀf�t�H���g�Ŏ��������}�N�����g�p������̂�
	// �����̃}�N���̓t�@�C�����ƁE�֐����ƁE�s���ƂɃ}�N���̓��e���قȂ邽�߈����Ƃ��ēn������K�v������

	// ���̊֐��͈����Ɂu__FILE__�v�}�N����n�����ƁI
	static void LogFilePath(std::string path) {
		Debug::Log("FilePath : " + path);
	}

	// ���̊֐��͈����Ɂu__func__�v�}�N����n�����ƁI�@��__func__�͎��ۂɂ̓}�N���ł͂Ȃ�
	static void LogFunction(std::string funcName) {
		Debug::Log("Function : " + funcName);
	}

	// ���̊֐��͈����Ɂu__LINE__�v�}�N����n�����ƁI
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

		// �S�Ẵ��O���擾����
		static std::vector<std::shared_ptr<std::string>> AllLog() {
			return Debug::_Internal_GetStrPtrLogTypeIf();
		}

		// �ʏ�̃��O���擾����
		static std::vector<std::shared_ptr<std::string>> NormalLog() {
			return Debug::_Internal_GetStrPtrLogTypeIf( []( LogType logType ) { return logType == LogType::Log; } );
		}

		// �댯�ȃ��O���擾����
		static std::vector<std::shared_ptr<std::string>> DengerLog() {
			return Debug::_Internal_GetStrPtrLogTypeIf( []( LogType logType ) { return logType != LogType::Log; } );
		}

		// ��x���̃��O���擾����
		static std::vector<std::shared_ptr<std::string>> WarningLog() {
			return Debug::_Internal_GetStrPtrLogTypeIf( []( LogType logType ) { return logType == LogType::Warning; } );
		}

		// ���x���̃��O���擾����
		static std::vector<std::shared_ptr<std::string>> AssertionLog() {
			return Debug::_Internal_GetStrPtrLogTypeIf( []( LogType logType ) { return logType == LogType::Assertion; } );
		}

		// �G���[�̃��O���擾����
		static std::vector<std::shared_ptr<std::string>> ErrorLog() {
			return Debug::_Internal_GetStrPtrLogTypeIf( []( LogType logType ) { return logType == LogType::Error; } );
		}

		// ��O�̃��O���擾����
		static std::vector<std::shared_ptr<std::string>> ExceptionLog() {
			return Debug::_Internal_GetStrPtrLogTypeIf( []( LogType logType ) { return logType == LogType::Exception; } );
		}

	};

	struct StackTraceTag {

		// ����^�O�̑S�Ẵ��O���擾����
		static std::vector<std::shared_ptr<std::string>> AllLog( std::string tagName ) {
			return Debug::_Internal_GetStrPtrLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return true; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A�ʏ�̃��O���擾����
		static std::vector<std::shared_ptr<std::string>> NormalLog( std::string tagName ) {
			return Debug::_Internal_GetStrPtrLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType == LogType::Log; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A�댯�ȃ��O���擾����
		static std::vector<std::shared_ptr<std::string>> DengerLog( std::string tagName ) {
			return Debug::_Internal_GetStrPtrLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType != LogType::Log; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A��x���̃��O���擾����
		static std::vector<std::shared_ptr<std::string>> WarningLog( std::string tagName ) {
			return Debug::_Internal_GetStrPtrLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType == LogType::Warning; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A���x���̃��O���擾����
		static std::vector<std::shared_ptr<std::string>> AssertionLog( std::string tagName ) {
			return Debug::_Internal_GetStrPtrLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType == LogType::Assertion; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A�G���[�̃��O���擾����
		static std::vector<std::shared_ptr<std::string>> ErrorLog( std::string tagName ) {
			return Debug::_Internal_GetStrPtrLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType == LogType::Error; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A��O�̃��O���擾����
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

		// �v�f���Ȃ��Ȃ�܂ŏo��
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

		// �v�f���Ȃ��Ȃ�܂ŏo��
		for( auto i = 0; i < instance.m_logIndex; i++ ) {
			if( logTypeCond( instance.m_log[i].first.first ) && tagNameCond( instance.m_log[i].first.second ) ) {
				++count;
			}
		}

		return count;
	}

public:

	struct Count {

		// �S�Ẵ��O�̐����擾����
		static ulonglong_t AllLog() {
			return Debug::GetInstance().m_log.size();
		}

		// �ʏ�̃��O�̐����擾����
		static ulonglong_t NormalLog() {
			return Debug::_Internal_CountLogTypeIf( []( LogType logType ) { return logType == LogType::Log; } );
		}

		// �댯�ȃ��O�̐����擾����
		static ulonglong_t DengerLog() {
			return Debug::_Internal_CountLogTypeIf( []( LogType logType ) { return logType != LogType::Log; } );
		}

		// ��x���̃��O�̐����擾����
		static ulonglong_t WarningLog() {
			return Debug::_Internal_CountLogTypeIf( []( LogType logType ) { return logType == LogType::Warning; } );
		}

		// ���x���̃��O�̐����擾����
		static ulonglong_t AssertionLog() {
			return Debug::_Internal_CountLogTypeIf( []( LogType logType ) { return logType == LogType::Assertion; } );
		}

		// �G���[�̃��O�̐����擾����
		static ulonglong_t ErrorLog() {
			return Debug::_Internal_CountLogTypeIf( []( LogType logType ) { return logType == LogType::Error; } );
		}

		// ��O�̃��O�̐����擾����
		static ulonglong_t ExceptionLog() {
			return Debug::_Internal_CountLogTypeIf( []( LogType logType ) { return logType == LogType::Exception; } );
		}

	};

	struct CountForTag {

		// ����^�O���A�S�Ẵ��O�̐����擾����
		static ulonglong_t AllLog( std::string tagName ) {
			return Debug::_Internal_CountLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return true; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A�ʏ�̃��O�̐����擾����
		static ulonglong_t NormalLog( std::string tagName ) {
			return Debug::_Internal_CountLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType == LogType::Log; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A�댯�ȃ��O�̐����擾����
		static ulonglong_t DengerLog( std::string tagName ) {
			return Debug::_Internal_CountLogTypeIfAndTagNameIf(
				[]( LogType logType ) { return logType != LogType::Log; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A��x���̃��O�̐����擾����
		static ulonglong_t WarningLog( std::string tagName ) {
			return Debug::_Internal_CountLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Warning; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A���x���̃��O�̐����擾����
		static ulonglong_t AssertionLog( std::string tagName ) {
			return Debug::_Internal_CountLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Assertion; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A�G���[�̃��O�̐����擾����
		static ulonglong_t ErrorLog( std::string tagName ) {
			return Debug::_Internal_CountLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Error; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A��O�̃��O�̐����擾����
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
			// �Z�}�t�H�̃t���O��false�Ȃ�true�ɂȂ�܂ő҂�
			if(instance.m_semaphore == false) instance.m_semaphore.wait_true();

			// �Z�}�t�H��false�ɕύX
			instance.m_semaphore.false_flag();
		}

		// �������݃t�@�C���̓ǂݍ���
		std::ofstream ofs( "Log/" + path, std::ios::out | openMode );

		// �f�B���N�g�������݂��Ȃ��Ȃ�f�B���N�g�����쐬����
		if( !ofs ) {
			FileUtility::Utility::CreateFolder( "Log" );
			ofs.open( "Log/" + path, std::ios::out | openMode );
		}

		// �t�@�C�����J�����珈�����s��
		if( ofs ) {
			if( instance.m_timeStamp ) {
				// �������o��
				ofs << "========Start========\n";
				ofs << ( System::Date::YYYY_MM_SS() + " " + System::Date::HH_MM_SS() + "\n" );
				ofs << "=====================\n";
			}
			if(!instance.m_header.empty()) {
				// �w�b�_�[���o��
				ofs << (instance.m_header + "\n");
			}
			// �v�f���Ȃ��Ȃ�܂ŏo��
			for( auto i = 0; i < instance.m_logIndex; i++ ) {
				if(logTypeCond( instance.m_log[i].first.first ) ) {
					ofs << instance.m_log[i].second << std::endl;
				}
			}
			if(!instance.m_footer.empty()) {
				// �t�b�^�[���o��
				ofs << instance.m_footer + "\n";
			}
			if( instance.m_timeStamp ) {
				// �������o��
				ofs << "=========End=========\n";
				ofs << ( System::Date::YYYY_MM_SS() + " " + System::Date::HH_MM_SS() + "\n" );
				ofs << "=====================\n";
			}
		}

		if (instance.m_multiThreadParse) {
			// �Z�}�t�H��true�ɕύX
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
			// �Z�}�t�H�̃t���O��false�Ȃ�true�ɂȂ�܂ő҂�
			if (instance.m_semaphore == false) instance.m_semaphore.wait_true();

			// �Z�}�t�H��false�ɕύX
			instance.m_semaphore.false_flag();
		}

		// �������݃t�@�C���̓ǂݍ���
		std::ofstream ofs( "Log/" + path, std::ios::out | openMode );

		// �f�B���N�g�������݂��Ȃ��Ȃ�f�B���N�g�����쐬����
		if( !ofs ) {
			FileUtility::Utility::CreateFolder( "Log" );
			ofs.open( "Log/" + path, std::ios::out | openMode );
		}

		// �t�@�C�����J�����珈�����s��
		if( ofs ) {
			if( instance.m_timeStamp ) {
				// �������o��
				ofs << "========Start========\n";
				ofs << ( System::Date::YYYY_MM_SS() + " " + System::Date::HH_MM_SS() + "\n" );
				ofs << "=====================\n";
			}
			if (!instance.m_header.empty()) {
				// �w�b�_�[���o��
				ofs << (instance.m_header + "\n");
			}
			// �v�f���Ȃ��Ȃ�܂ŏo��
			for( auto i = 0; i < instance.m_logIndex; i++ ) {
				if( logTypeCond( instance.m_log[i].first.first ) && tagNameCond( instance.m_log[i].first.second ) ) {
					ofs << instance.m_log[i].second << std::endl;
				}
			}
			if (!instance.m_footer.empty()) {
				// �t�b�^�[���o��
				ofs << instance.m_footer + "\n";
			}
			if( instance.m_timeStamp ) {
				// �������o��
				ofs << "=========End=========\n";
				ofs << ( System::Date::YYYY_MM_SS() + " " + System::Date::HH_MM_SS() + "\n" );
				ofs << "=====================\n";
			}
		}

		if (instance.m_multiThreadParse) {
			// �Z�}�t�H��true�ɕύX
			instance.m_semaphore.true_flag();
		}
	}

public:

	//===================================================
	// ���O���e�L�X�g�t�@�C���ɏo�͂���
	//===================================================
	struct Parse {

		// �S�Ẵ��O���e�L�X�g�t�@�C���ɏo��
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

		// �S�Ẵ��O���e�L�X�g�t�@�C���̖����ɒǋL
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

		// �ʏ�̃��O�݂̂��e�L�X�g�t�@�C���ɏo��
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

		// �댯�ȃ��O�݂̂��e�L�X�g�t�@�C���ɏo��
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

		// ��x���݂̂��e�L�X�g�t�@�C���ɏo��
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

		// ���x���݂̂��e�L�X�g�t�@�C���ɏo��
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

		// �G���[�݂̂��e�L�X�g�t�@�C���ɏo��
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

		// ��O�݂̂��e�L�X�g�t�@�C���ɏo��
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
	// ����^�O�̃��O�݂̂��e�L�X�g�t�@�C���ɏo�͂���
	//===================================================
	struct ParseForTag {

	private:

		// �����I�ȃp�X�̉��H ��ParseForTag���̊֐��ł̂ݎg�p����
		static std::string _Internal_ParseToFilePathCreater( const std::string& defaultPath, const std::string& tagName, const std::string path ) {
			std::string ret;

			if( path == defaultPath ) {
				// �p�X�����g�œ��͂��Ă��Ȃ��ꍇ�̓^�O���ƘA�������p�X�ɕύX����
				ret = tagName + "_" + path;
			}
			else {
				// �p�X�����g�œ��͂��Ă���ꍇ�͎��ȐӔC�Ȃ̂œ��͂��ꂽ�p�X���g�p����
				ret = path;
			};

			return ret;
		}

	public:

		// ����^�O�̑S�Ẵ��O���e�L�X�g�t�@�C���ɏo��
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

		// ����^�O�̑S�Ẵ��O���e�L�X�g�t�@�C���̖����ɒǋL
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

		// ����^�O���A�ʏ�̃��O�݂̂��e�L�X�g�t�@�C���ɏo��
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

		// ����^�O���A�댯�ȃ��O�݂̂��e�L�X�g�t�@�C���ɏo��
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

		// ����^�O���A��x���݂̂��e�L�X�g�t�@�C���ɏo��
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

		// ����^�O���A���x���݂̂��e�L�X�g�t�@�C���ɏo��
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

		// ����^�O���A�G���[�݂̂��e�L�X�g�t�@�C���ɏo��
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

		// ����^�O���A��O�݂̂��e�L�X�g�t�@�C���ɏo��
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


		// �S�Ẵ��O���폜
		static void AllLog() {
			Debug::_Internal_LogDestroyLogTypeIf();
		}

		// �ʏ�̃��O�݂̂��폜
		static void NormalLog() {
			Debug::_Internal_LogDestroyLogTypeIf( []( LogType logType ) { return logType == LogType::Log; } );
		}

		// �댯�ȃ��O�݂̂��폜
		static void DengerLog() {
			Debug::_Internal_LogDestroyLogTypeIf( []( LogType logType ) { return logType != LogType::Log; } );
		}

		// ��x���݂̂��폜
		static void WarningLog() {
			Debug::_Internal_LogDestroyLogTypeIf( []( LogType logType ) { return logType == LogType::Warning; } );
		}

		// ���x���݂̂��폜
		static void AssertionLog() {
			Debug::_Internal_LogDestroyLogTypeIf( []( LogType logType ) { return logType == LogType::Assertion; } );
		}

		// �G���[�݂̂��폜
		static void ErrorLog() {
			Debug::_Internal_LogDestroyLogTypeIf( []( LogType logType ) { return logType == LogType::Error; } );
		}

		// ��O�݂̂��폜
		static void ExceptionLog() {
			Debug::_Internal_LogDestroyLogTypeIf( []( LogType logType ) { return logType == LogType::Exception; } );
		}


	};

	struct LogDestroyForTag {

		// ����^�O�̑S�Ẵ��O���폜
		static void AllLog( std::string tagName) {
			Debug::_Internal_LogDestroyLogTypeIfAndTagNameIf(
				[]( Debug::LogType logType ) { return true; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A�ʏ�̃��O�݂̂��폜
		static void NormalLog( std::string tagName) {
			Debug::_Internal_LogDestroyLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Log; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A�댯�ȃ��O�݂̂��폜
		static void DengerLog( std::string tagName) {
			Debug::_Internal_LogDestroyLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType != LogType::Log; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; } 
			);
		}

		// ����^�O���A��x���݂̂��폜
		static void WarningLog( std::string tagName) {
			Debug::_Internal_LogDestroyLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Warning; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A���x���݂̂��폜
		static void AssertionLog( std::string tagName) {
			Debug::_Internal_LogDestroyLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Assertion; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A�G���[�݂̂��폜
		static void ErrorLog( std::string tagName) {
			Debug::_Internal_LogDestroyLogTypeIfAndTagNameIf( 
				[]( LogType logType ) { return logType == LogType::Error; },
				[ & ]( std::string _tagName ) { return _tagName == tagName; }
			);
		}

		// ����^�O���A��O�݂̂��폜
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

			// ���O������𐶐�
			Debug::_Internal_CreateLogString(args...);

			// ����̃��O�����݂��邩�ǂ������肷��
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
		// _USE_WINAPI���`�����ꍇ�̂�
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
		// _USE_WINAPI���`�����ꍇ�̂�
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
		// _USE_WINAPI���`�����ꍇ�̂�
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



// ���O��char�^���g�p�����ꍇ�A���l���\�������



