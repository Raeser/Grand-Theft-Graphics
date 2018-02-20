#pragma once

#include <cmath>
#include "vector2.h"
namespace Corgi

{
	template <class T>
	class Vector3
	{
	public:

		T x;
		T y;
		T z;

		//Creates a null-vector
		Vector3<T>();

		//Creates a vector (aX, aY, aZ)
		Vector3<T>(const T& aX, const T& aY, const T& aZ);

		//Copy constructor (compiler generated)

		Vector3<T>(const Vector3<T>& aVector3) = default;

		//Assignment operator (compiler generated)

		Vector3<T>& operator=(const Vector3<T>& aVector3) = default;

		//Destructor (compiler generated)

		~Vector3<T>() = default;

		//Returns the squared length of the vector

		T Length2() const;

		//Returns the length of the vector

		T Length() const;

		//Returns a normalized copy of this

		Vector3<T> GetNormalized() const;

		//Normalizes the vector

		void Normalize();

		//Returns the dot product of this and aVector

		T Dot(const Vector3<T>& aVector) const;

		//Returns the cross product of this and aVector

		Vector3<T> Cross(const Vector3<T>& aVector) const;

		Vector2<T> V2() const;

	};

	//Returns the vector sum of aVector0 and aVector1

	template <class T>
	Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		Vector3<T> returnThis;
		returnThis.x = aVector0.x + aVector1.x;
		returnThis.y = aVector0.y + aVector1.y;
		returnThis.z = aVector0.z + aVector1.z;

		return returnThis;
	}

	//Returns the vector difference of aVector0 and aVector1

	template <class T>
	Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		Vector3<T> returnThis;
		returnThis.x = aVector0.x - aVector1.x;
		returnThis.y = aVector0.y - aVector1.y;
		returnThis.z = aVector0.z - aVector1.z;

		return returnThis;
	}

	//Returns the vector aVector multiplied by the scalar aScalar

	template <class T>
	Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
	{
		Vector3<T> returnThis;
		returnThis.x = aVector.x * aScalar;
		returnThis.y = aVector.y * aScalar;
		returnThis.z = aVector.z * aScalar;

		return returnThis;
	}

	template <class T>
	Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar)
	{
		Vector3<T> returnThis;
		returnThis.x = aVector.x / aScalar;
		returnThis.y = aVector.y / aScalar;
		returnThis.z = aVector.z / aScalar;

		return returnThis;
	}

	//Returns the vector aVector multiplied by the scalar aScalar

	template <class T>
	Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector)
	{
		Vector3<T> returnThis;
		returnThis.x = aVector.x * aScalar;
		returnThis.y = aVector.y * aScalar;
		returnThis.z = aVector.z * aScalar;

		return returnThis;
	}

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)


	//Equivalent to setting aVector0 to (aVector0 + aVector1)

	template <class T>
	void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
		aVector0.z += aVector1.z;
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1)

	template <class T>
	void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
		aVector0.z -= aVector1.z;
	}

	//Equivalent to setting aVector to (aVector * aScalar)

	template <class T>
	void operator*=(Vector3<T>& aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
		aVector.z *= aScalar;
	}

	template <class T>
	void operator/=(Vector3<T>& aVector, const T& aScalar)
	{
		aVector.x /= aScalar;
		aVector.y /= aScalar;
		aVector.z /= aScalar;
	}

	//Equivalent to setting aVector to (aVector / aScalar)

	

	//Creates a null-vector
	template <class T>
	Vector3<T>::Vector3()
	{
		x = static_cast<T>(0);
		y = static_cast<T>(0);
		z = static_cast<T>(0);
	}

	//Creates a vector (aX, aY, aZ)
	template <class T>
	Vector3<T>::Vector3(const T& aX, const T& aY, const T& aZ)
	{
		x = aX;
		y = aY;
		z = aZ;
	}

	//Returns the squared length of the vector
	template <class T>
	T Vector3<T>::Length2() const
	{
		T returnThis = ((x*x) + (y*y) + (z*z));
		return returnThis;
	}

	//Returns the length of the vector
	template <class T>
	T Vector3<T>::Length() const
	{
		T returnThis = ((x*x) + (y*y) + (z*z));
		returnThis = sqrt(returnThis);
		return returnThis;
	}

	//Returns a normalized copy of this
	template <class T>
	Vector3<T> Vector3<T>::GetNormalized() const
	{
		Vector3<T> returnThis;

		T vectorLength = ((x*x) + (y*y) + (z*z));
		vectorLength = sqrt(vectorLength);
		returnThis.x = x / vectorLength;
		returnThis.y = y / vectorLength;
		returnThis.z = z / vectorLength;

		return returnThis;
	}

	//Normalizes the vector
	template <class T>
	void Vector3<T>::Normalize()
	{
		T vectorLength = ((x*x) + (y*y) + (z*z));
		vectorLength = sqrt(vectorLength);
		x /= vectorLength;
		y /= vectorLength;
		z /= vectorLength;
	}

	//Returns the dot product of this and aVector
	template <class T>
	T Vector3<T>::Dot(const Vector3<T>& aVector) const
	{
		T returnThis = ((x *aVector.x) + (y * aVector.y) + (z * aVector.z));
		return returnThis;
	}

	//Returns the cross product of this and aVector
	template <class T>
	Vector3<T> Vector3<T>::Cross(const Vector3<T>& aVector) const
	{
		Vector3<T> returnThis;

		returnThis.x = (y * aVector.z) - (z * aVector.y);
		returnThis.y = (z * aVector.x) - (x * aVector.z);
		returnThis.z = (x * aVector.y) - (y * aVector.x);

		return returnThis;
	}

	template<class T>
	inline Vector2<T> Vector3<T>::V2() const
	{
		return Vector2<T>(x, y);
	}
}

typedef Corgi::Vector3<float> Vector3f;
typedef Corgi::Vector3<int> Vector3int;