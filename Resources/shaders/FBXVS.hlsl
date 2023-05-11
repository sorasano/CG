#include "FBX.hlsli"

//�G���g���[�|�C���g
VSOutput main(VSInput input)
{
	//�@���Ƀ��[���h�s��ɂ��X�P�[�����O ��]��K�p
	float4 wnormal = normalize(mul(world, float4(input.normal, 0)));
	//�s�N�Z���V�F�[�_�ɓn���l
	VSOutput output;
	//�s��ɂ����W�Ԋ�
	output.svpos = mul(mul(viewproj, world), input.pos);
	//���_���W�����[���h���W�ɕϊ�
	output.worldPosition = mul(world, output.svpos);
	//���[���h�@�������̃X�e�[�W�ɓn��
	output.normal = wnormal.xyz;
	//���͒l�����̂܂܎��̃X�e�[�W�n��
	output.uv = input.uv;
	//���[���h���Wx
	return output;
}