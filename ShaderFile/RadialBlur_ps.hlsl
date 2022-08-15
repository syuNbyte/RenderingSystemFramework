
// Include
#include"DeferredTextureSampler.hlsl"
#include"ConstantBuffer.hlsl"
#include"ConstValue.hlsl"

struct PsIn {
   float4 position : SV_POSITION;
   float2 uv : TEXCOORD0;
};

// �Q�l�T�C�g
// https://light11.hatenadiary.com/entry/2019/01/04/232733

interface RadialBlurBase {
   float4 GetBlurResult(PsIn psIn);
};

class ApplyBlur : RadialBlurBase {
   float4 GetBlurResult(PsIn psIn) {
      const int sampleCount = 8;
      const float strength = 0.5f;  // 

      float4 color = 0.0f;
      float2 symmetryUv = psIn.uv - 0.5f;    // UV��-0.5f�`+0.5f�ɕϊ�
      float distance = length(symmetryUv);   // �O���ɍs���قǂ��̒l���傫���Ȃ�

      for (int i = 0; i < sampleCount; i++) {
         float uvOffset = 1.0f - strength * i / sampleCount * distance;
         color += ResultTexture.Sample(Sampler, symmetryUv * uvOffset + 0.5f);
      }

      color /= sampleCount;

      return color;
   }
};

class NoBlur : RadialBlurBase {
   float4 GetBlurResult(PsIn psIn) {
      float4 color = ResultTexture.Sample(Sampler, psIn.uv);
      return color;
   }
};

RadialBlurBase base;

float4 main(PsIn psIn) : SV_TARGET{
   return base.GetBlurResult(psIn);
}