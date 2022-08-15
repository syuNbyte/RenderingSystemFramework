
// Include
#include"Struct.hlsl"
#include"ConstantBuffer.hlsl"

// 頂点座標をワールド座標に変換
float4 GetWorldConvertPosition(float4 vertexPos) {
   return mul(vertexPos, World);
}

// ワールド・ビュー・プロジェクション変換行列を適用した座標に変換
float4 GetWVPConvertPosition(float4 vertexPos) {
   return mul(mul(mul(vertexPos, World), View), Projection);
}

// ワールド座標での法線の向きに変換した法線を正規化して返す
float4 GetWorldNormal(float4 normal) {
   float4 N = normal;
   N.w = 0;

   // ワールド座標での法線の向きに変換した法線を正規化して返す
   return normalize(mul(N, World));
}

// main
VsOut main(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, float4 color : COLOR) {
	VsOut vsOut = (VsOut)0;
	vsOut.worldPosition = GetWorldConvertPosition(pos);
	vsOut.position = GetWVPConvertPosition(pos);
   vsOut.normal = GetWorldNormal(normal);
   vsOut.uv = uv;
   vsOut.color = color;
	return vsOut;
}