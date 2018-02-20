#include "PBRFunctions.hlsli"

cbuffer SpotLightData : register(b1)
{
	float4 myPosition;
	float3 myDirection;
	float myRange;
	float3 Color;
	float myAngle;
}



PixelOutput main(VertexToPixel input)
{
	LightCalculationData spotLightData;
	PixelOutput returnMe;
	returnMe.myColor = float4(0, 0, 0, 1);
	float3 myColor = Color * myRange;
	float3 toLight = myPosition.xyz - PixelShader_WorldPos(input).myColor.xyz;
	float toLightDistance = toLight.x * toLight.x + toLight.y * toLight.y + toLight.z * toLight.z;
	toLight = normalize(toLight);
	float lightRange2 = myRange * myRange;

	float lambertattenuation = saturate(dot(PixelShader_ObjectNormal(input).myColor.xyz, toLight));
	float linearattenuation = toLightDistance / lightRange2;
	linearattenuation = 1.f - linearattenuation;
	linearattenuation = saturate(linearattenuation);
	float physicalattenuation = 1.f / (toLightDistance * 0.5f);

	float halfangle = myAngle / 2.f;
	float cutPoint = 1.f - (halfangle / 90.f);

	float3 fromLight = normalize(-toLight);
	float3 dir = normalize(myDirection);
	float x = dot(fromLight, dir);

	float angleattenuation = (x - cutPoint) * (1.f / (1.f - cutPoint));

	float attenuation = angleattenuation * lambertattenuation * linearattenuation *physicalattenuation;


	spotLightData.LightColor.xyz = myColor.xyz;
	
	spotLightData.LightColor.w = 1.f;
	spotLightData.toLight = toLight;

	returnMe.myColor.rgb = PixelShader_Albedo(input).myColor.xyz * attenuation * myColor.xyz * PixelShader_CalculatePBR(input, spotLightData).myColor.xyz;
	returnMe.myColor.a = PixelShader_Albedo(input).myColor.a;
	return returnMe;
}
