
// Include
#include"ConstantBuffer.hlsl"
#include"DeferredTextureSampler.hlsl"

struct VsOut {
	float4 position	 : SV_POSITION;
	float2 uvCenter	 : TEXCOORD0;
	float4 uvUp			 : TEXCOORD1;
	float4 uvDown		 : TEXCOORD2;
	float4 uvRight		 : TEXCOORD3;
	float4 uvLeft		 : TEXCOORD4;
	float4 uvRightUp	 : TEXCOORD5;
	float4 uvLeftUp	 : TEXCOORD6;
	float4 uvRightDown : TEXCOORD7;
	float4 uvLeftDown	 : TEXCOORD8;
};

// ���[���h�E�r���[�E�v���W�F�N�V�����ϊ��s���K�p�������W�ɕϊ�
float4 GetWVPConvertPosition(float4 vertexPos) {
	return mul(mul(mul(vertexPos, World), View), Projection);
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

	// �e�N�X�`���T�C�Y�̎擾
	float2 texSize;
	float level;
	NormalTexture.GetDimensions(0, texSize.x, texSize.y, level);

	// �f�[�^����
	vsOut.position = ConvertToScreenCoord(pos);

	// �^�񒆂̃s�N�Z��
	vsOut.uvCenter = uv;

	// �@���e�N�X�`�����猻�ݑI�𒆂�UV���W�t�߂̃s�N�Z����UV�����擾����
	{
		// ���S�s�N�Z������̃I�t�Z�b�g
		float offset = 0.2f;

		// ��̃s�N�Z��
		vsOut.uvUp.xy = uv + float2(0.0f, -offset / texSize.y);

		// ���̃s�N�Z��
		vsOut.uvDown.xy = uv + float2(0.0f, offset / texSize.y);

		// �E�̃s�N�Z��
		vsOut.uvRight.xy = uv + float2(offset / texSize.x, 0.0f);

		// ���̃s�N�Z��
		vsOut.uvLeft.xy = uv + float2(-offset / texSize.x, 0.0f);

		// �E��̃s�N�Z��
		vsOut.uvRightUp.xy = uv + float2(offset / texSize.x, -offset / texSize.y);

		// ����̃s�N�Z��
		vsOut.uvLeftUp.xy = uv + float2(-offset / texSize.x, -offset / texSize.y);

		// �E���̃s�N�Z��
		vsOut.uvRightDown.xy = uv + float2(offset / texSize.x, offset / texSize.y);

		// �����̃s�N�Z��
		vsOut.uvLeftDown.xy = uv + float2(-offset / texSize.x, offset / texSize.y);
	}

	// �[�x�e�N�X�`�����猻�ݑI�𒆂�UV���W�t�߂̃s�N�Z����UV�����擾����
	{
		// ���S�s�N�Z������̃I�t�Z�b�g
		float offset = 1.0f;

		// ��̃s�N�Z��
		vsOut.uvUp.zw = uv + float2(0.0f, -offset / texSize.y);

		// ���̃s�N�Z��
		vsOut.uvDown.zw = uv + float2(0.0f, offset / texSize.y);

		// �E�̃s�N�Z��
		vsOut.uvRight.zw = uv + float2(offset / texSize.x, 0.0f);

		// ���̃s�N�Z��
		vsOut.uvLeft.zw = uv + float2(-offset / texSize.x, 0.0f);

		// �E��̃s�N�Z��
		vsOut.uvRightUp.zw = uv + float2(offset / texSize.x, -offset / texSize.y);

		// ����̃s�N�Z��
		vsOut.uvLeftUp.zw = uv + float2(-offset / texSize.x, -offset / texSize.y);

		// �E���̃s�N�Z��
		vsOut.uvRightDown.zw = uv + float2(offset / texSize.x, offset / texSize.y);

		// �����̃s�N�Z��
		vsOut.uvLeftDown.zw = uv + float2(-offset / texSize.x, offset / texSize.y);
	}

	// �f�[�^��Ԃ�
	return vsOut;
}