#include "CS_Text.hlsli"

[maxvertexcount(4)]
void main(point VertexToGeometry input[1], inout TriangleStream<GeometryToPixel> output)
{
	float4 position = myPosition;
	const float4 offset[4] = //det funkar iallafall
	{
		{ input[0].myPosition.x, -input[0].myPosition.y,0,0 },
		{ input[0].myPosition.x + input[0].mySize.x, -input[0].myPosition.y, 0,0 },
		{ input[0].myPosition.x, -input[0].myPosition.y - input[0].mySize.y, 0,0 },
		{ input[0].myPosition.x + input[0].mySize.x, -input[0].myPosition.y - input[0].mySize.y, 0,0 },
	};
	float2 topUV = input[0].myTopLeftUV;
	float2 botUV = input[0].myBotRightUV;
	const float2 uv[4] =
	{
		{ topUV.x, topUV.y }, 
		{ botUV.x, topUV.y }, 
		{ topUV.x, botUV.y }, 
		{ botUV.x, botUV.y }, 
	};

	for (int i = 0; i < 4; ++i)
	{
		GeometryToPixel vertex;
		vertex.myPosition = position +  offset[i];
		vertex.myPosition = mul(toProjection, vertex.myPosition);
		vertex.myColor = input[0].myColor;
		vertex.myTrash = float2(13, 37);
		vertex.myUV = uv[i];
		output.Append(vertex);
	}
	output.RestartStrip();
}
