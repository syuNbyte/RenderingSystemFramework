
// MyFile
#include"HresultUtility.h"

std::string HresultUtility::GetHresultMessage(HRESULT hr) {

	auto message = std::system_category().message(hr);
	auto pos = message.find_last_of("\n");
	if (pos != std::string::npos) message.erase(pos);
	pos = message.find_last_of("\r");
	if (pos != std::string::npos) message.erase(pos);
	return "HresultMessage[" + message + "]";
}

std::string HresultUtility::GetHresultCodeMeaning(HRESULT hr) {
	auto code = HRESULT_CODE(hr);
	switch (code) {
		case S_OK: return "HresultMeaning[Operation was successful.]"; break;												// 操作に成功した
		case E_ABORT: return "HresultMeaning[Operation was abort.]"; break;													// 操作が中止された
		case E_ACCESSDENIED: return "HresultMeaning[Access denied.]"; break;													// アクセス拒否エラー
		case E_FAIL: return "HresultMeaning[Unspecified error.]"; break;														// 不特定のエラー
		case E_HANDLE: return "HresultMeaning[Invalid handle.]"; break;														// 無効なハンドル
		case E_INVALIDARG: return "HresultMeaning[Invalid args.]"; break;														// 一つ以上の引数が無効
		case E_NOINTERFACE: return "HresultMeaning[Unsupported interfaces.]"; break;										// インターフェイスがサポートされていない
		case E_NOTIMPL: return "HresultMeaning[Not implementation.]"; break;													// 未実装
		case E_OUTOFMEMORY: return "HresultMeaning[Could not allocate the required memory.]"; break;					// 必要なメモリを割り当てられませんでした
		case E_POINTER: return "HresultMeaning[Invalid pointer.]"; break;														// 無効なポインター
		case E_UNEXPECTED: return "HresultMeaning[Unexpected error.]"; break;												// 予期しないエラー
		case ERROR_PATH_NOT_FOUND: return "HresultMeaning[The system cannot find the path specified.]"; break;	// 指定したパスが見つかりません
		default: return "HresultMeaning[The string for the code is not defined.]";											// コードに対応する文字列が定義されていない
	}
}

longlong_t HresultUtility::GetHresultCode(HRESULT hr) {
	return HRESULT_CODE(hr);
}

longlong_t HresultUtility::GetHresultFacility(HRESULT hr) {
	return HRESULT_FACILITY(hr);
}

bool HresultUtility::IsSuccess(HRESULT hr) {
	return SUCCEEDED(hr);
}

bool HresultUtility::IsFailed(HRESULT hr) {
	return FAILED(hr);
}