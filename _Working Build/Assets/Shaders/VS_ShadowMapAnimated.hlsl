#include "CS_ShadowMap.hlsli"

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
	float4 vertexViewPos = mul(toLight, vertexWorldPos);
	float4 vertexProjectionPos = mul(lightProjection, vertexViewPos);

	returnMe.myWorldPosition = vertexProjectionPos;


	return returnMe;
}