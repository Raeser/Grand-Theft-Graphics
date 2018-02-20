#pragma once

#include <cmath>
#include <DirectXMath.h>
#include "CorgiMath.h"
#include "Quaternion.h"
#include <emmintrin.h> //SSE 2


#define USE_SIMD true
#pragma warning( push ) 
#pragma warning(disable: 4201)	//Prevents the warning that complains about the union and structs being nameless
								//They dont have names for backwards compability with original .myMatrix[];

namespace Corgi
{

	template <class T>
	class Matrix44
	{
	public:
		Matrix44<T>();
		Matrix44<T>(const Matrix44<T>& aMatrix);	
		Matrix44<T>(const DirectX::CXMMATRIX& aMatrix);
		Matrix44<T>(const float arr[16]);
		Matrix44<T>& operator=(const Matrix44<T>& aMatrix);
		void SetPosition(Vector3f aVector);

		static Matrix44<T> CreateFromVector4(const Corgi::Vector4<T>& v1, const Corgi::Vector4<T>& v2, const Corgi::Vector4<T>& v3, const Corgi::Vector4<T>& pos);
		static Matrix44<T> CreateFromVector3(const Corgi::Vector3<T>& v1, const Corgi::Vector3<T>& v2, const Corgi::Vector3<T>& v3, const Corgi::Vector3<T>& pos);
		static Matrix44<T> CreateRotateAroundX(T aAngleInRadians);
		static Matrix44<T> CreateRotateAroundY(T aAngleInRadians);
		static Matrix44<T> CreateRotateAroundZ(T aAngleInRadians);
		static Matrix44<T> CreateRotateAroundAxis(const Vector4<T>& aAxis, T aAngleInRadians);
		static Matrix44<T> CreateRotationFromQuat(const Quaternion<T>& aQuaterion);
		static Quaternion<T> CreateQuatFromMatrix(const Matrix44& aMatrix);


		static Matrix44<T> CreateOrthographicMatrix(T aRight, T aLeft, T aNear, T aFar, T aTop, T aBottom);
		static Matrix44<T> LookAt(const Vector3<T>& EyePos, const Vector3<T>& aPosToLookAt, const Vector3<T>& aUp);


		Matrix44<T> GetFastInverse() const;
		static Matrix44<T> Transpose(const Matrix44<T>& aMatrixToTranspose);
		static Matrix44<T> TransposeOpenGL(const Matrix44<T>& aMatrixToTranspose);
		static void TransposeThis();

		Corgi::Vector3<T> GetPosition() const;
		Corgi::Vector3<T> GetRotation() const;
		union
		{
			struct
			{
				__m128 m1, m2, m3, m4;
			};
			struct
			{
				T myMatrix[16];
			};
		};


		static Matrix44<T> RegularMult(const Matrix44<T>& aMatrix0, const Matrix44<T>& aMatrix1);
		static void SIMDMult(Matrix44<T>& aMatrix0, const Matrix44<T>& aMatrix1);


	private:
		Matrix44<T>(const Corgi::Vector4<T>& v1, const Corgi::Vector4<T>& v2, const Corgi::Vector4<T>& v3, const Corgi::Vector4<T>& pos);
		Matrix44<T>(const Corgi::Vector3<T>& v1, const Corgi::Vector3<T>& v2, const Corgi::Vector3<T>& v3, const Corgi::Vector3<T>& pos);
		


	};


	template <class T>
	Matrix44<T> operator+(const Matrix44<T>& aMatrix0, const Matrix44<T>& aMatrix1)
	{

		Matrix44<T> returnThis;
		returnThis.myMatrix[0] = aMatrix0.myMatrix[0] + aMatrix1.myMatrix[0];
		returnThis.myMatrix[1] = aMatrix0.myMatrix[1] + aMatrix1.myMatrix[1];
		returnThis.myMatrix[2] = aMatrix0.myMatrix[2] + aMatrix1.myMatrix[2];
		returnThis.myMatrix[3] = aMatrix0.myMatrix[3] + aMatrix1.myMatrix[3];

		returnThis.myMatrix[4] = aMatrix0.myMatrix[4] + aMatrix1.myMatrix[4];
		returnThis.myMatrix[5] = aMatrix0.myMatrix[5] + aMatrix1.myMatrix[5];
		returnThis.myMatrix[6] = aMatrix0.myMatrix[6] + aMatrix1.myMatrix[6];
		returnThis.myMatrix[7] = aMatrix0.myMatrix[7] + aMatrix1.myMatrix[7];

		returnThis.myMatrix[8] = aMatrix0.myMatrix[8] + aMatrix1.myMatrix[8];
		returnThis.myMatrix[9] = aMatrix0.myMatrix[9] + aMatrix1.myMatrix[9];
		returnThis.myMatrix[10] = aMatrix0.myMatrix[10] + aMatrix1.myMatrix[10];
		returnThis.myMatrix[11] = aMatrix0.myMatrix[11] + aMatrix1.myMatrix[11];

		returnThis.myMatrix[12] = aMatrix0.myMatrix[12] + aMatrix1.myMatrix[12];
		returnThis.myMatrix[13] = aMatrix0.myMatrix[13] + aMatrix1.myMatrix[13];
		returnThis.myMatrix[14] = aMatrix0.myMatrix[14] + aMatrix1.myMatrix[14];
		returnThis.myMatrix[15] = aMatrix0.myMatrix[15] + aMatrix1.myMatrix[15];
		return returnThis;
	}

