
// Reference
// http://maverickproj.web.fc2.com/d3d11_10.html

// Include
#include"ConstantBuffer.hlsl"
#include"DeferredTextureSampler.hlsl"

struct VsIn {
	float4 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VsOut {
	float4 position	 : SV_POSITION;
	float2 uv			 : TEXCOORD0;
	float4 tex0			 : TEXCOORD1;
	float4 tex1			 : TEXCOORD2;
	float4 tex2			 : TEXCOORD3;
	float4 tex3			 : TEXCOORD4;
	float4 tex4			 : TEXCOORD5;
	float4 tex5			 : TEXCOORD6;
	float4 tex6			 : TEXCOORD7;
	float4 tex7			 : TEXCOORD8;
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

interface GausBlurBase {
	VsOut GetBlurResult(VsIn vsIn);
};

// 横方向ブラー用の処理
class GausBlurX : GausBlurBase {
	VsOut GetBlurResult(VsIn vsIn) {
		// インスタンスの構築
		VsOut vsOut;

		// テクスチャサイズの取得
		float2 texSize;
		float level;
		ResultTexture.GetDimensions(0, texSize.x, texSize.y, level);

		// データを代入
		vsOut.position = ConvertToScreenCoord(vsIn.pos);
		vsOut.uv = vsIn.uv;
		vsOut.tex0.xy = float2( 1.0f / texSize.x, 0.0f);	// 1テクセル右にずらすオフセット
		vsOut.tex1.xy = float2( 3.0f / texSize.x, 0.0f);	// 3テクセル右にずらすオフセット
		vsOut.tex2.xy = float2( 5.0f / texSize.x, 0.0f);	// 5テクセル右にずらすオフセット
		vsOut.tex3.xy = float2( 7.0f / texSize.x, 0.0f);	// 7テクセル右にずらすオフセット
		vsOut.tex4.xy = float2( 9.0f / texSize.x, 0.0f);	// 9テクセル右にずらすオフセット
		vsOut.tex5.xy = float2(11.0f / texSize.x, 0.0f);	// 11テクセル右にずらすオフセット
		vsOut.tex6.xy = float2(13.0f / texSize.x, 0.0f);	// 13テクセル右にずらすオフセット
		vsOut.tex7.xy = float2(15.0f / texSize.x, 0.0f);	// 15テクセル右にずらすオフセット

		vsOut.tex0.zw = vsOut.tex0.xy * -1.0f;				// 1テクセル左にずらすオフセット
		vsOut.tex1.zw = vsOut.tex1.xy * -1.0f;				// 3テクセル左にずらすオフセット
		vsOut.tex2.zw = vsOut.tex2.xy * -1.0f;				// 5テクセル左にずらすオフセット
		vsOut.tex3.zw = vsOut.tex3.xy * -1.0f;				// 7テクセル左にずらすオフセット
		vsOut.tex4.zw = vsOut.tex4.xy * -1.0f;				// 9テクセル左にずらすオフセット
		vsOut.tex5.zw = vsOut.tex5.xy * -1.0f;				// 11テクセル左にずらすオフセット
		vsOut.tex6.zw = vsOut.tex6.xy * -1.0f;				// 13テクセル左にずらすオフセット
		vsOut.tex7.zw = vsOut.tex7.xy * -1.0f;				// 15テクセル左にずらすオフセット
		
		// オフセットに基準テクセルのUV座標を足し算して
		// 実際にサンプリングするUV座標に変換する
		vsOut.tex0 += float4(vsIn.uv, vsIn.uv);
		vsOut.tex1 += float4(vsIn.uv, vsIn.uv);
		vsOut.tex2 += float4(vsIn.uv, vsIn.uv);
		vsOut.tex3 += float4(vsIn.uv, vsIn.uv);
		vsOut.tex4 += float4(vsIn.uv, vsIn.uv);
		vsOut.tex5 += float4(vsIn.uv, vsIn.uv);
		vsOut.tex6 += float4(vsIn.uv, vsIn.uv);
		vsOut.tex7 += float4(vsIn.uv, vsIn.uv);

		return vsOut;
	}
};

// 縦方向ブラー用の処理
class GausBlurY : GausBlurBase {
	VsOut GetBlurResult(VsIn vsIn) {
		// インスタンスの構築
		VsOut vsOut;

		// テクスチャサイズの取得
		float2 texSize;
		float level;
		ResultTexture.GetDimensions(0, texSize.x, texSize.y, level);

		// データを代入
		vsOut.position = ConvertToScreenCoord(vsIn.pos);
		vsOut.uv = vsIn.uv;
		vsOut.tex0.xy = float2(0.0f,  1.0f / texSize.y);	// 1テクセル下にずらすオフセット
		vsOut.tex1.xy = float2(0.0f,  3.0f / texSize.y);	// 3テクセル下にずらすオフセット
		vsOut.tex2.xy = float2(0.0f,  5.0f / texSize.y);	// 5テクセル下にずらすオフセット
		vsOut.tex3.xy = float2(0.0f,  7.0f / texSize.y);	// 7テクセル下にずらすオフセット
		vsOut.tex4.xy = float2(0.0f,  9.0f / texSize.y);	// 9テクセル下にずらすオフセット
		vsOut.tex5.xy = float2(0.0f, 11.0f / texSize.y);	// 11テクセル下にずらすオフセット
		vsOut.tex6.xy = float2(0.0f, 13.0f / texSize.y);	// 13テクセル下にずらすオフセット
		vsOut.tex7.xy = float2(0.0f, 15.0f / texSize.y);	// 15テクセル下にずらすオフセット

		vsOut.tex0.zw = vsOut.tex0.xy * -1.0f;					// 1テクセル上にずらすオフセット
		vsOut.tex1.zw = vsOut.tex1.xy * -1.0f;					// 3テクセル上にずらすオフセット
		vsOut.tex2.zw = vsOut.tex2.xy * -1.0f;					// 5テクセル上にずらすオフセット
		vsOut.tex3.zw = vsOut.tex3.xy * -1.0f;					// 7テクセル上にずらすオフセット
		vsOut.tex4.zw = vsOut.tex4.xy * -1.0f;					// 9テクセル上にずらすオフセット
		vsOut.tex5.zw = vsOut.tex5.xy * -1.0f;					// 11テクセル上にずらすオフセット
		vsOut.tex6.zw = vsOut.tex6.xy * -1.0f;					// 13テクセル上にずらすオフセット
		vsOut.tex7.zw = vsOut.tex7.xy * -1.0f;					// 15テクセル上にずらすオフセット

		// オフセットに基準テクセルのUV座標を足し算して
		// 実際にサンプリングするUV座標に変換する
		vsOut.tex0 += float4(vsIn.uv, vsIn.uv);
		vsOut.tex1 += float4(vsIn.uv, vsIn.uv);
		vsOut.tex2 += float4(vsIn.uv, vsIn.uv);
		vsOut.tex3 += float4(vsIn.uv, vsIn.uv);
		vsOut.tex4 += float4(vsIn.uv, vsIn.uv);
		vsOut.tex5 += float4(vsIn.uv, vsIn.uv);
		vsOut.tex6 += float4(vsIn.uv, vsIn.uv);
		vsOut.tex7 += float4(vsIn.uv, vsIn.uv);

		return vsOut;
	}
};

// ブラー処理なし
class NoBlur : GausBlurBase {
	VsOut GetBlurResult(VsIn vsIn) {
		// インスタンスの構築
		VsOut vsOut;

		// データを代入
		vsOut.position = ConvertToScreenCoord(vsIn.pos);
		vsOut.uv = vsIn.uv;

		return vsOut;
	}
};

GausBlurBase base;

// 縦横ガウスブラー
VsOut main(VsIn vsIn) {
	return base.GetBlurResult(vsIn);
}