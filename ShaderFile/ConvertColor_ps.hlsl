
// Include
#include"DeferredTextureSampler.hlsl"

struct VsIn {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};


// �F�ϊ����s���N���X�̃C���^�[�t�F�C�X
interface ColorConverter {
	float4 GetResult(VsIn vsIn);
};

// �K���}�ϊ�
class GammaConvert : ColorConverter {

	// ���݂̐F����P�x���擾����
	float GetBrightness(float4 color) {
		//return (color.r + color.g + color.b) / 3.0f;
		return max(max(color.r, color.g), color.b);
	}

	// �P�x���K���}�␳����
	float GetGammaResult(float brightness, float gamma) {
		float y = pow(brightness, gamma);
		return y;
	}

	// �F���P�x�␳����
	float4 ConvertToBrightness(float4 color, float brightness) {
		float b = GetBrightness(color);	// ���̐F����P�x�𒊏o
		float bias = b / brightness;		// ���̋P�x���猩�ċP�x�␳�����P�x�̊������Z�o����

		return float4(color.rgb * bias, color.a);
	}

	// �F���K���}�␳����
	float4 ConvertToGamma(float4 color, float gamma) {
		float b = GetBrightness(color);			// ���̐F����P�x�𒊏o
		float rb = GetGammaResult(b, gamma);	// �K���}�␳�����P�x���擾
		float bias = b / rb;							// ���̋P�x���猩�ăK���}�␳�����P�x�̊������Z�o����

		return float4(color.rgb * bias, color.a);
	}

	float4 GetResult(VsIn vsIn) {
		float gamma = 2.2f;
		float4 color = ResultTexture.Sample(Sampler, vsIn.uv);
		return ConvertToGamma(color, gamma);
	}
};

// �O���[�X�P�[���ϊ�
class GrayConvert : ColorConverter {
	float4 GetResult(VsIn vsIn) {
		float4 color = ResultTexture.Sample(Sampler, vsIn.uv);
		float4 param = float4(0.299, 0.587, 0.114, 0.0f);
		float dotResult = dot(color, param);
		return float4(dotResult, dotResult, dotResult, color.a);
	}
};

// �����ϊ�
class WhiteBlackConvert : ColorConverter {
	float4 GetResult(VsIn vsIn) {
		float4 color = ResultTexture.Sample(Sampler, vsIn.uv);
		float4 param = float4(0.299, 0.587, 0.114, 0.0f);
		float gray = dot(color, param);
		if (gray < 0.5f) {
			return float4(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else {
			return float4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
};

// �Z�s�A���ϊ�
class SepiaConvert : ColorConverter {
	float4 GetResult(VsIn vsIn) {
		float4 color = ResultTexture.Sample(Sampler, vsIn.uv);
		float4 param = float4(0.299, 0.587, 0.114, 0.0f);
		float gray = dot(color, param);
		float3 mulColor = float3(1.07f, 0.74f, 0.43f);
		return float4(gray * mulColor, color.a);
	}
};

// �l�K�e�B�u�ϊ�
class NegativeConvert : ColorConverter {
	float4 GetResult(VsIn vsIn) {
		float4 color = ResultTexture.Sample(Sampler, vsIn.uv);
		color.rgb = 1.0f - color.rgb;
		return color;
	}
};

// �ϊ��Ȃ�
class NoConvert : ColorConverter {
	float4 GetResult(VsIn vsIn) {
		float4 color = ResultTexture.Sample(Sampler, vsIn.uv);
		return color;
	}
};

ColorConverter base;

float4 main(VsIn vsIn) : SV_TARGET{
	return base.GetResult(vsIn);
}

