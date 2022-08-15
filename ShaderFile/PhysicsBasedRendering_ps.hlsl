
// Include
#include"DeferredTextureSampler.hlsl"
#include"ConstantBuffer.hlsl"
#include"ConstValue.hlsl"

struct VsIn {
   float4 position : SV_POSITION;
   float2 uv : TEXCOORD0;
};

// �e���v���[�g�\��
/*
float4 main(VsIn vsIn) : SV_TARGET{
   // �e�N�X�`����������擾����
   float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
   float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
   float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
   float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
   float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
   float roughness = material.r;                           // �e��
   float metalness = material.g;                           // �����x
   float specular = material.b;                            // �X�y�L�������̋���
   float smooth = 1.0f - roughness;
   float3 N = normal.xyz;
   float3 V = normalize(CameraPosition.xyz - position.xyz);
   float3 L = normalize(LightDirection.xyz);
   float3 H = normalize(L + V);

   if (depth.r == 0.0f) discard;
}
*/


/*

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
    float3 H = normalize(L+V);

    // �e����0.5�ŌŒ�B
    float roughness = 0.5f;

    float energyBias = lerp(0.0f, 0.5f, roughness);
    float energyFactor = lerp(1.0, 1.0/1.51, roughness);

    // �����Ɍ������x�N�g���ƃn�[�t�x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
    float dotLH = saturate(dot(L,H));

    // �����Ɍ������x�N�g���ƃn�[�t�x�N�g���A
    // �������s�ɓ��˂����Ƃ��̊g�U���˗ʂ����߂Ă���
    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;

    // �@���ƌ����Ɍ������x�N�g��w�𗘗p���Ċg�U���˗������߂�
    float dotNL = saturate(dot(N,L));
    float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

    // �@���Ǝ��_�Ɍ������x�N�g���𗘗p���Ċg�U���˗������߂�
    float dotNV = saturate(dot(N,V));
    float FV =  (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

    // �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗���
    // ��Z���čŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
    return (FL*FV * energyFactor);

    

//===================================================================================
//===================================================================================

   // step-4 �t���l�����˂��l�������g�U���ˌ������߂�

   // �@���ƌ����Ɍ������x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
   float dotNL = saturate(dot(N, L));

   // �@���Ǝ����Ɍ������x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
   float dotNV = saturate(dot(N, V));

   // �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗���
   // ��Z���čŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
   return (dotNL * dotNV);
}

float4 main(VsIn vsIn) : SV_TARGET{
   // �e�N�X�`����������擾����
   float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
   float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
   float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
   float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
   float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
   float roughness = material.r;                           // �e��
   float metalness = material.g;                           // �����x
   float specular = material.b;                            // �X�y�L�������̋���
   float smooth = 1.0f - roughness;
   float N = normal.xyz;                                    // �@��
   float V = normalize(CameraPosition.xyz - position.xyz);  // �s�N�Z������J�����ւ̃x�N�g��
   float L = normalize(LightDirection.xyz);                 // ���C�g�x�N�g��
   float H = normalize(L + V);                              // �킩���

   if (depth.r == 0.0f) discard;

   // PBR�Ɏg�p����p�����[�^
   float3 specColor = albedo.rgb;                          // �X�y�L�������̐F

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
   //return float4(diffuse, 1.0f);
   //return float4(lambertDiffuse, 1.0f);
   //return float4(spec, 1.0f);
}

*/

