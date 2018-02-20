#pragma once
#include <cmath>
#include "Vector3.h"

namespace Corgi

{
	template <class T>
	class Vector4
	{
	public:

		T x;
		T y;
		T z;
		T w;

		//Creates a null-vector
		Vector4<T>();

		//Creates a vector (aX, aY, aZ)
		Vector4<T>(const T& aX, const T& aY, const T& aZ, const T& aW);

		Vector4<T>(const Vector3<T>& v, const T& w);

		//Copy constructor (compiler generated)

		Vector4<T>(const Vector4<T>& aVector4) = default;

		//Assignment operator (compiler generated)

		Vector4<T>& operator=(const Vector4<T>& aVector4) = default;

		//Destructor (compiler generated)

		~Vector4<T>() = default;

		//Returns the squared length of the vector

		T Length2() const;

		//Returns the length of the vector

		T Length() const;

		//Returns a normalized copy of this

		Vector4<T> GetNormalized() const;

		//Normalizes the vector

		void Normalize();

		//Returns the dot product of this and aVector

		T Dot(const Vector4<T>& aVector) const;

		//Returns the cross product of this and aVector

		Vector4<T> Cross(const Vector4<T>& aVector) const;

		Vector3<T> V3() const;

	};

	//Returns the vector sum of aVector0 and aVector1

	template <class T>
	Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		Vector4<T> returnThis;
		returnThis.x = aVector0.x + aVector1.x;
		returnThis.y = aVector0.y + aVector1.y;
		returnThis.z = aVector0.z + aVector1.z;
		returnThis.w = aVector0.w + aVector1.w;

		return returnThis;
	}

	//Returns the vector difference of aVector0 and aVector1

	template <class T>
	Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		Vector4<T> returnThis;
		returnThis.x = aVector0.x - aVector1.x;
		returnThis.y = aVector0.y - aVector1.y;
		returnThis.z = aVector0.z - aVector1.z;
		returnThis.w = aVector0.w - aVector1.w;

		return returnThis;
	}

	//Returns the vector aVector multiplied by the scalar aScalar

	template <class T>
	Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar)
	{
		Vector4<T> returnThis;
		returnThis.x = aVector.x * aScalar;
		returnThis.y = aVector.y * aScalar;
		returnThis.z = aVector.z * aScalar;
		returnThis.w = aVector.w * aScalar;

		return returnThis;
	}

	template <class T>
	Vector4<T> operator/(const Vector4<T>& aVector, const T& aScalar)
	{
		Vector4<T> returnThis;
		returnThis.x = aVector.x / aScalar;
		returnThis.y = aVector.y / aScalar;
		returnThis.z = aVector.z / aScalar;
		returnThis.w = aVector.w / aScalar;

		return returnThis;
	}

	//Returns the vector aVector multiplied by the scalar aScalar

	template <class T>
	Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector)
	{
		Vector4<T> returnThis;
		returnThis.x = aVector.x * aScalar;
		returnThis.y = aVector.y * aScalar;
		returnThis.z = aVector.z * aScalar;
		returnThis.w = aVector.w * aScalar;

		return returnThis;
	}

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)

	template <class T>
	Vector4<T> operator/(const T& aScalar, const Vector4<T>& aVector)
	{
		Vector4<T> returnThis;
		returnThis.x = aVector.x / aScalar;
		returnThis.y = aVector.y / aScalar;
		returnThis.z = aVector.z / aScalar;
		returnThis.w = aVector.w / aScalar;

		return returnThis;
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1)

	template <class T>
	void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
		aVector0.w += aVector1.w;
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1)

	template <class T>
	void operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
		aVector0.w -= aVector1.w;
	}

	//Equivalent to setting aVector to (aVector * aScalar)

	template <class T>
	void operator*=(Vector4<T>& aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
		aVector.w *= aScalar;
	}

	template <class T>
	void operator/=(Vector4<T>& aVector, const T& aScalar)
	{
		aVector.x /= aScalar;
		aVector.y /= aScalar;
		aVector.z /= aScalar;
		aVector.w /= aScalar;
	}

	//Equivalent to setting aVector to (aVector / aScalar)


	//Creates a null-vector
	template <class T>
	Vector4<T>::Vector4()
	{
		x = static_cast<T>(0);
		y = static_cast<T>(0);
		z = static_cast<T>(0);
		w = static_cast<T>(0);
	}

	//Creates a vector (aX, aY, aZ)
	template <class T>
	Vector4<T>::Vector4(const T& aX, const T& aY, const T& aZ, const T& aW)
	{
		x = aX;
		y = aY;
		z = aZ;
		w = aW;
	}

	template<class T>
	inline Vector4<T>::Vector4(const Vector3<T>& v, const T & aW)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = aW;
	}

	//Returns the squared length of the vector
	template <class T>
	T Vector4<T>::Length2() const
	{
		T returnThis = ((x*x) + (y*y) + (z*z) + (w*w));
		return returnThis;
	}

	//Returns the length of the vector
	template <class T>
	T Vector4<T>::Length() const
	{
		T returnThis = ((x*x) + (y*y) + (z*z) + (w*w));
		returnThis = sqrt(returnThis);
		return returnThis;
	}

	//Returns a normalized copy of this
	template <class T>
	Vector4<T> Vector4<T>::GetNormalized() const
	{
		Vector4<T> returnThis;

		T vectorLength = ((x*x) + (y*y) + (z*z) + (w*w));
		vectorLength = sqrt(vectorLength);
		returnThis.x = x / vectorLength;
		returnThis.y = y / vectorLength;
		returnThis.z = z / vectorLength;
		returnThis.w = w / vectorLength;

		return returnThis;
	}

	//Normalizes the vector
	template <class T>
	void Vector4<T>::Normalize()
	{
		T vectorLength = ((x*x) + (y*y) + (z*z) + (w*w));
		vectorLength = sqrt(vectorLength);
		x /= vectorLength;
		y /= vectorLength;
		z /= vectorLength;
		w /= vectorLength;
	}

	//Returns the dot product of this and aVector
	template <class T>
	T Vector4<T>::Dot(const Vector4<T>& aVector) const
	{
		T returnThis = ((x * aVector.x) + (y * aVector.y) + (z * aVector.z) + (w * aVector.w));
		return returnThis;
	}

	//Returns the cross product of this and aVector
	template <class T>
	Vector4<T> Vector4<T>::Cross(const Vector4<T>& aVector) const
	{
		Vector4<T> returnThis;

		returnThis.x = (y * aVector.z) - (z * aVector.y);
		returnThis.y = (z * aVector.x) - (x * aVector.z);
		returnThis.z = (x * aVector.y) - (y * aVector.x);
		returnThis.w = w;
		return returnThis;
	}
	template<class T>
	inline Vector3<T> Vector4<T>::V3() const
	{
		Vector3<T> v;
		v.x = x;
		v.y = y;
		v.z = z;

		return v;
	}
}

typedef Corgi::Vector4<float> Vector4f;
typedef Corgi::Vector4<int> Vector4int;