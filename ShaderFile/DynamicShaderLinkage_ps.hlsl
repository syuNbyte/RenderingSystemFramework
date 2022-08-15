
// Include
#include"Struct.hlsl"

interface Base_ps {
	float4 GetColor();
};

class PSRed : Base_ps {
	float4	GetColor() {
		return	float4(1, 0, 0, 1);
	}
};

class PSGreen : Base_ps {
	float4	GetColor() {
		return	float4(0, 1, 0, 1);
	}
};

cbuffer BasePSInterfaces {
	PSRed	red;
	PSGreen	green;
};

// Interfaceのインスタンスを作成しておく
Base_ps base;

// main
float4 main(VsOut vsOut) : SV_Target{
	vsOut.color = base.GetColor();
	return vsOut.color;
}