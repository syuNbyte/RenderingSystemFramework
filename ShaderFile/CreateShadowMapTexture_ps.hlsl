// ���C�g���猩���I�u�W�F�N�g�̐[�x������������

// Include
#include"DeferredTextureSampler.hlsl"

struct PsIn {
	float4 position : SV_POSITION;
};

float4 main(PsIn psIn) : SV_TARGET{
	float depth = psIn.position;						// �����������Y��ɃV���h�E�̂ݏo�͂����
	//float depth = psIn.position.z;					// ���f���̗֊s�������W���M�W���M����
	return float4(depth, 0.0f, 0.0f, 1.0f);
}

