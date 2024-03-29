#pragma once

#include<string>

namespace ErrorType {

	using ErrorType = const char*;
	using ErrorLocation = const char*;


	constexpr ErrorType failed_to_create_element = "要素の作成に失敗しました";
	constexpr ErrorType failed_to_add_element = "要素の追加に失敗しました";
	constexpr ErrorType failed_to_exist_elemet = "要素が存在しません";
	constexpr ErrorType failed_to_get_element = "要素の取得に失敗しました";
	constexpr ErrorType failed_to_load_element = "要素の読み込みに失敗しました";
	constexpr ErrorType failed_to_write_element = "要素の書き込みに失敗しました";

	constexpr ErrorType unknown = "エラー理由が不明";
	constexpr ErrorType setting_failed = "設定に失敗しました";
	constexpr ErrorType function_exe_failed = "関数の実行に失敗しました";

}

template<class ClassType>
class ErrorClass {
public:
	// クラス名をstd::string型に変換して取得する
	std::string name() {
		return std::string( typeid( ClassType ).name() );
	}
};