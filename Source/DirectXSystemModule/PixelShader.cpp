
// TNLIB
#include"Debug.hpp"
#include"FileUtility.hpp"

// MyFile
#include"PixelShader.h"
#include"ShaderCompiler.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

DXPixelShader::DXPixelShader() {}

DXPixelShader::~DXPixelShader() {}

bool DXPixelShader::IsCsoFile() {
	return DXShaderCompiler::IsCsoFile(m_path);
}

void DXPixelShader::CompileShader(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob) {
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

void DXPixelShader::ReadCompiledShaderFile(void*& shaderObject, std::size_t& shaderObjectSize) {
	DXShaderCompiler::ReadCompiledShaderFile(
		m_path,
		shaderObject,
		shaderObjectSize
	);
}

void DXPixelShader::CompileShaderFromFile(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob) {
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

void DXPixelShader::CreatePixelShader(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize) {
	if (m_ps) {
		m_ps.Reset();
		Debug::Log("This PixelShader was previously created.");
	}

	auto hr = device->CreatePixelShader(
		shaderObject,
		shaderObjectSize,
		nullptr,
		m_ps.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXPixelShader::CreatePixelShader Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXPixelShader::CreatePixelShader Method.\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXPixelShader::CreatePixelShader Method.\n",
			hr
		);
#endif
	}
}

void DXPixelShader::Create(DirectXDevice* device) {
	Debug::BracketBegin("DXPixelShader::Create");

	void* shaderObject = nullptr;
	std::size_t shaderObjectSize = 0;
	DirectXBinaryObject* blob = nullptr;
	DirectXBinaryObject* errorBlob = nullptr;
	CompileShader(shaderObject, shaderObjectSize, blob, errorBlob);
	CreatePixelShader(device, shaderObject, shaderObjectSize);

	Debug::BracketEnd();
}

void DXPixelShader::CreateAndCompile(DirectXDevice* device) {
	Debug::BracketBegin("DXPixelShader::CreateAndCompile");

	void* shaderObject = nullptr;
	std::size_t shaderObjectSize = 0;
	DirectXBinaryObject* blob = nullptr;
	DirectXBinaryObject* errorBlob = nullptr;
	CompileShaderFromFile(shaderObject, shaderObjectSize, blob, errorBlob);
	CreatePixelShader(device, shaderObject, shaderObjectSize);

	Debug::BracketEnd();
}

void DXPixelShader::SetFilePath(const std::string& path) {
	m_path = path;
}

void DXPixelShader::SetEntryFuncName(const std::string& funcName) {
	m_funcName = funcName;
}

void DXPixelShader::SetVersion(const std::string& version) {
	// �o�[�W����������͈ȉ��̂悤�Ȍ`���ł���K�v������
	// �V�F�[�_�[�^�C�v_���������_����̐��l_���������_�ȉ��̐��l
	// Example : �s�N�Z���V�F�[�_�[�ŃV�F�[�_�[���f��5.0�̏ꍇ
	// ps_5_0
	m_version = version;
}

void DXPixelShader::PSSetShader(DirectXDeviceContext* context) {
	context->PSSetShader(m_ps.Get(), nullptr, 0);
}



DXPixelShaderLink::DXPixelShaderLink() {}

DXPixelShaderLink::~DXPixelShaderLink() {}

bool DXPixelShaderLink::IsCsoFile() {
	return DXShaderCompiler::IsCsoFile(m_path);
}

void DXPixelShaderLink::CompileShader(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob) {
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

void DXPixelShaderLink::ReadCompiledShaderFile(void*& shaderObject, std::size_t& shaderObjectSize) {
	DXShaderCompiler::ReadCompiledShaderFile(
		m_path,
		shaderObject,
		shaderObjectSize
	);
}

void DXPixelShaderLink::CompileShaderFromFile(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob) {
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

void DXPixelShaderLink::CreateClassLinkage(DirectXDevice* device) {
	auto hr = device->CreateClassLinkage(
		m_classLinkage.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXPixelShaderLink::CreateClassLinkage Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXPixelShaderLink::CreateClassLinkage Method.\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXPixelShaderLink::CreateClassLinkage Method.\n",
			hr
		);
#endif
	}
}

void DXPixelShaderLink::CreatePixelShader(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize) {
	auto hr = device->CreatePixelShader(
		shaderObject,
		shaderObjectSize,
		m_classLinkage.Get(),
		m_ps.GetAddressOf()
	);

	IF_HRS(hr) {
		Debug::Log("DXPixelShaderLink::CreatePixelShader Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXPixelShaderLink::CreatePixelShader Method.\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXPixelShaderLink::CreatePixelShader Method.\n",
			hr
		);
#endif
	}
}

void DXPixelShaderLink::Create(DirectXDevice* device) {
	Debug::BracketBegin("DXPixelShaderLink::Create");

	void* shaderObject = nullptr;
	std::size_t shaderObjectSize = 0;
	DirectXBinaryObject* blob = nullptr;
	DirectXBinaryObject* errorBlob = nullptr;
	CompileShader(shaderObject, shaderObjectSize, blob, errorBlob);
	CreateClassLinkage(device);
	CreatePixelShader(device, shaderObject, shaderObjectSize);

	Debug::BracketEnd();
}

void DXPixelShaderLink::CreateAndCompile(DirectXDevice* device) {
	Debug::BracketBegin("DXPixelShaderLink::CreateAndCompile");

	void* shaderObject = nullptr;
	std::size_t shaderObjectSize = 0;
	DirectXBinaryObject* blob = nullptr;
	DirectXBinaryObject* errorBlob = nullptr;
	CompileShaderFromFile(shaderObject, shaderObjectSize, blob, errorBlob);
	CreateClassLinkage(device);
	CreatePixelShader(device, shaderObject, shaderObjectSize);

	Debug::BracketEnd();
}

void DXPixelShaderLink::CreateClassInstance(const std::string& key) {
	Debug::BracketBegin("DXPixelShaderLink::CreateClassInstance");

	ComObject::ClassInstance classInstance;

	// �擾����N���X�Ƀf�[�^�����o�����݂��Ȃ��ꍇ
	// CreateClassInstance���\�b�h���g�p���Ȃ��ƃV�F�[�_�[�ɃZ�b�g����ۂɃG���[
	auto hr = m_classLinkage->CreateClassInstance(
		key.c_str(),						// ���ۂɎg�p����HLSL���Œ�`����Ă���interface���p�������N���X��
		0,
		0,
		0,
		0,
		classInstance.GetAddressOf()
	);

	IF_HRS(hr) {
		m_classInstances.emplace(key, std::move(classInstance));
		Debug::Log("DXPixelShaderLink::CreateClassInstance Method Success.");
		Debug::Log("Success ClassName[", key, "]");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXPixelShaderLink::CreateClassInstance Method.\n",
			"Failed ClassName[", key, "]\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXPixelShaderLink::CreateClassInstance Method.\nFailed ClassName[" + key + "]\n",
			hr
		);
#endif
	}

	Debug::BracketEnd();
}

void DXPixelShaderLink::GetClassInstance(const std::string& key) {
	ComObject::ClassInstance classInstance;

	auto hr = m_classLinkage->GetClassInstance(
		key.c_str(),
		0,
		classInstance.GetAddressOf()
	);

	IF_HRS(hr) {
		m_classInstances.emplace(key, std::move(classInstance));
		Debug::Log("DXPixelShaderLink::CreateClassInstance Method Success.");
		Debug::Log("Success ClassName[", key, "]");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXPixelShaderLink::CreateClassInstance Method.\n",
			"Failed ClassName[", key, "]\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXPixelShaderLink::CreateClassInstance Method.\nFailed ClassName[" + key + "]\n",
			hr
		);
#endif
	}
}

void DXPixelShaderLink::SetFilePath(const std::string& path) {
	m_path = path;
}

void DXPixelShaderLink::SetEntryFuncName(const std::string& funcName) {
	m_funcName = funcName;
}

void DXPixelShaderLink::SetVersion(const std::string& version) {
	// �o�[�W����������͈ȉ��̂悤�Ȍ`���ł���K�v������
	// �V�F�[�_�[�^�C�v_���������_����̐��l_���������_�ȉ��̐��l
	// Example : �s�N�Z���V�F�[�_�[�ŃV�F�[�_�[���f��5.0�̏ꍇ
	// ps_5_0
	m_version = version;
}

void DXPixelShaderLink::PSSetShader(DirectXDeviceContext* context, const std::string& key) {
	context->PSSetShader(
		m_ps.Get(),
		m_classInstances[key].GetAddressOf(),
		1 // �z��ł͂Ȃ��C���X�^���X�𒼐ړn���Ă���̂łP
	);
}
