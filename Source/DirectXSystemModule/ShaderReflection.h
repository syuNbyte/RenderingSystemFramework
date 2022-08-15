#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"SafetyDataType.h"

// ShaderReflection�̔ėp�I�ȃ��\�b�h��񋟂���N���X
class DXShaderReflection {
public:
	// Shader
	static ComObject::ShaderReflection GetShaderReflection(void*& shaderObject, std::size_t& shaderObjectSize);
	static ComObject::ShaderReflection GetShaderReflection(DirectXBinaryObject*& blob);
};

// ShaderReflectionVariable�̔ėp�I�ȃ��\�b�h��񋟂���N���X
class DXShaderReflectionVariable {
public:
	static void ViewDesc(ShaderReflectionVariable* var);
};

// ShaderReflectionType�̔ėp�I�ȃ��\�b�h��񋟂���N���X
class DXShaderReflectionType {
public:
	static void ViewBaseType(ShaderReflectionType* type);
	static void ViewSelfType(ShaderReflectionType* type);
	static void ViewSubType(ShaderReflectionType* type);
	static void ViewSelfAndSubType(ShaderReflectionType* type);
	static void ViewAllHierarchy(ShaderReflectionType* type);
};