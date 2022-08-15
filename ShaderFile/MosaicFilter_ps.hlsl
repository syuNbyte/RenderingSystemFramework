
// Include
#include"DeferredTextureSampler.hlsl"
#include"ConstantBuffer.hlsl"
#include"ConstValue.hlsl"

// 参考サイト
// https://styly.cc/ja/tips/unity-mosaic-effect/

struct PsIn {
   float4 position : SV_POSITION;
   float2 uv : TEXCOORD0;
};

interface MosaicFilterBase {
   float4 GetMosaicResult(PsIn psIn);
};

// モザイク処理
class MosaicFilter : MosaicFilterBase {
   float4 GetMosaicResult(PsIn psIn) {
      float average = 8.0f;                           // 平均化するテクセル数（縦横）
      float mosaicResolution = average * average;     // モザイクするテクセル矩形範囲
      float2 mosaicResolutionXY = float2(2.0f, 1.0f); // モザイクの縦横比？
      float2 mosaic = mosaicResolution * mosaicResolutionXY;
      return ResultTexture.Sample(Sampler, floor(psIn.uv * mosaic) / mosaic);
   }
};

// モザイク処理なし
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