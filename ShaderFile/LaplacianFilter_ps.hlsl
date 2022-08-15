
// Include
#include"DeferredTextureSampler.hlsl"

struct VsIn {
	float4 position	 : SV_POSITION;
	float2 uvCenter	 : TEXCOORD0;
	float4 uvUp			 : TEXCOORD1;
	float4 uvDown		 : TEXCOORD2;
	float4 uvRight		 : TEXCOORD3;
	float4 uvLeft		 : TEXCOORD4;
	float4 uvRightUp	 : TEXCOORD5;
	float4 uvLeftUp	 : TEXCOORD6;
	float4 uvRightDown : TEXCOORD7;
	float4 uvLeftDown	 : TEXCOORD8;
};

float3 GetNormalAverage(VsIn vsIn) {
	//���͂̃s�N�Z���̖@���̒l�̕��ς��v�Z����B
	float3 Normal;
	Normal  = NormalTexture.Sample(Sampler, vsIn.uvCenter).xyz * -8.0f;
	Normal += NormalTexture.Sample(Sampler, vsIn.uvUp.xy).xyz;
	Normal += NormalTexture.Sample(Sampler, vsIn.uvDown.xy).xyz;
	Normal += NormalTexture.Sample(Sampler, vsIn.uvRight.xy).xyz;
	Normal += NormalTexture.Sample(Sampler, vsIn.uvLeft.xy).xyz;
	Normal += NormalTexture.Sample(Sampler, vsIn.uvRightUp.xy).xyz;
	Normal += NormalTexture.Sample(Sampler, vsIn.uvLeftUp.xy).xyz;
	Normal += NormalTexture.Sample(Sampler, vsIn.uvRightDown.xy).xyz;
	Normal += NormalTexture.Sample(Sampler, vsIn.uvLeftDown.xy).xyz;
	return Normal;
}

float GetDepthAverage(VsIn vsIn) {
	//���͂̃s�N�Z���̐[�x�l�̕��ς��v�Z����B
	float depth;
	depth  = DepthTexture.Sample(Sampler, vsIn.uvUp.zw).x;
	depth += DepthTexture.Sample(Sampler, vsIn.uvDown.zw).x;
	depth += DepthTexture.Sample(Sampler, vsIn.uvRight.zw).x;
	depth += DepthTexture.Sample(Sampler, vsIn.uvLeft.zw).x;
	depth += DepthTexture.Sample(Sampler, vsIn.uvRightUp.zw).x;
	depth += DepthTexture.Sample(Sampler, vsIn.uvLeftUp.zw).x;
	depth += DepthTexture.Sample(Sampler, vsIn.uvRightDown.zw).x;
	depth += DepthTexture.Sample(Sampler, vsIn.uvLeftDown.zw).x;
	depth /= 8.0f;
	return depth;
}

interface LaplacianFilterBase {
	float4 GetColor(VsIn vsIn);
};

class LaplacianFilter : LaplacianFilterBase {
	float4 GetColor(VsIn vsIn) {
		float4 albedo = ResultTexture.Sample(Sampler, vsIn.uvCenter);
		float3 normal = GetNormalAverage(vsIn);
		float depth = GetDepthAverage(vsIn);
		float depthCenter = DepthTexture.Sample(Sampler, vsIn.uvCenter).x;

			//�@���̌v�Z���ʁA���邢�͐[�x�l�̌v�Z���ʂ����ȏ�Ȃ�G�b�W�Ƃ݂Ȃ��B
		if (length(normal) >= 0.2f || abs(depth - depthCenter) > 0.001f) {
			float4 edgeColor = float4(0.0f, 1.0f, 0.0f, 1.0f);
			return edgeColor;
		}
		else {
			return albedo;
		}
	}
};

class NoFilter : LaplacianFilterBase {
	float4 GetColor(VsIn vsIn) {
		float4 albedo = ResultTexture.Sample(Sampler, vsIn.uvCenter);
		return albedo;
	}
};

LaplacianFilterBase base;
LaplacianFilter laplacian;
NoFilter no;

float4 main(VsIn vsIn) : SV_TARGET{

	return base.GetColor(vsIn);

	//float4 albedo = ResultTexture.Sample(Sampler, vsIn.uvCenter);
	
	////���͂̃s�N�Z���̖@���̒l�̕��ς��v�Z����B
	//float3 Normal = GetNormalAverage(vsIn);
	////Normal  = NormalTexture.Sample(Sampler, vsIn.uvCenter).xyz * -8.0f;
	////Normal += NormalTexture.Sample(Sampler, vsIn.uvUp.xy).xyz;
	////Normal += NormalTexture.Sample(Sampler, vsIn.uvDown.xy).xyz;
	////Normal += NormalTexture.Sample(Sampler, vsIn.uvRight.xy).xyz;
	////Normal += NormalTexture.Sample(Sampler, vsIn.uvLeft.xy).xyz;
	////Normal += NormalTexture.Sample(Sampler, vsIn.uvRightUp.xy).xyz;
	////Normal += NormalTexture.Sample(Sampler, vsIn.uvLeftUp.xy).xyz;
	////Normal += NormalTexture.Sample(Sampler, vsIn.uvRightDown.xy).xyz;
	////Normal += NormalTexture.Sample(Sampler, vsIn.uvLeftDown.xy).xyz;

	////���͂̃s�N�Z���̐[�x�l�̕��ς��v�Z����B
	//float depth = GetDepthAverage(vsIn);
	//float depthCenter = DepthTexture.Sample(Sampler, vsIn.uvCenter).x;
	////depth  = DepthTexture.Sample(Sampler, vsIn.uvUp.zw).x;
	////depth += DepthTexture.Sample(Sampler, vsIn.uvDown.zw).x;
	////depth += DepthTexture.Sample(Sampler, vsIn.uvRight.zw).x;
	////depth += DepthTexture.Sample(Sampler, vsIn.uvLeft.zw).x;
	////depth += DepthTexture.Sample(Sampler, vsIn.uvRightUp.zw).x;
	////depth += DepthTexture.Sample(Sampler, vsIn.uvLeftUp.zw).x;
	////depth += DepthTexture.Sample(Sampler, vsIn.uvRightDown.zw).x;
	////depth += DepthTexture.Sample(Sampler, vsIn.uvLeftDown.zw).x;
	////depth /= 8.0f;

	//float4 ret;
	////�@���̌v�Z���ʁA���邢�͐[�x�l�̌v�Z���ʂ����ȏ�Ȃ�G�b�W�Ƃ݂Ȃ��B
	//if (length(Normal) >= 0.2f || abs(depth - depthCenter) > 0.001f) {
	//	float4 edgeColor = float4(0.0f, 1.0f, 0.0f, 1.0f);
	//	ret = edgeColor;
	//}
	//else {
	//	ret = float4(1.0f, 1.0f, 1.0f, 1.0f) * albedo;
	//	//ret = albedo;
	//}

	//return ret;
}

