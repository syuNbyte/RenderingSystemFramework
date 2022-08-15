#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"SafetyDataType.h"

// DirectX�̒��_�o�b�t�@�������N���X
class DXVertexBuffer {
protected:
	ComObject::VertexBuffer m_buf;	// ���_�o�b�t�@
protected:
	BufferDesc m_desc;					// ���_�o�b�t�@�쐬����BufferDesc
protected:
	uint_t m_stride;						// ���_���1�̃o�C�g��
	uint_t m_offset;						// ���_���Ԃ̃I�t�Z�b�g�o�C�g���@����{�I�Ƀ[��
	uint_t m_quantity;					// ���_��
public:
	DXVertexBuffer();
public:
	// ��instance�͒��_�f�[�^���i�[����z��Ȃǂ̐擪�A�h���X
	void Create(DirectXDevice* device, void* instance);

	// ���_���̃f�[�^�T�C�Y��ݒ�
	void SetStride(const std::size_t& stride);

	// ���_���Ԃ̃I�t�Z�b�g�o�C�g����ݒ�
	void SetOffset(const std::size_t& offset);

	// ���_����ݒ�
	void SetQuantity(const std::size_t& quantity);

	// �쐬�ς݂̒��_�o�b�t�@�����肷��
	bool IsCreated();
public:
	// ���_�o�b�t�@�̃f�[�^���X�V����
	void UpdateVertexBuffer(DirectXDeviceContext* context, void* instance);

	// IASetVertexBuffer�̃��b�p�[
	void IASetVertexBuffer(DirectXDeviceContext* context);
};