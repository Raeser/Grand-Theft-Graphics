float4 triple_lerp(float4 c1, float4 c2, float4 c3, float t)
{
	float y1 = t * 2.0;
	float y2 = t * 2.0 - 1.0;

	float4 g, g2, g3;

	g = lerp(c1, c2, y1) * step(y1, 1.0);
	g2 = lerp(c2, c3, y2) * step(y2, 1.0) * step(1.0, y1);

	g = g + g2;
	return g;
}

/*float3 rotate(float3 pos, float3 dir, float d)
{
	// Specify the angle in radians:
	float angle = d * 3.14 / 180.0; // d degrees, CCW

	float3 q;
	q.x = pos.x * (dir.x*dir.x * (1.0 - cos(angle)) + cos(angle))
		+ pos.y * (dir.x*dir.y * (1.0 - cos(angle)) + dir.z * sin(angle))
		+ pos.z * (dir.x*dir.z * (1.0 - cos(angle)) - dir.y * sin(angle));

	q.y = pos.x * (dir.y*dir.x * (1.0 - cos(angle)) - dir.z * sin(angle))
		+ pos.y * (dir.y*dir.y * (1.0 - cos(angle)) + cos(angle))
		+ pos.z * (dir.y*dir.z * (1.0 - cos(angle)) + dir.x * sin(angle));

	q.z = pos.x * (dir.z*dir.x * (1.0 - cos(angle)) + dir.y * sin(angle))
		+ pos.y * (dir.z*dir.y * (1.0 - cos(angle)) - dir.x * sin(angle))
		+ pos.z * (dir.z*dir.z * (1.0 - cos(angle)) + cos(angle));

	return q;
}*/


float4x4 rotationMatrix(float3 axis, float angle, float precent, float3 worldpos)
{
	axis = normalize(axis);
	float s = sin(0 + angle * precent);
	float c = cos(0 + angle * precent);
	float oc = 1.0 - c;

	//float4x4 trans = float4x4(1.0, 0.0, 0.0, 0.0,
	//	0.0, 1.0, 0.0, 0.0,
	//	0.0, 0.0, 1.0, 0.0,
	//	worldpos.x, worldpos.y, worldpos.z, 1.0);

	float4x4 rot = float4x4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,
		oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,
		oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0,
		0.0, 0.0, 0.0, 1.0);
	//worldpos[0], worldpos[1], worldpos[2], 1.0);


	return rot;
	//return mul(transpose(rot), trans);
	//return trans;
}

float rescale(float oldVal, float min, float max)
{
	return ((oldVal - min) / (max - min));
}

float rescale_p(float oldVal, float min, float max)
{
	return ((max - min) * oldVal) + min;
}