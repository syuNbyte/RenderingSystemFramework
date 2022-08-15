#pragma once

// MyFile
#include"DirectXUtilityModule//DirectXDataTypes.h"

// DirectXのサンプラーを扱うクラス
class DXSampler {
protected:
	ComObject::SamplerState m_state;
protected:
	SamplerDesc m_desc;
protected:
	int_t m_registNumber;
public:
	DXSampler();
	DXSampler(const DXSampler& sampler);
	DXSampler(DXSampler&& sampler);
public:
	// サンプラーを作成する
	void Create(DirectXDevice* device, const SamplerDesc& desc);

	// シェーダー内で使用されているサンプラーステートの番号を設定する
	void SetRegistNumber(const int_t& registNumber);

	// 保持するサンプラーステートを取得する
	ComObject::SamplerState GetSamplerState();
public:
	// 頂点シェーダー内のサンプラーの情報を設定する
	void VSSetSampler(DirectXDeviceContext* context);

	// ピクセルシェーダー内のサンプラーの情報を設定する
	void PSSetSampler(DirectXDeviceContext* context);
};