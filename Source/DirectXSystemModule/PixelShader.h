#pragma once

// STL
#include<string>
#include<unordered_map>

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"SafetyDataType.h"

// DirectX�̃s�N�Z���V�F�[�_�[�N���X���p������C���^�[�t�F�C�X�N���X
class IDXPixelShader {
public:
	virtual void SetFilePath(const std::string& path) = 0;
	virtual void SetEntryFuncName(const std::string& funcName) = 0;
	virtual void SetVersion(const std::string& version) = 0;
	virtual void Create(DirectXDevice* device) = 0;
};

// DirectX�̃s�N�Z���V�F�[�_�[�������N���X
class DXPixelShader : public IDXPixelShader {
protected:
	ComObject::PixelShader m_ps;
protected:
	std::string m_path;								// �V�F�[�_�[�t�@�C���̃p�X
	std::string m_funcName;							// �V�F�[�_�[�t�@�C���̃G���g���֐���
	std::string m_version;							// �V�F�[�_�[�t�@�C���̎g�p����V�F�[�_�[���f���o�[�W����
public:
	DXPixelShader();
	~DXPixelShader();
protected:
	bool IsCsoFile();
	void CompileShader(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void ReadCompiledShaderFile(void*& shaderObject, std::size_t& shaderObjectSize);
	void CompileShaderFromFile(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void CreatePixelShader(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize);
public:
	void Create(DirectXDevice* device);					// �ݒ肳��Ă�����Ńs�N�Z���V�F�[�_�[���쐬����
	void CreateAndCompile(DirectXDevice* device);	// �ݒ肳��Ă�����ŃV�F�[�_�[�t�@�C�����R���p�C�����Ă���s�N�Z���V�F�[�_�[���쐬����
	void SetFilePath(const std::string& path);
	void SetEntryFuncName(const std::string& funcName);
	void SetVersion(const std::string& version);
public:
	void PSSetShader(DirectXDeviceContext* context);
};


// �N���X�����P�[�W�p�ɒ������ꂽDirectX�̒��_�V�F�[�_�[�������N���X
class DXPixelShaderLink : public IDXPixelShader {
protected:
	ComObject::PixelShader m_ps;																	// �s�N�Z���V�F�[�_�[
	ComObject::ClassLinkage m_classLinkage;													// �N���X�����P�[�W
	std::unordered_map<std::string, ComObject::ClassInstance> m_classInstances;	// �N���X�C���X�^���X
protected:
	std::string m_path;																				// �V�F�[�_�[�t�@�C���̃p�X
	std::string m_funcName;																			// �V�F�[�_�[�t�@�C���̃G���g���֐���
	std::string m_version;																			// �V�F�[�_�[�t�@�C���̎g�p����V�F�[�_�[���f���o�[�W����
public:
	DXPixelShaderLink();
	~DXPixelShaderLink();
protected:
	bool IsCsoFile();
	void CompileShader(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void ReadCompiledShaderFile(void*& shaderObject, std::size_t& shaderObjectSize);
	void CompileShaderFromFile(void*& shaderObject, std::size_t& shaderObjectSize, DirectXBinaryObject*& blob, DirectXBinaryObject*& errorBlob);
	void CreateClassLinkage(DirectXDevice* device);
	void CreatePixelShader(DirectXDevice* device, void*& shaderObject, std::size_t& shaderObjectSize);
public:
	void Create(DirectXDevice* device);						// �ݒ肳��Ă�����Ńs�N�Z���V�F�[�_�[���쐬����
	void CreateAndCompile(DirectXDevice* device);		// �ݒ肳��Ă�����ŃV�F�[�_�[�t�@�C�����R���p�C�����Ă���s�N�Z���V�F�[�_�[���쐬����
	void CreateClassInstance(const std::string& key);	// ���I�V�F�[�_�[�����N�̃N���X�C���X�^���X���쐬����
	void GetClassInstance(const std::string& key);		// ���I�V�F�[�_�[�����N�̃N���X�C���X�^���X���擾����
	void SetFilePath(const std::string& path);
	void SetEntryFuncName(const std::string& funcName);
	void SetVersion(const std::string& version);
public:
	void PSSetShader(DirectXDeviceContext* context, const std::string& key);
};

/*
* ===========================
* DXVertexShader Example
* ===========================
* 
* // �C���X�^���X�̍쐬
* DXVertexShader ps;
*
* // ���O���̐ݒ�
* ps.SetFilePath("main_ps.hlsl");
* ps.SetEntryFuncName("main");
* ps.SetVersion("ps_5_0");
*
* // �쐬
* ps.Create(DirectXManager::GetDevice());
*
* // �g�p
* ps.psSetShader(DirectXManager::GetImmediateContext().Get());
*
*/



/*
* ===========================
* DXVertexShaderLink Example
* ===========================
* 
* // �C���X�^���X�̍쐬
* DXVertexShader ps;
*
* // ���O���̐ݒ�
* ps.SetFilePath("main_ps.hlsl");
* ps.SetEntryFuncName("main");
* ps.SetVersion("ps_5_0");
*
* // �쐬
* ps.Create(DirectXManager::GetDevice());
*
* // �N���X�C���X�^���X�̍쐬
* // �f�[�^�����o�[�i�����o�ϐ��j������ꍇ��GetClassInstance���\�b�h���g�p����
* // �f�[�^�����o�[���Ȃ��ꍇ��CreateClassInstance���\�b�h���g�p����
* ps.CreateClassInstance(
*	"HLSL�V�F�[�_�[����Interface���p������f�[�^�����o�[�������Ȃ��N���X��"
* );
* ps.GetClassInstance(
*	"HLSL�V�F�[�_�[����Interface���p������f�[�^�����o�[�����N���X���萔�o�b�t�@���ɐ錾����Ă���ꍇ�A���̃C���X�^���X��"
*  // �錾����Ă��Ȃ��ꍇ�͒萔�o�b�t�@���ɃC���X�^���X�̐錾������K�v������
* );
*
* // �g�p
* ��1�FCreateClassInstance���\�b�h/GetClassInstance���\�b�h���g�p�����ۂ̈����̕�����Ɠ���������
* ps.psSetShader(DirectXManager::GetImmediateContext().Get(), "��1");
*
*/