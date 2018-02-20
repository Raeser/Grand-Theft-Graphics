#include "CS_DEFModel.hlsli"
cbuffer InstanceData : register(b4)
{
	float4x4 toRealWorld;
}

VertexToPixel main(VertexInput input)
{
	VertexToPixel returnMe;


	float4 vertexObjectPos = float4(input.myPosition.xyz, 1);
	float4 vertexWorldPos = mul(toWorld, vertexObjectPos);
	float4 vertexViewPos = mul(toCamera, vertexWorldPos);
	float4 vertexProjectionPos = mul(toProjection, vertexViewPos);

	float3x3 toWorld33 = (float3x3)(toWorld);
	float3 normal = mul(toRealWorld, input.myNormals.xyz).xyz;
	float3 tangent = mul(toWorld33, input.myTangents.xyz);
	float3 biTangents = mul(toWorld33, input.myBiTangents.xyz);
	float3x3 TangentSpace = float3x3(biTangents, tangent, normal);

	float4 PixelWorldPos = mul(toRealWorld, vertexObjectPos);

	returnMe.myPosition = vertexProjectionPos;
	returnMe.myTangentSpaceMatrix = TangentSpace;
	returnMe.myUV = input.myUV;
	returnMe.myWorldPosition = PixelWorldPos;

	return returnMe;
}