
// ��̃e�N�X�`���ƃT���v���[�̃Z�b�g

Texture2D<float4> Texture				 : register(t0);	// �e�N�X�`��
Texture2D<float4> NormalTexture		 : register(t1);	// �@���e�N�X�`��

Texture2D<float4> ShadowMapTexture	 : register(t2);	// �V���h�E�}�b�v�p�̃��C�g

SamplerState Sampler : register(s0);	// �T���v���[