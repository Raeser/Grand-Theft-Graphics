#define ANIMATED
#include "CS_Particle.hlsli"
#include "Functions.hlsli"

[maxvertexcount(4)]
void main(point VertexToGeometry input[1], inout TriangleStream<GeometryToPixelAnimated> OutputStream)
{

	float4 vertexObjectPos = float4(input[0].myPosition.xyz, 1);
	float4 vertexViewPosition = mul(toCamera, vertexObjectPos);

	float3 pos = vertexViewPosition.xyz;
	float3 dir = input[0].myDirection.xyz;
	float percent = input[0].myLifetime / lifetime;

	unsigned int totalframes = (columns * rows);
	unsigned int currentframe = totalframes * percent;
	unsigned int nextframe = min(currentframe + 1, totalframes - 1);
	float framePercent = min(rescale(percent, (float)currentframe / totalframes, (float)nextframe / totalframes), 1);

	float2 scale = lerp(startsize, endsize, percent);


	float width = scale.x;
	float height = scale.y;

	float4 vtx[4] = 
	{
		{ pos.x - width, pos.y + height, pos.z, 1 },
		{ pos.x + width, pos.y + height, pos.z, 1 },
		{ pos.x - width, pos.y - height, pos.z, 1 },
		{ pos.x + width, pos.y - height, pos.z, 1 }
	};

	float2 cellUV;
	cellUV.x = (float)(currentframe % columns) / (float)columns;
	cellUV.y = floor(currentframe / columns) / (float)rows;

	float2 uv0[4];
	uv0[0] = cellUV + float2(1.0f / (float)rows, 0.0f / (float)columns);
	uv0[1] = cellUV + float2(1.0f / (float)rows, 1.0f / (float)columns);
	uv0[2] = cellUV + float2(0.0f / (float)rows, 0.0f / (float)columns);
	uv0[3] = cellUV + float2(0.0f / (float)rows, 1.0f / (float)columns);


	float2 nextCellUV;
	nextCellUV.x = (float)(nextframe % columns) / (float)columns;
	nextCellUV.y = floor(nextframe / columns) / (float)rows;

	float2 uv1[4];
	uv1[0] = nextCellUV + float2(1.0f / (float)rows, 0.0f / (float)columns);
	uv1[1] = nextCellUV + float2(1.0f / (float)rows, 1.0f / (float)columns);
	uv1[2] = nextCellUV + float2(0.0f / (float)rows, 0.0f / (float)columns);
	uv1[3] = nextCellUV + float2(0.0f / (float)rows, 1.0f / (float)columns);

	GeometryToPixelAnimated output;

	for (int i = 0; i < 4; i++)
	{
		output.myLifetime = percent;
		output.myPosition = vtx[i];

		output.myPosition = mul(toProjection, output.myPosition);
		output.myTexCoord1 = uv0[i];
		output.myTexCoord2 = uv1[i];
		output.myFramePercent = framePercent;

		OutputStream.Append(output);
	}
}