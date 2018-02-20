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


struct LightCalculationData
{
	float4 LightColor;
	float3 toLight;
};

cbuffer viewData : register(b0)
{
	float3 viewPosition;
	float TrashBoye;
}

SamplerState textureSampler;


texture2D DeferredTextures[6] : register(t0);