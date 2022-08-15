#pragma once

// LibraryInclude
#pragma comment(lib, "DirectXTex.lib")

// DirectXTex
#include"DirectXTex.h"

// STL
#include<string>

// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXVector.h"

// DirectX��ShaderResourceView�������N���X
class DXShaderResourceView {
protected:
	ComObject::ShaderResourceView m_srv;	// �摜�f�[�^
	DirectX::TexMetadata m_metaData;			// �摜�̃��^�f�[�^
protected:
	int_t m_registNumber;						// �e�N�X�`���o�b�t�@�̎g�p���Ă��郌�W�X�^�ԍ�
private:
	std::string m_path;							// �e�N�X�`���̃f�B���N�g���p�X�i�}���`�o�C�g�j
	std::wstring m_wpath;						// �e�N�X�`���̃f�B���N�g���p�X�i���C�h�j
	DXVector2Int m_size;							// �e�N�X�`���̉摜�T�C�Y
private:
	void CreateWICTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);
	void CreateDDSTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);
	void CreateTGATextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);
	void CreateMipMapWICTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);
	void CreateMipMapDDSTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);
	void CreateMipMapTGATextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);
public:
	// �ʏ�̃e�N�X�`�����쐬����
	void CreateTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);

	// �݂��Ճ}�b�v�e�N�X�`�����쐬����
	void CreateMipMapTextureFromFile(DirectXDevice* device, DirectXDeviceContext* context);

	// �V�F�[�_�[�t�@�C�����Ŏg�p����Ă���e�N�X�`���o�b�t�@�̎g�p�ԍ���ݒ肷��
	void SetRegistNumber(const int_t& registNumber);

	// �g�p����e�N�X�`���t�@�C���̃f�B���N�g���p�X��ݒ肷��
	void SetFilePath(const std::string& path);

	// ���̃e�N�X�`�����L���ȃf�[�^��ێ����Ă��邩�𔻒肷��
	bool HasTexture();

	// ���̃e�N�X�`����ShaderResourceView���擾����
	ComObject::ShaderResourceView GetSRV();

	// ���̉摜�̃f�B���N�g���p�X���擾����
	std::string GetFilePath();
	std::wstring GetFilePathWide();

	// ���̉摜�̃T�C�Y���擾����
	DXVector2Int GetSize();
public:
	// ���_�V�F�[�_�[�ɂ��̃e�N�X�`����ݒ肷��
	void VSSetShaderResources(DirectXDeviceContext* context);

	// �s�N�Z���V�F�[�_�[�Ɏq�̃e�N�X�`����ݒ肷��
	void PSSetShaderResources(DirectXDeviceContext* context);
};