
// Include
#include"DeferredTextureSampler.hlsl"
#include"ConstantBuffer.hlsl"
#include"ConstValue.hlsl"

struct PsIn {
   float4 position : SV_POSITION;
   float2 uv : TEXCOORD0;
};

interface RimLightBase {
   float4 GetLightingResult(PsIn psIn);
};

class RimLight : RimLightBase {
   float4 GetLightingResult(PsIn psIn) {
      float4 normal = NormalTexture.Sample(Sampler,psIn.uv);
      float4 position = PositionTexture.Sample(Sampler, psIn.uv);
      float4 color = ResultTexture.Sample(Sampler, psIn.uv);
      float3 N = normal.xyz;
      float3 V = normalize(CameraPosition.xyz - position.xyz);
      float3 L = normalize(LightDirection.xyz);
      float3 H = normalize(L + V);

      float rim_power = 5.0;                 // リムライトの強さ
      float rim = 1.0 - dot(normal, V);      // 内積の値を反転しモデルの縁部分が1.0、視線と平行部分を0.0にする。
      rim = pow(abs(rim), rim_power);        // リムライトの強さを補正
      float light_rim = max(dot(L, V), 0.0); // 太陽との逆光を計算（頂点シェーダでOK）

      // モデルの縁計算と、太陽の逆光計算を乗算
      color.xyz += (rim * light_rim);
      return color;
   }
};

class TestRimLight : RimLightBase {
   float4 GetLightingResult(PsIn psIn) {
      float4 normal = NormalTexture.Sample(Sampler, psIn.uv);
      float4 position = PositionTexture.Sample(Sampler, psIn.uv);
      float4 color = ResultTexture.Sample(Sampler, psIn.uv);
      float3 N = normal.xyz;
      float3 V = normalize(CameraPosition.xyz - position.xyz);
      float3 L = normalize(float3(0.0f, 0.0f, -1.0f));
      //float3 L = normalize(-LightDirection.xyz);
      float3 H = normalize(L + V);

      float rim_power = 5.0;                 // リムライトの強さ
      float rim = 1.0 - dot(normal, V);      // 内積の値を反転しモデルの縁部分が1.0、視線と平行部分を0.0にする。
      rim = pow(abs(rim), rim_power);        // リムライトの強さを補正
      float light_rim = max(dot(L, V), 0.0); // 太陽との逆光を計算（頂点シェーダでOK）

      // モデルの縁計算と、太陽の逆光計算を乗算
      //color.xyz += (rim * light_rim);
      color.xyz = (rim * light_rim);
      return color;
   }
};

class NoLight : RimLightBase {
   float4 GetLightingResult(PsIn psIn) {
      float4 color = ResultTexture.Sample(Sampler, psIn.uv).rgba;
      return color;
   }
};

RimLightBase base;

float4 main(PsIn psIn) : SV_TARGET{
   float depth = DepthTexture.Sample(Sampler, psIn.uv).x;
   if (depth == 0.0f) discard;
   return base.GetLightingResult(psIn);
}