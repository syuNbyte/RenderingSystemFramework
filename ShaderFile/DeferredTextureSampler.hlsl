
// 遅延レンダリング用のテクスチャとサンプラー

Texture2D<float4> AlbedoTexture : register(t0);			// 色情報テクスチャ
Texture2D<float4> NormalTexture : register(t1);			// 法線情報テクスチャ
Texture2D<float4> PositionTexture : register(t2);		// 座標テクスチャ
Texture2D<float4> DepthTexture : register(t3);			// 深度テクスチャ
Texture2D<float4> MaterialTexture :register(t4);		// 質感テクスチャ（ラフネス, メタルネス）
Texture2D<float4> BrightnessTexture : register(t5);	// 輝度テクスチャ

// 処理途中の結果を一時的に格納し
// 最終的にバックバッファに送るデータ
Texture2D<float4> ResultTexture : register(t6);			// 結果テクスチャ

SamplerState Sampler : register(s0);		// サンプラー