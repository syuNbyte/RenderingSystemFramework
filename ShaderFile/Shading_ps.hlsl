
// Include
#include"DeferredTextureSampler.hlsl"
#include"ConstantBuffer.hlsl"
#include"ConstValue.hlsl"

struct PsIn {
   float4 position : SV_POSITION;
   float2 uv : TEXCOORD0;
};




interface Shading {
   float4 ShadingResult(PsIn psIn);
};

class NoShading : Shading {
   float4 ShadingResult(PsIn psIn) {
      float4 albedo = AlbedoTexture.Sample(Sampler, psIn.uv);
      return albedo.rgba;
   }
};

class LambertShading : Shading {

   float3 Lambert(float3 albedo, float3 normal) {
      return saturate(dot(-LightDirection.xyz, normal.xyz));
   }

   float3 HalfLambert(float3 albedo, float3 normal) {
      return albedo / PI;
   }

   float3 Phong(float3 position, float3 normal) {
      float3 H = normalize(normalize(-LightDirection.xyz) + normalize(CameraPosition.xyz - position.xyz));
      return pow(max(0, dot(normalize(normal), H)), 50.0f);
   }

   float3 PhongEx(float3 position, float3 normal, float specularPower) {
      float3 H = normalize(normalize(-LightDirection.xyz) + normalize(CameraPosition.xyz - position.xyz));
      return pow(max(0, dot(normalize(normal), H)), 100.0f * specularPower);
   }


   float4 ShadingResult(PsIn psIn) {
      // �e�N�X�`����������擾����
      float4 albedo = AlbedoTexture.Sample(Sampler, psIn.uv);
      float4 normal = NormalTexture.Sample(Sampler, psIn.uv);
      float4 position = PositionTexture.Sample(Sampler, psIn.uv);
      float4 depth = DepthTexture.Sample(Sampler, psIn.uv);
      float4 material = MaterialTexture.Sample(Sampler, psIn.uv);
      float roughness = material.r;                            // �e��
      float metalness = material.g;                            // �����x
      float specular = material.b;                             // �X�y�L�������̋���
      //float emissive = material.a;                           // �G�~�b�V�u
      float smooth = 1.0f - roughness;
      float3 N = normal.xyz;
      float3 V = normalize(CameraPosition.xyz - position.xyz);
      float3 L = normalize(-LightDirection.xyz);
      float3 H = normalize(L + V);

      float3 lambert = Lambert(albedo.xyz, normal.xyz) * albedo.rgb;
      return float4(lambert, albedo.a);
   }
};

class LambertAndPhongShading : Shading {

   float LambertParam(float3 normal) {
      return saturate(dot(-LightDirection.xyz, normal.xyz));
   }

   float HalfLambertParam(float3 normal) {
      float lambert = LambertParam(normal);
      float halflambert = lambert * 0.5f + 0.5f;
      return halflambert * halflambert;
   }

   float3 Lambert(float3 albedo, float3 normal) {
      return saturate(dot(-LightDirection.xyz, normal.xyz));
   }

   float3 HalfLambert(float3 albedo, float3 normal) {
      return albedo / PI;
   }

   float3 Phong(float3 position, float3 normal) {
      float3 H = normalize(normalize(-LightDirection.xyz) + normalize(CameraPosition.xyz - position.xyz));
      return pow(max(0, dot(normalize(normal), H)), 50.0f);
   }

   float3 PhongEx(float3 position, float3 normal, float specularPower) {
      float3 H = normalize(normalize(-LightDirection.xyz) + normalize(CameraPosition.xyz - position.xyz));
      return pow(max(0, dot(normalize(normal), H)), 100.0f * specularPower);
   }


