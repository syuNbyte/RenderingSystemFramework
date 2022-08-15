
// TNLIB
#include"Debug.hpp"
#include"FileUtility.hpp"

// MyFile
#include"VertexShader.h"
#include"ShaderCompiler.h"
#include"ShaderReflection.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

DXVertexShader::DXVertexShader() {}

DXVertexShader::~DXVertexShader() {}

bool DXVertexShader::IsCsoFile() {
	return DXShaderCompiler::IsCsoFile(m_path);
}

void DXVertexShader::CompileShader(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob) {
	DXShaderCompiler::CompileShader(
		m_path,
		m_funcName,
		m_version,
		shaderObject,
		shaderObjectSize,
		blob,
		errorBlob
	);
}

void DXVertexShader::ReadCompiledShaderFile(void*& shaderObject, std::size_t& shaderObjectSize) {
	DXShaderCompiler::ReadCompiledShaderFile(
		m_path,
		shaderObject,
		shaderObjectSize
	);
}

void DXVertexShader::CompileShaderFromFile(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob) {
	DXShaderCompiler::CompileShaderFromFile(
		m_path,
		m_funcName,
		m_version,
		shaderObject,
		shaderObjectSize,
		blob,
		errorBlob
	);
}

void DXVertexShader::CreateVertexShader(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize) {
	if (m_vs) {
		m_vs.Reset();
		Debug::Log("This VertexShader was previously created.");
	}

	auto hr = device->CreateVertexShader(
		shaderObject,
		shaderObjectSize,
		nullptr,
		m_vs.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXVertexShader::CreateVertexShader Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXVertexShader::CreateVertexShader Method.\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXVertexShader::CreateVertexShader Method.\n",
			hr
		);
#endif
	}
}

void DXVertexShader::CreateInputLayout(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize) {
	if (m_layout) {
		m_layout.Reset();
		Debug::Log("This InputLayout was previously created.");
	}

	auto hr = device->CreateInputLayout(
		m_elements.data(),
		m_elements.size(),
		shaderObject,
		shaderObjectSize,
		m_layout.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXVertexShader::CreateInputLayout Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXVertexShader::CreateInputLayout Method.\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXVertexShader::CreateInputLayout Method.\n",
			hr
		);
#endif
	}
}

void DXVertexShader::ReleaseBlobData(DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob) {
	// Blobの開放
	if (blob != nullptr) blob->Release();
	if (errorBlob != nullptr) errorBlob->Release();
}

void DXVertexShader::Create(DirectXDevice* device) {
	Debug::BracketBegin("DXVertexShader::Create");

	void* shaderObject = nullptr;
	std::size_t shaderObjectSize = 0;
	DirectXBinaryObject* blob = nullptr;
	DirectXBinaryObject* errorBlob = nullptr;
	CompileShader(shaderObject, shaderObjectSize, blob, errorBlob);
	CreateVertexShader(device, shaderObject, shaderObjectSize);
	CreateInputLayout(device, shaderObject, shaderObjectSize);
	ReleaseBlobData(blob, errorBlob);

	Debug::BracketEnd();
}

void DXVertexShader::CreateAndCompile(DirectXDevice* device) {
	Debug::BracketBegin("DXVertexShader::CreateAndCompile");

	void* shaderObject = nullptr;
	std::size_t shaderObjectSize = 0;
	DirectXBinaryObject* blob = nullptr;
	DirectXBinaryObject* errorBlob = nullptr;
	CompileShaderFromFile(shaderObject, shaderObjectSize, blob, errorBlob);
	CreateVertexShader(device, shaderObject, shaderObjectSize);
	CreateInputLayout(device, shaderObject, shaderObjectSize);
	ReleaseBlobData(blob, errorBlob);

	Debug::BracketEnd();
}

void DXVertexShader::SetFilePath(const std::string& path) {
	m_path = path;
}

void DXVertexShader::SetEntryFuncName(const std::string& funcName) {
	m_funcName = funcName;
}

