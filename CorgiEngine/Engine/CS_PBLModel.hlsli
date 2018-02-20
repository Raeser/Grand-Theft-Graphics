struct VertexInput
{
	float4 myPosition	:	POSITION;
	float4 myNormals	:	NORMALS;
	float4 myTangents	:	TANGENTS;
	float4 myBiTangents	:	BITANGENTS;
	float2 myUV			:	UV;
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
	float4 myColor		:	SV_TARGET;
};

struct LightCalculationData
{
	float4 LightColor;
	float3 toLight;
};

cbuffer directionalLight : register(b0)
{
	float4 DirectionalLightDirection;
	float4 DirectionalLightColor;
}

cbuffer CameraData : register(b1)
{
	float4x4 cameraOrientation;
	float4x4 toCamera;
	float4x4 toProjection;
};

cbuffer viewData : register(b2)
{
	float3 viewPosition;
}

cbuffer InstanceData : register(b3)
{
	float4x4 toWorld;
}

cbuffer PointLightData : register(b4)
{
	uint numberOfUsedPointLights;
	uint3 PointTrash;
	struct
	{
		float4 myPosition;
		float3 myColor;
		float myRange;
	} pointLights[8];
}

cbuffer SpotLightData : register(b5)
{
	uint numberOfUsedSpotLights;
	uint SpotTrash;
	uint SpotTrash2;
	uint SpotTrash3;

	struct
	{
		float4 myPosition;
		float3 myDirection;
		float myRange;
		float3 myColor;
		float myAngle;
	} spotLights[8];
}

SamplerState textureSampler;

texture2D instanceTexture	:	register(t0);

texture2D normalTexture		:	register(t1);

texture2D ambientTexture	:	register(t2);

texture2D emissiveTexture	:	register(t3);

texture2D metalnessTexture	:	register(t4);

texture2D roughnessTexture	:	register(t5);

TextureCube EnvironmentCubemapTexture : register(t6);


