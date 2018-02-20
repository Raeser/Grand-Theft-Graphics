#include "CS_Fullscreen.hlsli"
static const float texelSize = 1.f / 256.f;
static const uint kernelSize = 7;
static const float weights[kernelSize] = { 0.129001f, 0.142521f, 0.151303f, 0.15435f, 0.151303f, 0.142521f, 0.129001f };
static const float texelOffset[kernelSize] = { -3.f, -2.f, -1.f, 0.f, 1.f, 2.f, 3.f };

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnValue;
	float3 blurColor = (float3) 0;

	for (uint i = 0; i < kernelSize; ++i)
	{
		float2 textureCoords = input.myUV;
		textureCoords.x += texelSize * texelOffset[i];
		float3 resource = FullscreenTextures[0].SampleLevel(instanceSampler, textureCoords, 0).rgb;
		blurColor += resource * weights[i];
	}

	returnValue.myColor.rgb = blurColor.rgb;
	returnValue.myColor.a = 1.0f;

	return returnValue;
}