   float4 ShadingResult(PsIn psIn) {
      // �e�N�X�`����������擾����
      float4 albedo = AlbedoTexture.Sample(Sampler, psIn.uv);
      float4 normal = NormalTexture.Sample(Sampler, psIn.uv);
      float4 position = PositionTexture.Sample(Sampler, psIn.uv);
      float4 depth = DepthTexture.Sample(Sampler, psIn.uv);
      float4 material = MaterialTexture.Sample(Sampler, psIn.uv);
      float roughness = material.r;                            // �e��
      float metalness = material.g;                            // �����x
      float specular = material.b;                             // �X�y�L�������̋���
      //float emissive = material.a;                           // �G�~�b�V�u
      float smooth = 1.0f - roughness;
      float3 N = normal.xyz;
      float3 V = normalize(CameraPosition.xyz - position.xyz);
      float3 L = normalize(-LightDirection.xyz);
      float3 H = normalize(L + V);

      // �����o�[�g���˂ƃt�H������
      float lambert = HalfLambertParam(normal.xyz);
      float3 phong = PhongEx(position.xyz, normal.xyz, specular);
      float3 lambertParam = (lambert * albedo.rgb);
      float3 phongParam = phong;
      //float3 phongParam = (phong * lerp(float3(1.0f, 1.0f, 1.0f), albedo.rgb, metalness));
      float3 color = lambertParam + phongParam;
      return float4(color, albedo.a);
   }
};

