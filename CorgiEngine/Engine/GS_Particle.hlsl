#include "CS_Particle.hlsli"
#include "Functions.hlsli"

[maxvertexcount(4)]
void main(point VertexToGeometry input[1], inout TriangleStream<GeometryToPixel> OutputStream)
{
	float4 vertexObjectPos = float4(input[0].myPosition.xyz, 1);
	float4 vertexViewPosition = mul(toCamera, vertexObjectPos);

	float3 pos = vertexViewPosition.xyz;
	float3 dir = input[0].myDirection.xyz;
	float percent = input[0].myLifetime / lifetime;

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

	//TODO: This is the golden ticket.
	//Keep working on this rotation to get a good back and forth.
	//TODO: Implement all these features (noise, rotation, includes) in all shaders.

#ifdef ROTATE
	float d = percent * 100;
	up = rotate(up, normal, d);
	right = rotate(right, normal, d);
#endif


	float2 uv[4];
	uv[0] = float2(1, 0);
	uv[1] = float2(1, 1);
	uv[2] = float2(0, 0);
	uv[3] = float2(0, 1);

	GeometryToPixel output;
	for (int i = 0; i < 4; i++)
	{
		output.myLifetime = percent;
		//output.myDirection = input[0].myPosition.xyz; //WHY THO?!?
		output.myPosition = vtx[i];
		output.myPosition = mul(toProjection, output.myPosition);

		output.myUV = uv[i];
		OutputStream.Append(output);
	}
}