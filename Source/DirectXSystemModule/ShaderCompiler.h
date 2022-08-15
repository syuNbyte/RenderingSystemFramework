#pragma once

// STL
#include<string>

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"SafetyDataType.h"

// DirectXのシェーダーをコンパイルするメソッドを持つクラス
class DXShaderCompiler {
public:
	static bool IsCsoFile(
		const std::string& path
	);

	static void CompileShader(
		const std::string& path,
		const std::string& funcName,
		const std::string& version,
		void*& shaderObject,
		std::size_t& shaderObjectSize,
		DirectXBinaryObject*& blob,
		DirectXBinaryObject*& errorBlob
	);
	
	static void ReadCompiledShaderFile(
		const std::string& path,
		void*& shaderObject,
		std::size_t& shaderObjectSize
	);
	
	static void CompileShaderFromFile(
		const std::string& path,
		const std::string& funcName,
		const std::string& version,
		void*& shaderObject,
		std::size_t& shaderObjectSize,
		DirectXBinaryObject*& blob,
		DirectXBinaryObject*& errorBlob
	);
};