
// Include
#include"ConstantBuffer.hlsl"
#include"TextureSampler.hlsl"

struct VsOut {
	float4 position : SV_POSITION;
	float4 worldPosition : TEXCOORD;
	float4 normal : TEXCOORD1;
	float4 tangent : TEXCOORD2;
	float4 bitangent : TEXCOORD3;
	float2 uv : TEXCOORD4;
	float4 color : COLOR;
	float4 lwvpPosition : TEXCOORD5;
};

// マルチレンダーターゲットで書き込みを行うレンダーターゲット
struct DeferredRenderTargets {
	float4 albedo : SV_Target0;
	float4 normal : SV_Target1;
	float4 position : SV_Target2;
	float4 depth : SV_Target3;
	float4 material : SV_Target4;
	//float4 brightness : SV_Target5;
};

// アルベドテクスチャに書き込む色情報を取得する
float4 GetAlbedo(VsOut vsOut) {
	return float4(vsOut.color);
}

// テクスチャからアルベドテクスチャに書き込む色情報を取得する
float4 GetAlbedoTexture(VsOut vsOut) {
	float4 texColor = Texture.Sample(Sampler, vsOut.uv);
	return float4(texColor.rgb * vsOut.color.rgb, texColor.a + vsOut.color.a);
}

// 法線テクスチャに書き込む法線情報を取得する
float4 GetNormal(VsOut vsOut) {
	// ワールド座標系で見た時の法線ベクトルの向き
	return float4(vsOut.normal.xyz, 1.0f);
}

// 法線マップから法線テクスチャに書き込む法線情報を取得する
float4 GetNormalTexture(VsOut vsOut) {
	// 法線マップを取得してワールド法線に変換したものを返す
	float4 texColor = NormalTexture.Sample(Sampler, vsOut.uv);
	texColor.rgb = texColor.rgb * 2.0f - 1.0f; // 0.0～0.5がマイナス方向のベクトル、0.5～1.0がプラス方向のベクトル
	float4 N = float4(texColor.rgb, 0.0f);
	float3 normal = normalize(mul(N, World)).xyz;
	normal = normalize(normal + vsOut.normal.xyz);
	return float4(normal, 1.0f);

	//input.normal = normalize(input.normal);
	//input.tangent = normalize(input.tangent);

	//// Read and unpack normal from map
	//float3 normalFromMap = NormalMap.Sample(Sampler, input.uv).xyz * 2 - 1;

	//// Transform from tangent to world space
	//float3 N = input.normal;
	//float3 T = normalize(input.tangent - N * dot(input.tangent, N));
	//float3 B = cross(T, N);

	//float3x3 TBN = float3x3(T, B, N);
	//input.normal = normalize(mul(normalFromMap, TBN));
}


// 座標テクスチャに書き込む座標情報を取得する
float4 GetPosition(VsOut vsOut) {
	// ワールド座標系で見た際の座標値
	return float4(vsOut.worldPosition.xyz, 1.0f);
}

// 深度テクスチャに書き込む深度情報を取得する
float4 GetDepth(VsOut vsOut) {
	// カメラから見た際の深度値 
	// wで割ると遠いほうが黒くなる　※wで割らないと変化が分かりずらい
	return float4(vsOut.position.z / vsOut.position.w, vsOut.position.z, 0.0f, 1.0f);
}

// 質感テクスチャに書き込む質感情報を取得する
float4 GetMaterial(VsOut vsOut) {
	// 定数バッファの値を書き込む
	return float4(Roughness, Metalness, Specular, 1.0f/*Emissive*/);
}

// 輝度テクスチャに書き込む輝度情報を取得する
float4 GetBrightness(float4 albedo) {
	float brightness = (albedo.r + albedo.g + albedo.b) / 3.0f;
	return brightness;
}

float4 GetShadowMapResult(VsOut vsOut, float4 albedo) {
	//以下のどのタイプのUV座標の求め方でもシャドウは出るし、処理結果に違いはない（あっても誤差程度？だとおもう）

	// タイプ１
	//float2 shadowMapUV = vsOut.lwvpPosition.xy / vsOut.lwvpPosition.w;
	//shadowMapUV *= float2(0.5f, -0.5f);
	//shadowMapUV += 0.5f;

	// タイプ２
	float2 shadowMapUV;
	shadowMapUV.x = (1.0f + vsOut.lwvpPosition.x / vsOut.lwvpPosition.w) * 0.5f;
	shadowMapUV.y = (1.0f - vsOut.lwvpPosition.y / vsOut.lwvpPosition.w) * 0.5f;

	if (shadowMapUV.x >= 0.0f && shadowMapUV.x <= 1.0f) {
		if (shadowMapUV.y >= 0.0f && shadowMapUV.y <= 1.0f) {
			float zInShadowMap = ShadowMapTexture.Sample(Sampler, shadowMapUV).r;
			float zInLVP = vsOut.lwvpPosition.z / vsOut.lwvpPosition.w;

			//if (zInLVP - zInShadowMap > 0.0f) {
			//if(zInLVP > zInShadowMap + 0.005f) {	// こっちだと逆に影部分が明るくなる
			if(zInLVP < zInShadowMap + 0.005f) {	// なぜか条件式を反転させないと動かない
				albedo.rgb *= 0.5f;
			}
		}
	}

	return albedo;
}

