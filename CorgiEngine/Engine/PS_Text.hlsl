#include "CS_Text.hlsli"
PixelOutput main(GeometryToPixel input)
{
	PixelOutput returnMe;
	float4 textureColor = FontTexture.Sample(textureSampler, input.myUV);
	returnMe.myColor.rgba = input.myColor * textureColor;
	return returnMe;
}