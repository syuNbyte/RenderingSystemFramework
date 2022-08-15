#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"SafetyDataType.h"

// DirectX�̒萔�o�b�t�@�������N���X
class DXConstantBuffer {
protected:
	ComObject::ConstantBuffer m_cbuf;	// �萔�o�b�t�@�{��
	BufferDesc m_desc;						// �萔�o�b�t�@�̍쐬����BufferDesc
protected:
	std::size_t m_size;						// �萔�o�b�t�@�̃T�C�Y
	int_t m_registNumber;					// �萔�o�b�t�@�̎g�p���Ă��郌�W�X�^�ԍ�
protected:
	int_t m_cpuAccessFlag;					// �萔�o�b�t�@���쐬����ۂ�CreateMapUnmap���g�p������CreateUpdateSubReesource���g�p��������ۑ�����t���O
public:
	DXConstantBuffer();
public:
	// Map/Unmap�ōX�V����萔�o�b�t�@���쐬����
	void CreateMapUnmap(DirectXDevice* device);

	// UpdateSubResource�ōX�V����萔�o�b�t�@���쐬����
	void CreateUpdateSubResource(DirectXDevice* device);

	// �萔�o�b�t�@�̃T�C�Y��ݒ肷��
	void SetBufferSize(const std::size_t size);

	// �V�F�[�_�[���Ŏg�p����Ă���萔�o�b�t�@�̔ԍ���ݒ肷��
	void SetRegisterNumber(const int_t& registNumber);

	// �쐬�ς݂̒萔�o�b�t�@���𔻒肷��
	bool IsCreated();
public:
	// �K�؂ȕ��@�Œ萔�o�b�t�@���X�V����
	void Update(DirectXDeviceContext* context, void* instance);

	// Map / Unmap���g�p���Ē萔�o�b�t�@�̓��e���X�V����@��instance�ɑΏۂƂȂ���Ԃ̃A�h���X��n��
	void MapUnmap(DirectXDeviceContext* context, void* instance);

	// UpdateSubResource���g�p���Ē萔�o�b�t�@�̓��e���X�V����@��instance�ɑΏۂƂȂ���Ԃ̃A�h���X��n��
	void UpdateSubResource(DirectXDeviceContext* context, void* instance);
public:
	// MapUnmap / UpdateSubResource�̎��s��ɒ��_�V�F�[�_�[�̒萔�o�b�t�@�̓��e���X�V����
	void VSSetConstantBuffers(DirectXDeviceContext* context);

	// MapUnmap / UpdateSubResource�̎��s��Ƀs�N�Z���V�F�[�_�[�̒萔�o�b�t�@�̓��e���X�V����
	void PSSetConstantBuffers(DirectXDeviceContext* context);
};


// DXConstantBuffer��MapUnmap�o�[�W������UpdateSubResource�o�[�W�����ɕ������
// ��������������Ė��ʂȔ��ʏ����Ȃǂ��Ȃ������Ƃ��ł��邽�߂��ꂢ�ɋL�q���邱�Ƃ��\����
// �Ƃ肠�����A���͂���Ȃɂ��C�Ȃ�
// �����A�����N���X����t����Ƃ�����
// UpdateSubResource�o�[�W������DXConstantBuffer, MapUnmap�o�[�W������DXConstantBufferWrite