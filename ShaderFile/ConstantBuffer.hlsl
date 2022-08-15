
// �J�����p�����[�^�p
cbuffer CameraParam : register(b0) {
	float3 CameraPosition;
	float3 CameraViewDirection;
	uint2 ViewPortSize;
	matrix View;
	matrix Projection;
};

// ���[���h�s��p
cbuffer WorldMatrix : register(b1) {
	matrix World;
}

// �����x�[�X�����_�����O�}�e���A���p
cbuffer PBRMaterial : register(b2) {
	float4 Albedo;
	float Roughness;
	float Metalness;
	float Specular;
	//float Emissive;
}

// �f�B���N�V���i�����C�g�p
cbuffer DirectionalLight : register(b3) {
	matrix LightView;
	matrix LightProjection;
	float3 LightDirection;
	float4 LightColor;
	float3 AmbientLight;
};