// �����Ȃ�
/*

// �U�d�̂̔��˗��iF0�j��4%�Ƃ���
#define _DielectricF0 0.04f;

// �@�����z�֐��iD���j
float D_GGX(float ndoth, float perceptualRoughness) {
   float a = ndoth * perceptualRoughness;
   float k = perceptualRoughness / (1.0 - ndoth * ndoth + a * a);
   return k * k * INV_PI;
}

// �@�����z�֐��iD���j
half D_GGX(half perceptualRoughness, half ndoth, half3 normal, half3 halfDir) {
   half3 ncrossh = cross(normal, halfDir);
   half a = ndoth * perceptualRoughness;
   half k = perceptualRoughness / (dot(ncrossh, ncrossh) + a * a);
   half d = k * k * INV_PI;
   return min(d, 65504.0);
}

// �􉽌������iV���j
float V_SmithGGXCorrelated(float ndotl, float ndotv, float alpha) {
   float lambdaV = ndotl * (ndotv * (1 - alpha) + alpha);
   float lambdaL = ndotv * (ndotl * (1 - alpha) + alpha);
   return 0.5f / (lambdaV + lambdaL + 0.0001);
}

// �t���l�����iF���j
half3 F_Schlick(half3 f0, half cos) {
   return f0 + (1 - f0) * pow(1 - cos, 5);
}

// �g�U���˃��f��
half Fd_Burley(half ndotv, half ndotl, half ldoth, half roughness) {
   half fd90 = 0.5 + 2 * ldoth * ldoth * roughness;
   half lightScatter = (1 + (fd90 - 1) * pow(1 - ndotl, 5));
   half viewScatter = (1 + (fd90 - 1) * pow(1 - ndotv, 5));

   half diffuse = lightScatter * viewScatter;
   // �{���͂���Diffuse���΂Ŋ���ׂ�������Unity�ł̓��K�V�[�ȃ��C�e�B���O�Ƃ̌݊�����ۂ��ߊ���Ȃ�
   //diffuse /= UNITY_PI;
   return diffuse;
}

float3 BRDF(float3 albedo, float roughness, float metalness, float3 normal, float3 viewDir, float3 lightDir, float3 lightColor) {
   float3 halfDir = normalize(lightDir + viewDir);
   half ndotv = abs(dot(normal, viewDir));
   float ndotl = max(0, dot(normal, lightDir));
   float ndoth = max(0, dot(normal, halfDir));
   half ldoth = max(0, dot(lightDir, halfDir));
   half reflectivity = lerp(_DielectricF0, 1, metalness);
   half3 f0 = lerp(_DielectricF0, albedo, metalness);

   // Diffuse
   half diffuseTerm = Fd_Burley(ndotv, ndotl, ldoth, roughness) * ndotl;
   half3 diffuse = albedo * (1 - reflectivity) * lightColor * diffuseTerm;

   // Specular
   float alpha = roughness * roughness;
   float V = V_SmithGGXCorrelated(ndotl, ndotv, alpha);
   float D = D_GGX(roughness, ndotv, normal, halfDir);
   float3 F = F_Schlick(f0, ldoth); // �}�C�N���t�@�Z�b�g�x�[�X�̃X�y�L����BRDF�ł�cos��ldoth���g����
   float3 specular = V * D * F * ndotl * lightColor;
   // �{����Specular�Ƀ΂��|����ׂ��ł͂Ȃ����AUnity�ł̓��K�V�[�ȃ��C�e�B���O�Ƃ̌݊�����ۂ��߁ADiffuse�����炸��Specular��PI���|����
   specular *= PI;
   specular = max(0, specular);

   half3 color = diffuse + specular;
   return color;
}


float4 main(VsIn vsIn) : SV_TARGET{
   // �e�N�X�`����������擾����
   float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
   float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
   float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
   float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
   float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
   float roughness = material.r;
   float metalness = material.g;
   float specularPower = material.b;

   return float4(
      BRDF(
         albedo.xyz,
         roughness,
         metalness,
         normal.xyz,
         normalize(CameraPosition.xyz - position.xyz),
         LightDirection,
         LightColor
      ),
   1.0f);
}

*/