	template <class T>
	Matrix44<T> operator-(const Matrix44<T>& aMatrix0, const Matrix44<T>& aMatrix1)
	{
		Matrix44<T> returnThis;
		returnThis.myMatrix[0] = aMatrix0.myMatrix[0] - aMatrix1.myMatrix[0];
		returnThis.myMatrix[1] = aMatrix0.myMatrix[1] - aMatrix1.myMatrix[1];
		returnThis.myMatrix[2] = aMatrix0.myMatrix[2] - aMatrix1.myMatrix[2];
		returnThis.myMatrix[3] = aMatrix0.myMatrix[3] - aMatrix1.myMatrix[3];
		returnThis.myMatrix[4] = aMatrix0.myMatrix[4] - aMatrix1.myMatrix[4];
		returnThis.myMatrix[5] = aMatrix0.myMatrix[5] - aMatrix1.myMatrix[5];
		returnThis.myMatrix[6] = aMatrix0.myMatrix[6] - aMatrix1.myMatrix[6];
		returnThis.myMatrix[7] = aMatrix0.myMatrix[7] - aMatrix1.myMatrix[7];				
		returnThis.myMatrix[8] = aMatrix0.myMatrix[8] - aMatrix1.myMatrix[8];
		returnThis.myMatrix[9] = aMatrix0.myMatrix[9] - aMatrix1.myMatrix[9];
		returnThis.myMatrix[10] = aMatrix0.myMatrix[10] - aMatrix1.myMatrix[10];
		returnThis.myMatrix[11] = aMatrix0.myMatrix[11] - aMatrix1.myMatrix[11];
		returnThis.myMatrix[12] = aMatrix0.myMatrix[12] - aMatrix1.myMatrix[12];
		returnThis.myMatrix[13] = aMatrix0.myMatrix[13] - aMatrix1.myMatrix[13];
		returnThis.myMatrix[14] = aMatrix0.myMatrix[14] - aMatrix1.myMatrix[14];
		returnThis.myMatrix[15] = aMatrix0.myMatrix[15] - aMatrix1.myMatrix[15];

		return returnThis;
	}

	template <class T>
	Matrix44<T> operator*(const Matrix44<T>& aMatrix0, const Matrix44<T>& aMatrix1)
	{
		Matrix44<T> returnThis = aMatrix0;
		if (USE_SIMD)
		{	
			Matrix44<T>::SIMDMult(returnThis, aMatrix1);
		}
		else
		{
			returnThis = Matrix44<T>::RegularMult(aMatrix0, aMatrix1);
		}

		return returnThis;
	}


	template <class T>
	void operator+=(Matrix44<T>& aMatrix0, const Matrix44<T>& aMatrix1)
	{
		aMatrix0.myMatrix[0] += aMatrix1.myMatrix[0];
		aMatrix0.myMatrix[1] += aMatrix1.myMatrix[1];
		aMatrix0.myMatrix[2] += aMatrix1.myMatrix[2];
		aMatrix0.myMatrix[3] += aMatrix1.myMatrix[3];
		
		aMatrix0.myMatrix[4] += aMatrix1.myMatrix[4];
		aMatrix0.myMatrix[5] += aMatrix1.myMatrix[5];
		aMatrix0.myMatrix[6] += aMatrix1.myMatrix[6];
		aMatrix0.myMatrix[7] += aMatrix1.myMatrix[7];
		
		aMatrix0.myMatrix[8] += aMatrix1.myMatrix[8];
		aMatrix0.myMatrix[9] += aMatrix1.myMatrix[9];
		aMatrix0.myMatrix[10] += aMatrix1.myMatrix[10];
		aMatrix0.myMatrix[11] += aMatrix1.myMatrix[11];

		aMatrix0.myMatrix[12] += aMatrix1.myMatrix[12];
		aMatrix0.myMatrix[13] += aMatrix1.myMatrix[13];
		aMatrix0.myMatrix[14] += aMatrix1.myMatrix[14];
		aMatrix0.myMatrix[15] += aMatrix1.myMatrix[15];
	}


	template <class T>
	void operator-=(Matrix44<T>& aMatrix0, const Matrix44<T>& aMatrix1)
	{
		aMatrix0.myMatrix[0] -= aMatrix1.myMatrix[0];
		aMatrix0.myMatrix[1] -= aMatrix1.myMatrix[1];
		aMatrix0.myMatrix[2] -= aMatrix1.myMatrix[2];
		aMatrix0.myMatrix[3] -= aMatrix1.myMatrix[3];

		aMatrix0.myMatrix[4] -= aMatrix1.myMatrix[4];
		aMatrix0.myMatrix[5] -= aMatrix1.myMatrix[5];
		aMatrix0.myMatrix[6] -= aMatrix1.myMatrix[6];
		aMatrix0.myMatrix[7] -= aMatrix1.myMatrix[7];

		aMatrix0.myMatrix[8] -= aMatrix1.myMatrix[8];
		aMatrix0.myMatrix[9] -= aMatrix1.myMatrix[9];
		aMatrix0.myMatrix[10] -= aMatrix1.myMatrix[10];
		aMatrix0.myMatrix[11] -= aMatrix1.myMatrix[11];

		aMatrix0.myMatrix[12] -= aMatrix1.myMatrix[12];
		aMatrix0.myMatrix[13] -= aMatrix1.myMatrix[13];
		aMatrix0.myMatrix[14] -= aMatrix1.myMatrix[14];
		aMatrix0.myMatrix[15] -= aMatrix1.myMatrix[15];
	}

	template <class T>
	void operator*=(Matrix44<T>& aMatrix0, const Matrix44<T>& aMatrix1)
	{
		if (USE_SIMD)
		{
			Matrix44<T>::SIMDMult(aMatrix0, aMatrix1);
		}
		else
		{
			aMatrix0 = Matrix44<T>::RegularMult(aMatrix0, aMatrix1);		
		}
	}

