
// 一つのテクスチャとサンプラーのセット

Texture2D<float4> Texture				 : register(t0);	// テクスチャ
Texture2D<float4> NormalTexture		 : register(t1);	// 法線テクスチャ

Texture2D<float4> ShadowMapTexture	 : register(t2);	// シャドウマップ用のライト

SamplerState Sampler : register(s0);	// サンプラー