struct VertexInput
{
	float2 mySize		: SIZE;
	float2 myTopLeftUV	: UVTOPLEFT;
	float2 myBotRightUV	: UVTOPRIGHT;
	float2 myPosition	: POSITION;
	float4 myColor		: COLOR;
};

struct VertexToGeometry
{
	float4	myPosition		:	POSITION;
	float4	myColor			:	COLOR;
	float2	mySize			:	SIZE;
	float2	myTopLeftUV		:	UVTOPLEFT;
	float2  myBotRightUV	:	UVTOPRIGHT;
};

struct GeometryToPixel
{
	float4 myPosition	:	SV_POSITION;
	float4 myColor		:	COLOR;
	float2 myUV			:	UV;
	float2 myTrash		:	TRASH;
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

cbuffer instanceBuffer : register(b1)
{
	float4 myPosition;
};
SamplerState textureSampler;
texture2D FontTexture	:	register(t0);
