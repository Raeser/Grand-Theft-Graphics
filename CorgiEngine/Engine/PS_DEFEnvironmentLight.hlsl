
#include "PBRFunctions.hlsli"
cbuffer directionalLight : register(b1)
{
	float4 DirectionalLightDirection;
	float4 DirectionalLightColor;
}



PixelOutput main(VertexToPixel input)
{
	LightCalculationData EnvironmentLightData;
	EnvironmentLightData.LightColor = DirectionalLightColor;
	EnvironmentLightData.toLight = DirectionalLightDirection.xyz * -1.0f;
	PixelOutput returnMe;
	returnMe.myColor.rgb = PixelShader_CalculatePBR(input, EnvironmentLightData).myColor.rgb;
	returnMe.myColor.a = PixelShader_Albedo(input).myColor.a;
	return returnMe;
}
