
// TNLIB
#include"Debug.hpp"

// MyFile
#include"ConstantBuffer.h"
#include"DirectXUtilityModule/HresultUtility.h"
#include"DirectXUtilityModule/MessageBoxUtility.h"

DXConstantBuffer::DXConstantBuffer() {
	m_size = 0;
	m_registNumber = -1;
	m_cpuAccessFlag = -1;
}

void DXConstantBuffer::CreateMapUnmap(DirectXDevice* device) {
	Debug::BracketBegin("DXConstantBuffer::CreateMapUnmap");

	BufferDesc desc;

	// BufferDescの初期化
	{
		ZeroMemory(&desc, sizeof(BufferDesc));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = m_size;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPUからの書き込みを許可
	}

	// ConstantBufferの作成
	{
		auto hr = device->CreateBuffer(
			&desc,
			nullptr,
			m_cbuf.GetAddressOf()
		);

		IF_HRS(hr) {
			m_desc = desc;
			m_cpuAccessFlag = 0;
			Debug::Log("DXConstantBuffer::CreateMapUnmap Method Success.");
		}

		IF_HRF(hr) {
			m_cpuAccessFlag = -1;

			Debug::LogError(
				"DXConstantBuffer::CreateMapUnmap Method.\n",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXConstantBuffer::CreateMapUnmap Method.\n",
				hr
			);
#endif
		}
	}

	Debug::BracketEnd();
}

void DXConstantBuffer::CreateUpdateSubResource(DirectXDevice* device) {
	Debug::BracketBegin("DXConstantBuffer::CreateUpdateSubResource");

	BufferDesc desc;

	// BufferDescの初期化
	{
		ZeroMemory(&desc, sizeof(BufferDesc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = m_size;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = NULL;
	}

	// ConstantBufferの作成
	{
		// 以前に作成されている定数バッファの場合は解放する
		// ※解放せずに再作成すると出力ウィンドウで解放エラーが出力される
		if (m_cbuf) {
			m_cbuf.Reset();
			Debug::Log("This ConstantBuffer was previously created.");
		}

		auto hr = device->CreateBuffer(
			&desc,
			nullptr,
			m_cbuf.GetAddressOf()
		);

		IF_HRS(hr) {
			m_desc = desc;
			m_cpuAccessFlag = 1;
			Debug::Log("DXConstantBuffer::CreateUpdateSubResource Method Success.");
		}

		IF_HRF(hr) {
			m_cpuAccessFlag = -1;

			Debug::LogError(
				"DXConstantBuffer::CreateUpdateSubResource Method.\n",
				HresultUtility::GetHresultMessage(hr),
				"\n",
				HresultUtility::GetHresultCodeMeaning(hr)
			);
#if _DEBUG
			MessageBoxUtility::HresultErrorMessage(
				"DXConstantBuffer::CreateUpdateSubResource Method.\n",
				hr
			);
#endif
		}
	}

	Debug::BracketEnd();
}

void DXConstantBuffer::SetBufferSize(const std::size_t size) {
	m_size = size;
}

void DXConstantBuffer::SetRegisterNumber(const int_t& registNumber) {
	m_registNumber = registNumber;
}

bool DXConstantBuffer::IsCreated() {
	return (m_cbuf);
}

void DXConstantBuffer::Update(DirectXDeviceContext* context, void* instance) {
	if (m_cpuAccessFlag != -1) {
		if (m_cpuAccessFlag == 0) MapUnmap(context, instance);
		else if (m_cpuAccessFlag == 1) UpdateSubResource(context, instance);
	}
}

void DXConstantBuffer::MapUnmap(DirectXDeviceContext* context, void* instance) {
	MappedSubResource subResource;

	// Mapでデータを取得する
	auto hr = context->Map(
		m_cbuf.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&subResource
	);

	IF_HRS(hr) {
		// データのコピー
		::memcpy_s(
			subResource.pData,
			subResource.RowPitch,
			instance,
			m_size
		);

		// Unmapでデータへのアクセスを終了する
		context->Unmap(
			m_cbuf.Get(),
			0
		);
	}

	IF_HRF(hr) {
		Debug::LogError(
			"DXConstantBuffer::MapUnmap Method.\n",
			HresultUtility::GetHresultMessage(hr) + "\n",
			HresultUtility::GetHresultCodeMeaning(hr)
		);
#if _DEBUG
		MessageBoxUtility::HresultErrorMessage(
			"DXConstantBuffer::MapUnmap Method.\n",
			hr
		);
#endif
	}
}

void DXConstantBuffer::UpdateSubResource(DirectXDeviceContext* context, void* instance) {
	context->UpdateSubresource(
		m_cbuf.Get(),
		0,
		nullptr,
		instance,
		0,
		0
	);
}

void DXConstantBuffer::VSSetConstantBuffers(DirectXDeviceContext* context) {
	context->VSSetConstantBuffers(
		m_registNumber,
		1,
		m_cbuf.GetAddressOf()
	);
}

void DXConstantBuffer::PSSetConstantBuffers(DirectXDeviceContext* context) {
	context->PSSetConstantBuffers(
		m_registNumber,
		1,
		m_cbuf.GetAddressOf()
	);
}
