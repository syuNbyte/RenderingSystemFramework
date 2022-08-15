#pragma once

// STL
#include<string>
#include<memory>

// TNLIB
#include"SafetyDataType.h"

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"DirectXUtilityModule/DirectXVector.h"

// DirectX�̃e�N�X�`���������N���X
class DXTexture {
private:
	ComObject::ShaderResourceView m_srv;	// �摜�f�[�^
protected:
	int_t m_registNumber;						// �e�N�X�`���o�b�t�@�̎g�p���Ă��郌�W�X�^�ԍ�
private:
	std::string m_path;							// �e�N�X�`���̃f�B���N�g���p�X
	DXVector2Int m_size;							// �e�N�X�`���̉摜�T�C�Y
public:
	// �쐬����
	void Create(DirectXDevice* device, DirectXDeviceContext* context, const std::string& path);

	// �V�F�[�_�[�t�@�C�����Ŏg�p����Ă���e�N�X�`���o�b�t�@�̎g�p�ԍ���ݒ肷��
	void SetRegistNumber(const int_t& registNumber);

	// ���̃e�N�X�`�����L���ȃf�[�^��ێ����Ă��邩�𔻒肷��
	bool HasTexture();

	// ���̃e�N�X�`����ShaderResourceView���擾����
	ComObject::ShaderResourceView GetSRV();

	// ���̉摜�̃f�B���N�g���p�X���擾����
	std::string GetFilePath();

	// ���̉摜�̃T�C�Y���擾����
	DXVector2Int GetSize();
public:
	// ���_�V�F�[�_�[�ɂ��̃e�N�X�`����ݒ肷��
	void VSSetShaderResources(DirectXDeviceContext* context);

	// �s�N�Z���V�F�[�_�[�Ɏq�̃e�N�X�`����ݒ肷��
	void PSSetShaderResources(DirectXDeviceContext* context);
};