// ����
/*

// Trowbride-Reitz
float GGX(float3 halfVector, float3 normal, float roughness) {
   float alpha2 = pow(roughness, 4.0);
   float NH = dot(normal, halfVector);
   float d = NH * NH * (alpha2 - 1.0) + 1.0;
   return alpha2 / (PI * d * d);
}

// Smith Joint Masking and Shadowing Function
float MaskingAndShadowing(float3 normal, float3 lightDirection, float3 viewDirection, float roughness) {
   float alpha2 = pow(roughness, 4.0);
   float NL = max(dot(normal, lightDirection), 0.0);
   float NV = max(dot(normal, viewDirection), 0.0);
   float lambdaL = NV * sqrt(NL * NL * (1.0 - alpha2) + alpha2);
   float lambdaV = NL * sqrt(NV * NV * (1.0 - alpha2) + alpha2);
   return 0.5 / (lambdaL + lambdaV + 1e-6);
}

// Schlick �� Fresnel �̋ߎ���
float3 Fresnel(float3 normal, float3 lightDirection, float3 specularColor) {
   float NL = max(dot(lightDirection, normal), 0.0);
   return specularColor + (1.0 - specularColor) * pow(1.0 - NL, 5.0);
}

 // ���K�������o�[�gBRDF
float3 DiffuseBRDF(
   float3 viewDirection,
   float3 lightDirection,
   float3 normal,
   float3 albedo,
   float roughness
) {
   return albedo / PI;
}

// Cook-Torrance
float SpecularBRDF(
   float3 viewDirection,
   float3 lightDirection,
   float3 normal,
   float roughness,
   float3 specularColor
) {
   float3 halfVector = normalize(lightDirection + viewDirection);
   float3 ret = float3(1.0, 1.0, 1.0);
   ret *= GGX(halfVector, normal, roughness);
   ret *= MaskingAndShadowing(normal, lightDirection, viewDirection, roughness);
   ret *= Fresnel(halfVector, lightDirection, specularColor);
   return ret;
}

float4 main(VsIn vsIn) : SV_TARGET{
   // �e�N�X�`����������擾����
   float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
   float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
   float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
   float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
   float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
   float roughness = material.r;
   float metalness = material.g;
   float specular = material.b;

   if (depth.r == 0.0f) discard;

   // ���̑��p�����[�^
   float3 viewDirection = normalize(CameraPosition - position);
   float3 lightIrradiance = max(dot(normal, LightDirection), 0.0f) * specular;
   float3 fr = float3(0.0f, 0.0f, 0.0f);

   // diffuse
   fr += DiffuseBRDF(viewDirection, LightDirection, normal, albedo, roughness);

   // speular
   fr += SpecularBRDF(viewDirection, LightDirection, normal, roughness, albedo);

   // ret
   //float3 intensity = fr * lightIrradiance + AmbientLight;
   float3 intensity = fr * lightIrradiance;

   // return
   return float4(intensity, 1.0f);
}

*/

// ����
/*

float3 lambertBRDF(float3 c, float3 i, float3 o, float3 n, float roughness) {
   return c / PI;
}

// CTD
float3 GGX_Distribution(float3 l, float3 v, float3 n, float3 h, float roughness) {
   float alpha2 = pow(roughness, 4.0f);
   float nh2 = pow(dot(n, h), 2.0f);
   return alpha2 / (PI * pow(nh2 * (alpha2 - 1.0f) + 1.0f, 2.0f));
}

// CTG
float3 GGX_GeometryTerm(float3 l, float3 v, float3 n, float3 h, float roughness) {
   float k = pow(roughness + 1.0f, 2.0f) / 8.0f;
   float ln = dot(l, n);
   float vn = dot(v, n);
   return (ln / (ln * (1.0f - k) + k)) * (vn / (vn * (1.0f - k) + k));
}

// CTF
float3 Schlick(float3 l, float3 v, float3 n, float3 h, float roughness) {
   float f0 = max(dot(l, n), 0.0f);
   float vh = dot(v, h);
   return f0 + pow(1.0f - vh, 5.0f) * (1.0f - f0);
}

float3 cookTorranceBRDF(float3 l, float3 v, float3 n, float3 r) {
   float3 h = normalize(l + v);
   return float3(
      GGX_Distribution(l, v, n, h, r) * Schlick(l, v, n, h, r) * GGX_GeometryTerm(l, v, n, h, r)
      /
      (4.0 * dot(l, n) * dot(v, n))
      );
}

float3 pbrBRDF(float3 c, float3 i, float3 o, float3 n, float3 r) {
   float k0 = 0.75; // TODO
   return float3(
      lambertBRDF(c, i, o, n, r) * k0
      +
      cookTorranceBRDF(i, o, n, r) * (1.0f - k0)
      );
}

float lambert(float3 direction, float3 normal) {
   return max(0.0f, dot(direction, normal));
}

float3 directionalLight(float3 albedo, float3 normal, float3 pos, float roughness) {
   float3 ret = float3(0.0f, 0.0f, 0.0f);
   float3 li = lambert(normal, -LightDirection) * LightColor;
   float3 color = albedo;
   float3 lightdir = LightDirection;
   float3 destcamera = CameraPosition - pos;
   ret = li * pbrBRDF(color, lightdir, destcamera, normal, roughness) * color;
   return ret;
}

float4 main(VsIn vsIn) : SV_TARGET{
   // �e�N�X�`����������擾����
   float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
   float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
   float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
   float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
   float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
   float roughness = material.r;
   float metalness = material.g;
   float specPower = material.b;

   if (depth.r == 0.0f) discard;

   return float4(directionalLight(albedo, normal, position, roughness), 1.0f);
}

*/

