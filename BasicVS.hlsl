#include "Basic.hlsli"

//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

VSOutput main(float4 pos : POSITION,float3 nomal : NOMAL, float2 uv : TEXCOORD)
{
    VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
    //output.svpos = pos;
    output.svpos = mul(mat,pos);//���W�ɍs�����Z
    output.nomal = nomal;
    output.uv = uv;
    return output;
}

