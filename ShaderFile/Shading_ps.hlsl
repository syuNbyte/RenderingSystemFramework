
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
      // テクスチャから情報を取得する
      float4 albedo = AlbedoTexture.Sample(Sampler, psIn.uv);
      float4 normal = NormalTexture.Sample(Sampler, psIn.uv);
      float4 position = PositionTexture.Sample(Sampler, psIn.uv);
      float4 depth = DepthTexture.Sample(Sampler, psIn.uv);
      float4 material = MaterialTexture.Sample(Sampler, psIn.uv);
      float roughness = material.r;                            // 粗さ
      float metalness = material.g;                            // 金属度
      float specular = material.b;                             // スペキュラ光の強さ
      //float emissive = material.a;                           // エミッシブ
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
      // テクスチャから情報を取得する
      float4 albedo = AlbedoTexture.Sample(Sampler, psIn.uv);
      float4 normal = NormalTexture.Sample(Sampler, psIn.uv);
      float4 position = PositionTexture.Sample(Sampler, psIn.uv);
      float4 depth = DepthTexture.Sample(Sampler, psIn.uv);
      float4 material = MaterialTexture.Sample(Sampler, psIn.uv);
      float roughness = material.r;                            // 粗さ
      float metalness = material.g;                            // 金属度
      float specular = material.b;                             // スペキュラ光の強さ
      //float emissive = material.a;                           // エミッシブ
      float smooth = 1.0f - roughness;
      float3 N = normal.xyz;
      float3 V = normalize(CameraPosition.xyz - position.xyz);
      float3 L = normalize(-LightDirection.xyz);
      float3 H = normalize(L + V);

      // ランバート反射とフォン反射
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

   // ベックマン分布を計算する
   float Beckmann(float m, float t) {
      float t2 = t * t;
      float t4 = t * t * t * t;
      float m2 = m * m;
      float D = 1.0f / (4.0f * m2 * t4);
      D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
      return D;
   }
   // フレネルを計算。Schlick近似を使用
   float SpcFresnel(float f0, float u) {
       // from Schlick
      return f0 + (1 - f0) * pow(1 - u, 5);
   }
   /// <summary>
   /// クックトランスモデルの鏡面反射を計算
   /// </summary>
   /// <param name="L">光源に向かうベクトル</param>
   /// <param name="V">視点に向かうベクトル</param>
   /// <param name="N">法線ベクトル</param>
   /// <param name="metaric">金属度</param>
   float CookTrranceSpecular(float3 L, float3 V, float3 N, float metaric) {
      float microfacet = 0.76f;
      // 金属度を垂直入射の時のフレネル反射率として扱う
      // 金属度が高いほどフレネル反射は大きくなる
      float f0 = metaric;
      // ライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
      float3 H = normalize(L + V);

      // 各種ベクトルがどれくらい似ているかを内積を利用して求める
      float NdotH = saturate(dot(N, H));
      float VdotH = saturate(dot(V, H));
      float NdotL = saturate(dot(N, L));
      float NdotV = saturate(dot(N, V));

      // D項をベックマン分布を用いて計算する
      float D = Beckmann(microfacet, NdotH);
      // F項をSchlick近似を用いて計算する
      float F = SpcFresnel(f0, VdotH);
      // G項を求める
      float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));
      // m項を求める
      float m = PI * NdotV * NdotH;
      // ここまで求めた、値を利用して、クックトランスモデルの鏡面反射を求める
      return max(F * D * G / m, 0.0);
   }

   /// <summary>
   /// フレネル反射を考慮した拡散反射を計算
   /// </summary>
   /// <remark>
   /// この関数はフレネル反射を考慮した拡散反射率を計算します
   /// フレネル反射は、光が物体の表面で反射する現象のとこで、鏡面反射の強さになります
   /// 一方拡散反射は、光が物体の内部に入って、内部錯乱を起こして、拡散して反射してきた光のことです
   /// つまりフレネル反射が弱いときには、拡散反射が大きくなり、フレネル反射が強いときは、拡散反射が小さくなります
   ///
   /// </remark>
   /// <param name="N">法線</param>
   /// <param name="L">光源に向かうベクトル。光の方向と逆向きのベクトル。</param>
   /// <param name="V">視線に向かうベクトル。</param>
   /// <param name="roughness">粗さ。0～1の範囲。</param>
   float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V) {
       // step-4 フレネル反射を考慮した拡散反射光を求める
       // 光源に向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
      float3 H = normalize(L + V);

      // 3. 法線と光源に向かうベクトルがどれだけ似ているかを内積で求める
      float dotNL = saturate(dot(N, L));

      float dotNV = saturate(dot(N, V));

      // 法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を乗算して
      // 最終的な拡散反射率を求めている。PIで除算しているのは正規化を行うため
      return (dotNL * dotNV) / PI;
   }

   float4 ShadingResult(PsIn psIn) {
         // テクスチャから情報を取得する
      float4 albedo = AlbedoTexture.Sample(Sampler, psIn.uv);
      float4 normal = NormalTexture.Sample(Sampler, psIn.uv);
      float4 position = PositionTexture.Sample(Sampler, psIn.uv);
      float4 depth = DepthTexture.Sample(Sampler, psIn.uv);
      float4 material = MaterialTexture.Sample(Sampler, psIn.uv);
      float roughness = material.r;                            // 粗さ
      float metalness = material.g;                            // 金属度
      float specular = material.b;                             // スペキュラ光の強さ
      //float emissive = material.a;                           // エミッシブ
      float smooth = 1.0f - roughness;
      float3 specColor = albedo.rgb;                          // スペキュラ光の色
      float3 N = normal.xyz;
      float3 V = normalize(CameraPosition.xyz - position.xyz);
      float3 L = normalize(-LightDirection.xyz);
      float3 H = normalize(L + V);


      /*
       // 法線を計算
      float3 normal = GetNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);

      // step-2 アルベドカラー、スペキュラカラー、金属度をサンプリングする
      // アルベドカラー（拡散反射光）
      float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
      // スペキュラカラー（鏡面反射光）
      float3 specColor = g_specularMap.SampleLevel(g_sampler, psIn.uv, 0).rgb;
      // 金属度
      float metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;

      // 視線に向かって伸びるベクトルを計算する
      float3 toEye = normalize(eyePos - psIn.worldPos);

      */


      float3 lig = 0.0f;

       // step-3 ディズニーベースの拡散反射を実装する
       // フレネル反射を考慮した拡散反射を計算
      float diffuseFromFresnel = CalcDiffuseFromFresnel(N, L, V);
      // 正規化Lambert拡散反射を求める
      float NdotL = saturate(dot(N, L));
      float3 lambertDiffuse = LightColor * NdotL;
      // 最終的な拡散反射光を計算する
      float3 diffuse = albedo * diffuseFromFresnel * lambertDiffuse;

      // step-6 クックトランスモデルを利用した鏡面反射率を計算する
      // クックトランスモデルの鏡面反射率を計算する
      float3 spec = CookTrranceSpecular(L, V, N, metalness) * LightColor;
      // 金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
      // スペキュラカラーの強さを鏡面反射率として扱う
      float specTerm = length(specColor.xyz);
      spec *= lerp(float3(specTerm, specTerm, specTerm), specColor, metalness);

      // step-7 鏡面反射率を使って、拡散反射光と鏡面反射光を合成する
      // 鏡面反射率が高ければ、拡散反射は弱くなる
      lig += diffuse * (1.0f - specTerm) + spec;

      // 環境光による底上げ
      lig += AmbientLight * albedo;

      float4 finalColor = 1.0f;
      finalColor.xyz = lig;
      return finalColor;
   }
};

