
// Include
#include"DeferredTextureSampler.hlsl"
#include"ConstantBuffer.hlsl"
#include"ConstValue.hlsl"

struct VsIn {
   float4 position : SV_POSITION;
   float2 uv : TEXCOORD0;
};

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
   //float emissive = material.a;                           // エミッシブ
   float smooth = 1.0f - roughness;
   float N = normal.xyz;
   float V = normalize(CameraPosition.xyz - position.xyz);
   float L = normalize(LightDirection.xyz);
   float H = normalize(L + V);

   // ランバート反射とフォン反射
   float3 lambert = Lambert(albedo.xyz, normal.xyz);
   //float3 lambert = HalfLambert(albedo.xyz, normal.xyz);
   //float3 phong = Phong(position.xyz, normal.xyz);
   float3 phong = PhongEx(position.xyz, normal.xyz, specular);
   //return float4((lambert * albedo.rgb) + (phong * albedo.rgb), albedo.a);
   float3 lambertParam = (lambert * albedo.rgb);
   //float3 phongParam = (phong * lerp(float3(1.0f, 1.0f, 1.0f), albedo.rgb, metalness));
   float3 phongParam = phong;
   float3 color = lambertParam + phongParam;
   return float4(color, albedo.a);
}

