
// �x�������_�����O�p�̃e�N�X�`���ƃT���v���[

Texture2D<float4> AlbedoTexture : register(t0);			// �F���e�N�X�`��
Texture2D<float4> NormalTexture : register(t1);			// �@�����e�N�X�`��
Texture2D<float4> PositionTexture : register(t2);		// ���W�e�N�X�`��
Texture2D<float4> DepthTexture : register(t3);			// �[�x�e�N�X�`��
Texture2D<float4> MaterialTexture :register(t4);		// �����e�N�X�`���i���t�l�X, ���^���l�X�j
Texture2D<float4> BrightnessTexture : register(t5);	// �P�x�e�N�X�`��

// �����r���̌��ʂ��ꎞ�I�Ɋi�[��
// �ŏI�I�Ƀo�b�N�o�b�t�@�ɑ���f�[�^
Texture2D<float4> ResultTexture : register(t6);			// ���ʃe�N�X�`��

SamplerState Sampler : register(s0);		// �T���v���[