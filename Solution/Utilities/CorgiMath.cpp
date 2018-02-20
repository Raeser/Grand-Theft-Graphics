#include "stdafx.h"


float Corgi::ReciprocalSqrt(float x)
{
	long i;
	float y, r;
	y = x * 0.5f;
	i = *(long *)(&x);
	i = 0x5f3759df - (i >> 1); //Ehhh ??
	r = *(float *)(&i);
	r = r * (1.5f - r * r * y);
	return r;
}

float Corgi::RandomFloat(float aMin, float aMax)
{
	float random = static_cast<float>(rand());
	float randomMax = RAND_MAX;
	random /= randomMax;
	random = random * (aMax - aMin) + aMin;

	return random;
}

float Corgi::RADIANS(float deg)
{
	return deg * (PI / 180.f);
}

float Corgi::DEGREES(float rad)
{
	return rad * (180.f / PI);
}

void Corgi::RadialFlip(float & f)
{

	if (f > PI2)
	{
		f -= PI2;
	}
	if (f < -PI2)
	{
		f += PI2;
	}

}

void Corgi::RadialFlip(Vector3f & v)
{

	RadialFlip(v.x);
	RadialFlip(v.y);
	RadialFlip(v.z);

}

void Corgi::RadialFlip(Vector4f & v)
{

	Vector3f v3 = v.V3();
	RadialFlip(v3);
	v = Vector4f(v3, v.w);

}
