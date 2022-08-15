
// カメラパラメータ用
cbuffer CameraParam : register(b0) {
	float3 CameraPosition;
	float3 CameraViewDirection;
	uint2 ViewPortSize;
	matrix View;
	matrix Projection;
};

// ワールド行列用
cbuffer WorldMatrix : register(b1) {
	matrix World;
}

// 物理ベースレンダリングマテリアル用
cbuffer PBRMaterial : register(b2) {
	float4 Albedo;
	float Roughness;
	float Metalness;
	float Specular;
	//float Emissive;
}

// ディレクショナルライト用
cbuffer DirectionalLight : register(b3) {
	matrix LightView;
	matrix LightProjection;
	float3 LightDirection;
	float4 LightColor;
	float3 AmbientLight;
};

