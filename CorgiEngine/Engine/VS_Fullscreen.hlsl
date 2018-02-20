#include "CS_Fullscreen.hlsli"

VertexToPixel main(VertexInput input)
{

	VertexToPixel returnValue;
	returnValue.myPosition = input.myPosition;
	returnValue.myUV = input.myUV;
	return returnValue;
}