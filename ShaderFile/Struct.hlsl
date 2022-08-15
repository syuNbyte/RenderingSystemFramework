// Type Define
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
};