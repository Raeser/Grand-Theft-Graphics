#include "PBRFunctions.hlsli"
cbuffer directionalLight : register(b1)
{
	float4 DirectionalLightDirection;
	float4 DirectionalLightColor;
}



PixelOutput main(VertexToPixel input)
{
	float depthValue = 1.f;
	float depthBias = 0.001f;
	float4 pixelInWorldSpace = DeferredTextures[0].Sample(textureSampler, input.myUV.xy);
	float4 lightPosInLightSpace = mul(toLight, pixelInWorldSpace);
	float4 lightPosInLightProjectionSpace = mul(lightProjection, lightPosInLightSpace); 
	float2 projectTexCoord;
	// Calculate the projected texture coordinates.
	projectTexCoord.x = lightPosInLightProjectionSpace.x / lightPosInLightProjectionSpace.w / 2 + .5f;
	projectTexCoord.y = -lightPosInLightProjectionSpace.y / lightPosInLightProjectionSpace.w / 2 + .5f;
	PixelOutput returnMe;
	returnMe.myColor = float4(0, 0, 0, 0);
	float lightDepthValue = 0;
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		depthValue = DepthTexture.Sample(textureSampler, projectTexCoord).x;

		lightDepthValue = lightPosInLightProjectionSpace.z / lightPosInLightProjectionSpace.w;

		lightDepthValue = lightDepthValue - depthBias;
	}

	LightCalculationData EnvironmentLightData;
	EnvironmentLightData.LightColor = DirectionalLightColor;
	EnvironmentLightData.toLight = DirectionalLightDirection.xyz;
	
	float4 output = float4(0,0,0,0);
	output.rgb = PixelShader_CalculatePBR(input, EnvironmentLightData).myColor.rgb;
	output.a = PixelShader_Albedo(input).myColor.a;
	returnMe.myColor = output;
	if (lightDepthValue > depthValue)
	{
		returnMe.myColor = output * .9f;
	}

	return returnMe;
}
