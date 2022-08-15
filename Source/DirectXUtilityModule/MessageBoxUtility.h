#pragma once

// STL
#include<string>

// Windows
#include<windows.h>

// Windowsのメッセージボックスに対するユーティリティ関数群
class MessageBoxUtility {
public:

	// 使用するボタン
	enum class UseButton {
		Ok							= 0x00000000L,
		OkCancel					= 0x00000001L,
		AbortRetryIgnore		= 0x00000002L,
		YesNoCancel				= 0x00000003L,
		YesNo						= 0x00000004L,
		RetryCancel				= 0x00000005L,
		CancelRetryContinue	= 0x00000006L
	};

	// 左上のアイコン
	enum class Icon {
		Hand				= 0x00000010L,		// hand
		Question			= 0x00000020L,		// ?
		Exclamation		= 0x00000030L,		// !
		Asterisk			= 0x00000040L,		// *
		Warning			= 0x00000030L,		// !
		Error				= 0x00000010L,		// hand
		Info				= 0x00000040L,		// *
		Stop				= 0x00000010L		// hand
	};

private:
	static inline bool enableFlag = true;
public:
	// メッセージボックスの機能の有効/無効を切り替える
	static void SetEnableFlag(bool flag);
public:
	static void Message(Icon icon, UseButton button, const std::string& boxHeader, const std::string& message);
	static void Message(const std::string& message);
	static void ErrorMessage(const std::string& message);
	static void HresultMessage(Icon icon, UseButton button, const std::string& boxHeader, HRESULT hr);
	static void HresultMessage(Icon icon, UseButton button, const std::string& boxHeader, const std::string& message, HRESULT hr);
	static void HresultMessage(HRESULT hr);
	static void HresultMessage(const std::string& message, HRESULT hr);
	static void HresultErrorMessage(HRESULT hr);
	static void HresultErrorMessage(const std::string& message, HRESULT hr);

};