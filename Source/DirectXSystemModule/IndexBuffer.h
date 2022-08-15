#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"SafetyDataType.h"

// DirectXのインデックスバッファを扱うクラス
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
	// ※instanceはインデックスデータを格納する配列などの先頭アドレス
	void Create(DirectXDevice* device, void* instance);
	
	// インデックス情報間のオフセットバイト数を設定
	void SetOffset(const size_t& offset);

	// 頂点数を設定
	void SetQuantity(const std::size_t& quantity);

	// 作成済みのインデックスバッファか判定する
	bool IsCreated();
public:
	// インデックスバッファのデータを更新する
	void UpdateIndexBuffer(DirectXDeviceContext* context, void* instance);

	// IASetIndexBufferのラッパー
	void IASetIndexBuffer(DirectXDeviceContext* context);
};