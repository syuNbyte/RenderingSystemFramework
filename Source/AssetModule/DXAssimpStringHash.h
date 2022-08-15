#pragma once

// STL
#include<functional>

// MyFile
#include"AssimpLoader.hpp"

namespace std {

	template<>
	class hash<aiString> {
	public:
		hash() = default;
		hash(const hash&) = default;
		hash(hash&&) = default;
		~hash() = default;
	public:
		hash& operator=(const hash&) = default;
		hash& operator=(hash&&) = default;
	public:
		size_t operator()(const aiString& assimpString) const {
			return std::hash<std::string>()(assimpString.C_Str());
		}
	};

}