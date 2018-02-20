#pragma once
#include "Vector.h"

#define PIHalf PI/2.f
#define PI 3.14159265359f
#define PI2 6.28318530718f

namespace Corgi
{
	float ReciprocalSqrt(float x);

	float RandomFloat(float MIN, float MAX);

	float RADIANS(float deg);
	float DEGREES(float rad);

	template<class T>
	T Lerp(T aStart, T aEnd, float aTime);


	template<class T>
	Vector4<T> Lerp(Vector4<T> aStart, Vector4<T> aEnd, float aTime);

	template<class T>
	Vector3<T> Lerp(Vector3<T> aStart, Vector3<T> aEnd, float aTime);


	template<class T>
	T Lerp(T aStart, T aEnd, float aTime)
	{
		T returnValue = aStart + ((aEnd - aStart) * aTime);
		return returnValue;
	}

	template<class T>
	Vector4<T> Lerp(Vector4<T> aStart, Vector4<T> aEnd, float aTime)
	{
		Corgi::Vector4<T> returnThis;

		returnThis.x = aStart.x + ((aEnd.x - aStart.x) * aTime);
		returnThis.y = aStart.y + ((aEnd.y - aStart.y) * aTime);
		returnThis.z = aStart.z + ((aEnd.z - aStart.z) * aTime);
		returnThis.w = aStart.w + ((aEnd.w - aStart.w) * aTime);

		return returnThis;
	}

	template<class T>
	Vector3<T> Lerp(Vector3<T> aStart, Vector3<T> aEnd, float aTime)
	{
		Vector3<T> returnThis;

		returnThis.x = aStart.x + ((aEnd.x - aStart.x) * aTime);
		returnThis.y = aStart.y + ((aEnd.y - aStart.y) * aTime);
		returnThis.z = aStart.z + ((aEnd.z - aStart.z) * aTime);

		return returnThis;
	}

	void RadialFlip(float& f);


	void RadialFlip(Vector3f& v);

	void RadialFlip(Vector4f& v);


}