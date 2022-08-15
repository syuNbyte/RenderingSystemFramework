// ライトから見たオブジェクトの深度情報を書き込む

// Include
#include"DeferredTextureSampler.hlsl"

struct PsIn {
	float4 position : SV_POSITION;
};

float4 main(PsIn psIn) : SV_TARGET{
	float depth = psIn.position;						// こっちだと綺麗にシャドウのみ出力される
	//float depth = psIn.position.z;					// モデルの輪郭部分がジャギジャギする
	return float4(depth, 0.0f, 0.0f, 1.0f);
}

