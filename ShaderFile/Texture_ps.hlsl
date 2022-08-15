
// Include
#include"Struct.hlsl"
#include"ConstantBuffer.hlsl"
#include"TextureSampler.hlsl"

// main
float4 main(VsOut vsOut) : SV_Target{
	float4 color = Texture.Sample(Sampler, vsOut.uv);
	if (color.a != 1.0f) discard;
	color.rgb *= vsOut.color.rgb;
	color.a = vsOut.color.a;
	return color;
}