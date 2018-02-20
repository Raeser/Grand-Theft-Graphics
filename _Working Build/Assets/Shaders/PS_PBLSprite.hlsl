#include "CS_PBLSprite.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnValue;
	float4 textureColor = instanceTexture.Sample(instanceSampler, input.myUV.xy).rgba;
	textureColor = textureColor.rgba;// *input.myColor;
	returnValue.myColor = textureColor;
	return returnValue;
}