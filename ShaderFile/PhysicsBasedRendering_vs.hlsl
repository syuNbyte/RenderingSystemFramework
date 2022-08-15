
// Include
#include"ConstantBuffer.hlsl"

struct VsOut {
	float4 position : SV_POSITION0;
	float2 uv : TEXCOORD0;
};

// ���[���h�E�r���[�E�v���W�F�N�V�����ϊ��s���K�p�������W�ɕϊ�
float4 GetWVPConvertPosition(float4 vertexPos) {
	return mul(mul(mul(vertexPos, World), View), Projection);
}

// �X�N���[���S�̂ɂ��ꂢ�ɓ\��t����p�̕ϊ����s������
float4 TestConvert(float2 uv) {
	return float4(
		lerp(-1.0f, 1.0f, uv.x),
		lerp(1.0f, -1.0f, uv.y),
		0.0f,
		0.0f
	);
}

// �X�N���[�����W�n�ɕϊ�����
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
	// �C���X�^���X�̍\�z
	VsOut vsOut;

	// �f�[�^����
	vsOut.position = ConvertToScreenCoord(pos);
	vsOut.uv = uv;

	// �f�[�^��Ԃ�
	return vsOut;
}