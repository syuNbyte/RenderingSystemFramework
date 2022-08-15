
// Include
#include"Struct.hlsl"
#include"ConstantBuffer.hlsl"

// main
float4 main(VsOut vsOut) : SV_Target{
	return vsOut.color;
}