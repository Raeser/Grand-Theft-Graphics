#include "CS_ShadowMap.hlsli"

VertexToPixel main(VertexInput input)
{
	VertexToPixel returnMe;
	float4 vertexObjectPos = float4(input.myPosition.xyz, 1);
	float4 vertexWorldPos = mul(toWorld, vertexObjectPos);
	float4 vertexViewPos = mul(toLight, vertexWorldPos);
	float4 vertexProjectionPos = mul(lightProjection, vertexViewPos);

	returnMe.myWorldPosition = vertexProjectionPos;


	return returnMe;
}