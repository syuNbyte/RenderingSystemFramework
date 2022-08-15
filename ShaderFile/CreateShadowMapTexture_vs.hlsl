
// Include
#include"ConstantBuffer.hlsl"

struct VsIn {
   float4 pos : POSITION;
   float3 normal : NORMAL;
   float3 tangent : TANGENT;
   float3 bitangent : BITANGENT;
   float2 uv : TEXCOORD;
   float4 color : COLOR;
};

struct VsOut {
   float4 position : SV_POSITION;
};

// 頂点座標をワールド座標に変換
float4 GetWorldConvertPosition(float4 vertexPos) {
   return mul(vertexPos, World);
}

// ワールド・ビュー・プロジェクション変換行列を適用した座標に変換（ライト用）
float4 GetLightWVPConvertPosition(float4 vertexPos) {
   return mul(mul(mul(vertexPos, World), LightView), LightProjection);
}

// main
VsOut main(VsIn vsIn) {
   VsOut vsOut = (VsOut)0;
   vsOut.position = GetLightWVPConvertPosition(vsIn.pos);
   return vsOut;
}