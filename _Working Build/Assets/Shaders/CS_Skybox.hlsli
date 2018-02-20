struct VertexInput
{
	float4 myPosition	:	POSITION;
	float4 myNormals	:	NORMALS;
	float2 myUV			:	UV;
};

struct VertexToPixel
{
	float4 myPosition	:	SV_POSITION;
	float4 myDirection	:	DIRECTION;

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
}

