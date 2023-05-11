#include "FBX.hlsli"
//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);

//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{

	//テクスチャマッピング
	float4 texcolor = tex.Sample(smp,input.uv);
	float4 _Color = texcolor;

	//Camera
	float3 eyeDir = normalize(cameraPos - input.worldPosition);
	//Light
	float3 lightPos = float3(1, 1, 1);	//ライトの位置
	float3 lightDir = normalize(lightPos);
	float4 lightColor = float4(1, 1, 1, 1); //ライトの色

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

	//陰影とテクスチャの色を合成
	return abs;
}

////Lambert反射
//float3 light = normalize(float3(1,-1,1));	//右下奥 向きのライト
//float diffuse = saturate(dot(-light, input.normal));
//float brightness = diffuse + 0.3f;
//float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
