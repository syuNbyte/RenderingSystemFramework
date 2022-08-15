#pragma once

// MyFile
#include"DirectXUtilityModule/DirectXDataTypes.h"
#include"SafetyDataType.h"

// DirectXの定数バッファを扱うクラス
class DXConstantBuffer {
protected:
	ComObject::ConstantBuffer m_cbuf;	// 定数バッファ本体
	BufferDesc m_desc;						// 定数バッファの作成時のBufferDesc
protected:
	std::size_t m_size;						// 定数バッファのサイズ
	int_t m_registNumber;					// 定数バッファの使用しているレジスタ番号
protected:
	int_t m_cpuAccessFlag;					// 定数バッファを作成する際にCreateMapUnmapを使用したかCreateUpdateSubReesourceを使用したかを保存するフラグ
public:
	DXConstantBuffer();
public:
	// Map/Unmapで更新する定数バッファを作成する
	void CreateMapUnmap(DirectXDevice* device);

	// UpdateSubResourceで更新する定数バッファを作成する
	void CreateUpdateSubResource(DirectXDevice* device);

	// 定数バッファのサイズを設定する
	void SetBufferSize(const std::size_t size);

	// シェーダー内で使用されている定数バッファの番号を設定する
	void SetRegisterNumber(const int_t& registNumber);

	// 作成済みの定数バッファかを判定する
	bool IsCreated();
public:
	// 適切な方法で定数バッファを更新する
	void Update(DirectXDeviceContext* context, void* instance);

	// Map / Unmapを使用して定数バッファの内容を更新する　※instanceに対象となる実態のアドレスを渡す
	void MapUnmap(DirectXDeviceContext* context, void* instance);

	// UpdateSubResourceを使用して定数バッファの内容を更新する　※instanceに対象となる実態のアドレスを渡す
	void UpdateSubResource(DirectXDeviceContext* context, void* instance);
public:
	// MapUnmap / UpdateSubResourceの実行後に頂点シェーダーの定数バッファの内容を更新する
	void VSSetConstantBuffers(DirectXDeviceContext* context);

	// MapUnmap / UpdateSubResourceの実行後にピクセルシェーダーの定数バッファの内容を更新する
	void PSSetConstantBuffers(DirectXDeviceContext* context);
};


// DXConstantBufferをMapUnmapバージョンとUpdateSubResourceバージョンに分けると
// 処理が分割されて無駄な判別処理などをなくすことができるためきれいに記述することが可能だが
// とりあえず、今はそんなにやる気ない
// ただ、もしクラス名を付けるとしたら
// UpdateSubResourceバージョンがDXConstantBuffer, MapUnmapバージョンがDXConstantBufferWrite