// �������ǂ��ꂢ����Ȃ�
/*

float D_GGX(float3 H, float3 N, float _roughness) {
   float NdotH = saturate(dot(H, N));
   float roughness = saturate(_roughness);
   float alpha = roughness * roughness;
   float alpha2 = alpha * alpha;
   float t = ((NdotH * NdotH) * (alpha2 - 1.0) + 1.0);
   return alpha2 / (PI * t * t);
}

float Flesnel(float3 V, float3 H) {
   float reflectance = 0.5f;
   float VdotH = saturate(dot(V, H));
   float F0 = saturate(reflectance);
   float F = pow(1.0 - VdotH, 5.0);
   F *= (1.0 - F0);
   F += F0;
   return F;
}

float G_CookTorrance(float3 L, float3 V, float3 H, float3 N) {
   float NdotH = saturate(dot(N, H));
   float NdotL = saturate(dot(N, L));
   float NdotV = saturate(dot(N, V));
   float VdotH = saturate(dot(V, H));

   float NH2 = 2.0 * NdotH;
   float g1 = (NH2 * NdotV) / VdotH;
   float g2 = (NH2 * NdotL) / VdotH;
   float G = min(1.0, min(g1, g2));
   return G;
}

float4 GetColor(
   float4 _albedo,
   float4 _normal,
   float4 _position,
   float _roughness,
   float _metalness,
   float _specPower
) {
   float3 ambientLight = AmbientLight * _albedo.rgb;
   float3 lightDirNormal = normalize(-LightDirection);
   float NdotL = saturate(dot(_normal, lightDirNormal));
   float3 viewDirNormal = normalize((float4(CameraPosition.xyz, 1.0f) - _position).xyz);
   float NdotV = saturate(dot(_normal, viewDirNormal));
   float3 halfVec = normalize(lightDirNormal + viewDirNormal);

   float D = D_GGX(halfVec, _normal, _roughness);
   float F = Flesnel(viewDirNormal, halfVec);
   float G = G_CookTorrance(lightDirNormal, viewDirNormal, halfVec, _normal);

   float specularRef = (D * F * G) / (4.0f * NdotV * NdotL + 0.000001f);
   float3 diffuseRef = LightColor.rgb * _albedo.rgb * NdotL;

   return float4(ambientLight + diffuseRef + specularRef, 1.0f);
}

float4 main(VsIn vsIn) : SV_TARGET{
   // �e�N�X�`����������擾����
   float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
   float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
   float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
   float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
   float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
   float roughness = material.r;
   float metalness = material.g;
   float specPower = material.b;

   if (depth.r == 0.0f) discard;

   return GetColor(albedo, normal, position, roughness, metalness, specPower);
}

*/

