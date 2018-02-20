#include "CS_Water.hlsli"

//VertexToPixel main(VertexInput input)
//{
//	VertexToPixel returnMe;
//	returnMe.myPosition = input.myPosition;
//	returnMe.myColor = input.myColor;
//	return returnMe;
//}

VertexToPixel main(VertexInput input)
{
	VertexToPixel returnMe;
	float4 vertexObjectPos = float4(input.myPosition.xyz, 1);
	
	float4 vertexWorldPos = mul(toWorld, vertexObjectPos);
	float4 vertexViewPos = mul(toCamera, vertexWorldPos);
	float4 vertexProjectionPos = mul(toProjection, vertexViewPos);

	float3x3 toWorld33 = (float3x3)(toWorld);
	float3 normal = mul(toWorld33, input.myNormals.xyz);
	float3 tangent = mul(toWorld33, input.myTangents.xyz);
	float3 biTangents = mul(toWorld33, input.myBiTangents.xyz);
	float3x3 TangentSpace = float3x3( biTangents, tangent, normal);


	returnMe.myPosition = vertexProjectionPos;
	returnMe.myTangentSpaceMatrix = TangentSpace;
	returnMe.myUV = input.myUV;
	returnMe.myWorldPosition = vertexWorldPos;

	return returnMe;
}