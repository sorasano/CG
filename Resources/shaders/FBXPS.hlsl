#include "FBX.hlsli"
//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);

//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{

	//�e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp,input.uv);
	float4 _Color = texcolor;

	//Camera
	float3 eyeDir = normalize(cameraPos - input.worldPosition);
	//Light
	float3 lightPos = float3(1, 1, 1);	//���C�g�̈ʒu
	float3 lightDir = normalize(lightPos);
	float4 lightColor = float4(1, 1, 1, 1); //���C�g�̐F

	//Ambient
	float4 ambient = _Color * 0.3 * lightColor;
	ambient.w = 1;

	//diffuse
	float intensity = saturate(dot(normalize(input.normal), lightPos));
	float4 diffuse = _Color * intensity * lightColor;

	//specular
	input.normal = normalize(input.normal);
	float3 reflectDir = -lightDir + 2 * input.normal * dot(input.normal, lightDir);
	float4 specular = pow(saturate(dot(reflectDir, eyeDir)), 20) * lightColor;

	//abs
	float4 abs = ambient + diffuse + specular;

	//�A�e�ƃe�N�X�`���̐F������
	return abs;
}

////Lambert����
//float3 light = normalize(float3(1,-1,1));	//�E���� �����̃��C�g
//float diffuse = saturate(dot(-light, input.normal));
//float brightness = diffuse + 0.3f;
//float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
