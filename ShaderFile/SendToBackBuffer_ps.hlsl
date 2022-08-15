
// Include
#include"DeferredTextureSampler.hlsl"

struct VsIn {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(VsIn vsIn) : SV_TARGET{
	float4 color = ResultTexture.Sample(Sampler, vsIn.uv);
	return color;
}

