#include "Basic.hlsli"

//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

VSOutput main(float4 pos : POSITION,float3 nomal : NOMAL, float2 uv : TEXCOORD)
{
    VSOutput output; // ピクセルシェーダーに渡す値
    //output.svpos = pos;
    output.svpos = mul(mat,pos);//座標に行列を乗算
    output.nomal = nomal;
    output.uv = uv;
    return output;
}

