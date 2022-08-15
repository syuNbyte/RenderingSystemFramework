
// Include
#include"ConstantBuffer.hlsl"

struct VsOut {
	float4 position : SV_POSITION0;
	float2 uv : TEXCOORD0;
};

// ワールド・ビュー・プロジェクション変換行列を適用した座標に変換
float4 GetWVPConvertPosition(float4 vertexPos) {
	return mul(mul(mul(vertexPos, World), View), Projection);
}

// スクリーン全体にきれいに貼り付ける用の変換を行いたい
float4 TestConvert(float2 uv) {
	return float4(
		lerp(-1.0f, 1.0f, uv.x),
		lerp(1.0f, -1.0f, uv.y),
		0.0f,
		0.0f
	);
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

	// データを代入
	vsOut.position = ConvertToScreenCoord(pos);
	vsOut.uv = uv;

	// データを返す
	return vsOut;
}