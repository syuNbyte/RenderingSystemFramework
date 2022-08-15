
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

// ���_���W�����[���h���W�ɕϊ�
float4 GetWorldConvertPosition(float4 vertexPos) {
   return mul(vertexPos, World);
}

// ���[���h�E�r���[�E�v���W�F�N�V�����ϊ��s���K�p�������W�ɕϊ�
float4 GetWVPConvertPosition(float4 vertexPos) {
   return mul(mul(mul(vertexPos, World), View), Projection);
}

// ���[���h�E�r���[�E�v���W�F�N�V�����ϊ��s���K�p�������W�ɕϊ��i���C�g�p�j
float4 GetLightWVPConvertPosition(float4 vertexPos) {
   return mul(mul(mul(vertexPos, World), LightView), LightProjection);
}

// ���[���h���W�ł̖@���̌����ɕϊ������@���𐳋K�����ĕԂ�
float4 GetWorldNormal(float3 normal) {
   float4 N = float4(normal, 0.0f);
   // ���[���h���W�ł̖@���̌����ɕϊ������@���𐳋K�����ĕԂ�
   return normalize(mul(N, World));
}

// ���[���h���W�ł̐ڐ��̌����ɕϊ������ڐ��𐳋K�����ĕԂ�
float4 GetWorldTangent(float3 tangent) {
   float4 T = float4(tangent, 0.0f);
   return normalize(mul(T, World));
}

// ���[���h���W�ł̏]�ڐ��̌����ɕϊ������]�ڐ��𐳋K�����ĕԂ�
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