void DXVertexShader::SetVersion(const std::string& version) {
	// バージョン文字列は以下のような形式である必要がある
	// シェーダータイプ_浮動小数点より上の数値_浮動小数点以下の数値
	// Example : 頂点シェーダーでシェーダーモデル5.0の場合
	// vs_5_0
	m_version = version;
}

void DXVertexShader::SetInputElements(InputElementDesc* elements, const std::size_t& size) {
	m_elements.clear();
	m_elements.resize(size);
	for (int_t i = 0; i < size; i++) m_elements[i] = elements[i];
}

void DXVertexShader::VSSetShader(DirectXDeviceContext* context) {
	context->VSSetShader(m_vs.Get(), nullptr, 0);
}

void DXVertexShader::IASetInputLayout(DirectXDeviceContext* context) {
	context->IASetInputLayout(m_layout.Get());
}



DXVertexShaderLink::DXVertexShaderLink() {}

DXVertexShaderLink::~DXVertexShaderLink() {}

bool DXVertexShaderLink::IsCsoFile() {
	return DXShaderCompiler::IsCsoFile(m_path);
}

void DXVertexShaderLink::CompileShader(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob) {
	DXShaderCompiler::CompileShader(
		m_path,
		m_funcName,
		m_version,
		shaderObject,
		shaderObjectSize,
		blob,
		errorBlob
	);
}

void DXVertexShaderLink::ReadCompiledShaderFile(void*& shaderObject, std::size_t& shaderObjectSize) {
	DXShaderCompiler::ReadCompiledShaderFile(
		m_path,
		shaderObject,
		shaderObjectSize
	);
}

void DXVertexShaderLink::CompileShaderFromFile(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob) {
	DXShaderCompiler::CompileShaderFromFile(
		m_path,
		m_funcName,
		m_version,
		shaderObject,
		shaderObjectSize,
		blob,
		errorBlob
	);
}

void DXVertexShaderLink::CreateClassLinkage(DirectXDevice* device) {
	auto hr = device->CreateClassLinkage(
		m_classLinkage.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXVertexShaderLink::CreateClassLinkage Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXVertexShaderLink::CreateClassLinkage Method.\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXVertexShaderLink::CreateClassLinkage Method.\n",
			hr
		);
#endif
	}
}

void DXVertexShaderLink::CreateVertexShader(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize) {

	auto hr = device->CreateVertexShader(
		shaderObject,
		shaderObjectSize,
		m_classLinkage.Get(),
		m_vs.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXVertexShaderLink::CreateVertexShader Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXVertexShaderLink::CreateVertexShader Method.\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXVertexShaderLink::CreateVertexShader Method.\n",
			hr
		);
#endif
	}
}

void DXVertexShaderLink::CreateInputLayout(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize) {
	auto hr = device->CreateInputLayout(
		m_elements.data(),
		m_elements.size(),
		shaderObject,
		shaderObjectSize,
		m_layout.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXVertexShaderLink::CreateInputLayout Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXVertexShaderLink::CreateInputLayout Method.\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXVertexShaderLink::CreateInputLayout Method.\n",
			hr
		);
#endif
	}
}

void DXVertexShaderLink::ReleaseBlobData(DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob) {
	// Blobの開放
	if (blob != nullptr) blob->Release();
	if (errorBlob != nullptr) errorBlob->Release();
}

void DXVertexShaderLink::Create(DirectXDevice* device) {
	Debug::BracketBegin("DXVertexShaderLink::Create");

	void* shaderObject = nullptr;
	std::size_t shaderObjectSize = 0;
	DirectXBinaryObject* blob = nullptr;
	DirectXBinaryObject* errorBlob = nullptr;
	CompileShader(shaderObject, shaderObjectSize, blob, errorBlob);
	CreateClassLinkage(device);
	CreateVertexShader(device, shaderObject, shaderObjectSize);
	CreateInputLayout(device, shaderObject, shaderObjectSize);
	ReleaseBlobData(blob, errorBlob);

	Debug::BracketEnd();
}

