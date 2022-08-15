
// Include
#include"Struct.hlsl"

interface Base_vs {
	float4 GetColor();
};

class VSRed : Base_vs {
	float4	GetColor() {
		return	float4(1, 0, 0, 1);
	}
};

class VSGreen : Base_vs {
	float4	GetColor() {
		return	float4(0, 1, 0, 1);
	}
};

cbuffer BaseVSInterfaces {
	VSRed	red;
	VSGreen	green;
};

// Interfaceのインスタンスを作成しておく
Base_vs base;

// main
VsOut main(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, float4 color : COLOR) {
	VsOut vsOut = (VsOut)0;
	vsOut.color = base.GetColor();
	return vsOut;
}