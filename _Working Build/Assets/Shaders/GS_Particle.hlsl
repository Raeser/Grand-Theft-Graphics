#include "CS_Particle.hlsli"
#include "Functions.hlsli"

static const float pi = 3.14159f;

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


	//#ifdef ROTATE
	float d = percent;
	float degreeToRotate = endDegree;
	float rotationAngleRad = degreeToRotate * (pi/180);

	pos[0] = 0.f;
	pos[1] = 0.f;
	pos[2] = 0.f;

	float4x4 rot = rotationMatrix(float3(0.f, 0.f,1.f), d, rotationAngleRad, pos.xyz);

	rot[3][0] = vertexViewPosition.x;
	rot[3][1] = vertexViewPosition.y;
	rot[3][2] = vertexViewPosition.z;

	//rot.y = vertexViewPosition.y;
	//rot.z = vertexViewPosition.z;

	//up = rotate(up, normal, d);
	//right = rotate(right, normal, d);
	//#endif

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



	float2 uv[4];
	uv[0] = float2(0, 0);
	uv[1] = float2(1, 0);
	uv[2] = float2(0, 1);
	uv[3] = float2(1, 1);

	GeometryToPixel output;


	//float angle = percent;
	
	for (int i = 0; i < 4; i++)
	{
		output.myLifetime = percent;
		//output.myDirection = input[0].myPosition.xyz; //WHY THO?!?
		output.myPosition = mul(vtx[i], rot);
		//output.myPosition = vtx[i];
		output.myPosition = mul(toProjection, output.myPosition);

		output.myUV = uv[i];
		OutputStream.Append(output);
	}
}