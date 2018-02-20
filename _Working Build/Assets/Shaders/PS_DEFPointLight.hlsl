#include "PBRFunctions.hlsli"

cbuffer PointLightData : register(b1)
{
	float4 myPosition;
	float3 Color;
	float myRange;
}



PixelOutput main(VertexToPixel input)
{
	LightCalculationData PointlightData;
	PixelOutput returnMe;
	returnMe.myColor = float4(0, 0, 0, 0);

	float3 myColor = Color * myRange;
	float3 toLight = myPosition.xyz - PixelShader_WorldPos(input).myColor.xyz;
	float toLightDistance = toLight.x * toLight.x + toLight.y * toLight.y + toLight.z * toLight.z;
	toLight = normalize(toLight);
	float lightRange2 = myRange * myRange;
	float lambertattenuation = saturate(dot(PixelShader_ObjectNormal(input).myColor.xyz, toLight));
	float linearattenuation = toLightDistance / lightRange2;

	linearattenuation = 1.0f - linearattenuation;
	linearattenuation = saturate(linearattenuation);
	float physicalattenuation = 1.0f / (toLightDistance * 0.1f);
	float attenuation = lambertattenuation * linearattenuation *physicalattenuation;

	PointlightData.LightColor.xyz = myColor.xyz;
	PointlightData.LightColor.w = 1.f;
	PointlightData.toLight = toLight;

	returnMe.myColor.rgb = PixelShader_Albedo(input).myColor.xyz * attenuation * myColor.xyz * PixelShader_CalculatePBR(input, PointlightData).myColor.xyz;
	returnMe.myColor.a = PixelShader_Albedo(input).myColor.a;
	return returnMe;
}
