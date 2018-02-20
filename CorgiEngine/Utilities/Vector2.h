#pragma once
#include <cmath>

namespace Corgi

{
	template <class T>
	class Vector2
	{
	public:

		T x;
		T y;

		//Creates a null-vector
		Vector2<T>();

		//Creates a vector (aX, aY, aZ)
		Vector2<T>(const T& aX, const T& aY);

	//	Vector2<T>(const POINT& p);

		//Copy constructor (compiler generated)

		Vector2<T>(const Vector2<T>& Vector2) = default;

		//Assignment operator (compiler generated)

		Vector2<T>& operator=(const Vector2<T>& Vector2) = default;

		//Destructor (compiler generated)

		~Vector2<T>() = default;

		//Returns the squared length of the vector

		T Length2() const;

		//Returns the length of the vector

		T Length() const;

		//Returns a normalized copy of this

		Vector2<T> GetNormalized() const;

		//Normalizes the vector

		void Normalize();

		//Returns the dot product of this and aVector

		T Dot(const Vector2<T>& aVector) const;

		//Returns the cross product of this and aVector


	};

	//Returns the vector sum of aVector0 and aVector1

	template <class T>
	Vector2<T> operator+(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		Vector2<T> returnThis;
		returnThis.x = aVector0.x + aVector1.x;
		returnThis.y = aVector0.y + aVector1.y;

		return returnThis;
	}

	//Returns the vector difference of aVector0 and aVector1

	template <class T>
	Vector2<T> operator-(const Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		Vector2<T> returnThis;
		returnThis.x = aVector0.x - aVector1.x;
		returnThis.y = aVector0.y - aVector1.y;

		return returnThis;
	}

	//Returns the vector aVector multiplied by the scalar aScalar

	template <class T>
	Vector2<T> operator*(const Vector2<T>& aVector, const T& aScalar)
	{
		Vector2<T> returnThis;
		returnThis.x = aVector.x * aScalar;
		returnThis.y = aVector.y * aScalar;

		return returnThis;
	}

	//Returns the vector aVector multiplied by the scalar aScalar

	template <class T>
	Vector2<T> operator*(const T& aScalar, const Vector2<T>& aVector)
	{
		Vector2<T> returnThis;
		returnThis.x = aVector.x * aScalar;
		returnThis.y = aVector.y * aScalar;

		return returnThis;
	}



	template <class T>
	Vector2<T> operator/(const T& aScalar, const Vector2<T>& aVector)
	{
		Vector2<T> returnThis;
		returnThis.x = aVector.x / aScalar;
		returnThis.y = aVector.y / aScalar;

		return returnThis;
	}

	//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1 / aScalar)

	template <class T>
	Vector2<T> operator/(const Vector2<T>& aVector, const T& aScalar)
	{
		Vector2<T> returnThis;
		returnThis.x = aVector.x / aScalar;
		returnThis.y = aVector.y / aScalar;

		return returnThis;
	}

	//Equivalent to setting aVector0 to (aVector0 + aVector1)

	template <class T>
	void operator+=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x += aVector1.x;
		aVector0.y += aVector1.y;
	}

	//Equivalent to setting aVector0 to (aVector0 - aVector1)

	template <class T>
	void operator-=(Vector2<T>& aVector0, const Vector2<T>& aVector1)
	{
		aVector0.x -= aVector1.x;
		aVector0.y -= aVector1.y;
	}

	//Equivalent to setting aVector to (aVector * aScalar)

	template <class T>
	void operator*=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector.x *= aScalar;
		aVector.y *= aScalar;
	}

	template <class T>
	void operator/=(Vector2<T>& aVector, const T& aScalar)
	{
		aVector.x /= aScalar;
		aVector.y /= aScalar;
	}



	//Creates a null-vector
	template <class T>
	Vector2<T>::Vector2()
	{
		x = static_cast<T>(0);
		y = static_cast<T>(0);
	}

	//Creates a vector (aX, aY, aZ)
	template <class T>
	Vector2<T>::Vector2(const T& aX, const T& aY)
	{
		x = aX;
		y = aY;
	}

	//template<class T>
	//inline Vector2<T>::Vector2(const POINT & p)
	//{
	//	x = static_cast<T>(p.x);
	//	y = static_cast<T>(p.y);
	//}

	//Returns the squared length of the vector
	template <class T>
	T Vector2<T>::Length2() const
	{
		T returnThis = ((x*x) + (y*y));
		return returnThis;
	}

	//Returns the length of the vector
	template <class T>
	T Vector2<T>::Length() const
	{
		T returnThis = ((x*x) + (y*y));
		returnThis = sqrt(returnThis);
		return returnThis;
	}

	//Returns a normalized copy of this
	template <class T>
	Vector2<T> Vector2<T>::GetNormalized() const
	{
		Vector2<T> returnThis;

		T vectorLength = ((x*x) + (y*y));
		vectorLength = sqrt(vectorLength);
		returnThis.x = x / vectorLength;
		returnThis.y = y / vectorLength;

		return returnThis;
	}

	//Normalizes the vector
	template <class T>
	void Vector2<T>::Normalize()
	{
		T vectorLength = ((x*x) + (y*y));
		vectorLength = sqrt(vectorLength);
		x /= vectorLength;
		y /= vectorLength;
	}

	//Returns the dot product of this and aVector
	template <class T>
	T Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		T returnThis = ((x *aVector.x) + (y * aVector.y));
		return returnThis;
	}
}

typedef Corgi::Vector2<float> Vector2f;
typedef Corgi::Vector2<int> Vector2int;