/*

#define RF0_DIELECTRICS         0.04

void ComputeAlbedoRf0(float3 albedo, float metalness, out float3 Rf0, out float3 outAlbedo) {
   outAlbedo = albedo * saturate(1.0 - metalness);
   Rf0 = lerp(RF0_DIELECTRICS, albedo, metalness);
}

float Pow5(float x) {
   float xx = x * x;
   return xx * xx * x;
}

float3 DiffuseTermBurley(float roughness, float NoL, float NoV, float VoH) {
   
   // float f = 2.0 * VoH * VoH * roughness - 0.5;
   // float FdV = f * pow(saturate(1.0 - NoV), 5.0) + 1.0;
   // float FdL = f * pow(saturate(1.0 - NoL), 5.0) + 1.0;
   // float d = FdV * FdL;
   // return d / PI;
   float FD90 = 0.5 + 2 * VoH * VoH * roughness;
   float FdV = 1 + (FD90 - 1) * Pow5(1 - NoV);
   float FdL = 1 + (FD90 - 1) * Pow5(1 - NoL);
   return ((1 / PI) * FdV * FdL);
}

float DistributionTermGgx(float roughness, float NoH) {
   float p = pow(roughness, 4.0f);
   float q1 = pow(NoH * NoH * (p - 1) + 1, 2.0f);
   float q = 1 / (PI * q1);

   return p * q;
}

float GeometryTermSmith(float roughness, float NoV, float NoL) {
   float k = pow((roughness + 1.0), 2.0) / 8.0;
   float q1 = NoV * (1 - k) + k;
   float q2 = NoL * (1 - k) + k;
   float G1 = NoV / q1;
   float G2 = NoL / q2;

   return G1 * G2;
}

float3 FresnelTermShlick(float3 F0, float cosa) {
   return F0 + (1 - F0) * pow(saturate(1.0 - cosa), 5.0);
}


float3 RedGuard(float3 v) {
   if (any(v < 0) || any(isnan(v)) || any(!isfinite(v))) {
      return float3(1.0, 0.0, 0.0);
   }

   return v;
}

float3 SpecularTerm(float roughness, float NoH, float NoL, float NoV, float3 F0, float VoH) {
   float3 p = DistributionTermGgx(roughness, NoH) * GeometryTermSmith(roughness, NoV, NoL) * FresnelTermShlick(F0, VoH);
   return RedGuard(p);
}

float3 Brdf(float roughness, float metalness, float3 Albedo, float3 N, float3 V, float3 L) {
   float3 H = normalize(L + V);
   float NoH = saturate(dot(N, H));
   float VoH = saturate(dot(V, H));
   float NoL = saturate(dot(N, L));
   float NoV = saturate(dot(N, V));
   float3 albedo;
   float3 F0;
   ComputeAlbedoRf0(Albedo, metalness, F0, albedo);
   float roughnessL = max(0.01f, roughness);

   return (DiffuseTermBurley(roughness, NoL, NoV, VoH) * albedo + SpecularTerm(roughnessL, NoH, NoL, NoV, F0, VoH)) * NoL;
   //return NoL * albedo;

}

float4 main(VsIn vsIn) : SV_TARGET{
   // �e�N�X�`����������擾����
   float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
   float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
   float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
   float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
   float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
   float roughness = material.r;
   float metalness = material.g;
   float specPower = material.b;

   if (depth.r == 0.0f) discard;

   float N = normal.xyz;
   float V = normalize(CameraPosition.xyz - position.xyz);
   float L = normalize(-LightDirection.xyz);

   return float4(
      Brdf(
         roughness,
         metalness,
         albedo.rgb,
         N,
         V,
         L
      ),
      1.0f
   );
}

*/

// �����Ȃ�
/*

struct GeometricContext {
   float3 position;
   float3 normal;
   float3 viewDir;
};

struct IncidentLight {
   float3 direction;
   float3 color;
};

struct DirectionalLight {
   float3 direction;
   float3 color;
};

struct PointLight {
   float3 position;
   float3 color;
   float distance;
   float decay;
};

struct SpotLight {
   float3 position;
   float3 direction;
   float3 color;
   float distance;
   float decay;
   float coneCos;
   float penumbraCos;
};

// Normalized Lambert
float3 DiffuseBRDF(float3 diffuseColor) {
   return diffuseColor / PI;
}

float3 F_Schlick(float3 specularColor, float3 H, float3 V) {
   return (specularColor + (1.0 - specularColor) * pow(1.0 - saturate(dot(V, H)), 5.0));
}

float D_GGX(float a, float dotNH) {
   float a2 = a * a;
   float dotNH2 = dotNH * dotNH;
   float d = dotNH2 * (a2 - 1.0) + 1.0;
   return a2 / (PI * d * d);
}

float G_Smith_Schlick_GGX(float a, float dotNV, float dotNL) {
   float k = a * a * 0.5 + EPSILON;
   float gl = dotNL / (dotNL * (1.0 - k) + k);
   float gv = dotNV / (dotNV * (1.0 - k) + k);
   return gl * gv;
}

// Cook-Torrance
float3 SpecularBRDF(const in IncidentLight directLight, const in GeometricContext geometry, float3 specularColor, float roughnessFactor) {

   float3 N = geometry.normal;
   float3 V = geometry.viewDir;
   float3 L = directLight.direction;

   float dotNL = saturate(dot(N, L));
   float dotNV = saturate(dot(N, V));
   float3 H = normalize(L + V);
   float dotNH = saturate(dot(N, H));
   float dotVH = saturate(dot(V, H));
   float dotLV = saturate(dot(L, V));
   float a = roughnessFactor * roughnessFactor;

   float D = D_GGX(a, dotNH);
   float G = G_Smith_Schlick_GGX(a, dotNV, dotNL);
   float3 F = F_Schlick(specularColor, V, H);
   return (F * (G * D)) / (4.0 * dotNL * dotNV + EPSILON);
}

float4 main(VsIn vsIn) : SV_TARGET{
   // �e�N�X�`����������擾����
   float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
   float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
   float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
   float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
   float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
   float roughness = material.r;
   float metalness = material.g;
   float specPower = material.b;

   if (depth.r == 0.0f) discard;


}

*/


