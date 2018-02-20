#include "CS_DEFLightRendering.hlsli"
VertexToPixel main(VertexInput input)
{
	VertexToPixel returnValue;
	returnValue.myPosition = input.myPosition;
	returnValue.myUV = input.myUV;
	return returnValue;
}