// 誘電体の反射率（F0）は4%とする
uniform float _DielectricF0 = 0.04f;

class DisneyBasePhysicsBasedRendering : Shading {
   float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv) {
   //float3 binSpaceNormal = g_normalMap.SampleLevel(g_sampler, uv, 0.0f).xyz;
   //binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

   //float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

   //return newNormal;
   }

   // ベックマン分布を計算する
   float Beckmann(float m, float t) {
      float t2 = t * t;
      float t4 = t * t * t * t;
      float m2 = m * m;
      float D = 1.0f / (4.0f * m2 * t4);
      D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
      return D;
   }

   // フレネルを計算。Schlick近似を使用
   float SpcFresnel(float f0, float u) {
      // from Schlick
      return f0 + (1 - f0) * pow(1 - u, 5);
   }

   // <summary>
   // Cook-Torranceモデルの鏡面反射を計算
   // </summary>
   // <param name="L">光源に向かうベクトル</param>
   // <param name="V">視点に向かうベクトル</param>
   // <param name="N">法線ベクトル</param>
   // <param name="metallic">金属度</param>
   float CookTorranceSpecular(float3 L, float3 V, float3 N, float metallic) {
      //float microfacet = 1.0f;
      float microfacet = 0.76f;

      // 金属度を垂直入射の時のフレネル反射率として扱う
      // 金属度が高いほどフレネル反射は大きくなる
      float f0 = metallic;

      // ライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
      float3 H = normalize(L + V);

      // 各種ベクトルがどれくらい似ているかを内積を利用して求める
      float NdotH = saturate(dot(N, H));
      float VdotH = saturate(dot(V, H));
      float NdotL = saturate(dot(N, L));
      float NdotV = saturate(dot(N, V));

      // D項をベックマン分布を用いて計算する
      float D = Beckmann(microfacet, NdotH);

      // F項をSchlick近似を用いて計算する
      float F = SpcFresnel(f0, VdotH);

      // G項を求める
      float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

      // m項を求める
      float m = PI * NdotV * NdotH;

      // ここまで求めた、値を利用して、Cook-Torranceモデルの鏡面反射を求める
      return max(F * D * G / m, 0.0);
   }

   // <summary>
   // フレネル反射を考慮した拡散反射を計算
   // </summary>
   // <remark>
   // この関数はフレネル反射を考慮した拡散反射率を計算します
   // フレネル反射は、光が物体の表面で反射する現象のとこで、鏡面反射の強さになります
   // 一方拡散反射は、光が物体の内部に入って、内部錯乱を起こして、拡散して反射してきた光のことです
   // つまりフレネル反射が弱いときには、拡散反射が大きくなり、フレネル反射が強いときは、拡散反射が小さくなります
   // </remark>
   // <param name="N">法線</param>
   // <param name="L">光源に向かうベクトル。光の方向と逆向きのベクトル。</param>
   // <param name="V">視線に向かうベクトル。</param>
   float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V) {

       // step-1 ディズニーベースのフレネル反射による拡散反射を真面目に実装する。
       // 光源に向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
      float3 H = normalize(L + V);

      // 粗さは0.5で固定。
      float roughness = 0.5f;

      float energyBias = lerp(0.0f, 0.5f, roughness);
      float energyFactor = lerp(1.0, 1.0 / 1.51, roughness);

      // 光源に向かうベクトルとハーフベクトルがどれだけ似ているかを内積で求める
      float dotLH = saturate(dot(L, H));

      // 光源に向かうベクトルとハーフベクトル、
      // 光が平行に入射したときの拡散反射量を求めている
      float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;

      // 法線と光源に向かうベクトルwを利用して拡散反射率を求める
      float dotNL = saturate(dot(N, L));
      float FL = (1 + (Fd90 - 1) * pow(1 - dotNL, 5));

      // 法線と視点に向かうベクトルを利用して拡散反射率を求める
      float dotNV = saturate(dot(N, V));
      float FV = (1 + (Fd90 - 1) * pow(1 - dotNV, 5));

      // 法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を
      // 乗算して最終的な拡散反射率を求めている。PIで除算しているのは正規化を行うため
      return (FL * FV * energyFactor);
   }

   float4 ShadingResult(PsIn psIn) {
      // テクスチャから情報を取得する
      float4 albedo = AlbedoTexture.Sample(Sampler, psIn.uv);
      float4 normal = NormalTexture.Sample(Sampler, psIn.uv);
      float4 position = PositionTexture.Sample(Sampler, psIn.uv);
      float4 depth = DepthTexture.Sample(Sampler, psIn.uv);
      float4 material = MaterialTexture.Sample(Sampler, psIn.uv);
      float roughness = material.r;                            // 粗さ
      float metalness = material.g;                            // 金属度
      float specular = material.b;                             // スペキュラ光の強さ
      //float emissive = material.a;                           // エミッシブ
      float smooth = 1.0f - roughness;
      float3 specColor = albedo.rgb;                          // スペキュラ光の色
      float3 N = normal.xyz;
      float3 V = normalize(CameraPosition.xyz - position.xyz);
      float3 L = normalize(-LightDirection.xyz);
      float3 H = normalize(L + V);

         // フレネル反射を考慮した拡散反射を計算
      float diffuseFromFresnel = CalcDiffuseFromFresnel(
         N,
         L,
         V
      );

      // 正規化ランバート拡散反射を求める
      float NdotL = saturate(dot(N, L));
      float3 lambertDiffuse = LightColor * NdotL / PI;

      // 最終的な拡散反射光を計算する
      float3 diffuse = albedo.rgb * diffuseFromFresnel * lambertDiffuse;

      // クック・トランスモデルの鏡面反射率を計算する
      float3 spec = CookTorranceSpecular(
         L,
         V,
         N,
         metalness
      );

      // ライトの色を乗算
      spec *= LightColor;

      // 金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
      // スペキュラカラーの強さを鏡面反射率として扱う
      spec *= lerp(
         float3(1.0f, 1.0f, 1.0f),
         specColor,
         metalness
      );

      // step-6 滑らかさを使って、拡散反射光と鏡面反射光を合成する
      // 滑らかさが高ければ、拡散反射は弱くなる
      float3 lig = diffuse * roughness + spec;

      // 環境光による底上げ
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
      // 本来はこのDiffuseをπで割るべきだけどUnityではレガシーなライティングとの互換性を保つため割らない
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
      float3 F = F_Schlick(f0, ldoth); // マイクロファセットベースのスペキュラBRDFではcosはldothが使われる
      float3 specular = V * D * F * ndotl * lightColor;
      // 本来はSpecularにπを掛けるべきではないが、Unityではレガシーなライティングとの互換性を保つため、Diffuseを割らずにSpecularにPIを掛ける
      specular *= PI;
      specular = max(0, specular);

      half3 color = diffuse + specular;
      return half4(color, 1);
   }

   float4 ShadingResult(PsIn psIn) {
      // テクスチャから情報を取得する
      float4 albedo = AlbedoTexture.Sample(Sampler, psIn.uv);
      float4 normal = NormalTexture.Sample(Sampler, psIn.uv);
      float4 position = PositionTexture.Sample(Sampler, psIn.uv);
      float4 depth = DepthTexture.Sample(Sampler, psIn.uv);
      float4 material = MaterialTexture.Sample(Sampler, psIn.uv);
      float roughness = material.r;                            // 粗さ
      float metalness = material.g;                            // 金属度
      float specular = material.b;                             // スペキュラ光の強さ
      //float emissive = material.a;                           // エミッシブ
      float smooth = 1.0f - roughness;
      float3 specColor = albedo.rgb;                          // スペキュラ光の色
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