/*

// # Copyright Disney Enterprises, Inc.  All rights reserved.
// #
// # Licensed under the Apache License, Version 2.0 (the "License");
// # you may not use this file except in compliance with the License
// # and the following modification to it: Section 6 Trademarks.
// # deleted and replaced with:
// #
// # 6. Trademarks. This License does not grant permission to use the
// # trade names, trademarks, service marks, or product names of the
// # Licensor and its affiliates, except as required for reproducing
// # the content of the NOTICE file.
// #
// # You may obtain a copy of the License at
// # http://www.apache.org/licenses/LICENSE-2.0
float SchlickFresnel(float cosTheta) {
   return pow(clamp((1 - cosTheta), 0, 1), 5.0);
}

float D_GTR1(float NdotH, float a) {
   float a2 = a * a;
   float tmp = 1 + (a2 - 1) * NdotH * NdotH;
   return (a2 - 1) / (PI * log(a2) * tmp);
}

float D_GTR2aniso(float NdotH, float HdotX, float HdotY, float ax, float ay) {
   float tmp = (HdotX * HdotX) / (ax * ax) + (HdotY * HdotY) / (ay * ay) + NdotH * NdotH;
   return 1 / (PI * ax * ay * tmp * tmp);
}

float G_GGX(float NdotV, float a) {
   float a2 = a * a;
   float down = NdotV + sqrt(a2 + NdotV * NdotV - a2 * NdotV * NdotV);
   return 1 / down;
}

float G_GGXaniso(float NdotV, float VdotX, float VdotY, float ax, float ay) {
   float tmp = VdotX * VdotX * ax * ax + VdotY * VdotY * ay * ay + NdotV * NdotV;
   float down = NdotV + sqrt(tmp);
   return 1 / down;
}

float3 DisneyBRDF(float3 L, float3 V, float3 N, float3 H, float3 X, float3 Y, float3 baseColor, float subsurface, float metallic, float specular, float specularTint, float roughness, float anisotropic, float sheen, float sheenTint, float clearcoat, float clearcoatGloss) {
   float NdotL = dot(N, L);
   float NdotV = dot(N, V);
   if (NdotL < 0 || NdotV < 0) return float3(0);

   float NdotH = dot(N, H);
   float LdotH = dot(L, H);

   float luminance = 0.3 * baseColor.r + 0.6 * baseColor.g + 0.1 * baseColor.b;
   float3 C_tint = luminance > 0 ? baseColor / luminance : float3(1);
   float3 C_spec = mix(specular * 0.08 * mix(float3(1), C_tint, specularTint), baseColor, metallic);
   float3 C_sheen = mix(float3(1), C_tint, sheenTint);

   // diffuse
   float F_i = SchlickFresnel(NdotL);
   float F_o = SchlickFresnel(NdotV);
   float F_d90 = 0.5 + 2 * LdotH * LdotH * roughness;
   float F_d = mix(1.0, F_d90, F_i) * mix(1.0, F_d90, F_o);

   float F_ss90 = LdotH * LdotH * roughness;
   float F_ss = mix(1.0, F_ss90, F_i) * mix(1.0, F_ss90, F_o);
   float ss = 1.25 * (F_ss * (1 / (NdotL + NdotV) - 0.5) + 0.5);

   float FH = SchlickFresnel(LdotH);
   float3 F_sheen = FH * sheen * C_sheen;

   float3 BRDFdiffuse = ((1 / PI) * mix(F_d, ss, subsurface) * baseColor + F_sheen) * (1 - metallic);

   // specular
   float aspect = sqrt(1 - anisotropic * 0.9);
   float roughness2 = roughness * roughness;
   float a_x = max(0.001, roughness2 / aspect);
   float a_y = max(0.001, roughness2 * aspect);
   float D_s = D_GTR2aniso(NdotH, dot(H, X), dot(H, Y), a_x, a_y);
   float3 F_s = mix(C_spec, float3(1), FH);
   float G_s = G_GGXaniso(NdotL, dot(L, X), dot(L, Y), a_x, a_y) * G_GGXaniso(NdotV, dot(V, X), dot(V, Y), a_x, a_y);

   float3 BRDFspecular = G_s * F_s * D_s;

   // clearcoat
   float D_r = D_GTR1(NdotH, mix(0.1, 0.001, clearcoatGloss));
   float F_r = mix(0.04, 1.0, FH);
   float G_r = G_GGX(NdotL, 0.25) * G_GGX(NdotV, 0.25);

   float3 BRDFclearcoat = float3(0.25 * clearcoat * G_r * F_r * D_r);

   return BRDFdiffuse + BRDFspecular + BRDFclearcoat;
}

float4 main(VsIn vsIn) : SV_TARGET{
   // �e�N�X�`����������擾����
   float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
   float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
   float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
   float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
   float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
   float roughness = material.r;                           // �e��
   float metalness = material.g;                           // �����x
   float specular = material.b;                            // �X�y�L�������̋���
   float smooth = 1.0f - roughness;
   float N = normal.xyz;
   float V = normalize(CameraPosition.xyz - position.xyz);
   float L = normalize(-LightDirection.xyz);
   float H = normalize(L + V);

   if (depth.r == 0.0f) discard;

   // �ׂ������̃p�����[�^
   float subsurface = 0.0;
   float specular = 0.5;
   float specularTint = 0.0;
   float sheen = 0.0;
   float sheenTint = 0.5;
   float clearcoat = 0.0;
   float clearcoatGloss = 1.0;

   float3 color = DisneyBRDF(
      L,
      V,
      N,
      H,
      // tangent,
      // bitangent,
      albedo,
      subsurface,
      metalness,
      specular,
      specularTint,
      roughness,
      anisotropic,
      sheen,
      sheenTint,
      clearcoat,
      clearcoatGloss
   )

}

*/