class PhysicsBasedRendering : Shading {
   float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv) {
   //float3 binSpaceNormal = g_normalMap.SampleLevel(g_sampler, uv, 0.0f).xyz;
   //binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

   //float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

      float3 newNormal = 0.0f;
      return newNormal;
   }

   // �x�b�N�}�����z���v�Z����
   float Beckmann(float m, float t) {
      float t2 = t * t;
      float t4 = t * t * t * t;
      float m2 = m * m;
      float D = 1.0f / (4.0f * m2 * t4);
      D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
      return D;
   }
   // �t���l�����v�Z�BSchlick�ߎ����g�p
   float SpcFresnel(float f0, float u) {
       // from Schlick
      return f0 + (1 - f0) * pow(1 - u, 5);
   }
   /// <summary>
   /// �N�b�N�g�����X���f���̋��ʔ��˂��v�Z
   /// </summary>
   /// <param name="L">�����Ɍ������x�N�g��</param>
   /// <param name="V">���_�Ɍ������x�N�g��</param>
   /// <param name="N">�@���x�N�g��</param>
   /// <param name="metaric">�����x</param>
   float CookTrranceSpecular(float3 L, float3 V, float3 N, float metaric) {
      float microfacet = 0.76f;
      // �����x�𐂒����˂̎��̃t���l�����˗��Ƃ��Ĉ���
      // �����x�������قǃt���l�����˂͑傫���Ȃ�
      float f0 = metaric;
      // ���C�g�Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
      float3 H = normalize(L + V);

      // �e��x�N�g�����ǂꂭ�炢���Ă��邩����ς𗘗p���ċ��߂�
      float NdotH = saturate(dot(N, H));
      float VdotH = saturate(dot(V, H));
      float NdotL = saturate(dot(N, L));
      float NdotV = saturate(dot(N, V));

      // D�����x�b�N�}�����z��p���Čv�Z����
      float D = Beckmann(microfacet, NdotH);
      // F����Schlick�ߎ���p���Čv�Z����
      float F = SpcFresnel(f0, VdotH);
      // G�������߂�
      float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));
      // m�������߂�
      float m = PI * NdotV * NdotH;
      // �����܂ŋ��߂��A�l�𗘗p���āA�N�b�N�g�����X���f���̋��ʔ��˂����߂�
      return max(F * D * G / m, 0.0);
   }

   /// <summary>
   /// �t���l�����˂��l�������g�U���˂��v�Z
   /// </summary>
   /// <remark>
   /// ���̊֐��̓t���l�����˂��l�������g�U���˗����v�Z���܂�
   /// �t���l�����˂́A�������̂̕\�ʂŔ��˂��錻�ۂ̂Ƃ��ŁA���ʔ��˂̋����ɂȂ�܂�
   /// ����g�U���˂́A�������̂̓����ɓ����āA�����������N�����āA�g�U���Ĕ��˂��Ă������̂��Ƃł�
   /// �܂�t���l�����˂��ア�Ƃ��ɂ́A�g�U���˂��傫���Ȃ�A�t���l�����˂������Ƃ��́A�g�U���˂��������Ȃ�܂�
   ///
   /// </remark>
   /// <param name="N">�@��</param>
   /// <param name="L">�����Ɍ������x�N�g���B���̕����Ƌt�����̃x�N�g���B</param>
   /// <param name="V">�����Ɍ������x�N�g���B</param>
   /// <param name="roughness">�e���B0�`1�͈̔́B</param>
   float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V) {
       // step-4 �t���l�����˂��l�������g�U���ˌ������߂�
       // �����Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
      float3 H = normalize(L + V);

      // 3. �@���ƌ����Ɍ������x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
      float dotNL = saturate(dot(N, L));

      float dotNV = saturate(dot(N, V));

      // �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗�����Z����
      // �ŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
      return (dotNL * dotNV) / PI;
   }

   float4 ShadingResult(PsIn psIn) {
         // �e�N�X�`����������擾����
      float4 albedo = AlbedoTexture.Sample(Sampler, psIn.uv);
      float4 normal = NormalTexture.Sample(Sampler, psIn.uv);
      float4 position = PositionTexture.Sample(Sampler, psIn.uv);
      float4 depth = DepthTexture.Sample(Sampler, psIn.uv);
      float4 material = MaterialTexture.Sample(Sampler, psIn.uv);
      float roughness = material.r;                            // �e��
      float metalness = material.g;                            // �����x
      float specular = material.b;                             // �X�y�L�������̋���
      //float emissive = material.a;                           // �G�~�b�V�u
      float smooth = 1.0f - roughness;
      float3 specColor = albedo.rgb;                          // �X�y�L�������̐F
      float3 N = normal.xyz;
      float3 V = normalize(CameraPosition.xyz - position.xyz);
      float3 L = normalize(-LightDirection.xyz);
      float3 H = normalize(L + V);


      /*
       // �@�����v�Z
      float3 normal = GetNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);

      // step-2 �A���x�h�J���[�A�X�y�L�����J���[�A�����x���T���v�����O����
      // �A���x�h�J���[�i�g�U���ˌ��j
      float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
      // �X�y�L�����J���[�i���ʔ��ˌ��j
      float3 specColor = g_specularMap.SampleLevel(g_sampler, psIn.uv, 0).rgb;
      // �����x
      float metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;

      // �����Ɍ������ĐL�т�x�N�g�����v�Z����
      float3 toEye = normalize(eyePos - psIn.worldPos);

      */


      float3 lig = 0.0f;

       // step-3 �f�B�Y�j�[�x�[�X�̊g�U���˂���������
       // �t���l�����˂��l�������g�U���˂��v�Z
      float diffuseFromFresnel = CalcDiffuseFromFresnel(N, L, V);
      // ���K��Lambert�g�U���˂����߂�
      float NdotL = saturate(dot(N, L));
      float3 lambertDiffuse = LightColor * NdotL;
      // �ŏI�I�Ȋg�U���ˌ����v�Z����
      float3 diffuse = albedo * diffuseFromFresnel * lambertDiffuse;

      // step-6 �N�b�N�g�����X���f���𗘗p�������ʔ��˗����v�Z����
      // �N�b�N�g�����X���f���̋��ʔ��˗����v�Z����
      float3 spec = CookTrranceSpecular(L, V, N, metalness) * LightColor;
      // �����x��������΁A���ʔ��˂̓X�y�L�����J���[�A�Ⴏ��Δ�
      // �X�y�L�����J���[�̋��������ʔ��˗��Ƃ��Ĉ���
      float specTerm = length(specColor.xyz);
      spec *= lerp(float3(specTerm, specTerm, specTerm), specColor, metalness);

      // step-7 ���ʔ��˗����g���āA�g�U���ˌ��Ƌ��ʔ��ˌ�����������
      // ���ʔ��˗���������΁A�g�U���˂͎キ�Ȃ�
      lig += diffuse * (1.0f - specTerm) + spec;

      // �����ɂ���グ
      lig += AmbientLight * albedo;

      float4 finalColor = 1.0f;
      finalColor.xyz = lig;
      return finalColor;
   }
};