	template <class T>
	Vector4<T> operator*(const Vector4<T>& aVector, const Matrix44<T>& aMatrix)
	{
		Vector4<T> returnThis;

		returnThis.x = aVector.x * aMatrix.myMatrix[0] + aVector.y * aMatrix.myMatrix[4] + aVector.z * aMatrix.myMatrix[8] + aVector.w * aMatrix.myMatrix[12];
		returnThis.y = aVector.x * aMatrix.myMatrix[1] + aVector.y * aMatrix.myMatrix[5] + aVector.z * aMatrix.myMatrix[9] + aVector.w * aMatrix.myMatrix[13];
		returnThis.z = aVector.x * aMatrix.myMatrix[2] + aVector.y * aMatrix.myMatrix[6] + aVector.z * aMatrix.myMatrix[10] + aVector.w * aMatrix.myMatrix[14];
		returnThis.w = aVector.x * aMatrix.myMatrix[3] + aVector.y * aMatrix.myMatrix[7] + aVector.z * aMatrix.myMatrix[11] + aVector.w * aMatrix.myMatrix[15];
		return returnThis;
	}

	template <class T>
	Quaternion<T> operator*(const Quaternion<T>& aQuat, const Matrix44<T>& aMatrix)
	{
		Quaternion<T> returnThis;

		returnThis.x = aQuat.x * aMatrix.myMatrix[0] + aQuat.y * aMatrix.myMatrix[4] + aQuat.z * aMatrix.myMatrix[8];
		returnThis.y = aQuat.x * aMatrix.myMatrix[1] + aQuat.y * aMatrix.myMatrix[5] + aQuat.z * aMatrix.myMatrix[9];
		returnThis.z = aQuat.x * aMatrix.myMatrix[2] + aQuat.y * aMatrix.myMatrix[6] + aQuat.z * aMatrix.myMatrix[10];
		returnThis.w = aQuat.w;
		return returnThis;
	}

	template <class T>
	bool operator==(const Matrix44<T>& aMatrix0, const Matrix44<T>& aMatrix1)
	{

		if (aMatrix0.myMatrix[0] != aMatrix1.myMatrix[0])
		{
			return false;
		}
		if (aMatrix0.myMatrix[1] != aMatrix1.myMatrix[1])
		{
			return false;
		}
		if (aMatrix0.myMatrix[2] != aMatrix1.myMatrix[2])
		{
			return false;
		}
		if (aMatrix0.myMatrix[3] != aMatrix1.myMatrix[3])
		{
			return false;
		}
		if (aMatrix0.myMatrix[4] != aMatrix1.myMatrix[4])
		{
			return false;
		}
		if (aMatrix0.myMatrix[5] != aMatrix1.myMatrix[5])
		{
			return false;
		}
		if (aMatrix0.myMatrix[6] != aMatrix1.myMatrix[6])
		{
			return false;
		}
		if (aMatrix0.myMatrix[7] != aMatrix1.myMatrix[7])
		{
			return false;
		}
		if (aMatrix0.myMatrix[8] != aMatrix1.myMatrix[8])
		{
			return false;
		}
		if (aMatrix0.myMatrix[9] != aMatrix1.myMatrix[9])
		{
			return false;
		}
		if (aMatrix0.myMatrix[10] != aMatrix1.myMatrix[10])
		{
			return false;
		}
		if (aMatrix0.myMatrix[11] != aMatrix1.myMatrix[11])
		{
			return false;
		}
		if (aMatrix0.myMatrix[12] != aMatrix1.myMatrix[12])
		{
			return false;
		}
		if (aMatrix0.myMatrix[13] != aMatrix1.myMatrix[13])
		{
			return false;
		}
		if (aMatrix0.myMatrix[14] != aMatrix1.myMatrix[14])
		{
			return false;
		}
		if (aMatrix0.myMatrix[15] != aMatrix1.myMatrix[15])
		{
			return false;
		}


		return true;
	}

	template<class T>
	inline Matrix44<T>& Matrix44<T>::operator=(const Matrix44<T>& aMatrix)
	{
		myMatrix[0] = aMatrix.myMatrix[0];
		myMatrix[1] = aMatrix.myMatrix[1];
		myMatrix[2] = aMatrix.myMatrix[2];
		myMatrix[3] = aMatrix.myMatrix[3];
		myMatrix[4] = aMatrix.myMatrix[4];
		myMatrix[5] = aMatrix.myMatrix[5];
		myMatrix[6] = aMatrix.myMatrix[6];
		myMatrix[7] = aMatrix.myMatrix[7];
		myMatrix[8] = aMatrix.myMatrix[8];
		myMatrix[9] = aMatrix.myMatrix[9];
		myMatrix[10] = aMatrix.myMatrix[10];
		myMatrix[11] = aMatrix.myMatrix[11];
		myMatrix[12] = aMatrix.myMatrix[12];
		myMatrix[13] = aMatrix.myMatrix[13];
		myMatrix[14] = aMatrix.myMatrix[14];
		myMatrix[15] = aMatrix.myMatrix[15];

		return *this;
	}

	template <class T>
	Matrix44<T>::Matrix44()
	{
		myMatrix[0] = 1;
		myMatrix[1] = 0;
		myMatrix[2] = 0;
		myMatrix[3] = 0;
		myMatrix[4] = 0;
		myMatrix[5] = 1;
		myMatrix[6] = 0;
		myMatrix[7] = 0;
		myMatrix[8] = 0;
		myMatrix[9] = 0;
		myMatrix[10] = 1;
		myMatrix[11] = 0;
		myMatrix[12] = 0;
		myMatrix[13] = 0;
		myMatrix[14] = 0;
		myMatrix[15] = 1;


	}