/*

float3 NormalizedLambert(float3 diffuseColor) {
   return diffuseColor / PI;
}

float3 F_Schlick(float3 F0, float3 H, float3 V) {
   return (F0 + (1.0 - F0) * pow(1.0 - max(dot(V, H), 0), 5.0));
}

float D_GGX(float a, float NoH) {
   float a2 = a * a;
   float NoH2 = NoH * NoH;
   float d = NoH2 * (a2 - 1.0) + 1.0;
   return a2 / (PI * d * d);
}

float G_SchlickGGX(float a, float NoV) {
   float EPSILON = 0.001;
   float k = a * a / 2 + EPSILON;
   float up = NoV;
   float down = NoV * (1 - k) + k;
   return up / down;
}

float G_Smith(float a, float NoV, float NoL) {
   float ggx0 = G_SchlickGGX(1, NoV);
   float ggx1 = G_SchlickGGX(1, NoL);
   return ggx0 * ggx1;
}

float3 BRDF(float3 albedo, float metallic, float roughness, float3 N, float3 V, float3 L, float3 H) {
   float EPSILON = 0.001;
   float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);
   float NoH = max(dot(N, H), 0);
   float NoV = max(dot(N, V), 0);
   float NoL = max(dot(N, L), 0);
   float a = roughness * roughness;

   // specular
   float3 F = F_Schlick(F0, H, V);
   float D = D_GGX(a, NoH);
   float G = G_Smith(a, NoV, NoL);
   float3 up = F * D * G;
   float down = max(4.0 * NoV * NoL, EPSILON);
   float3 specular = up / down;

   // diffuse
   float3 diffuse = NormalizedLambert(albedo);

   float3 kD = float3(1.0, 1.0f, 1.0f) - F;
   kD *= 1.0 - metallic;
   return kD * diffuse + specular;
}

float4 main(VsIn vsIn) : SV_TARGET{
   // �e�N�X�`����������擾����
   float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
   float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
   float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
   float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
   float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
   float roughness = material.r;                            // �e��
   float metalness = material.g;                            // �����x
   float specular = material.b;                             // �X�y�L�������̋���
   //float emissive = material.a;                             // �G�~�b�V�u
   float smooth = 1.0f - roughness;
   float N = normal.xyz;
   float V = normalize(CameraPosition.xyz - position.xyz);
   float L = normalize(LightDirection.xyz);
   float H = normalize(L + V);

   if (depth.r == 0.0f) discard;

   float3 color = BRDF(
      albedo.rgb,
      metalness,
      roughness,
      N,
      V,
      L,
      H
   );

   return float4(color, 1.0f);
}

*/


