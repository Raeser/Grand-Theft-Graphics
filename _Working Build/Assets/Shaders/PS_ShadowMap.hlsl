#include "CS_ShadowMap.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput output;
	output.myDepth = input.myWorldPosition.z;
	return output;
}