	template <class T>
	Matrix44<T>::Matrix44(const Matrix44<T>& aMatrix)
	{
		myMatrix[0] = aMatrix.myMatrix[0];
		myMatrix[1] = aMatrix.myMatrix[1];
		myMatrix[2] = aMatrix.myMatrix[2];
		myMatrix[3] = aMatrix.myMatrix[3];
		myMatrix[4] = aMatrix.myMatrix[4];
		myMatrix[5] = aMatrix.myMatrix[5];
		myMatrix[6] = aMatrix.myMatrix[6];
		myMatrix[7] = aMatrix.myMatrix[7];
		myMatrix[8] = aMatrix.myMatrix[8];
		myMatrix[9] = aMatrix.myMatrix[9];
		myMatrix[10] = aMatrix.myMatrix[10];
		myMatrix[11] = aMatrix.myMatrix[11];
		myMatrix[12] = aMatrix.myMatrix[12];
		myMatrix[13] = aMatrix.myMatrix[13];
		myMatrix[14] = aMatrix.myMatrix[14];
		myMatrix[15] = aMatrix.myMatrix[15];

	}

	template<class T>
	inline Matrix44<T>::Matrix44(const Corgi::Vector4<T>& v1, const Corgi::Vector4<T>& v2, const Corgi::Vector4<T>& v3, const Corgi::Vector4<T>& pos)
	{
		myMatrix[0] = v1.x;
		myMatrix[1] = v1.y;
		myMatrix[2] = v1.z;
		myMatrix[3] = v1.w;

		myMatrix[4] = v2.x;
		myMatrix[5] = v2.y;
		myMatrix[6] = v2.z;
		myMatrix[7] = v2.w;

		myMatrix[8] = v3.x;
		myMatrix[9] = v3.y;
		myMatrix[10] = v3.z;
		myMatrix[11] = v3.w;

		myMatrix[12] = pos.x;
		myMatrix[13] = pos.y;
		myMatrix[14] = pos.z;
		myMatrix[15] = pos.w;
	}

	template<class T>
	inline Matrix44<T>::Matrix44(const Corgi::Vector3<T>& v1, const Corgi::Vector3<T>& v2, const Corgi::Vector3<T>& v3, const Corgi::Vector3<T>& pos)
	{
		myMatrix[0] = v1.x;
		myMatrix[1] = v1.y;
		myMatrix[2] = v1.z;
		myMatrix[3] = 0;

		myMatrix[4] = v2.x;
		myMatrix[5] = v2.y;
		myMatrix[6] = v2.z;
		myMatrix[7] = 0;

		myMatrix[8] = v3.x;
		myMatrix[9] = v3.y;
		myMatrix[10] = v3.z;
		myMatrix[11] = 0;

		myMatrix[12] = pos.x;
		myMatrix[13] = pos.y;
		myMatrix[14] = pos.z;
		myMatrix[15] = 1;
	}

	template<class T>
	inline Matrix44<T>::Matrix44(const DirectX::CXMMATRIX& aMatrix)
	{
		memcpy(myMatrix, &aMatrix, sizeof(T) * 16);
	}

	template<class T>
	inline Matrix44<T>::Matrix44(const float arr[16])
	{
		memcpy(myMatrix, arr, sizeof(T) * 16);
	}

	template<class T>
	inline void Matrix44<T>::SetPosition(Vector3f aVector)
	{
		myMatrix[12] = aVector.x;
		myMatrix[13] = aVector.y;
		myMatrix[14] = aVector.z;
	}

	template<class T>
	inline Matrix44<T> Matrix44<T>::CreateRotationFromQuat(const Quaternion<T>& aQuaterion)
	{
		Matrix44<T> m;

		//const float x = sin(aQuaterion.x / 2);
		//const float y = sin(aQuaterion.y / 2);
		//const float z = sin(aQuaterion.z / 2);
		////const float z = 0;
		//const float w = cos(aQuaterion.w / 2);
 
 		const float x = aQuaterion.x;
 		const float y = aQuaterion.y;
 		const float z = aQuaterion.z;
 		const float w = aQuaterion.w;

		m.myMatrix[0] = 1 - 2 * y*y - 2 * z*z;
		m.myMatrix[1] = 2 * x*y + 2 * w * z;
		m.myMatrix[2] = 2 * x*z - 2 * w*y;

		m.myMatrix[4] = 2 * x*y - 2 * w *z;
		m.myMatrix[5] = 1 - 2 * x*x - 2 * z*z;
		m.myMatrix[6] = 2 * y*z + 2 * w*x;

		m.myMatrix[8] = 2 * x*z + 2 * w*y;
		m.myMatrix[9] = 2 * y*z - 2 * w*x;
		m.myMatrix[10] = 1 - 2 * x*x - 2 * y*y;

// 		m.myMatrix[12] = 1;
// 		m.myMatrix[13] = 1;
// 		m.myMatrix[14] = 1;

		return m;
	}

	template<class T>
	inline Quaternion<T> Matrix44<T>::CreateQuatFromMatrix(const Matrix44<T>& aMatrix)
	{
		Quaternion<T> q;
		Matrix44<T> m = aMatrix;

		float t, s;

		if ((m.myMatrix[0] + m.myMatrix[5] + m.myMatrix[10]) > 0.0f)
		{
			t = m.myMatrix[0] + m.myMatrix[5] + m.myMatrix[10] + 1.f;
			s = Corgi::ReciprocalSqrt(t) * 0.5f;
			q.w = s*t;
			q.z = (m.myMatrix[1] - m.myMatrix[4]) *s;
			q.y = (m.myMatrix[8] - m.myMatrix[2]) *s;
			q.x = (m.myMatrix[6] - m.myMatrix[9] * s);

		}
		else if ((m.myMatrix[0] > m.myMatrix[5]) && (m.myMatrix[0] > m.myMatrix[10]))
		{
			t = m.myMatrix[0] - m.myMatrix[5] - m.myMatrix[10] + 1.f;
			s = Corgi::ReciprocalSqrt(t) * 0.5f;

			q.x = s*t;
			q.y = (m.myMatrix[1] + m.myMatrix[4]) *s;
			q.z = (m.myMatrix[8] + m.myMatrix[2]) *s;
			q.w = (m.myMatrix[6] - m.myMatrix[9] * s);

		}
		else if (m.myMatrix[5] > m.myMatrix[10])
		{
			t = - m.myMatrix[0] + m.myMatrix[5] - m.myMatrix[10] + 1.f;
			s = Corgi::ReciprocalSqrt(t) * 0.5f;

			q.y = s*t;
			q.x = (m.myMatrix[1] + m.myMatrix[4]) *s;
			q.w = (m.myMatrix[8] - m.myMatrix[2]) *s;
			q.z = (m.myMatrix[6] + m.myMatrix[9] * s);
		}
		else
		{
			t = -m.myMatrix[0] - m.myMatrix[5] + m.myMatrix[10] + 1.f;
			s = Corgi::ReciprocalSqrt(t) * 0.5f;

			q.z = s*t;
			q.w = (m.myMatrix[1] - m.myMatrix[4]) *s;
			q.x = (m.myMatrix[8] + m.myMatrix[2]) *s;
			q.y = (m.myMatrix[6] + m.myMatrix[9] * s);
		}

		return q;
	}

