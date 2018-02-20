#include "CS_Fullscreen.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnMe;
	float3 color = FullscreenTextures[0].Sample(instanceSampler, input.myUV).rgb;

	
	
	
	
	float numberofhalfpixels = 16.f * 2.f;
	float halfpixelsize = 1.f / numberofhalfpixels;
	color = color / numberofhalfpixels;
	color = color * (numberofhalfpixels - 2.0f);
	color.r = color.r + halfpixelsize;
	color.g = color.g + halfpixelsize;
	color.b = color.b + halfpixelsize;
	
	returnMe.myColor.rgb = VolumeTexture.Sample(instanceSampler, color).rgb;
	//returnMe.myColor.rgb = color;
	returnMe.myColor.a = 1.0f;
	return returnMe; 
}