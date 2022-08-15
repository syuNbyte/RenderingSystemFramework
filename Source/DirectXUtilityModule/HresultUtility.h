#pragma once

// STL
#include<string>

// Windows
#include<windows.h>

// TNLIB
#include"SafetyDataType.h"

// HRESULT�^�̔ėp�I�Ȋ֐��̃��b�p
class HresultUtility {
public:

	static std::string GetHresultMessage(HRESULT hr);
	static std::string GetHresultCodeMeaning(HRESULT hr);
	static longlong_t GetHresultCode(HRESULT hr);
	static longlong_t GetHresultFacility(HRESULT hr);
	static bool IsSuccess(HRESULT hr);
	static bool IsFailed(HRESULT hr);

};

#define IF_HRS(hr) if(SUCCEEDED(hr))	// HRESULT�^�̖߂�l���������Ă��邩�𔻒肷��
#define IF_HRF(hr) if(FAILED(hr))		// HRESULT�^�̖߂�l�����s���Ă��邩�𔻒肷��

/*
* ==============================================================
* Example�i��j
* ==============================================================
* IF_HRS(hr){
*	// �������̏���
* }
* 
* IF_HRF(hr){
*	// ���s���̏���
* }
*/