#include "CS_Skybox.hlsli"

TextureCube instanceTexture : register(t0);
SamplerState instanceSampler;

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnMe;
	float4 textureColor = instanceTexture.Sample(instanceSampler, input.myDirection.xyz, 0).rgba;
	returnMe.myColor = textureColor.rgba;
	return returnMe;
}