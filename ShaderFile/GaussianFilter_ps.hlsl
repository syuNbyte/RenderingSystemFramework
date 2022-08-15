
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

// ブラー処理
class ApplyBlur : GausBlurBase {
	float4 BlurResult(PsIn psIn) {
		float weight = 1.0f / 16.0f;	
		float offsetX = 1.0f;
		float4 ret = 0.0f;
		float4 depth = DepthTexture.Sample(Sampler, psIn.uv).y;

		// 今までのバグの原因が分かりました
		// 今までの適切に表示できなかった理由はガウシアンブラーに適用されているウェイトが問題でした
		// なぜならガウスブラーは全てのピクセルに掛けるウェイトの合計が1.0fが望ましい値だからです
		// それを今までは全てのピクセルに対して1.0fをかけていました。
		// それだとウェイトの合計値が16.0fになってしまうため、大幅にウェイトの合計規定値をオーバーしています。
		// そのため適切に反映できませんでした

		// 基準テクセルからプラス方向に8テクセル重み付きでサンプリング
		ret += ResultTexture.Sample(Sampler, psIn.tex0.xy) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex1.xy) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex2.xy) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex3.xy) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex4.xy) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex5.xy) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex6.xy) * weight;
		ret += ResultTexture.Sample(Sampler, psIn.tex7.xy) * weight;

		// 基準テクセルからマイナス方向に8テクセル重み付きでサンプリング
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