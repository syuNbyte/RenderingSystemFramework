
// Include
#include"DeferredTextureSampler.hlsl"
#include"ConstantBuffer.hlsl"
#include"ConstValue.hlsl"

struct PsIn {
   float4 position : SV_POSITION;
   float2 uv : TEXCOORD0;
};

float4 main(PsIn psIn) : SV_TARGET {
   float4 color = ResultTexture.Sample(Sampler, psIn.uv);
   return color;
}