void DXVertexShaderLink::CreateAndCompile(DirectXDevice* device) {
	Debug::BracketBegin("DXVertexShaderLink::CreateAndCompile");

	void* shaderObject = nullptr;
	std::size_t shaderObjectSize = 0;
	DirectXBinaryObject* blob = nullptr;
	DirectXBinaryObject* errorBlob = nullptr;
	CompileShaderFromFile(shaderObject, shaderObjectSize, blob, errorBlob);
	CreateClassLinkage(device);
	CreateVertexShader(device, shaderObject, shaderObjectSize);
	CreateInputLayout(device, shaderObject, shaderObjectSize);
	ReleaseBlobData(blob, errorBlob);

	Debug::BracketEnd();
}

void DXVertexShaderLink::CreateClassInstance(const std::string& key) {
	Debug::BracketBegin("DXVertexShaderLink::CreateClassInstance");

	ComObject::ClassInstance classInstance;

	// 取得するクラスにデータメンバが存在しない場合
	// CreateClassInstanceメソッドを使用しないとシェーダーにセットする際にエラー
	auto hr = m_classLinkage->CreateClassInstance(
		key.c_str(),						// 実際に使用するHLSL側で定義されているinterfaceを継承したクラス名
		0,
		0,
		0,
		0,
		classInstance.GetAddressOf()
	);

	IF_HRS(hr) {
		m_classInstances.emplace(key, std::move(classInstance));
		Debug::Log("DXVertexShaderLink::CreateClassInstance Method Success.");
		Debug::Log("Success ClassName[", key, "]");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXVertexShaderLink::CreateClassInstance Method.\n",
			"Failed ClassName[", key, "]\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXVertexShaderLink::CreateClassInstance Method.\nFailed ClassName[" + key + "]\n",
			hr
		);
#endif
	}
}

void DXVertexShaderLink::GetClassInstance(const std::string& key) {
	ComObject::ClassInstance classInstance;

	// 取得するクラスにデータメンバが存在しない場合
	// CreateClassInstanceメソッドを使用しないとシェーダーにセットする際にエラー
	auto hr = m_classLinkage->GetClassInstance(
		key.c_str(),						// 動的シェーダーリンク用の定数バッファ内のクラスインスタンスの変数名
		0,
		classInstance.GetAddressOf()
	);

	IF_HRS(hr) {
		m_classInstances.emplace(key, std::move(classInstance));
		Debug::Log("DXVertexShaderLink::GetClassInstance Method Success.");
		Debug::Log("Success ClassName[", key, "]");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXVertexShaderLink::GetClassInstance Method.\n",
			"Failed ClassName[", key, "]\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXVertexShaderLink::GetClassInstance Method\nFailed ClassName[" + key + "]\n",
			hr
		);
#endif
	}
}

void DXVertexShaderLink::SetFilePath(const std::string& path) {
	m_path = path;
}

void DXVertexShaderLink::SetEntryFuncName(const std::string& funcName) {
	m_funcName = funcName;
}

void DXVertexShaderLink::SetVersion(const std::string& version) {
	m_version = version;
}

void DXVertexShaderLink::SetInputElements(InputElementDesc* elements, const std::size_t& size) {
	m_elements.clear();
	m_elements.resize(size);
	for (int_t i = 0; i < size; i++) m_elements[i] = elements[i];
}

void DXVertexShaderLink::VSSetShader(DirectXDeviceContext* context, const std::string& key) {
	context->VSSetShader(
		m_vs.Get(),
		m_classInstances[key].GetAddressOf(),
		1	// 配列ではなくインスタンスを直接渡しているので１
	);
}

void DXVertexShaderLink::IASetInputLayout(DirectXDeviceContext* context) {
	context->IASetInputLayout(
		m_layout.Get()
	);
}
