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
	float4 myPosition				:	SV_POSITION;
	float4 myWorldPosition			:	WPOS;
	float3x3 myTangentSpaceMatrix	:	TANMATRIX;
	float2 myUV						:	UV;
};

struct PixelOutput
{
	float4 myWorldPosition				:	SV_TARGET0;
	float4 myAlbedo						:	SV_TARGET1;
	float4 myNormals					:	SV_TARGET2;
	float4 myMetallic					:	SV_TARGET3;
	float4 myRoughness					:	SV_TARGET4;
	float4 myEmissiveAmbSpecAmbDiff		:	SV_TARGET5;
};

struct ColorOutput
{
	float4 myColor;
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
	float DTHACK;
}

cbuffer InstanceData : register(b2)
{
	float4x4 toWorld;
}

cbuffer BoneBuffer : register(b3)
{
	matrix Bones[64];
}

TextureCube EnvironmentCubemapTexture : register(t0);

texture2D albedoTexture	:	register(t1);

texture2D normalTexture		:	register(t2);

texture2D DirtTexture	:	register(t3);

texture2D SandTexture	:	register(t4);

texture2D MossyTexture	:	register(t5);

texture2D SplatmapTexture	:	register(t6);

SamplerState textureSampler :	register(s1);