// �U�d�̂̔��˗��iF0�j��4%�Ƃ���
uniform float _DielectricF0 = 0.04f;

class DisneyBasePhysicsBasedRendering : Shading {
   float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv) {
   //float3 binSpaceNormal = g_normalMap.SampleLevel(g_sampler, uv, 0.0f).xyz;
   //binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

   //float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

   //return newNormal;
   }

   // �x�b�N�}�����z���v�Z����
   float Beckmann(float m, float t) {
      float t2 = t * t;
      float t4 = t * t * t * t;
      float m2 = m * m;
      float D = 1.0f / (4.0f * m2 * t4);
      D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
      return D;
   }

   // �t���l�����v�Z�BSchlick�ߎ����g�p
   float SpcFresnel(float f0, float u) {
      // from Schlick
      return f0 + (1 - f0) * pow(1 - u, 5);
   }

   // <summary>
   // Cook-Torrance���f���̋��ʔ��˂��v�Z
   // </summary>
   // <param name="L">�����Ɍ������x�N�g��</param>
   // <param name="V">���_�Ɍ������x�N�g��</param>
   // <param name="N">�@���x�N�g��</param>
   // <param name="metallic">�����x</param>
   float CookTorranceSpecular(float3 L, float3 V, float3 N, float metallic) {
      //float microfacet = 1.0f;
      float microfacet = 0.76f;

      // �����x�𐂒����˂̎��̃t���l�����˗��Ƃ��Ĉ���
      // �����x�������قǃt���l�����˂͑傫���Ȃ�
      float f0 = metallic;

      // ���C�g�Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
      float3 H = normalize(L + V);

      // �e��x�N�g�����ǂꂭ�炢���Ă��邩����ς𗘗p���ċ��߂�
      float NdotH = saturate(dot(N, H));
      float VdotH = saturate(dot(V, H));
      float NdotL = saturate(dot(N, L));
      float NdotV = saturate(dot(N, V));

      // D�����x�b�N�}�����z��p���Čv�Z����
      float D = Beckmann(microfacet, NdotH);

      // F����Schlick�ߎ���p���Čv�Z����
      float F = SpcFresnel(f0, VdotH);

      // G�������߂�
      float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

      // m�������߂�
      float m = PI * NdotV * NdotH;

      // �����܂ŋ��߂��A�l�𗘗p���āACook-Torrance���f���̋��ʔ��˂����߂�
      return max(F * D * G / m, 0.0);
   }

   // <summary>
   // �t���l�����˂��l�������g�U���˂��v�Z
   // </summary>
   // <remark>
   // ���̊֐��̓t���l�����˂��l�������g�U���˗����v�Z���܂�
   // �t���l�����˂́A�������̂̕\�ʂŔ��˂��錻�ۂ̂Ƃ��ŁA���ʔ��˂̋����ɂȂ�܂�
   // ����g�U���˂́A�������̂̓����ɓ����āA�����������N�����āA�g�U���Ĕ��˂��Ă������̂��Ƃł�
   // �܂�t���l�����˂��ア�Ƃ��ɂ́A�g�U���˂��傫���Ȃ�A�t���l�����˂������Ƃ��́A�g�U���˂��������Ȃ�܂�
   // </remark>
   // <param name="N">�@��</param>
   // <param name="L">�����Ɍ������x�N�g���B���̕����Ƌt�����̃x�N�g���B</param>
   // <param name="V">�����Ɍ������x�N�g���B</param>
   float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V) {

       // step-1 �f�B�Y�j�[�x�[�X�̃t���l�����˂ɂ��g�U���˂�^�ʖڂɎ�������B
       // �����Ɍ������x�N�g���Ǝ����Ɍ������x�N�g���̃n�[�t�x�N�g�������߂�
      float3 H = normalize(L + V);

      // �e����0.5�ŌŒ�B
      float roughness = 0.5f;

      float energyBias = lerp(0.0f, 0.5f, roughness);
      float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

      // �����Ɍ������x�N�g���ƃn�[�t�x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
      float dotLH = saturate(dot(L, H));

      // �����Ɍ������x�N�g���ƃn�[�t�x�N�g���A
      // �������s�ɓ��˂����Ƃ��̊g�U���˗ʂ����߂Ă���
      float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;

      // �@���ƌ����Ɍ������x�N�g��w�𗘗p���Ċg�U���˗������߂�
      float dotNL = saturate(dot(N, L));
      float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

      // �@���Ǝ��_�Ɍ������x�N�g���𗘗p���Ċg�U���˗������߂�
      float dotNV = saturate(dot(N, V));
      float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

      // �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗���
      // ��Z���čŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
      return (FL * FV * energyFactor);
   }

   float4 ShadingResult(PsIn psIn) {
      // �e�N�X�`����������擾����
      float4 albedo = AlbedoTexture.Sample(Sampler, psIn.uv);
      float4 normal = NormalTexture.Sample(Sampler, psIn.uv);
      float4 position = PositionTexture.Sample(Sampler, psIn.uv);
      float4 depth = DepthTexture.Sample(Sampler, psIn.uv);
      float4 material = MaterialTexture.Sample(Sampler, psIn.uv);
      float roughness = material.r;                            // �e��
      float metalness = material.g;                            // �����x
      float specular = material.b;                             // �X�y�L�������̋���
      //float emissive = material.a;                           // �G�~�b�V�u
      float smooth = 1.0f - roughness;
      float3 specColor = albedo.rgb;                          // �X�y�L�������̐F
      float3 N = normal.xyz;
      float3 V = normalize(CameraPosition.xyz - position.xyz);
      float3 L = normalize(-LightDirection.xyz);
      float3 H = normalize(L + V);

         // �t���l�����˂��l�������g�U���˂��v�Z
      float diffuseFromFresnel = CalcDiffuseFromFresnel(
         N,
         L,
         V
      );

      // ���K�������o�[�g�g�U���˂����߂�
      float NdotL = saturate(dot(N, L));
      float3 lambertDiffuse = LightColor * NdotL / PI;

      // �ŏI�I�Ȋg�U���ˌ����v�Z����
      float3 diffuse = albedo.rgb * diffuseFromFresnel * lambertDiffuse;

      // �N�b�N�E�g�����X���f���̋��ʔ��˗����v�Z����
      float3 spec = CookTorranceSpecular(
         L,
         V,
         N,
         metalness
      );

      // ���C�g�̐F����Z
      spec *= LightColor;

      // �����x��������΁A���ʔ��˂̓X�y�L�����J���[�A�Ⴏ��Δ�
      // �X�y�L�����J���[�̋��������ʔ��˗��Ƃ��Ĉ���
      spec *= lerp(
         float3(1.0f, 1.0f, 1.0f),
         specColor,
         metalness
      );

      // step-6 ���炩�����g���āA�g�U���ˌ��Ƌ��ʔ��ˌ�����������
      // ���炩����������΁A�g�U���˂͎キ�Ȃ�
      float3 lig = diffuse * roughness + spec;

      // �����ɂ���グ
      lig += AmbientLight.rgb * albedo.rgb;

      return float4(lig.rgb, 1.0f);
   }
};

