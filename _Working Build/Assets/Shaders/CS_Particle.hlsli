struct VertexInput
{
	float4	myPosition	:	POSITION;
	float3	myDirection	:	DIRECTION;
	float	myLifetime	:	LIFETIME;
};

struct VertexToGeometry
{
	float4	myPosition	:	POSITION;
	float3	myDirection :	DIRECTION;
	float	myLifetime	:	LIFETIME;
};

struct GeometryToPixelAnimated
{
	float4 myPosition		: SV_POSITION;
	float2 myTexCoord1		: TEXCOORD1;
	float2 myTexCoord2		: TEXCOORD2;
	float myFramePercent	: FRAMEPERCENT;
	float myLifetime		: LIFETIME;
};

struct GeometryToPixel
{
	float4 myPosition	:	SV_POSITION;
	float2 myUV			:	UV;
	float  myLifetime	:	LIFETIME;
};

struct PixelOutput
{
	float4 myColor		:	SV_TARGET;
};

cbuffer CameraData : register(b0)
{
	float4x4 cameraOrientation;
	float4x4 toCamera;
	float4x4 toProjection;
};

cbuffer viewData : register(b1)
{
	float3 viewPosition;
	float tresh;
}

cbuffer InstanceData : register(b2)
{
	float4 worldPosition;
}


#ifdef ANIMATED
cbuffer ParticleSystemDataAnimated : register(b3)
{
	float4 col0;
	float4 col1;
	float4 col2;
	float2 startsize;
	float2 endsize;
	float lifetime;
	float trash;

	int columns;
	int rows;

}
#else
cbuffer ParticleSystemData : register(b3)
{
	float4 col0;
	float4 col1;
	float4 col2;
	float2 startsize;
	float2 endsize;
	float lifetime;
	float endDegree;
	float2 Trash;
}
#endif
texture2D ParticleTexture : register(t0);
SamplerState instanceSampler;
