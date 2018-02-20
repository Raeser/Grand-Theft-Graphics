struct VertexInput
{
	float4 myPosition	:	POSITION;
	float2 myUV			:	UV;
};

struct VertexToPixel
{
	float4 myPosition	:	SV_POSITION;
	float2 myUV			:	UV;
};

struct PixelOutput
{
	float4 myColor		:	SV_TARGET;
};

texture2D FullscreenTextures[2] : register(t0);
SamplerState instanceSampler : register(s0);