class TestPBR : Shading {

   inline half Fd_Burley(half ndotv, half ndotl, half ldoth, half roughness) {
      half fd90 = 0.5 + 2 * ldoth * ldoth * roughness;
      half lightScatter = (1 + (fd90 - 1) * pow(1 - ndotl, 5));
      half viewScatter = (1 + (fd90 - 1) * pow(1 - ndotv, 5));

      half diffuse = lightScatter * viewScatter;
      // �{���͂���Diffuse���΂Ŋ���ׂ�������Unity�ł̓��K�V�[�ȃ��C�e�B���O�Ƃ̌݊�����ۂ��ߊ���Ȃ�
      //diffuse /= UNITY_PI;
      return diffuse;
   }

   inline float V_SmithGGXCorrelated(float ndotl, float ndotv, float alpha) {
      float lambdaV = ndotl * (ndotv * (1 - alpha) + alpha);
      float lambdaL = ndotv * (ndotl * (1 - alpha) + alpha);

      return 0.5f / (lambdaV + lambdaL + 0.0001);
   }

   inline half D_GGX(half perceptualRoughness, half ndoth, half3 normal, half3 halfDir) {
      half3 ncrossh = cross(normal, halfDir);
      half a = ndoth * perceptualRoughness;
      half k = perceptualRoughness / (dot(ncrossh, ncrossh) + a * a);
      half d = k * k * INV_PI;
      return min(d, 65504.0h);
   }

