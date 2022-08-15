
// TNLIB
#include"Debug.hpp"

// MyFile
#include"ShaderReflection.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

ComObject::ShaderReflection DXShaderReflection::GetShaderReflection(void*& shaderObject, std::size_t& shaderObjectSize) {
	ComObject::ShaderReflection reflector;

	auto hr = D3DReflect(
		shaderObject,
		shaderObjectSize,
		IID_ID3D11ShaderReflection,
		reinterpret_cast<void**>(reflector.GetAddressOf())
	);

	IF_HRS(hr) {

		Debug::Log("DXShaderReflector::GetShaderReflection Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXShaderReflector::GetShaderReflection Method.\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXShaderReflector::GetShaderReflection Method.\n",
			hr
		);
#endif
	}

	return std::move(reflector);
}

ComObject::ShaderReflection DXShaderReflection::GetShaderReflection(DirectXBinaryObject*& blob) {
	ComObject::ShaderReflection reflector;

	auto hr = D3DReflect(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		reinterpret_cast<void**>(reflector.GetAddressOf())
	);

	IF_HRS(hr) {

		Debug::Log("DXShaderReflector::GetShaderReflection Method Success.");
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXShaderReflector::GetShaderReflection Method.\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXShaderReflector::GetShaderReflection Method.\n",
			hr
		);
#endif
	}

	return std::move(reflector);
}


//=====DXShaderReflectionVariable=====//


void DXShaderReflectionVariable::ViewDesc(ShaderReflectionVariable* var) {
	D3D11_SHADER_VARIABLE_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SHADER_VARIABLE_DESC));
	var->GetDesc(&desc);

	Debug::BracketBegin("ShaderVariableDesc");

	Debug::Log("DefaultValue : ", desc.DefaultValue);
	Debug::Log("Name : ", desc.Name);
	Debug::Log("uFlags : ", desc.uFlags);
	Debug::Log("StartOffset : ", desc.StartOffset);
	Debug::Log("StartSampler : ", desc.StartSampler);
	Debug::Log("StartTexture : ", desc.StartTexture);
	Debug::Log("Size : ", desc.Size);
	Debug::Log("SamplerSize :", desc.SamplerSize);
	Debug::Log("TextureSize : ", desc.TextureSize);

	Debug::BracketEnd();
}


//=====DXShaderReflectionVariable=====//


void DXShaderReflectionType::ViewBaseType(ShaderReflectionType* type) {
	D3D11_SHADER_TYPE_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SHADER_TYPE_DESC));
	auto base = type->GetBaseClass();
	if (base != nullptr) {
		base->GetDesc(&desc);

		Debug::BracketBegin("ShaderTypeDesc[Base]");

		Debug::Log("Name : ", desc.Name);
		Debug::Log("Type : ", desc.Type);
		Debug::Log("Class : ", desc.Class);
		Debug::Log("Members : ", desc.Members);
		Debug::Log("Elements : ", desc.Elements);
		Debug::Log("Rows : ", desc.Rows);
		Debug::Log("Columns : ", desc.Columns);
		Debug::Log("Offset : ", desc.Offset);

		Debug::BracketEnd();
	}
	else {
		Debug::BracketBegin("ShaderTypeDesc[Base]");
		Debug::Log("Base Nothing.");
		Debug::BracketEnd();
	}
}

void DXShaderReflectionType::ViewSelfType(ShaderReflectionType* type) {
	D3D11_SHADER_TYPE_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SHADER_TYPE_DESC));
	type->GetDesc(&desc);

	Debug::BracketBegin("ShaderTypeDesc[Self]");

	Debug::Log("Name : ", desc.Name);
	Debug::Log("Type : ", desc.Type);
	Debug::Log("Class : ", desc.Class);
	Debug::Log("Members : ", desc.Members);
	Debug::Log("Elements : ", desc.Elements);
	Debug::Log("Rows : ", desc.Rows);
	Debug::Log("Columns : ", desc.Columns);
	Debug::Log("Offset : ", desc.Offset);

	Debug::BracketEnd();
}

void DXShaderReflectionType::ViewSubType(ShaderReflectionType* type) {
	D3D11_SHADER_TYPE_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SHADER_TYPE_DESC));
	auto sub = type->GetSubType();
	if (sub != nullptr) {
		sub->GetDesc(&desc);

		Debug::BracketBegin("ShaderTypeDesc[Sub]");

		Debug::Log("Name : ", desc.Name);
		Debug::Log("Type : ", desc.Type);
		Debug::Log("Class : ", desc.Class);
		Debug::Log("Members : ", desc.Members);
		Debug::Log("Elements : ", desc.Elements);
		Debug::Log("Rows : ", desc.Rows);
		Debug::Log("Columns : ", desc.Columns);
		Debug::Log("Offset : ", desc.Offset);

		Debug::BracketEnd();
	}
	else {
		Debug::BracketBegin("ShaderTypeDesc[Sub]");
		Debug::Log("Sub Nothing.");
		Debug::BracketEnd();
	}
}

void DXShaderReflectionType::ViewSelfAndSubType(ShaderReflectionType* type) {
	DXShaderReflectionType::ViewSelfType(type);
	DXShaderReflectionType::ViewSubType(type);
}

void DXShaderReflectionType::ViewAllHierarchy(ShaderReflectionType* type) {
	ShaderReflectionType* base = nullptr;
	ShaderReflectionType* baseIf = nullptr;
	ShaderReflectionType* sub = nullptr;
	ShaderReflectionType* subIf = nullptr;
	do {
		baseIf = type->GetBaseClass();
		base = (baseIf != nullptr) ? baseIf : base;
	} while (baseIf != nullptr);

	{
		DXShaderReflectionType::ViewSelfType(base);
	}

	do {
		subIf = type->GetSubType();
		sub = (subIf != nullptr) ? subIf : sub;
		DXShaderReflectionType::ViewSelfAndSubType(sub);
	} while (sub != nullptr);
}