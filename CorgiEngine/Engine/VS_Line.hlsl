#include "CS_Line.hlsli"

VertexToPixel main(VertexInput input)
{
	VertexToPixel returnValue;
	float4 vertexObjectPos = input.myPosition;
	float4 vertexViewPos = mul(toCamera, vertexObjectPos);
	float4 vertexProjectionPos = mul(toProjection, vertexViewPos);

	returnValue.myPosition = vertexProjectionPos;
	returnValue.myColor = float4(input.myColor.rgb, 1);

	return returnValue;
}
