


// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"MessageBoxUtility.h"
#include"HresultUtility.h"

void MessageBoxUtility::SetEnableFlag(bool flag) {
	MessageBoxUtility::enableFlag = flag;
}

void MessageBoxUtility::Message(Icon icon, UseButton button, const std::string& boxHeader, const std::string& message) {
	if (MessageBoxUtility::enableFlag) {
		MessageBox(
			nullptr,
			message.c_str(),
			boxHeader.c_str(),
			static_cast<longlong_t>(icon) | static_cast<longlong_t>(button)
		);
	}
}

void MessageBoxUtility::Message(const std::string& message) {
	MessageBoxUtility::Message(
		Icon::Info,
		UseButton::Ok,
		"Message",
		message
	);
}

void MessageBoxUtility::ErrorMessage(const std::string& message) {
	MessageBoxUtility::Message(
		Icon::Error,
		UseButton::Ok,
		"Error",
		message
	);
}

void MessageBoxUtility::HresultMessage(Icon icon, UseButton button, const std::string& boxHeader, HRESULT hr) {
	if (MessageBoxUtility::enableFlag) {
		MessageBox(
			nullptr,
			(
				"===HresultMessage===\n" +
				HresultUtility::GetHresultMessage(hr) +
				"\n===HresultCodeMeaning===" +
				HresultUtility::GetHresultCodeMeaning(hr)
				).c_str(),
			boxHeader.c_str(),
			static_cast<longlong_t>(icon) | static_cast<longlong_t>(button)
		);
	}
}

void MessageBoxUtility::HresultMessage(Icon icon, UseButton button, const std::string& boxHeader, const std::string& message, HRESULT hr) {
	if (MessageBoxUtility::enableFlag) {
		MessageBox(
			nullptr,
			(
				message +
				"\n" +
				"===HresultMessage===\n" +
				HresultUtility::GetHresultMessage(hr) +
				"\n===HresultCodeMeaning===\n" +
				HresultUtility::GetHresultCodeMeaning(hr)
				).c_str(),
			boxHeader.c_str(),
			static_cast<longlong_t>(icon) | static_cast<longlong_t>(button)
		);
	}
}

void MessageBoxUtility::HresultMessage(HRESULT hr) {
	MessageBoxUtility::HresultMessage(Icon::Info, UseButton::Ok, "Message", hr);
}

void MessageBoxUtility::HresultMessage(const std::string& message, HRESULT hr) {
	MessageBoxUtility::HresultMessage(Icon::Error, UseButton::Ok, "Message", message, hr);
}

void MessageBoxUtility::HresultErrorMessage(HRESULT hr) {
	MessageBoxUtility::HresultMessage(Icon::Error, UseButton::Ok, "Error", hr);
}

void MessageBoxUtility::HresultErrorMessage(const std::string& message, HRESULT hr) {
	MessageBoxUtility::HresultMessage(Icon::Error, UseButton::Ok, "Error", message, hr);
}