	template<class T>
	inline Matrix44<T> Matrix44<T>::CreateFromVector4(const Corgi::Vector4<T>& v1, const Corgi::Vector4<T>& v2, const Corgi::Vector4<T>& v3, const Corgi::Vector4<T>& pos)
	{
		return Matrix44<T>(v1, v2, v3, pos);
	}

	template<class T>
	inline Matrix44<T> Matrix44<T>::CreateFromVector3(const Corgi::Vector3<T>& v1, const Corgi::Vector3<T>& v2, const Corgi::Vector3<T>& v3, const Corgi::Vector3<T>& pos)
	{
		return Matrix44<T>(v1, v2, v3, pos);
	}

	template<class T>
	inline Matrix44<T> Matrix44<T>::CreateRotateAroundX(T aAngleInRadians)
	{
		Matrix44<T> returnThis;

		returnThis.myMatrix[0] = 1;
		returnThis.myMatrix[1] = 0;
		returnThis.myMatrix[2] = 0;
				  
		returnThis.myMatrix[4] = 0;
		returnThis.myMatrix[5] = cos(aAngleInRadians);
		returnThis.myMatrix[6] = sin(aAngleInRadians);
	
		returnThis.myMatrix[8] = 0;
		returnThis.myMatrix[9] = -sin(aAngleInRadians);
		returnThis.myMatrix[10] = cos(aAngleInRadians);

		return returnThis;
	}

	template<class T>
	inline Matrix44<T> Matrix44<T>::CreateRotateAroundY(T aAngleInRadians)
	{
		Matrix44<T> returnThis;

		returnThis.myMatrix[0] = cos(aAngleInRadians);
		returnThis.myMatrix[1] = 0;
		returnThis.myMatrix[2] = -sin(aAngleInRadians);

		returnThis.myMatrix[4] = 0;
		returnThis.myMatrix[5] = 1;
		returnThis.myMatrix[6] = 0;

		returnThis.myMatrix[8] = sin(aAngleInRadians);
		returnThis.myMatrix[9] = 0;
		returnThis.myMatrix[10] = cos(aAngleInRadians);

		return returnThis;
	}

	template<class T>
	inline Matrix44<T> Matrix44<T>::CreateRotateAroundZ(T aAngleInRadians)
	{
		Matrix44<T> returnThis;

		returnThis.myMatrix[0] = cos(aAngleInRadians);
		returnThis.myMatrix[1] = sin(aAngleInRadians);
		returnThis.myMatrix[2] = 0;

		returnThis.myMatrix[4] = -sin(aAngleInRadians);
		returnThis.myMatrix[5] = cos(aAngleInRadians);
		returnThis.myMatrix[6] = 0;

		returnThis.myMatrix[8] = 0;
		returnThis.myMatrix[9] = 0;
		returnThis.myMatrix[10] = 1;

		return returnThis;
	}

	template<typename T>
	inline Matrix44<T> Matrix44<T>::CreateRotateAroundAxis(const Vector4<T>& aAxis, T aAngleInRadians)
	{
		T c = static_cast<T>(std::cos(static_cast<double>(aAngleInRadians)));
		T s = static_cast<T>(std::sin(static_cast<double>(aAngleInRadians)));
		Matrix44<T> returnMe;
		returnMe.myMatrix[0] = (aAxis.x*aAxis.x*(1 - c) + c);
		returnMe.myMatrix[1] = (aAxis.x*aAxis.y*(1 - c) + aAxis.z*s);
		returnMe.myMatrix[2] = (aAxis.x*aAxis.z*(1 - c) - aAxis.y*s);
		returnMe.myMatrix[3] = 0;
		returnMe.myMatrix[4] = (aAxis.x*aAxis.y*(1 - c) - aAxis.z*s);
		returnMe.myMatrix[5] = (aAxis.y*aAxis.y*(1 - c) + c);
		returnMe.myMatrix[6] = (aAxis.y*aAxis.z*(1 - c) + aAxis.x*s);
		returnMe.myMatrix[7] = 0;
		returnMe.myMatrix[8] = (aAxis.x*aAxis.z*(1 - c) + aAxis.y*s);
		returnMe.myMatrix[9] = (aAxis.y*aAxis.z*(1 - c) - aAxis.x*s);
		returnMe.myMatrix[10] = (aAxis.z*aAxis.z*(1 - c) + c);
		returnMe.myMatrix[11] = 0;
		returnMe.myMatrix[12] = 0;
		returnMe.myMatrix[13] = 0;
		returnMe.myMatrix[14] = 0;
		returnMe.myMatrix[15] = 1;
		return returnMe;
	}
	
