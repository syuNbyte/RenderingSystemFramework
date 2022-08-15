#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"SafetyDataType.h"

// DirectXの頂点バッファを扱うクラス
class DXVertexBuffer {
protected:
	ComObject::VertexBuffer m_buf;	// 頂点バッファ
protected:
	BufferDesc m_desc;					// 頂点バッファ作成時のBufferDesc
protected:
	uint_t m_stride;						// 頂点情報1つのバイト数
	uint_t m_offset;						// 頂点情報間のオフセットバイト数　※基本的にゼロ
	uint_t m_quantity;					// 頂点数
public:
	DXVertexBuffer();
public:
	// ※instanceは頂点データを格納する配列などの先頭アドレス
	void Create(DirectXDevice* device, void* instance);

	// 頂点情報のデータサイズを設定
	void SetStride(const std::size_t& stride);

	// 頂点情報間のオフセットバイト数を設定
	void SetOffset(const std::size_t& offset);

	// 頂点数を設定
	void SetQuantity(const std::size_t& quantity);

	// 作成済みの頂点バッファか判定する
	bool IsCreated();
public:
	// 頂点バッファのデータを更新する
	void UpdateVertexBuffer(DirectXDeviceContext* context, void* instance);

	// IASetVertexBufferのラッパー
	void IASetVertexBuffer(DirectXDeviceContext* context);
};