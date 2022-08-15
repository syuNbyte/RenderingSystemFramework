
// Include
#include"DeferredTextureSampler.hlsl"
#include"ConstantBuffer.hlsl"
#include"ConstValue.hlsl"

struct VsIn {
   float4 position : SV_POSITION;
   float2 uv : TEXCOORD0;
};

// Schlick �� Fresnel �̋ߎ���
float3 FresnelSchlick(float3 normal, float3 lightDirection, float3 specularColor) {
   float NL = max(dot(lightDirection, normal), 0.0);
   return specularColor + (1.0 - specularColor) * pow(1.0 - NL, 5.0);
}

float Fresnel(float3 N, float3 L, float V) {
   // �@���ƌ����Ɍ������x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
   float dotNL = saturate(dot(N, L));

   // �@���Ǝ����Ɍ������x�N�g�����ǂꂾ�����Ă��邩����ςŋ��߂�
   float dotNV = saturate(dot(N, V));

   // �@���ƌ����ւ̕����Ɉˑ�����g�U���˗��ƁA�@���Ǝ��_�x�N�g���Ɉˑ�����g�U���˗���
   // ��Z���čŏI�I�Ȋg�U���˗������߂Ă���BPI�ŏ��Z���Ă���̂͐��K�����s������
   return (dotNL * dotNV);
}

float3 Lambert(float3 albedo, float3 normal) {
   return saturate(dot(LightDirection.xyz, normal.xyz));
}

float3 HalfLambert(float3 albedo, float3 normal) {
   return albedo / PI;
}

float3 Phong(float3 position, float3 normal) {
   float3 H = normalize(normalize(LightDirection.xyz) + normalize(CameraPosition.xyz - position.xyz));
   return pow(max(0, dot(normalize(normal), H)), 50.0f);
}

float3 PhongEx(float3 position, float3 normal, float specularPower) {
   float3 H = normalize(normalize(LightDirection.xyz) + normalize(CameraPosition.xyz - position.xyz));
   return pow(max(0, dot(normalize(normal), H)), 100.0f * specularPower);
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
   float L = normalize(-LightDirection.xyz);
   float H = normalize(L + V);



   // �����o�[�g���˂ƃt�H������
   float3 lambert = Lambert(float3(1.0f, 1.0f, 1.0f), normal.xyz);
   float3 maxLambert = lambert * albedo.rgb;
   float3 minLambert = float3(0.5f, 0.5f, 0.5f) * albedo.rgb;
   lambert = lerp(minLambert, maxLambert, lambert);
   //float3 lambert = HalfLambert(albedo.xyz, normal.xyz);
   //float fresnel = Fresnel(N, L, V);
   float3 fresnel = FresnelSchlick(N, L, albedo.rgb);
   //float3 phong = Phong(position.xyz, normal.xyz);
   float3 phong = PhongEx(position.xyz, normal.xyz, specular);
   //return float4((lambert * albedo.rgb) + (phong * albedo.rgb), albedo.a);
   float3 lambertParam = (albedo.rgb * lambert);
   float3 phongParam = (phong * lerp(float3(1.0f, 1.0f, 1.0f), albedo.rgb, metalness));
   float3 color = lerp(lambertParam, lambertParam + phongParam, metalness);
   return float4(lambertParam, albedo.a);
}

