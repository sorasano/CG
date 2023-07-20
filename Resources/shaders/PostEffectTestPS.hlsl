#include "PostEffectTest.hlsli"

texture2D<float4> tex0 : register(t0);
texture2D<float4> tex1 : register(t1);

SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 colortex0 = tex0.Sample(smp,input.uv);
	float4 colortex1 = tex1.Sample(smp,input.uv);

	//���]
	float4 color = float4(1 - colortex0.rgb,1);

	if(fmod(input.uv.y,0.1f) < 0.05f){

		//���ςڂ���
		float u = (input.uv.x / 640);//(1 / ��ʉ���)
		float v = (input.uv.y / 360);//(1 / ��ʏc��)
		color = float4(0,0,0,0);

		//�c�����ɌJ��Ԃ�
		for(int i = -5; i < 6;i++){
			//�������ɌJ��Ԃ�
			for(int j = -5; j < 6;j++){
				color += tex1.Sample(smp,float2((u * i) + input.uv.x,(v * j) + input.uv.y));
			}
		}

		color /= 121;
	}

	return float4(color.rgb,1);
}