struct VertexInput
{
	float4 myPosition	:	POSITION;
	float4 myNormals	:	NORMALS;
	float4 myTangents	:	TANGENTS;
	float4 myBiTangents	:	BITANGENTS;
	float2 myUV			:	UV;
	float4 myBones		:	BONES;
	float4 myWeights	:	WEIGHTS;
};

struct VertexToPixel
{
	float4 myWorldPosition			:	SV_POSITION;
};

struct PixelOutput
{
	float myDepth : SV_TARGET;
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
	float TrashBoye;
}

cbuffer ModelViewProjection : register(b4)
{
	float4x4 toLight;
	float4x4 lightProjection;
}


cbuffer InstanceBuffer : register(b2)
{
	float4x4 toWorld;
}

cbuffer BoneBuffer : register(b3)
{
	matrix Bones[64];
}