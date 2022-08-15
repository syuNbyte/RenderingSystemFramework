
// TNLIB
#include"Debug.hpp"

// MyFile
#include"IndexBuffer.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

DXIndexBuffer::DXIndexBuffer() {
	m_offset = 0U;
	m_quantity = 0U;
}

void DXIndexBuffer::Create(DirectXDevice* device, void* instance) {
	Debug::BracketBegin("DXIndexBuffer::Create Method");

	BufferDesc desc;
	SubResourceData subResource;

	// BufferDesc�̏�����
	{
		ZeroMemory(&desc, sizeof(BufferDesc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(uint_t) * m_quantity;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = NULL;
	}

	// SubResourceData�̏�����
	{
		ZeroMemory(&subResource, sizeof(SubResourceData));
		subResource.pSysMem = instance;
	}

	// IndexBuffer�̍쐬
	{
		// �ȑO�ɍ쐬����Ă���C���f�b�N�X�o�b�t�@�̏ꍇ�͉������
		// ����������ɍč쐬����Əo�̓E�B���h�E�ŉ���G���[���o�͂����
		if (m_buf) {
			m_buf.Reset();
			Debug::Log("This IndexBuffer was previously created.");
		}

		auto hr = device->CreateBuffer(
			&desc,
			&subResource,
			m_buf.GetAddressOf()
		);

		IF_HRS(hr) {
			m_desc = desc;
			Debug::Log("DXIndexBuffer::Create Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXIndexBuffer::Create Method.\n",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXIndexBuffer::Create Method.\n",
				hr
			);
#endif
		}
	}

	Debug::BracketEnd();
}

void DXIndexBuffer::SetOffset(const size_t& offset) {
	m_offset = static_cast<uint_t>(offset);
}

void DXIndexBuffer::SetQuantity(const std::size_t& quantity) {
	m_quantity = static_cast<uint_t>(quantity);
}

bool DXIndexBuffer::IsCreated() {
	return (m_buf);
}

void DXIndexBuffer::UpdateIndexBuffer(DirectXDeviceContext* context, void* instance) {
	MappedSubResource subResource;

// Map�Ńf�[�^���擾����
	auto hr = context->Map(
		m_buf.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&subResource
	);

	IF_HRS(hr) {
		// �f�[�^�̃R�s�[
		::memcpy_s(
			subResource.pData,
			subResource.RowPitch,
			instance,
			sizeof(uint_t) * m_quantity
		);

		// Unmap�Ńf�[�^�ւ̃A�N�Z�X���I������
		context->Unmap(
			m_buf.Get(),
			0
		);
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXIndexBuffer::UpdateIndexBuffer Method.\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXIndexBuffer::UpdateIndexBuffer Method.\n",
			hr
		);
#endif
	}
}

void DXIndexBuffer::IASetIndexBuffer(DirectXDeviceContext* context) {
	context->IASetIndexBuffer(
		m_buf.Get(),
		DXGI_FORMAT_R32_UINT,
		m_offset
	);
}
