#include "CS_DEFModel.hlsli"

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
	vertexObjectPos.x = -vertexObjectPos.x;
	
	float4 vWeights = input.myWeights;
	uint4 vBones = uint4((uint)input.myBones.x, (uint)input.myBones.y, (uint)input.myBones.z, (uint)input.myBones.w);
	float4x4 finalMatrix;
	finalMatrix = vWeights.x * Bones[vBones.x];
	finalMatrix += vWeights.y * Bones[vBones.y];
	finalMatrix += vWeights.z * Bones[vBones.z];
	finalMatrix += vWeights.w * Bones[vBones.w];
	
	float4 vAnimatedPos = mul(finalMatrix, vertexObjectPos);
	
	vAnimatedPos.x = -vAnimatedPos.x;

	float4 vertexWorldPos = mul(toWorld, vAnimatedPos);
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