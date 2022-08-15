
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

// ���_���W�����[���h���W�ɕϊ�
float4 GetWorldConvertPosition(float4 vertexPos) {
   return mul(vertexPos, World);
}

// ���[���h�E�r���[�E�v���W�F�N�V�����ϊ��s���K�p�������W�ɕϊ��i���C�g�p�j
float4 GetLightWVPConvertPosition(float4 vertexPos) {
   return mul(mul(mul(vertexPos, World), LightView), LightProjection);
}

// main
VsOut main(VsIn vsIn) {
   VsOut vsOut = (VsOut)0;
   vsOut.position = GetLightWVPConvertPosition(vsIn.pos);
   return vsOut;
}