	template<class T>
	inline Matrix44<T> Matrix44<T>::GetFastInverse() const
	{
		Matrix44<T> returnThis;

		returnThis.myMatrix[0] = myMatrix[0];
		returnThis.myMatrix[1] = myMatrix[4];
		returnThis.myMatrix[2] = myMatrix[8];
		returnThis.myMatrix[3] = 0;

		returnThis.myMatrix[4] = myMatrix[1];
		returnThis.myMatrix[5] = myMatrix[5];
		returnThis.myMatrix[6] = myMatrix[9];
		returnThis.myMatrix[7] = 0;

		returnThis.myMatrix[8] = myMatrix[2];
		returnThis.myMatrix[9] = myMatrix[6];
		returnThis.myMatrix[10] = myMatrix[10];
		returnThis.myMatrix[11] = 0;

		returnThis.myMatrix[12] = -(myMatrix[12] * returnThis.myMatrix[0]) - (myMatrix[13] * returnThis.myMatrix[4]) - (myMatrix[14] * returnThis.myMatrix[8]);
		returnThis.myMatrix[13] = -(myMatrix[12] * returnThis.myMatrix[1]) - (myMatrix[13] * returnThis.myMatrix[5]) - (myMatrix[14] * returnThis.myMatrix[9]);
		returnThis.myMatrix[14] = -(myMatrix[12] * returnThis.myMatrix[2]) - (myMatrix[13] * returnThis.myMatrix[6]) - (myMatrix[14] * returnThis.myMatrix[10]);
		returnThis.myMatrix[15] = 1;

		return returnThis;
	}

	template<class T>
	inline Matrix44<T> Matrix44<T>::Transpose(const Matrix44<T>& aMatrixToTranspose)
	{
		Matrix44<T> returnThis;

		returnThis.myMatrix[0] = aMatrixToTranspose.myMatrix[0];
		returnThis.myMatrix[5] = aMatrixToTranspose.myMatrix[5];
		returnThis.myMatrix[10] = aMatrixToTranspose.myMatrix[10];
		returnThis.myMatrix[15] = aMatrixToTranspose.myMatrix[15];


		returnThis.myMatrix[1] = aMatrixToTranspose.myMatrix[4];
		returnThis.myMatrix[2] = aMatrixToTranspose.myMatrix[8];
		returnThis.myMatrix[3] = aMatrixToTranspose.myMatrix[12];

		returnThis.myMatrix[4] = aMatrixToTranspose.myMatrix[1];
		returnThis.myMatrix[6] = aMatrixToTranspose.myMatrix[9];
		returnThis.myMatrix[7] = aMatrixToTranspose.myMatrix[13];

		returnThis.myMatrix[8] = aMatrixToTranspose.myMatrix[2];
		returnThis.myMatrix[9] = aMatrixToTranspose.myMatrix[6];
		returnThis.myMatrix[11] = aMatrixToTranspose.myMatrix[14];

		returnThis.myMatrix[12] = aMatrixToTranspose.myMatrix[3];
		returnThis.myMatrix[13] = aMatrixToTranspose.myMatrix[7];
		returnThis.myMatrix[14] = aMatrixToTranspose.myMatrix[11];

		return returnThis;
	}

	template<class T>
	inline Matrix44<T> Matrix44<T>::TransposeOpenGL(const Matrix44<T>& aMatrixToTranspose)
	{
		Matrix44<T> returnThis;

		returnThis.myMatrix[0] = aMatrixToTranspose.myMatrix[0];
		returnThis.myMatrix[5] = aMatrixToTranspose.myMatrix[5];
		returnThis.myMatrix[10] = aMatrixToTranspose.myMatrix[10];
		returnThis.myMatrix[15] = aMatrixToTranspose.myMatrix[15];


		returnThis.myMatrix[1] = aMatrixToTranspose.myMatrix[4];
		returnThis.myMatrix[2] = aMatrixToTranspose.myMatrix[8];
		returnThis.myMatrix[3] = aMatrixToTranspose.myMatrix[3];

		returnThis.myMatrix[4] = aMatrixToTranspose.myMatrix[1];
		returnThis.myMatrix[6] = aMatrixToTranspose.myMatrix[9];
		returnThis.myMatrix[7] = aMatrixToTranspose.myMatrix[7];

		returnThis.myMatrix[8] = aMatrixToTranspose.myMatrix[2];
		returnThis.myMatrix[9] = aMatrixToTranspose.myMatrix[6];
		returnThis.myMatrix[11] = aMatrixToTranspose.myMatrix[11];

		returnThis.myMatrix[12] = aMatrixToTranspose.myMatrix[12];
		returnThis.myMatrix[13] = aMatrixToTranspose.myMatrix[13];
		returnThis.myMatrix[14] = aMatrixToTranspose.myMatrix[14];


		return returnThis;
	}

	template<class T>
	inline void Matrix44<T>::TransposeThis()
	{

		T temporaryValue1 = myMatrix[1];
		T temporaryValue2 = myMatrix[2];
		T temporaryValue3 = myMatrix[3];
		T temporaryValue6 = myMatrix[6];
		T temporaryValue7 = myMatrix[7];
		T temporaryValue11 = myMatrix[11];

		myMatrix[1] = myMatrix[4];
		myMatrix[4] = temporaryValue1;

		myMatrix[2] = myMatrix[8];
		myMatrix[8] = temporaryValue2;

		myMatrix[3] = myMatrix[12];
		myMatrix[12] = temporaryValue3;

		myMatrix[6] = myMatrix[9];
		myMatrix[9] = temporaryValue6;

		myMatrix[7] = myMatrix[13];
		myMatrix[13] = temporaryValue7;

		myMatrix[11] = myMatrix[14];
		myMatrix[14] = temporaryValue11;
	}

