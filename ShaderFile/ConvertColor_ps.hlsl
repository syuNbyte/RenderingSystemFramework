
// Include
#include"DeferredTextureSampler.hlsl"

struct VsIn {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};


// 色変換を行うクラスのインターフェイス
interface ColorConverter {
	float4 GetResult(VsIn vsIn);
};

// ガンマ変換
class GammaConvert : ColorConverter {

	// 現在の色から輝度を取得する
	float GetBrightness(float4 color) {
		//return (color.r + color.g + color.b) / 3.0f;
		return max(max(color.r, color.g), color.b);
	}

	// 輝度をガンマ補正する
	float GetGammaResult(float brightness, float gamma) {
		float y = pow(brightness, gamma);
		return y;
	}

	// 色を輝度補正する
	float4 ConvertToBrightness(float4 color, float brightness) {
		float b = GetBrightness(color);	// 元の色から輝度を抽出
		float bias = b / brightness;		// 元の輝度から見て輝度補正した輝度の割合を算出する

		return float4(color.rgb * bias, color.a);
	}

	// 色をガンマ補正する
	float4 ConvertToGamma(float4 color, float gamma) {
		float b = GetBrightness(color);			// 元の色から輝度を抽出
		float rb = GetGammaResult(b, gamma);	// ガンマ補正した輝度を取得
		float bias = b / rb;							// 元の輝度から見てガンマ補正した輝度の割合を算出する

		return float4(color.rgb * bias, color.a);
	}

	float4 GetResult(VsIn vsIn) {
		float gamma = 2.2f;
		float4 color = ResultTexture.Sample(Sampler, vsIn.uv);
		return ConvertToGamma(color, gamma);
	}
};

// グレースケール変換
class GrayConvert : ColorConverter {
	float4 GetResult(VsIn vsIn) {
		float4 color = ResultTexture.Sample(Sampler, vsIn.uv);
		float4 param = float4(0.299, 0.587, 0.114, 0.0f);
		float dotResult = dot(color, param);
		return float4(dotResult, dotResult, dotResult, color.a);
	}
};

// 白黒変換
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

// セピア調変換
class SepiaConvert : ColorConverter {
	float4 GetResult(VsIn vsIn) {
		float4 color = ResultTexture.Sample(Sampler, vsIn.uv);
		float4 param = float4(0.299, 0.587, 0.114, 0.0f);
		float gray = dot(color, param);
		float3 mulColor = float3(1.07f, 0.74f, 0.43f);
		return float4(gray * mulColor, color.a);
	}
};

// ネガティブ変換
class NegativeConvert : ColorConverter {
	float4 GetResult(VsIn vsIn) {
		float4 color = ResultTexture.Sample(Sampler, vsIn.uv);
		color.rgb = 1.0f - color.rgb;
		return color;
	}
};

// 変換なし
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

