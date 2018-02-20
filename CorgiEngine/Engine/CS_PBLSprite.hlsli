struct VertexInput
{
	float4 myPosition	:	POSITION;
	float4 myColor		:	COLOR;
	float2 myUV			:	UV;
};

struct VertexToPixel
{
	float4 myPosition	:	SV_POSITION;
	float4 myColor		:	COLOR;
	float2 myUV			:	UV;
};

struct PixelOutput
{
	float4 myColor		:	SV_TARGET;
};

cbuffer spriteData		: register(b0)
{
	float4 myColor;
	float3 myPosition;
	float trash;
	float2 myTargetSize;
	float2 myUVOffset;
}
texture2D instanceTexture : register(t0);
SamplerState instanceSampler;