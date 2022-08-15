
// Include
#include"ConstantBuffer.hlsl"
#include"DeferredTextureSampler.hlsl"

struct VsOut {
	float4 position	 : SV_POSITION;
	float2 uvCenter	 : TEXCOORD0;
	float4 uvUp			 : TEXCOORD1;
	float4 uvDown		 : TEXCOORD2;
	float4 uvRight		 : TEXCOORD3;
	float4 uvLeft		 : TEXCOORD4;
	float4 uvRightUp	 : TEXCOORD5;
	float4 uvLeftUp	 : TEXCOORD6;
	float4 uvRightDown : TEXCOORD7;
	float4 uvLeftDown	 : TEXCOORD8;
};

// ワールド・ビュー・プロジェクション変換行列を適用した座標に変換
float4 GetWVPConvertPosition(float4 vertexPos) {
	return mul(mul(mul(vertexPos, World), View), Projection);
}

// スクリーン座標系に変換する
float4 ConvertToScreenCoord(float4 pos) {
	float4 wpos = mul(pos, World);
	return float4(
		((wpos.x / ViewPortSize.x) * 2 - 1),
		-(1 - (wpos.y / ViewPortSize.y) * 2),
		0.0f,
		wpos.w
		);
}

VsOut main(float4 pos : POSITION, float2 uv : TEXCOORD) {
	// インスタンスの構築
	VsOut vsOut;

	// テクスチャサイズの取得
	float2 texSize;
	float level;
	NormalTexture.GetDimensions(0, texSize.x, texSize.y, level);

	// データを代入
	vsOut.position = ConvertToScreenCoord(pos);

	// 真ん中のピクセル
	vsOut.uvCenter = uv;

	// 法線テクスチャから現在選択中のUV座標付近のピクセルのUV情報を取得する
	{
		// 中心ピクセルからのオフセット
		float offset = 0.2f;

		// 上のピクセル
		vsOut.uvUp.xy = uv + float2(0.0f, -offset / texSize.y);

		// 下のピクセル
		vsOut.uvDown.xy = uv + float2(0.0f, offset / texSize.y);

		// 右のピクセル
		vsOut.uvRight.xy = uv + float2(offset / texSize.x, 0.0f);

		// 左のピクセル
		vsOut.uvLeft.xy = uv + float2(-offset / texSize.x, 0.0f);

		// 右上のピクセル
		vsOut.uvRightUp.xy = uv + float2(offset / texSize.x, -offset / texSize.y);

		// 左上のピクセル
		vsOut.uvLeftUp.xy = uv + float2(-offset / texSize.x, -offset / texSize.y);

		// 右下のピクセル
		vsOut.uvRightDown.xy = uv + float2(offset / texSize.x, offset / texSize.y);

		// 左下のピクセル
		vsOut.uvLeftDown.xy = uv + float2(-offset / texSize.x, offset / texSize.y);
	}

	// 深度テクスチャから現在選択中のUV座標付近のピクセルのUV情報を取得する
	{
		// 中心ピクセルからのオフセット
		float offset = 1.0f;

		// 上のピクセル
		vsOut.uvUp.zw = uv + float2(0.0f, -offset / texSize.y);

		// 下のピクセル
		vsOut.uvDown.zw = uv + float2(0.0f, offset / texSize.y);

		// 右のピクセル
		vsOut.uvRight.zw = uv + float2(offset / texSize.x, 0.0f);

		// 左のピクセル
		vsOut.uvLeft.zw = uv + float2(-offset / texSize.x, 0.0f);

		// 右上のピクセル
		vsOut.uvRightUp.zw = uv + float2(offset / texSize.x, -offset / texSize.y);

		// 左上のピクセル
		vsOut.uvLeftUp.zw = uv + float2(-offset / texSize.x, -offset / texSize.y);

		// 右下のピクセル
		vsOut.uvRightDown.zw = uv + float2(offset / texSize.x, offset / texSize.y);

		// 左下のピクセル
		vsOut.uvLeftDown.zw = uv + float2(-offset / texSize.x, offset / texSize.y);
	}

	// データを返す
	return vsOut;
}