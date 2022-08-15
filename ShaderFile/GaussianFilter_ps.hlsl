
// Include
#include"DeferredTextureSampler.hlsl"

struct PsIn {
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

interface GausBlurBase {
	float4 BlurResult(PsIn psIn);
};

// �u���[����
class ApplyBlur : GausBlurBase {
	float4 BlurResult(PsIn psIn) {
		float weight = 1.0f / 16.0f;	
		float offsetX = 1.0f;
		float4 ret = 0.0f;
		float4 depth = DepthTexture.Sample(Sampler, psIn.uv).y;

		// ���܂ł̃o�O�̌�����������܂���
		// ���܂ł̓K�؂ɕ\���ł��Ȃ��������R�̓K�E�V�A���u���[�ɓK�p����Ă���E�F�C�g�����ł���
		// �Ȃ��Ȃ�K�E�X�u���[�͑S�Ẵs�N�Z���Ɋ|����E�F�C�g�̍��v��1.0f���]�܂����l������ł�
		// ��������܂ł͑S�Ẵs�N�Z���ɑ΂���1.0f�������Ă��܂����B
		// ���ꂾ�ƃE�F�C�g�̍��v�l��16.0f�ɂȂ��Ă��܂����߁A�啝�ɃE�F�C�g�̍��v�K��l���I�[�o�[���Ă��܂��B
		// ���̂��ߓK�؂ɔ��f�ł��܂���ł���

		// ��e�N�Z������v���X������8�e�N�Z���d�ݕt���ŃT���v�����O
		ret += ResultTexture.Sample(Sampler, psIn.tex0.xy) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex1.xy) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex2.xy) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex3.xy) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex4.xy) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex5.xy) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex6.xy) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex7.xy) * weight;

		// ��e�N�Z������}�C�i�X������8�e�N�Z���d�ݕt���ŃT���v�����O
		ret += ResultTexture.Sample(Sampler, psIn.tex0.zw) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex1.zw) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex2.zw) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex3.zw) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex4.zw) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex5.zw) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex6.zw) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex7.zw) * weight;

		return ret;
	}
};

class NoBlur : GausBlurBase {
	float4 BlurResult(PsIn psIn) {
		float4 color = ResultTexture.Sample(Sampler, psIn.uv);
		return color;
	}
};

GausBlurBase gausBlur;

float4 main(PsIn psIn) : SV_TARGET{
	return gausBlur.BlurResult(psIn);
}