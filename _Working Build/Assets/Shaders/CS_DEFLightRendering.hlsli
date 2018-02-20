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


cbuffer ModelViewProjection : register(b2)
{
	float4x4 toLight;
	float4x4 lightProjection;
}


cbuffer InstanceBuffer : register(b3)
{
	float4x4 toWorld;
}

cbuffer CameraData : register(b4)
{
	float4x4 cameraOrientation;
	float4x4 toCamera;
	float4x4 toProjection;
};

SamplerState textureSampler;


texture2D DeferredTextures[6] : register(t0);

texture2D DepthTexture : register(t6);