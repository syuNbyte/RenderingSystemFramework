
// Include
#include"DeferredTextureSampler.hlsl"
#include"ConstantBuffer.hlsl"
#include"ConstValue.hlsl"

// �Q�l�T�C�g
// https://styly.cc/ja/tips/unity-mosaic-effect/

struct PsIn {
   float4 position : SV_POSITION;
   float2 uv : TEXCOORD0;
};

interface MosaicFilterBase {
   float4 GetMosaicResult(PsIn psIn);
};

// ���U�C�N����
class MosaicFilter : MosaicFilterBase {
   float4 GetMosaicResult(PsIn psIn) {
      float average = 8.0f;                           // ���ω�����e�N�Z�����i�c���j
      float mosaicResolution = average * average;     // ���U�C�N����e�N�Z����`�͈�
      float2 mosaicResolutionXY = float2(2.0f, 1.0f); // ���U�C�N�̏c����H
      float2 mosaic = mosaicResolution * mosaicResolutionXY;
      return ResultTexture.Sample(Sampler, floor(psIn.uv * mosaic) / mosaic);
   }
};

// ���U�C�N�����Ȃ�
class NoFilter : MosaicFilterBase {
   float4 GetMosaicResult(PsIn psIn) {
      float4 color = ResultTexture.Sample(Sampler, psIn.uv);
      return color;
   }
};

MosaicFilterBase base;

float4 main(PsIn psIn) : SV_TARGET{
   return base.GetMosaicResult(psIn);
}