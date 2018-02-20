#include "CS_Particle.hlsli"


VertexToGeometry main(VertexInput input)
{
	VertexToGeometry returnValue;
	returnValue.myPosition = input.myPosition;// +worldPosition;
	returnValue.myDirection = input.myDirection;
	returnValue.myLifetime = input.myLifetime;
	return returnValue;
}

//VertexToGeometry main(VertexInput input)
//{
//	VertexToGeometry returnValue;
//	float4 vertexObjectPos = float4(input.myPosition.xyz, 1);
//	float4 vertexViewPos = mul(toCamera, vertexObjectPos);
//
//	returnValue.myPosition = vertexViewPos;
//	returnValue.myColor = input.myColor.rgb;
//	returnValue.myWorldPos = vertexViewPos;
//	returnValue.mySize = input.mySize;
//
//	return returnValue;
//}