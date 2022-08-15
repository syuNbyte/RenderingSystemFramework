
// Include
#include"DeferredTextureSampler.hlsl"

struct VsIn {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

interface LoaderBase {
	float4 GetColor(VsIn vsIn);
};

class AlbedoLoader : LoaderBase {
	float4 GetColor(VsIn vsIn) {
		float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
		return albedo;
	}
};

class NormalLoader : LoaderBase {
	float4 GetColor(VsIn vsIn) {
		float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
		return normal;
	}
};

class PositionLoader : LoaderBase {
	float4 GetColor(VsIn vsIn) {
		float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
		return position;
	}
};

class DepthLoader : LoaderBase {
	float4 GetColor(VsIn vsIn) {
		float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
		return depth;
	}
};

class MaterialLoader : LoaderBase {
	float4 GetColor(VsIn vsIn) {
		float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
		return material;
	}
};

class BrightnessLoader : LoaderBase {
	float4 GetColor(VsIn vsIn) {
		float4 brightness = BrightnessTexture.Sample(Sampler, vsIn.uv);
		return brightness;
	}
};

LoaderBase loader;

float4 main(VsIn vsIn) : SV_TARGET{
	// テクスチャから情報を取得する
	//float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
	//float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
	//float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
	//float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
	//float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
	//float4 brightness = BrightnessTexture.Sample(Sampler, vsIn.uv);

	// TODO：とりあえずアルベドカラーを返す
	//return albedo;
	//return normal;
	//return position;
	//return depth;
	//return material;
	//return float4(material.r, 0.0f, 0.0f, 1.0f);s
	//return float4(material.g, 0.0f, 0.0f, 1.0f);
	//return float4(material.b, 0.0f, 0.0f, 1.0f);

	return loader.GetColor(vsIn);
}

