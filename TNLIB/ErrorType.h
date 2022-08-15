#pragma once

#include<string>

namespace ErrorType {

	using ErrorType = const char*;
	using ErrorLocation = const char*;


	constexpr ErrorType failed_to_create_element = "—v‘f‚Ìì¬‚É¸”s‚µ‚Ü‚µ‚½";
	constexpr ErrorType failed_to_add_element = "—v‘f‚Ì’Ç‰Á‚É¸”s‚µ‚Ü‚µ‚½";
	constexpr ErrorType failed_to_exist_elemet = "—v‘f‚ª‘¶İ‚µ‚Ü‚¹‚ñ";
	constexpr ErrorType failed_to_get_element = "—v‘f‚Ìæ“¾‚É¸”s‚µ‚Ü‚µ‚½";
	constexpr ErrorType failed_to_load_element = "—v‘f‚Ì“Ç‚İ‚İ‚É¸”s‚µ‚Ü‚µ‚½";
	constexpr ErrorType failed_to_write_element = "—v‘f‚Ì‘‚«‚İ‚É¸”s‚µ‚Ü‚µ‚½";

	constexpr ErrorType unknown = "ƒGƒ‰[——R‚ª•s–¾";
	constexpr ErrorType setting_failed = "İ’è‚É¸”s‚µ‚Ü‚µ‚½";
	constexpr ErrorType function_exe_failed = "ŠÖ”‚ÌÀs‚É¸”s‚µ‚Ü‚µ‚½";

}

template<class ClassType>
class ErrorClass {
public:
	// ƒNƒ‰ƒX–¼‚ğstd::stringŒ^‚É•ÏŠ·‚µ‚Äæ“¾‚·‚é
	std::string name() {
		return std::string( typeid( ClassType ).name() );
	}
};