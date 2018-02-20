struct VertexInput
{
	float4 myPosition	:	POSITION;
	float3 myColor		:	COLOR;
	float  myWidth : WIDTH;
};

struct VertexToPixel
{
	float4 myPosition	:	SV_POSITION;
	float4 myColor		:	COLOR;
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
