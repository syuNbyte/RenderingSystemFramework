#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"SafetyDataType.h"

// DirectX�̃C���f�b�N�X�o�b�t�@�������N���X
class DXIndexBuffer {
protected:
	ComObject::IndexBuffer m_buf;
protected:
	BufferDesc m_desc;
protected:
	uint_t m_offset;
	uint_t m_quantity;
public:
	DXIndexBuffer();
public:
	// ��instance�̓C���f�b�N�X�f�[�^���i�[����z��Ȃǂ̐擪�A�h���X
	void Create(DirectXDevice* device, void* instance);
	
	// �C���f�b�N�X���Ԃ̃I�t�Z�b�g�o�C�g����ݒ�
	void SetOffset(const size_t& offset);

	// ���_����ݒ�
	void SetQuantity(const std::size_t& quantity);

	// �쐬�ς݂̃C���f�b�N�X�o�b�t�@�����肷��
	bool IsCreated();
public:
	// �C���f�b�N�X�o�b�t�@�̃f�[�^���X�V����
	void UpdateIndexBuffer(DirectXDeviceContext* context, void* instance);

	// IASetIndexBuffer�̃��b�p�[
	void IASetIndexBuffer(DirectXDeviceContext* context);
};