#pragma once

// STL
#include<string>
#include<vector>
#include<unordered_map>

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"SafetyDataType.h"

// DirectX�̒��_�V�F�[�_�[�N���X���p������C���^�[�t�F�C�X�N���X
class IDXVertexShader {
public:
	virtual void SetFilePath(const std::string& path) = 0;
	virtual void SetEntryFuncName(const std::string& funcName) = 0;
	virtual void SetVersion(const std::string& version) = 0;
	virtual void SetInputElements(InputElementDesc* elements, const std::size_t& size) = 0;
	virtual void Create(DirectXDevice* device) = 0;
};

// DirectX�̒��_�V�F�[�_�[�������N���X
class DXVertexShader : public IDXVertexShader {
protected:
	ComObject::VertexShader m_vs;					// ���_�V�F�[�_�[
	ComObject::InputLayout m_layout;				// ���̓��C�A�E�g
	std::vector<InputElementDesc> m_elements;	// ���_�Ɏ���������Q
protected:
	std::string m_path;								// �V�F�[�_�[�t�@�C���̃p�X
	std::string m_funcName;							// �V�F�[�_�[�t�@�C���̃G���g���֐���
	std::string m_version;							// �V�F�[�_�[�t�@�C���̎g�p����V�F�[�_�[���f���o�[�W����
public:
	DXVertexShader();
	~DXVertexShader();
protected:
	bool IsCsoFile();
	void CompileShader(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void ReadCompiledShaderFile(void*& shaderObject, std::size_t& shaderObjectSize);
	void CompileShaderFromFile(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void CreateVertexShader(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize);
	void CreateInputLayout(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize);
	void ReleaseBlobData(DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
public:
	void Create(DirectXDevice* device);					// �ݒ肳��Ă�����Œ��_�V�F�[�_�[���쐬����
	void CreateAndCompile(DirectXDevice* device);	// �ݒ肳��Ă�����ŃV�F�[�_�[�t�@�C�����R���p�C�����Ă��璸�_�V�F�[�_�[���쐬����
	void SetFilePath(const std::string& path);
	void SetEntryFuncName(const std::string& funcName);
	void SetVersion(const std::string& version);
	void SetInputElements(InputElementDesc* elements, const std::size_t& size);
public:
	void VSSetShader(DirectXDeviceContext* context);
	void IASetInputLayout(DirectXDeviceContext* context);
};


// �N���X�����P�[�W�p�ɒ������ꂽDirectX�̒��_�V�F�[�_�[�������N���X
class DXVertexShaderLink : public IDXVertexShader {
protected:
	ComObject::VertexShader m_vs;																	// ���_�V�F�[�_�[
	ComObject::InputLayout m_layout;																// ���̓��C�A�E�g
	std::vector<InputElementDesc> m_elements;													// ���_�Ɏ���������Q
	ComObject::ClassLinkage m_classLinkage;													// �N���X�����P�[�W
	std::unordered_map<std::string, ComObject::ClassInstance> m_classInstances;	// �N���X�C���X�^���X
protected:
	std::string m_path;																				// �V�F�[�_�[�t�@�C���̃p�X
	std::string m_funcName;																			// �V�F�[�_�[�t�@�C���̃G���g���֐���
	std::string m_version;																			// �V�F�[�_�[�t�@�C���̎g�p����V�F�[�_�[���f���o�[�W����
public:
	DXVertexShaderLink();
	~DXVertexShaderLink();
protected:
	bool IsCsoFile();
	void CompileShader(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void ReadCompiledShaderFile(void*& shaderObject, std::size_t& shaderObjectSize);
	void CompileShaderFromFile(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void CreateClassLinkage(DirectXDevice* device);
	void CreateVertexShader(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize);
	void CreateInputLayout(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize);
	void ReleaseBlobData(DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
public:
	void Create(DirectXDevice* device);							// �ݒ肳��Ă�����Œ��_�V�F�[�_�[���쐬����
	void CreateAndCompile(DirectXDevice* device);			// �ݒ肳��Ă�����ŃV�F�[�_�[�t�@�C�����R���p�C�����Ă��璸�_�V�F�[�_�[���쐬����
	void CreateClassInstance(const std::string& key);		// ���I�V�F�[�_�[�����N�̃N���X�C���X�^���X���쐬����
	void GetClassInstance(const std::string& key);			// ���I�V�F�[�_�[�����N�̃N���X�C���X�^���X���擾����
	void SetFilePath(const std::string& path);
	void SetEntryFuncName(const std::string& funcName);
	void SetVersion(const std::string& version);
	void SetInputElements(InputElementDesc* elements, const std::size_t& size);
public:
	void VSSetShader(DirectXDeviceContext* context, const std::string& key);
	void IASetInputLayout(DirectXDeviceContext* context);
};

/*
* ===========================
* DXVertexShader Example
* ===========================
* 
* // ���͑����̒�`
* std::array<InputElementDesc, 3> elements{ {
*		InputElements::PositionElement,
*		InputElements::NormalElement,
*		InputElements::ColorElement,
* } };
* 
* // �C���X�^���X�̍쐬
* DXVertexShader vs;
* 
* // ���O���̐ݒ�
* vs.SetFilePath("main_vs.hlsl");
* vs.SetEntryFuncName("main");
* vs.SetVersion("vs_5_0");
* vs.SetInputElements(elements.data(), elements.size());
* 
* // �쐬
* vs.Create(DirectXManager::GetDevice());
* 
* // �g�p
* vs.VSSetShader(DirectXManager::GetImmediateContext().Get());
* vs.IASstInputLayout(DirectXManager::GetImmediateContext().Get());
* 
*/



/*
* ===========================
* DXVertexShaderLink Example
* ===========================
* 
* * // ���͑����̒�`
* std::array<InputElementDesc, 3> elements{ {
*		InputElements::PositionElement,
*		InputElements::NormalElement,
*		InputElements::ColorElement,
* } };
* 
* // �C���X�^���X�̍쐬
* DXVertexShader vs;
* 
* // ���O���̐ݒ�
* vs.SetFilePath("main_vs.hlsl");
* vs.SetEntryFuncName("main");
* vs.SetVersion("vs_5_0");
* vs.SetInputElements(elements.data(), elements.size());
* 
* // �쐬
* vs.Create(DirectXManager::GetDevice());
* 
* // �N���X�C���X�^���X�̍쐬
* // �f�[�^�����o�[�i�����o�ϐ��j������ꍇ��GetClassInstance���\�b�h���g�p����
* // �f�[�^�����o�[���Ȃ��ꍇ��CreateClassInstance���\�b�h���g�p����
* vs.CreateClassInstance(
*	"HLSL�V�F�[�_�[����Interface���p������f�[�^�����o�[�������Ȃ��N���X��"
* );
* vs.GetClassInstance(
*	"HLSL�V�F�[�_�[����Interface���p������f�[�^�����o�[�����N���X���萔�o�b�t�@���ɐ錾����Ă���ꍇ�A���̃C���X�^���X��"
*  // �錾����Ă��Ȃ��ꍇ�͒萔�o�b�t�@���ɃC���X�^���X�̐錾������K�v������
* );
* 
* // �g�p
* ��1�FCreateClassInstance���\�b�h/GetClassInstance���\�b�h���g�p�����ۂ̈����̕�����Ɠ���������
* vs.VSSetShader(DirectXManager::GetImmediateContext().Get(), "��1");
* vs.IASstInputLayout(DirectXManager::GetImmediateContext().Get(), "��1");
*
*/