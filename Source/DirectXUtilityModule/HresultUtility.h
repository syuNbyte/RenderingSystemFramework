#pragma once

// STL
#include<string>

// Windows
#include<windows.h>

// TNLIB
#include"SafetyDataType.h"

// HRESULT型の汎用的な関数のラッパ
class HresultUtility {
public:

	static std::string GetHresultMessage(HRESULT hr);
	static std::string GetHresultCodeMeaning(HRESULT hr);
	static longlong_t GetHresultCode(HRESULT hr);
	static longlong_t GetHresultFacility(HRESULT hr);
	static bool IsSuccess(HRESULT hr);
	static bool IsFailed(HRESULT hr);

};

#define IF_HRS(hr) if(SUCCEEDED(hr))	// HRESULT型の戻り値が成功しているかを判定する
#define IF_HRF(hr) if(FAILED(hr))		// HRESULT型の戻り値が失敗しているかを判定する

/*
* ==============================================================
* Example（例）
* ==============================================================
* IF_HRS(hr){
*	// 成功時の処理
* }
* 
* IF_HRF(hr){
*	// 失敗時の処理
* }
*/