/*

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
   float microfacet = 0.76f;
   //float microfacet = 0.76f;

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
   // step-4 �t���l�����˂��l�������g�U���ˌ������߂�

   // �@���ƌ����Ɍ������x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
   float dotNL = saturate(dot(N, L));

   // �@���Ǝ����Ɍ������x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
   float dotNV = saturate(dot(N, V));

   // �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗���
   // ��Z���čŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
   return (dotNL * dotNV);
}

float4 main(VsIn vsIn) : SV_TARGET{
   // �e�N�X�`����������擾����
   float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
   float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
   float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
   float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
   float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
   float roughness = material.r;                            // �e��
   float metalness = material.g;                            // �����x
   float specular = material.b;                             // �X�y�L�������̋���
   //float emissive = material.a;                           // �G�~�b�V�u
   float smooth = 1.0f - roughness;
   float3 specColor = albedo.rgb;                          // �X�y�L�������̐F
   float N = normal.xyz;
   float V = normalize(CameraPosition.xyz - position.xyz);
   float L = normalize(LightDirection.xyz);
   float H = normalize(L + V);

   if (depth.r == 0.0f) discard;

   //// PBR�Ɏg�p����p�����[�^
   //float roughness = material.r;                           // �e��
   //float metalness = material.g;                           // �����x
   //float specular = material.b;                            // �X�y�L�������̋���
   //float3 specColor = albedo.rgb;                          // �X�y�L�������̐F
   //float toCamera = normalize(position - CameraPosition);  // �s�N�Z������J�����ւ̃x�N�g��

   // �t���l�����˂��l�������g�U���˂��v�Z
   float diffuseFromFresnel = CalcDiffuseFromFresnel(
      N,
      L,
      V
   );

   // ���K�������o�[�g�g�U���˂����߂�
   float NdotL = saturate(dot(normal, LightDirection));
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
   spec *= lerp(float3(1.0f, 1.0f, 1.0f), specColor, metalness);

   // step-6 ���炩�����g���āA�g�U���ˌ��Ƌ��ʔ��ˌ�����������
   // ���炩����������΁A�g�U���˂͎キ�Ȃ�
   float3 lig = diffuse * smooth + spec;

   // �����ɂ���グ
   lig += AmbientLight.rgb * albedo.rgb;

   return float4(lig.rgb, 1.0f);
   return float4(diffuse, 1.0f);
   return float4(lambertDiffuse, 1.0f);
   return float4(spec, 1.0f);
}

*/


///////////////////////////////////////////////////
// �֐�
///////////////////////////////////////////////////

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

float4 main(VsIn vsIn) : SV_TARGET {

   // �e�N�X�`����������擾����
   float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
   float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
   float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
   float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
   float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
   float roughness = material.r;                            // �e��
   float metalness = material.g;                            // �����x
   float specular = material.b;                             // �X�y�L�������̋���
   //float emissive = material.a;                           // �G�~�b�V�u
   float smooth = 1.0f - roughness;
   float3 specColor = albedo.rgb;                          // �X�y�L�������̐F
   float N = normal.xyz;
   float V = normalize(CameraPosition.xyz - position.xyz);
   float L = normalize(LightDirection.xyz);
   float H = normalize(L + V);


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