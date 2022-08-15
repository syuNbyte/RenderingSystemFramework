
// Include
#include"DeferredTextureSampler.hlsl"

struct VsIn {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

// 輝度テクスチャに書き込む輝度情報を取得する
float4 GetBrightness(float4 albedo) {
	//float brightness = (albedo.r + albedo.g + albedo.b) / 3.0f;
	//if (brightness > 0.3f) {
	//	// 輝度が一定以上なら輝度情報を書き込む
	//	return brightness;
	//}
	//else {
	//	return float4(0.0f, 0.0f, 0.0f, 1.0f);
	//}
	float t = dot(albedo.rgb, float3(0.2125f, 0.7154f, 0.0721f));
	clip(t - 1.0f);
	return t;
}

float4 main(VsIn vsIn) : SV_TARGET{
	// テクスチャから情報を取得する
	float4 albedo = ResultTexture.Sample(Sampler, vsIn.uv);
	return GetBrightness(albedo);
}

