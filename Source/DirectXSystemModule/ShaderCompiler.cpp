
// TNLIB
#include"Debug.hpp"
#include"FileUtility.hpp"

// MyFile
#include"ShaderCompiler.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

bool DXShaderCompiler::IsCsoFile(const std::string& path) {
	if (path.size() != 0) {
		auto extension = FileUtility::Utility::FileExtension(path);
		return (extension == "cso");
	}
	else {
		Debug::LogWarning(
			"DXShaderCompiler::IsCsoFile Method.\n",
			"File path is not set."
		);
	}
}

void DXShaderCompiler::CompileShader(const std::string& path, const std::string& funcName, const std::string& version, void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob) {
	if (IsCsoFile(path)) {
		ReadCompiledShaderFile(
			path,
			shaderObject,
			shaderObjectSize
		);
	}
	else {
		CompileShaderFromFile(
			path,
			funcName,
			version,
			shaderObject,
			shaderObjectSize,
			blob,
			errorBlob
		);
	}
}

void DXShaderCompiler::ReadCompiledShaderFile(const std::string& path, void*& shaderObject, std::size_t& shaderObjectSize) {
	// コンパイル済みシェーダーファイル(.csoファイル)を読み込む
	auto size = FileUtility::Utility::FileSize(path);
	auto ifs = FileUtility::Binary::LoadFile(path);
	std::vector<char_t> data;
	data.resize(size);
	if (ifs.has_value()) {
		ifs.value().read(reinterpret_cast<char*>(data.data()), data.size());
		shaderObject = data.data();
		shaderObjectSize = data.size();
	}
	else {
		Debug::LogWarning(
			"DXShaderCompiler::CompileShader Method.\n",
			"File expansion failed.\n",
			"FilePath[", path, "]"
		);
	}
}

void DXShaderCompiler::CompileShaderFromFile(const std::string& path, const std::string& funcName, const std::string& version, void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob) {
#if _DEBUG
	auto shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
#else
	auto shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#endif
	auto wpath = Utility::ToWideStr(path);

	auto hr = D3DCompileFromFile(
		wpath.c_str(),								// filename LPCWST pshader_file_path
		nullptr,										// D3D_SHADER_MACRO *pDefines
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// ID3DInclude *pInclude
		funcName.c_str(),							// LPCSTR pEntrypoint
		version.c_str(),							// LPCSTR pTarget
		shaderFlags,								// UINT Flags1
		0,												// UINT Flags2
		&blob,										// DirectXBinaryObject** ppCode
		&errorBlob									// DirectXBinaryObject** ppErrorMsg
	);

	IF_HRS(hr) {
		shaderObject = blob->GetBufferPointer();
		shaderObjectSize = blob->GetBufferSize();
		Debug::Log("DXShaderCompiler::CompileShaderFromFile Method Success.");
	}

	IF_HRF(hr) {
		if (errorBlob != nullptr) {
			Debug::BracketBegin("ErrorBlob");
			Debug::Log(errorBlob->GetBufferPointer());
			Debug::BracketEnd();
		}
		Debug::LogError(
			"DXShaderCompiler::CompileShaderFromFile Method.\n",
			"Path[", path, "]\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXShaderCompiler::CompileShaderFromFile Method.\n",
			hr
		);
#endif
	}
}