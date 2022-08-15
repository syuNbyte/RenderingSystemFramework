
// TNLIB
#include"Debug.hpp"

// MyFile
#include"VertexBuffer.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

DXVertexBuffer::DXVertexBuffer() {
	m_stride = 0;
	m_offset = 0;
	m_quantity = 0;
}

void DXVertexBuffer::Create(DirectXDevice* device, void* instance) {
	Debug::BracketBegin("DXVertexBuffer::Create Method");

	BufferDesc desc;
	SubResourceData subResource;

	// BufferDesc�̏�����
	{
		ZeroMemory(&desc, sizeof(BufferDesc));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = m_stride * m_quantity;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	// SubResourceData�̏�����
	{
		ZeroMemory(&subResource, sizeof(SubResourceData));
		subResource.pSysMem = instance;
	}

	// VertexBuffer�̍쐬
	{
		// �ȑO�ɍ쐬����Ă��钸�_�o�b�t�@�̏ꍇ�͉������
		// ����������ɍč쐬����Əo�̓E�B���h�E�ŉ���G���[���o�͂����
		if (m_buf) {
			m_buf.Reset();
			Debug::Log("This VertexBuffer was previously created.");
		}

		auto hr = device->CreateBuffer(
			&desc,
			&subResource,
			m_buf.GetAddressOf()
		);

		IF_HRS(hr) {
			m_desc = desc;
			Debug::Log("DXVertexBuffer::Create Method Success.");
		}

		IF_HRF(hr) {
			Debug::LogError(
				"DXVertexBuffer::Create Method.\n",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXVertexBuffer::Create Method.\n",
				hr
			);
#endif
		}
	}

	Debug::BracketEnd();
}

void DXVertexBuffer::SetStride(const std::size_t& stride) {
	m_stride = static_cast<uint_t>(stride);
}

void DXVertexBuffer::SetOffset(const std::size_t& offset) {
	m_offset = static_cast<uint_t>(offset);
}

void DXVertexBuffer::SetQuantity(const std::size_t& quantity) {
	m_quantity = static_cast<uint_t>(quantity);
}

bool DXVertexBuffer::IsCreated() {
	return (m_buf);
}

void DXVertexBuffer::UpdateVertexBuffer(DirectXDeviceContext* context, void* instance) {
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
			m_stride * m_quantity
		);

		// Unmap�Ńf�[�^�ւ̃A�N�Z�X���I������
		context->Unmap(
			m_buf.Get(),
			0
		);
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXVertexBuffer::UpdateVertexBuffer Method.\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXVertexBuffer::UpdateVertexBuffer Method.\n",
			hr
		);
#endif
	}
}

void DXVertexBuffer::IASetVertexBuffer(DirectXDeviceContext* context) {
	context->IASetVertexBuffers(
		0U,
		1U,
		m_buf.GetAddressOf(),
		&m_stride,
		&m_offset
	);
}
