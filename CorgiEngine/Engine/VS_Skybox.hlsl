#include "CS_Skybox.hlsli"

VertexToPixel main(VertexInput input)
{ 
	VertexToPixel returnValue;
	returnValue.myDirection.xyz = normalize(-input.myPosition.xyz);
	returnValue.myDirection.w = 0.f;
	returnValue.myDirection = mul(cameraOrientation, returnValue.myDirection);
	returnValue.myDirection.y = -returnValue.myDirection.y;
	returnValue.myPosition = mul(toProjection, input.myPosition);
	return returnValue;
}