interface AlbedoGetter {
	float4 GetAlbedo(VsOut vsOut);
};

interface NormalGetter {
	float4 GetNormal(VsOut vsOut);
};

interface Getter {
	float4 GetAlbedo(VsOut vsOut);
	float4 GetNormal(VsOut vsOut);
};

class VertexColor : AlbedoGetter {
	float4 GetAlbedo(VsOut vsOut) {
		return vsOut.color;
	}
};

class TextureColor : AlbedoGetter {
	float4 GetAlbedo(VsOut vsOut) {
		float4 texColor = Texture.Sample(Sampler, vsOut.uv);
		return float4(texColor.rgb * vsOut.color.rgb, texColor.a);
	}
};

class VertexNormal : NormalGetter {
	float4 GetNormal(VsOut vsOut) {
		// ワールド座標系で見た時の法線ベクトルの向き
		return float4(vsOut.normal.xyz, 1.0f);
	}
};

class TextureNormal : NormalGetter {
	float4 GetNormal(VsOut vsOut) {
		// 法線マップを取得してワールド法線に変換したものを返す
		float4 texColor = NormalTexture.Sample(Sampler, vsOut.uv);
		texColor.rgb = texColor.rgb * 2.0f - 1.0f; // 0.0～0.5がマイナス方向のベクトル、0.5～1.0がプラス方向のベクトル
		float4 N = float4(texColor.rgb, 0.0f);
		float3 normal = normalize(mul(N, World)).xyz;
		normal = normalize(normal + vsOut.normal.xyz);
		return float4(normal, 1.0f);
	}
};

class VertexColorAndVertexNormal : Getter {
	float4 GetAlbedo(VsOut vsOut) {
		return vsOut.color;
	}
	float4 GetNormal(VsOut vsOut) {
		// ワールド座標系で見た時の法線ベクトルの向き
		return float4(vsOut.normal.xyz, 1.0f);
	}
};

class TextureColorAndVertexNormal : Getter {
	float4 GetAlbedo(VsOut vsOut) {
		float4 texColor = Texture.Sample(Sampler, vsOut.uv);
		return float4(texColor.rgb * vsOut.color.rgb, texColor.a);
	}
	float4 GetNormal(VsOut vsOut) {
		// ワールド座標系で見た時の法線ベクトルの向き
		return float4(vsOut.normal.xyz, 1.0f);
	}
};

class VertexColorAndNormalMapNormal : Getter {
	float4 GetAlbedo(VsOut vsOut) {
		return vsOut.color;
	}
	float4 GetNormal(VsOut vsOut) {
		// 法線マップを取得してワールド法線に変換したものを返す
		float4 texNormal = NormalTexture.Sample(Sampler, vsOut.uv);
		texNormal.rgb = texNormal.rgb * 2.0f - 1.0f; // 0.0～0.5がマイナス方向のベクトル、0.5～1.0がプラス方向のベクトル
		float4 N = float4(texNormal.rgb, 0.0f);
		float3 normal = normalize(mul(N, World)).xyz;
		normal = normalize(normal + vsOut.normal.xyz);
		return float4(normal, 1.0f);
	}
};

class TextureColorAndNormalMapNormal : Getter {
	float4 GetAlbedo(VsOut vsOut) {
		float4 texColor = Texture.Sample(Sampler, vsOut.uv);
		return float4(texColor.rgb * vsOut.color.rgb, texColor.a);
	}
	float4 GetNormal(VsOut vsOut) {
		// 法線マップを取得してワールド法線に変換したものを返す
		float4 texNormal = NormalTexture.Sample(Sampler, vsOut.uv);
		texNormal.rgb = texNormal.rgb * 2.0f - 1.0f; // 0.0～0.5がマイナス方向のベクトル、0.5～1.0がプラス方向のベクトル
		float4 N = float4(texNormal.rgb, 0.0f);
		float3 normal = normalize(mul(N, World)).xyz;
		normal = normalize(normal + vsOut.normal.xyz);
		return float4(normal, 1.0f);

		// この処理は大体で実装しているので実際にはもっとちゃんとした処理がある
	}
};

Getter getter;

DeferredRenderTargets main(VsOut vsOut) {
	// インスタンスの構築
	DeferredRenderTargets rts;

	// データの代入
	//rts.albedo = GetAlbedoTexture(vsOut);
	//rts.normal = GetNormalTexture(vsOut);
	rts.albedo = getter.GetAlbedo(vsOut);
	rts.albedo = GetShadowMapResult(vsOut, rts.albedo);
	rts.normal = getter.GetNormal(vsOut);
	rts.position = GetPosition(vsOut);
	rts.depth = GetDepth(vsOut);
	rts.material = GetMaterial(vsOut);
	//rts.brightness = GetBrightness(rts.albedo);

	// データを返す
	return rts;
}

