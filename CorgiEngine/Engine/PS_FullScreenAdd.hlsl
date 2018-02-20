#include "CS_Fullscreen.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnMe;
	returnMe.myColor.rgba = FullscreenTextures[0].SampleLevel(instanceSampler, input.myUV, 0).rgba;
	return returnMe;
}