	template<class T>
	inline Corgi::Vector3<T> Matrix44<T>::GetPosition() const
	{
		Corgi::Vector3<T> v;
		v.x = myMatrix[12];
		v.y = myMatrix[13];
		v.z = myMatrix[14];
		return v;
	}
	template<class T>
	inline Corgi::Vector3<T> Matrix44<T>::GetRotation() const
	{

		Corgi::Vector3<T> xV(myMatrix[0], myMatrix[1], myMatrix[2]);
		Corgi::Vector3<T> yV(myMatrix[4], myMatrix[5], myMatrix[6]);
		Corgi::Vector3<T> zV(myMatrix[8], myMatrix[9], myMatrix[10]);

		Corgi::Vector3<T> v;
		v.x = acos(xV.GetNormalized().Dot({ 1, 0, 0 }));
		v.y = acos(yV.GetNormalized().Dot({ 0, 1, 0 }));
		v.z = acos(zV.GetNormalized().Dot({ 0, 0, 1 }));
		return v;
	}

	template<class T>
	inline Matrix44<T> Matrix44<T>::RegularMult(const Matrix44 & aMatrix0, const Matrix44 & aMatrix1)
	{
		Matrix44 returnThis;

		returnThis.myMatrix[0] = aMatrix0.myMatrix[0] * aMatrix1.myMatrix[0] + aMatrix0.myMatrix[1] * aMatrix1.myMatrix[4] + aMatrix0.myMatrix[2] * aMatrix1.myMatrix[8] + aMatrix0.myMatrix[3] * aMatrix1.myMatrix[12];
		returnThis.myMatrix[1] = aMatrix0.myMatrix[0] * aMatrix1.myMatrix[1] + aMatrix0.myMatrix[1] * aMatrix1.myMatrix[5] + aMatrix0.myMatrix[2] * aMatrix1.myMatrix[9] + aMatrix0.myMatrix[3] * aMatrix1.myMatrix[13];
		returnThis.myMatrix[2] = aMatrix0.myMatrix[0] * aMatrix1.myMatrix[2] + aMatrix0.myMatrix[1] * aMatrix1.myMatrix[6] + aMatrix0.myMatrix[2] * aMatrix1.myMatrix[10] + aMatrix0.myMatrix[3] * aMatrix1.myMatrix[14];
		returnThis.myMatrix[3] = aMatrix0.myMatrix[0] * aMatrix1.myMatrix[3] + aMatrix0.myMatrix[1] * aMatrix1.myMatrix[7] + aMatrix0.myMatrix[2] * aMatrix1.myMatrix[11] + aMatrix0.myMatrix[3] * aMatrix1.myMatrix[15];

		returnThis.myMatrix[4] = aMatrix0.myMatrix[4] * aMatrix1.myMatrix[0] + aMatrix0.myMatrix[5] * aMatrix1.myMatrix[4] + aMatrix0.myMatrix[6] * aMatrix1.myMatrix[8] + aMatrix0.myMatrix[7] * aMatrix1.myMatrix[12];
		returnThis.myMatrix[5] = aMatrix0.myMatrix[4] * aMatrix1.myMatrix[1] + aMatrix0.myMatrix[5] * aMatrix1.myMatrix[5] + aMatrix0.myMatrix[6] * aMatrix1.myMatrix[9] + aMatrix0.myMatrix[7] * aMatrix1.myMatrix[13];
		returnThis.myMatrix[6] = aMatrix0.myMatrix[4] * aMatrix1.myMatrix[2] + aMatrix0.myMatrix[5] * aMatrix1.myMatrix[6] + aMatrix0.myMatrix[6] * aMatrix1.myMatrix[10] + aMatrix0.myMatrix[7] * aMatrix1.myMatrix[14];
		returnThis.myMatrix[7] = aMatrix0.myMatrix[4] * aMatrix1.myMatrix[3] + aMatrix0.myMatrix[5] * aMatrix1.myMatrix[7] + aMatrix0.myMatrix[6] * aMatrix1.myMatrix[11] + aMatrix0.myMatrix[7] * aMatrix1.myMatrix[15];

		returnThis.myMatrix[8] = aMatrix0.myMatrix[8] * aMatrix1.myMatrix[0] + aMatrix0.myMatrix[9] * aMatrix1.myMatrix[4] + aMatrix0.myMatrix[10] * aMatrix1.myMatrix[8] + aMatrix0.myMatrix[11] * aMatrix1.myMatrix[12];
		returnThis.myMatrix[9] = aMatrix0.myMatrix[8] * aMatrix1.myMatrix[1] + aMatrix0.myMatrix[9] * aMatrix1.myMatrix[5] + aMatrix0.myMatrix[10] * aMatrix1.myMatrix[9] + aMatrix0.myMatrix[11] * aMatrix1.myMatrix[13];
		returnThis.myMatrix[10] = aMatrix0.myMatrix[8] * aMatrix1.myMatrix[2] + aMatrix0.myMatrix[9] * aMatrix1.myMatrix[6] + aMatrix0.myMatrix[10] * aMatrix1.myMatrix[10] + aMatrix0.myMatrix[11] * aMatrix1.myMatrix[14];
		returnThis.myMatrix[11] = aMatrix0.myMatrix[8] * aMatrix1.myMatrix[3] + aMatrix0.myMatrix[9] * aMatrix1.myMatrix[7] + aMatrix0.myMatrix[10] * aMatrix1.myMatrix[11] + aMatrix0.myMatrix[11] * aMatrix1.myMatrix[15];

		returnThis.myMatrix[12] = aMatrix0.myMatrix[12] * aMatrix1.myMatrix[0] + aMatrix0.myMatrix[13] * aMatrix1.myMatrix[4] + aMatrix0.myMatrix[14] * aMatrix1.myMatrix[8] + aMatrix0.myMatrix[15] * aMatrix1.myMatrix[12];
		returnThis.myMatrix[13] = aMatrix0.myMatrix[12] * aMatrix1.myMatrix[1] + aMatrix0.myMatrix[13] * aMatrix1.myMatrix[5] + aMatrix0.myMatrix[14] * aMatrix1.myMatrix[9] + aMatrix0.myMatrix[15] * aMatrix1.myMatrix[13];
		returnThis.myMatrix[14] = aMatrix0.myMatrix[12] * aMatrix1.myMatrix[2] + aMatrix0.myMatrix[13] * aMatrix1.myMatrix[6] + aMatrix0.myMatrix[14] * aMatrix1.myMatrix[10] + aMatrix0.myMatrix[15] * aMatrix1.myMatrix[14];
		returnThis.myMatrix[15] = aMatrix0.myMatrix[12] * aMatrix1.myMatrix[3] + aMatrix0.myMatrix[13] * aMatrix1.myMatrix[7] + aMatrix0.myMatrix[14] * aMatrix1.myMatrix[11] + aMatrix0.myMatrix[15] * aMatrix1.myMatrix[15];
		
		return returnThis;
	}

