#include "CS_Fullscreen.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnMe;
	returnMe.myColor.rgb = FullscreenTextures[0].SampleLevel(instanceSampler, input.myUV, 0).rgb;
	returnMe.myColor.a = 1.0f;
	return returnMe;
}