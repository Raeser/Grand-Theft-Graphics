#include "CS_Fullscreen.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnValue;
	float3 resource = FullscreenTextures[0].SampleLevel(instanceSampler, input.myUV, 0).rgb;
	float3 glow = FullscreenTextures[1].SampleLevel(instanceSampler, input.myUV, 0).rgb;

	resource *= (1.0f - saturate(glow));
	returnValue.myColor.rgb = resource + glow;
	returnValue.myColor.a = 1.0f;
	return returnValue;

}