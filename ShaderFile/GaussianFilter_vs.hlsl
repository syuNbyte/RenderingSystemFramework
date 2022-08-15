
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

interface GausBlurBase {
	VsOut GetBlurResult(VsIn vsIn);
};

// �������u���[�p�̏���
class GausBlurX : GausBlurBase {
	VsOut GetBlurResult(VsIn vsIn) {
		// �C���X�^���X�̍\�z
		VsOut vsOut;

		// �e�N�X�`���T�C�Y�̎擾
		float2 texSize;
		float level;
		ResultTexture.GetDimensions(0, texSize.x, texSize.y, level);

		// �f�[�^����
		vsOut.position = ConvertToScreenCoord(vsIn.pos);
		vsOut.uv = vsIn.uv;
		vsOut.tex0.xy = float2( 1.0f / texSize.x, 0.0f);	// 1�e�N�Z���E�ɂ��炷�I�t�Z�b�g
		vsOut.tex1.xy = float2( 3.0f / texSize.x, 0.0f);	// 3�e�N�Z���E�ɂ��炷�I�t�Z�b�g
		vsOut.tex2.xy = float2( 5.0f / texSize.x, 0.0f);	// 5�e�N�Z���E�ɂ��炷�I�t�Z�b�g
		vsOut.tex3.xy = float2( 7.0f / texSize.x, 0.0f);	// 7�e�N�Z���E�ɂ��炷�I�t�Z�b�g
		vsOut.tex4.xy = float2( 9.0f / texSize.x, 0.0f);	// 9�e�N�Z���E�ɂ��炷�I�t�Z�b�g
		vsOut.tex5.xy = float2(11.0f / texSize.x, 0.0f);	// 11�e�N�Z���E�ɂ��炷�I�t�Z�b�g
		vsOut.tex6.xy = float2(13.0f / texSize.x, 0.0f);	// 13�e�N�Z���E�ɂ��炷�I�t�Z�b�g
		vsOut.tex7.xy = float2(15.0f / texSize.x, 0.0f);	// 15�e�N�Z���E�ɂ��炷�I�t�Z�b�g

		vsOut.tex0.zw = vsOut.tex0.xy * -1.0f;				// 1�e�N�Z�����ɂ��炷�I�t�Z�b�g
		vsOut.tex1.zw = vsOut.tex1.xy * -1.0f;				// 3�e�N�Z�����ɂ��炷�I�t�Z�b�g
		vsOut.tex2.zw = vsOut.tex2.xy * -1.0f;				// 5�e�N�Z�����ɂ��炷�I�t�Z�b�g
		vsOut.tex3.zw = vsOut.tex3.xy * -1.0f;				// 7�e�N�Z�����ɂ��炷�I�t�Z�b�g
		vsOut.tex4.zw = vsOut.tex4.xy * -1.0f;				// 9�e�N�Z�����ɂ��炷�I�t�Z�b�g
		vsOut.tex5.zw = vsOut.tex5.xy * -1.0f;				// 11�e�N�Z�����ɂ��炷�I�t�Z�b�g
		vsOut.tex6.zw = vsOut.tex6.xy * -1.0f;				// 13�e�N�Z�����ɂ��炷�I�t�Z�b�g
		vsOut.tex7.zw = vsOut.tex7.xy * -1.0f;				// 15�e�N�Z�����ɂ��炷�I�t�Z�b�g
		
		// �I�t�Z�b�g�Ɋ�e�N�Z����UV���W�𑫂��Z����
		// ���ۂɃT���v�����O����UV���W�ɕϊ�����
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

// �c�����u���[�p�̏���
class GausBlurY : GausBlurBase {
	VsOut GetBlurResult(VsIn vsIn) {
		// �C���X�^���X�̍\�z
		VsOut vsOut;

		// �e�N�X�`���T�C�Y�̎擾
		float2 texSize;
		float level;
		ResultTexture.GetDimensions(0, texSize.x, texSize.y, level);

		// �f�[�^����
		vsOut.position = ConvertToScreenCoord(vsIn.pos);
		vsOut.uv = vsIn.uv;
		vsOut.tex0.xy = float2(0.0f,  1.0f / texSize.y);	// 1�e�N�Z�����ɂ��炷�I�t�Z�b�g
		vsOut.tex1.xy = float2(0.0f,  3.0f / texSize.y);	// 3�e�N�Z�����ɂ��炷�I�t�Z�b�g
		vsOut.tex2.xy = float2(0.0f,  5.0f / texSize.y);	// 5�e�N�Z�����ɂ��炷�I�t�Z�b�g
		vsOut.tex3.xy = float2(0.0f,  7.0f / texSize.y);	// 7�e�N�Z�����ɂ��炷�I�t�Z�b�g
		vsOut.tex4.xy = float2(0.0f,  9.0f / texSize.y);	// 9�e�N�Z�����ɂ��炷�I�t�Z�b�g
		vsOut.tex5.xy = float2(0.0f, 11.0f / texSize.y);	// 11�e�N�Z�����ɂ��炷�I�t�Z�b�g
		vsOut.tex6.xy = float2(0.0f, 13.0f / texSize.y);	// 13�e�N�Z�����ɂ��炷�I�t�Z�b�g
		vsOut.tex7.xy = float2(0.0f, 15.0f / texSize.y);	// 15�e�N�Z�����ɂ��炷�I�t�Z�b�g

		vsOut.tex0.zw = vsOut.tex0.xy * -1.0f;					// 1�e�N�Z����ɂ��炷�I�t�Z�b�g
		vsOut.tex1.zw = vsOut.tex1.xy * -1.0f;					// 3�e�N�Z����ɂ��炷�I�t�Z�b�g
		vsOut.tex2.zw = vsOut.tex2.xy * -1.0f;					// 5�e�N�Z����ɂ��炷�I�t�Z�b�g
		vsOut.tex3.zw = vsOut.tex3.xy * -1.0f;					// 7�e�N�Z����ɂ��炷�I�t�Z�b�g
		vsOut.tex4.zw = vsOut.tex4.xy * -1.0f;					// 9�e�N�Z����ɂ��炷�I�t�Z�b�g
		vsOut.tex5.zw = vsOut.tex5.xy * -1.0f;					// 11�e�N�Z����ɂ��炷�I�t�Z�b�g
		vsOut.tex6.zw = vsOut.tex6.xy * -1.0f;					// 13�e�N�Z����ɂ��炷�I�t�Z�b�g
		vsOut.tex7.zw = vsOut.tex7.xy * -1.0f;					// 15�e�N�Z����ɂ��炷�I�t�Z�b�g

		// �I�t�Z�b�g�Ɋ�e�N�Z����UV���W�𑫂��Z����
		// ���ۂɃT���v�����O����UV���W�ɕϊ�����
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

// �u���[�����Ȃ�
class NoBlur : GausBlurBase {
	VsOut GetBlurResult(VsIn vsIn) {
		// �C���X�^���X�̍\�z
		VsOut vsOut;

		// �f�[�^����
		vsOut.position = ConvertToScreenCoord(vsIn.pos);
		vsOut.uv = vsIn.uv;

		return vsOut;
	}
};

GausBlurBase base;

// �c���K�E�X�u���[
VsOut main(VsIn vsIn) {
	return base.GetBlurResult(vsIn);
}