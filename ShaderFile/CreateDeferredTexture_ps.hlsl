
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

// �}���`�����_�[�^�[�Q�b�g�ŏ������݂��s�������_�[�^�[�Q�b�g
struct DeferredRenderTargets {
	float4 albedo : SV_Target0;
	float4 normal : SV_Target1;
	float4 position : SV_Target2;
	float4 depth : SV_Target3;
	float4 material : SV_Target4;
	//float4 brightness : SV_Target5;
};

// �A���x�h�e�N�X�`���ɏ������ސF�����擾����
float4 GetAlbedo(VsOut vsOut) {
	return float4(vsOut.color);
}

// �e�N�X�`������A���x�h�e�N�X�`���ɏ������ސF�����擾����
float4 GetAlbedoTexture(VsOut vsOut) {
	float4 texColor = Texture.Sample(Sampler, vsOut.uv);
	return float4(texColor.rgb * vsOut.color.rgb, texColor.a + vsOut.color.a);
}

// �@���e�N�X�`���ɏ������ޖ@�������擾����
float4 GetNormal(VsOut vsOut) {
	// ���[���h���W�n�Ō������̖@���x�N�g���̌���
	return float4(vsOut.normal.xyz, 1.0f);
}

// �@���}�b�v����@���e�N�X�`���ɏ������ޖ@�������擾����
float4 GetNormalTexture(VsOut vsOut) {
	// �@���}�b�v���擾���ă��[���h�@���ɕϊ��������̂�Ԃ�
	float4 texColor = NormalTexture.Sample(Sampler, vsOut.uv);
	texColor.rgb = texColor.rgb * 2.0f - 1.0f; // 0.0�`0.5���}�C�i�X�����̃x�N�g���A0.5�`1.0���v���X�����̃x�N�g��
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


// ���W�e�N�X�`���ɏ������ލ��W�����擾����
float4 GetPosition(VsOut vsOut) {
	// ���[���h���W�n�Ō����ۂ̍��W�l
	return float4(vsOut.worldPosition.xyz, 1.0f);
}

// �[�x�e�N�X�`���ɏ������ސ[�x�����擾����
float4 GetDepth(VsOut vsOut) {
	// �J�������猩���ۂ̐[�x�l 
	// w�Ŋ���Ɖ����ق��������Ȃ�@��w�Ŋ���Ȃ��ƕω��������肸�炢
	return float4(vsOut.position.z / vsOut.position.w, vsOut.position.z, 0.0f, 1.0f);
}

// �����e�N�X�`���ɏ������ގ��������擾����
float4 GetMaterial(VsOut vsOut) {
	// �萔�o�b�t�@�̒l����������
	return float4(Roughness, Metalness, Specular, 1.0f/*Emissive*/);
}

// �P�x�e�N�X�`���ɏ������ދP�x�����擾����
float4 GetBrightness(float4 albedo) {
	float brightness = (albedo.r + albedo.g + albedo.b) / 3.0f;
	return brightness;
}

float4 GetShadowMapResult(VsOut vsOut, float4 albedo) {
	//�ȉ��̂ǂ̃^�C�v��UV���W�̋��ߕ��ł��V���h�E�͏o�邵�A�������ʂɈႢ�͂Ȃ��i�����Ă��덷���x�H���Ƃ������j

	// �^�C�v�P
	//float2 shadowMapUV = vsOut.lwvpPosition.xy / vsOut.lwvpPosition.w;
	//shadowMapUV *= float2(0.5f, -0.5f);
	//shadowMapUV += 0.5f;

	// �^�C�v�Q
	float2 shadowMapUV;
	shadowMapUV.x = (1.0f + vsOut.lwvpPosition.x / vsOut.lwvpPosition.w) * 0.5f;
	shadowMapUV.y = (1.0f - vsOut.lwvpPosition.y / vsOut.lwvpPosition.w) * 0.5f;

	if (shadowMapUV.x >= 0.0f && shadowMapUV.x <= 1.0f) {
		if (shadowMapUV.y >= 0.0f && shadowMapUV.y <= 1.0f) {
			float zInShadowMap = ShadowMapTexture.Sample(Sampler, shadowMapUV).r;
			float zInLVP = vsOut.lwvpPosition.z / vsOut.lwvpPosition.w;

			//if (zInLVP - zInShadowMap > 0.0f) {
			//if(zInLVP > zInShadowMap + 0.005f) {	// ���������Ƌt�ɉe���������邭�Ȃ�
			if(zInLVP < zInShadowMap + 0.005f) {	// �Ȃ����������𔽓]�����Ȃ��Ɠ����Ȃ�
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
		// ���[���h���W�n�Ō������̖@���x�N�g���̌���
		return float4(vsOut.normal.xyz, 1.0f);
	}
};

class TextureNormal : NormalGetter {
	float4 GetNormal(VsOut vsOut) {
		// �@���}�b�v���擾���ă��[���h�@���ɕϊ��������̂�Ԃ�
		float4 texColor = NormalTexture.Sample(Sampler, vsOut.uv);
		texColor.rgb = texColor.rgb * 2.0f - 1.0f; // 0.0�`0.5���}�C�i�X�����̃x�N�g���A0.5�`1.0���v���X�����̃x�N�g��
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
		// ���[���h���W�n�Ō������̖@���x�N�g���̌���
		return float4(vsOut.normal.xyz, 1.0f);
	}
};

class TextureColorAndVertexNormal : Getter {
	float4 GetAlbedo(VsOut vsOut) {
		float4 texColor = Texture.Sample(Sampler, vsOut.uv);
		return float4(texColor.rgb * vsOut.color.rgb, texColor.a);
	}
	float4 GetNormal(VsOut vsOut) {
		// ���[���h���W�n�Ō������̖@���x�N�g���̌���
		return float4(vsOut.normal.xyz, 1.0f);
	}
};

class VertexColorAndNormalMapNormal : Getter {
	float4 GetAlbedo(VsOut vsOut) {
		return vsOut.color;
	}
	float4 GetNormal(VsOut vsOut) {
		// �@���}�b�v���擾���ă��[���h�@���ɕϊ��������̂�Ԃ�
		float4 texNormal = NormalTexture.Sample(Sampler, vsOut.uv);
		texNormal.rgb = texNormal.rgb * 2.0f - 1.0f; // 0.0�`0.5���}�C�i�X�����̃x�N�g���A0.5�`1.0���v���X�����̃x�N�g��
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
		// �@���}�b�v���擾���ă��[���h�@���ɕϊ��������̂�Ԃ�
		float4 texNormal = NormalTexture.Sample(Sampler, vsOut.uv);
		texNormal.rgb = texNormal.rgb * 2.0f - 1.0f; // 0.0�`0.5���}�C�i�X�����̃x�N�g���A0.5�`1.0���v���X�����̃x�N�g��
		float4 N = float4(texNormal.rgb, 0.0f);
		float3 normal = normalize(mul(N, World)).xyz;
		normal = normalize(normal + vsOut.normal.xyz);
		return float4(normal, 1.0f);

		// ���̏����͑�̂Ŏ������Ă���̂Ŏ��ۂɂ͂����Ƃ����Ƃ�������������
	}
};

Getter getter;

DeferredRenderTargets main(VsOut vsOut) {
	// �C���X�^���X�̍\�z
	DeferredRenderTargets rts;

	// �f�[�^�̑��
	//rts.albedo = GetAlbedoTexture(vsOut);
	//rts.normal = GetNormalTexture(vsOut);
	rts.albedo = getter.GetAlbedo(vsOut);
	rts.albedo = GetShadowMapResult(vsOut, rts.albedo);
	rts.normal = getter.GetNormal(vsOut);
	rts.position = GetPosition(vsOut);
	rts.depth = GetDepth(vsOut);
	rts.material = GetMaterial(vsOut);
	//rts.brightness = GetBrightness(rts.albedo);

	// �f�[�^��Ԃ�
	return rts;
}