   inline half3 F_Schlick(half3 f0, half cos) {
      return f0 + (1 - f0) * pow(1 - cos, 5);
   }

   half4 BRDF(half3 albedo, half metallic, half perceptualRoughness, float3 normal, float3 viewDir, float3 lightDir, float3 lightColor) {
      float3 halfDir = normalize(lightDir + viewDir);
      half ndotv = abs(dot(normal, viewDir));
      float ndotl = max(0, dot(normal, lightDir));
      float ndoth = max(0, dot(normal, halfDir));
      half ldoth = max(0, dot(lightDir, halfDir));
      half reflectivity = lerp(_DielectricF0, 1, metallic);
      half3 f0 = lerp(_DielectricF0, albedo, metallic);

      // Diffuse
      half diffuseTerm = Fd_Burley(ndotv, ndotl, ldoth, perceptualRoughness) * ndotl;
      half3 diffuse = albedo * (1 - reflectivity) * lightColor * diffuseTerm;

      // Specular
      float alpha = perceptualRoughness * perceptualRoughness;
      float V = V_SmithGGXCorrelated(ndotl, ndotv, alpha);
      float D = D_GGX(perceptualRoughness, ndotv, normal, halfDir);
      float3 F = F_Schlick(f0, ldoth); // �}�C�N���t�@�Z�b�g�x�[�X�̃X�y�L����BRDF�ł�cos��ldoth���g����
      float3 specular = V * D * F * ndotl * lightColor;
      // �{����Specular�Ƀ΂��|����ׂ��ł͂Ȃ����AUnity�ł̓��K�V�[�ȃ��C�e�B���O�Ƃ̌݊�����ۂ��߁ADiffuse�����炸��Specular��PI���|����
      specular *= PI;
      specular = max(0, specular);

      half3 color = diffuse + specular;
      return half4(color, 1);
   }

   float4 ShadingResult(PsIn psIn) {
      // �e�N�X�`����������擾����
      float4 albedo = AlbedoTexture.Sample(Sampler, psIn.uv);
      float4 normal = NormalTexture.Sample(Sampler, psIn.uv);
      float4 position = PositionTexture.Sample(Sampler, psIn.uv);
      float4 depth = DepthTexture.Sample(Sampler, psIn.uv);
      float4 material = MaterialTexture.Sample(Sampler, psIn.uv);
      float roughness = material.r;                            // �e��
      float metalness = material.g;                            // �����x
      float specular = material.b;                             // �X�y�L�������̋���
      //float emissive = material.a;                           // �G�~�b�V�u
      float smooth = 1.0f - roughness;
      float3 specColor = albedo.rgb;                          // �X�y�L�������̐F
      float3 N = normal.xyz;
      float3 V = normalize(CameraPosition.xyz - position.xyz);
      float3 L = normalize(-LightDirection.xyz);
      float3 H = normalize(L + V);

      float4 color = BRDF(
         albedo,
         metalness,
         roughness,
         normal,
         V,
         L,
         LightColor.rgb
      );

      return color;
   }
};

Shading shading;

float4 main(PsIn psIn) : SV_TARGET{
   if (DepthTexture.Sample(Sampler, psIn.uv).r == 0.0f) discard;
   return shading.ShadingResult(psIn);
}