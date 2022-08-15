
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
   float4 worldPosition : TEXCOORD;
   float4 normal : TEXCOORD1;
   float4 tangent : TEXCOORD2;
   float4 bitangent : TEXCOORD3;
   float2 uv : TEXCOORD4;
   float4 color : COLOR;
   float4 lwvpPosition : TEXCOORD5;
};

// 頂点座標をワールド座標に変換
float4 GetWorldConvertPosition(float4 vertexPos) {
   return mul(vertexPos, World);
}

// ワールド・ビュー・プロジェクション変換行列を適用した座標に変換
float4 GetWVPConvertPosition(float4 vertexPos) {
   return mul(mul(mul(vertexPos, World), View), Projection);
}

// ワールド・ビュー・プロジェクション変換行列を適用した座標に変換（ライト用）
float4 GetLightWVPConvertPosition(float4 vertexPos) {
   return mul(mul(mul(vertexPos, World), LightView), LightProjection);
}

// ワールド座標での法線の向きに変換した法線を正規化して返す
float4 GetWorldNormal(float3 normal) {
   float4 N = float4(normal, 0.0f);
   // ワールド座標での法線の向きに変換した法線を正規化して返す
   return normalize(mul(N, World));
}

// ワールド座標での接線の向きに変換した接線を正規化して返す
float4 GetWorldTangent(float3 tangent) {
   float4 T = float4(tangent, 0.0f);
   return normalize(mul(T, World));
}

// ワールド座標での従接線の向きに変換した従接線を正規化して返す
float4 GetWorldBiTangent(float3 bitangent) {
   float4 B = float4(bitangent, 0.0f);
   return normalize(mul(B, World));
}

// main
VsOut main(VsIn vsIn) {
   VsOut vsOut = (VsOut)0;
   vsOut.worldPosition = GetWorldConvertPosition(vsIn.pos);
   vsOut.position = GetWVPConvertPosition(vsIn.pos);
   vsOut.normal = GetWorldNormal(vsIn.normal);
   vsOut.tangent = GetWorldTangent(vsIn.tangent);
   vsOut.bitangent = GetWorldBiTangent(vsIn.bitangent);
   vsOut.uv = vsIn.uv;
   vsOut.color = vsIn.color;
   vsOut.lwvpPosition = GetLightWVPConvertPosition(vsIn.pos);
   return vsOut;
}