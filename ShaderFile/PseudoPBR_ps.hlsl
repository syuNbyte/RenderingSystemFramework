
// Include
#include"DeferredTextureSampler.hlsl"
#include"ConstantBuffer.hlsl"
#include"ConstValue.hlsl"

struct VsIn {
   float4 position : SV_POSITION;
   float2 uv : TEXCOORD0;
};

// Schlick の Fresnel の近似式
float3 FresnelSchlick(float3 normal, float3 lightDirection, float3 specularColor) {
   float NL = max(dot(lightDirection, normal), 0.0);
   return specularColor + (1.0 - specularColor) * pow(1.0 - NL, 5.0);
}

float Fresnel(float3 N, float3 L, float V) {
   // 法線と光源に向かうベクトルがどれだけ似ているかを内積で求める
   float dotNL = saturate(dot(N, L));

   // 法線と視線に向かうベクトルがどれだけ似ているかを内積で求める
   float dotNV = saturate(dot(N, V));

   // 法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を
   // 乗算して最終的な拡散反射率を求めている。PIで除算しているのは正規化を行うため
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
   // テクスチャから情報を取得する
   float4 albedo = AlbedoTexture.Sample(Sampler, vsIn.uv);
   float4 normal = NormalTexture.Sample(Sampler, vsIn.uv);
   float4 position = PositionTexture.Sample(Sampler, vsIn.uv);
   float4 depth = DepthTexture.Sample(Sampler, vsIn.uv);
   float4 material = MaterialTexture.Sample(Sampler, vsIn.uv);
   float roughness = material.r;                            // 粗さ
   float metalness = material.g;                            // 金属度
   float specular = material.b;                             // スペキュラ光の強さ
   //float emissive = material.a;                             // エミッシブ
   float smooth = 1.0f - roughness;
   float N = normal.xyz;
   float V = normalize(CameraPosition.xyz - position.xyz);
   float L = normalize(-LightDirection.xyz);
   float H = normalize(L + V);



   // ランバート反射とフォン反射
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

