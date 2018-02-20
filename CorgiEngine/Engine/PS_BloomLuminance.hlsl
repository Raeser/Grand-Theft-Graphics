#include "CS_Fullscreen.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnValue;
	float bloomCutoff = 0.6f;
	float3 resource = FullscreenTextures[0].SampleLevel(instanceSampler, input.myUV, 0).rgb;

	float3 relativeLuminance = (float3) 0;
	const float scalar = 0.5f;
	relativeLuminance.r = 0.2126f + scalar;
	relativeLuminance.g = 0.7152f + scalar;
	relativeLuminance.b = 0.0722f + scalar;
	float luminance = resource.r * relativeLuminance.r + resource.g * relativeLuminance.g + resource.b * relativeLuminance.b;
	luminance = saturate(luminance - bloomCutoff);
	float3 resourceLuminance = resource * luminance * (1 / bloomCutoff);

	returnValue.myColor.rgb = resourceLuminance.rgb;
	returnValue.myColor.a = 1.f;

	return returnValue;
}