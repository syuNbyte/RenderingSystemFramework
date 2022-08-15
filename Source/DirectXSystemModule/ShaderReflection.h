#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"SafetyDataType.h"

// ShaderReflectionの汎用的なメソッドを提供するクラス
class DXShaderReflection {
public:
	// Shader
	static ComObject::ShaderReflection GetShaderReflection(void*& shaderObject, std::size_t& shaderObjectSize);
	static ComObject::ShaderReflection GetShaderReflection(DirectXBinaryObject*& blob);
};

// ShaderReflectionVariableの汎用的なメソッドを提供するクラス
class DXShaderReflectionVariable {
public:
	static void ViewDesc(ShaderReflectionVariable* var);
};

// ShaderReflectionTypeの汎用的なメソッドを提供するクラス
class DXShaderReflectionType {
public:
	static void ViewBaseType(ShaderReflectionType* type);
	static void ViewSelfType(ShaderReflectionType* type);
	static void ViewSubType(ShaderReflectionType* type);
	static void ViewSelfAndSubType(ShaderReflectionType* type);
	static void ViewAllHierarchy(ShaderReflectionType* type);
};