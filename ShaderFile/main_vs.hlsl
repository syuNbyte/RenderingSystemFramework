
// Include
#include"Struct.hlsl"
#include"ConstantBuffer.hlsl"

// ���_���W�����[���h���W�ɕϊ�
float4 GetWorldConvertPosition(float4 vertexPos) {
   return mul(vertexPos, World);
}

// ���[���h�E�r���[�E�v���W�F�N�V�����ϊ��s���K�p�������W�ɕϊ�
float4 GetWVPConvertPosition(float4 vertexPos) {
   return mul(mul(mul(vertexPos, World), View), Projection);
}

// ���[���h���W�ł̖@���̌����ɕϊ������@���𐳋K�����ĕԂ�
float4 GetWorldNormal(float4 normal) {
   float4 N = normal;
   N.w = 0;

   // ���[���h���W�ł̖@���̌����ɕϊ������@���𐳋K�����ĕԂ�
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