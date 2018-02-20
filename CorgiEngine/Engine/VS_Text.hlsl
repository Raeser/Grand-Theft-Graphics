#include "CS_Text.hlsli"
//Welcome to the Text Shader, if you know what you are doing please proceed!
VertexToGeometry main(VertexInput input)
{
	VertexToGeometry output;
	float4 Position = float4(input.myPosition.x, input.myPosition.y, 0, 1);
	float4 vertexViewPos = mul(toCamera, Position);

	
	output.myPosition = Position;
	output.myTopLeftUV = input.myTopLeftUV;
	output.myBotRightUV = input.myBotRightUV;
	output.myColor = input.myColor;
	output.mySize = input.mySize;

	return output;
}

