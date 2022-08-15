
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
		case S_OK: return "HresultMeaning[Operation was successful.]"; break;												// ����ɐ�������
		case E_ABORT: return "HresultMeaning[Operation was abort.]"; break;													// ���삪���~���ꂽ
		case E_ACCESSDENIED: return "HresultMeaning[Access denied.]"; break;													// �A�N�Z�X���ۃG���[
		case E_FAIL: return "HresultMeaning[Unspecified error.]"; break;														// �s����̃G���[
		case E_HANDLE: return "HresultMeaning[Invalid handle.]"; break;														// �����ȃn���h��
		case E_INVALIDARG: return "HresultMeaning[Invalid args.]"; break;														// ��ȏ�̈���������
		case E_NOINTERFACE: return "HresultMeaning[Unsupported interfaces.]"; break;										// �C���^�[�t�F�C�X���T�|�[�g����Ă��Ȃ�
		case E_NOTIMPL: return "HresultMeaning[Not implementation.]"; break;													// ������
		case E_OUTOFMEMORY: return "HresultMeaning[Could not allocate the required memory.]"; break;					// �K�v�ȃ����������蓖�Ă��܂���ł���
		case E_POINTER: return "HresultMeaning[Invalid pointer.]"; break;														// �����ȃ|�C���^�[
		case E_UNEXPECTED: return "HresultMeaning[Unexpected error.]"; break;												// �\�����Ȃ��G���[
		case ERROR_PATH_NOT_FOUND: return "HresultMeaning[The system cannot find the path specified.]"; break;	// �w�肵���p�X��������܂���
		default: return "HresultMeaning[The string for the code is not defined.]";											// �R�[�h�ɑΉ����镶���񂪒�`����Ă��Ȃ�
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