	template<class T>
	inline void Matrix44<T>::SIMDMult(Matrix44 & aMatrix0, const Matrix44 & aMatrix1)
	{
		const __m128 a = aMatrix1.m1;
		const __m128 b = aMatrix1.m2;
		const __m128 c = aMatrix1.m3;
		const __m128 d = aMatrix1.m4;

		__m128 t1, t2;

		t1 = _mm_set1_ps(aMatrix0.myMatrix[0]);
		t2 = _mm_mul_ps(a, t1);
		t1 = _mm_set1_ps(aMatrix0.myMatrix[1]);
		t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
		t1 = _mm_set1_ps(aMatrix0.myMatrix[2]);
		t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
		t1 = _mm_set1_ps(aMatrix0.myMatrix[3]);
		t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

		_mm_store_ps(&aMatrix0.myMatrix[0], t2);



		t1 = _mm_set1_ps(aMatrix0.myMatrix[4]);
		t2 = _mm_mul_ps(a, t1);
		t1 = _mm_set1_ps(aMatrix0.myMatrix[5]);
		t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
		t1 = _mm_set1_ps(aMatrix0.myMatrix[6]);
		t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
		t1 = _mm_set1_ps(aMatrix0.myMatrix[7]);
		t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

		_mm_store_ps(&aMatrix0.myMatrix[4], t2);



		t1 = _mm_set1_ps(aMatrix0.myMatrix[8]);
		t2 = _mm_mul_ps(a, t1);
		t1 = _mm_set1_ps(aMatrix0.myMatrix[9]);
		t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
		t1 = _mm_set1_ps(aMatrix0.myMatrix[10]);
		t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
		t1 = _mm_set1_ps(aMatrix0.myMatrix[11]);
		t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

		_mm_store_ps(&aMatrix0.myMatrix[8], t2);

		t1 = _mm_set1_ps(aMatrix0.myMatrix[12]);
		t2 = _mm_mul_ps(a, t1);
		t1 = _mm_set1_ps(aMatrix0.myMatrix[13]);
		t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
		t1 = _mm_set1_ps(aMatrix0.myMatrix[14]);
		t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
		t1 = _mm_set1_ps(aMatrix0.myMatrix[15]);
		t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);

		_mm_store_ps(&aMatrix0.myMatrix[12], t2);
	}

	template<class T>
	inline Matrix44<T> Matrix44<T>::CreateOrthographicMatrix(T aRight, T aLeft, T aNear, T aFar, T aTop, T aBottom)
	{
		Matrix44<T> returnMat;

		returnMat.myMatrix[0] = 2 / (aRight / aLeft);
		returnMat.myMatrix[1] = 0;
		returnMat.myMatrix[2] = 0;
		returnMat.myMatrix[3] = -((aRight + aLeft) / (aRight - aLeft));
		returnMat.myMatrix[4] = 0;
		returnMat.myMatrix[5] = 2 / (aTop - aBottom);
		returnMat.myMatrix[6] = 0;
		returnMat.myMatrix[7] = -((aTop + aBottom) / (aTop - aBottom));
		returnMat.myMatrix[8] = 0;
		returnMat.myMatrix[9] = 0;
		returnMat.myMatrix[10] = (-2) / (aFar - aNear);
		returnMat.myMatrix[11] = -((aFar + aNear) / (aFar - aNear));
		returnMat.myMatrix[12] = 0;
		returnMat.myMatrix[13] = 0;
		returnMat.myMatrix[14] = 0;
		returnMat.myMatrix[15] = 1;

		return returnMat;
	}

	template<class T>
	inline Matrix44<T> Corgi::Matrix44<T>::LookAt(const Vector3<T>& EyePos, const Vector3<T>& aPosToLookAt, const Vector3<T>& aUp)
	{
		Vector3f zAxis = (aPosToLookAt - EyePos).GetNormalized();
		Vector3f xAxis = (aUp.Cross(zAxis)).GetNormalized();
		Vector3f yAxis = zAxis.Cross(xAxis);


		Matrix44 retMat;
		retMat.myMatrix[0] = xAxis.x;
		retMat.myMatrix[1] = yAxis.x;
		retMat.myMatrix[2] = zAxis.x;
		retMat.myMatrix[3] = 0;
		retMat.myMatrix[4] = xAxis.y;
		retMat.myMatrix[5] = yAxis.y;
		retMat.myMatrix[6] = zAxis.y;
		retMat.myMatrix[7] = 0;
		retMat.myMatrix[8] = xAxis.z;
		retMat.myMatrix[9] = yAxis.z;
		retMat.myMatrix[10] = zAxis.z;
		retMat.myMatrix[11] = 0;
		retMat.myMatrix[12] = xAxis.Dot({ -EyePos.x, -EyePos.y, -EyePos.z });
		retMat.myMatrix[13] = yAxis.Dot({ -EyePos.x, -EyePos.y, -EyePos.z });
		retMat.myMatrix[14] = zAxis.Dot({ -EyePos.x, -EyePos.y, -EyePos.z });
		retMat.myMatrix[15] = 1;

		return retMat;
	}
}

#pragma warning( pop ) 
typedef Corgi::Matrix44<float> CUMatrix44;
typedef Corgi::Matrix44<float> Matrix44;