#include "CS_PBLSprite.hlsli"

VertexToPixel main(VertexInput input)
{
	VertexToPixel returnValue;
	returnValue.myPosition.xy = (input.myPosition.rg / myTargetSize) + myPosition.rg;
	returnValue.myPosition.zw = input.myPosition.zw;
	returnValue.myColor = input.myColor * myColor;
	returnValue.myUV = myUVOffset + input.myUV;
	return returnValue;
}
