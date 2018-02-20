#pragma once
#include <cmath>
#include "Vector3.h"
#include "Matrix44.h"

namespace Corgi

{
	template <class T>
	class Matrix33
	{
	public:

		Matrix33<T>();			//Initiera till identitets-matrisen
		Matrix33<T>(const Matrix33<T>& aMatrix);	//Kopiera matrisen
		Matrix33<T>(const Matrix44<T>& aMatrix);	//Kopiera övre, vänstra 3x3-delen av 4x4-matrisen

		Matrix33<T>& operator=(const Matrix33<T>& aMatrix);

		static Matrix33<T> CreateRotateAroundX(T aAngleInRadians);
		static Matrix33<T> CreateRotateAroundY(T aAngleInRadians);
		static Matrix33<T> CreateRotateAroundZ(T aAngleInRadians);

		static Matrix33<T> Transpose(const Matrix33<T>& aMatrixToTranspose);
		static void TransposeThis();

		T myMatrix[9];

	private:

		
	};


	template <class T>
	Matrix33<T> operator+(const Matrix33<T>& aMatrix0, const Matrix33<T>& aMatrix1)
	{
		//Jag vill förtydliga att vi STARKT övervägde att döpa argumenten till theMatrix1 och theMatrix2:Reloaded
		//men antog att det också inte skulle vara okej med kodstandarden.
		//TL:DR Fixa er kodstandard för att möjliggöra matrix jokes.

		Matrix33<T> returnThis;
		returnThis.myMatrix[0] = aMatrix0.myMatrix[0] + aMatrix1.myMatrix[0];
		returnThis.myMatrix[1] = aMatrix0.myMatrix[1] + aMatrix1.myMatrix[1];
		returnThis.myMatrix[2] = aMatrix0.myMatrix[2] + aMatrix1.myMatrix[2];
		returnThis.myMatrix[3] = aMatrix0.myMatrix[3] + aMatrix1.myMatrix[3];
		returnThis.myMatrix[4] = aMatrix0.myMatrix[4] + aMatrix1.myMatrix[4];
		returnThis.myMatrix[5] = aMatrix0.myMatrix[5] + aMatrix1.myMatrix[5];
		returnThis.myMatrix[6] = aMatrix0.myMatrix[6] + aMatrix1.myMatrix[6];
		returnThis.myMatrix[7] = aMatrix0.myMatrix[7] + aMatrix1.myMatrix[7];
		returnThis.myMatrix[8] = aMatrix0.myMatrix[8] + aMatrix1.myMatrix[8];

		return returnThis;
	}

	template <class T>
	Matrix33<T> operator-(const Matrix33<T>& aMatrix0, const Matrix33<T>& aMatrix1)
	{
		Matrix33<T> returnThis;
		returnThis.myMatrix[0] = aMatrix0.myMatrix[0] - aMatrix1.myMatrix[0];
		returnThis.myMatrix[1] = aMatrix0.myMatrix[1] - aMatrix1.myMatrix[1];
		returnThis.myMatrix[2] = aMatrix0.myMatrix[2] - aMatrix1.myMatrix[2];
		returnThis.myMatrix[3] = aMatrix0.myMatrix[3] - aMatrix1.myMatrix[3];
		returnThis.myMatrix[4] = aMatrix0.myMatrix[4] - aMatrix1.myMatrix[4];
		returnThis.myMatrix[5] = aMatrix0.myMatrix[5] - aMatrix1.myMatrix[5];
		returnThis.myMatrix[6] = aMatrix0.myMatrix[6] - aMatrix1.myMatrix[6];
		returnThis.myMatrix[7] = aMatrix0.myMatrix[7] - aMatrix1.myMatrix[7];
		returnThis.myMatrix[8] = aMatrix0.myMatrix[8] - aMatrix1.myMatrix[8];

		return returnThis;
	}

	template <class T>
	Matrix33<T> operator*(const Matrix33<T>& aMatrix0, const Matrix33<T>& aMatrix1)
	{
		Matrix33<T> returnThis;

		returnThis.myMatrix[0] = aMatrix0.myMatrix[0] * aMatrix1.myMatrix[0] + aMatrix0.myMatrix[1] * aMatrix1.myMatrix[3] + aMatrix0.myMatrix[2] * aMatrix1.myMatrix[6];
		returnThis.myMatrix[1] = aMatrix0.myMatrix[0] * aMatrix1.myMatrix[1] + aMatrix0.myMatrix[1] * aMatrix1.myMatrix[4] + aMatrix0.myMatrix[2] * aMatrix1.myMatrix[7];
		returnThis.myMatrix[2] = aMatrix0.myMatrix[0] * aMatrix1.myMatrix[2] + aMatrix0.myMatrix[1] * aMatrix1.myMatrix[5] + aMatrix0.myMatrix[2] * aMatrix1.myMatrix[8];

		returnThis.myMatrix[3] = aMatrix0.myMatrix[3] * aMatrix1.myMatrix[0] + aMatrix0.myMatrix[4] * aMatrix1.myMatrix[3] + aMatrix0.myMatrix[5] * aMatrix1.myMatrix[6];
		returnThis.myMatrix[4] = aMatrix0.myMatrix[3] * aMatrix1.myMatrix[1] + aMatrix0.myMatrix[4] * aMatrix1.myMatrix[4] + aMatrix0.myMatrix[5] * aMatrix1.myMatrix[7];
		returnThis.myMatrix[5] = aMatrix0.myMatrix[3] * aMatrix1.myMatrix[2] + aMatrix0.myMatrix[4] * aMatrix1.myMatrix[5] + aMatrix0.myMatrix[5] * aMatrix1.myMatrix[8];

		returnThis.myMatrix[6] = aMatrix0.myMatrix[6] * aMatrix1.myMatrix[0] + aMatrix0.myMatrix[7] * aMatrix1.myMatrix[3] + aMatrix0.myMatrix[8] * aMatrix1.myMatrix[6];
		returnThis.myMatrix[7] = aMatrix0.myMatrix[6] * aMatrix1.myMatrix[1] + aMatrix0.myMatrix[7] * aMatrix1.myMatrix[4] + aMatrix0.myMatrix[8] * aMatrix1.myMatrix[7];
		returnThis.myMatrix[8] = aMatrix0.myMatrix[6] * aMatrix1.myMatrix[2] + aMatrix0.myMatrix[7] * aMatrix1.myMatrix[5] + aMatrix0.myMatrix[8] * aMatrix1.myMatrix[8];

		return returnThis;
	}

	template <class T>
	void operator+=(Matrix33<T>& aMatrix0, const Matrix33<T>& aMatrix1)
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
	}

	template <class T>
	void operator-=(Matrix33<T>& aMatrix0, const Matrix33<T>& aMatrix1)
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
	}

	template <class T>
	void operator*=(Matrix33<T>& aMatrix0, const Matrix33<T>& aMatrix1)
	{
		//Vi är väl medvetna om att Neo är ett dåligt namn, men vi saknar helt
		//förmågan att avgöra när något är smart eller dumt.
		//Du kan föreställa dig att den heter "temporaryMatrix".. eller Morpheus

		Matrix33<T> neo;
		neo.myMatrix[0] = aMatrix0.myMatrix[0] * aMatrix1.myMatrix[0] + aMatrix0.myMatrix[1] * aMatrix1.myMatrix[3] + aMatrix0.myMatrix[2] * aMatrix1.myMatrix[6];
		neo.myMatrix[1] = aMatrix0.myMatrix[0] * aMatrix1.myMatrix[1] + aMatrix0.myMatrix[1] * aMatrix1.myMatrix[4] + aMatrix0.myMatrix[2] * aMatrix1.myMatrix[7];
		neo.myMatrix[2] = aMatrix0.myMatrix[0] * aMatrix1.myMatrix[2] + aMatrix0.myMatrix[1] * aMatrix1.myMatrix[5] + aMatrix0.myMatrix[2] * aMatrix1.myMatrix[8];

		neo.myMatrix[3] = aMatrix0.myMatrix[3] * aMatrix1.myMatrix[0] + aMatrix0.myMatrix[4] * aMatrix1.myMatrix[3] + aMatrix0.myMatrix[5] * aMatrix1.myMatrix[6];
		neo.myMatrix[4] = aMatrix0.myMatrix[3] * aMatrix1.myMatrix[1] + aMatrix0.myMatrix[4] * aMatrix1.myMatrix[4] + aMatrix0.myMatrix[5] * aMatrix1.myMatrix[7];
		neo.myMatrix[5] = aMatrix0.myMatrix[3] * aMatrix1.myMatrix[2] + aMatrix0.myMatrix[4] * aMatrix1.myMatrix[5] + aMatrix0.myMatrix[5] * aMatrix1.myMatrix[8];

		neo.myMatrix[6] = aMatrix0.myMatrix[6] * aMatrix1.myMatrix[0] + aMatrix0.myMatrix[7] * aMatrix1.myMatrix[3] + aMatrix0.myMatrix[8] * aMatrix1.myMatrix[6];
		neo.myMatrix[7] = aMatrix0.myMatrix[6] * aMatrix1.myMatrix[1] + aMatrix0.myMatrix[7] * aMatrix1.myMatrix[4] + aMatrix0.myMatrix[8] * aMatrix1.myMatrix[7];
		neo.myMatrix[8] = aMatrix0.myMatrix[6] * aMatrix1.myMatrix[2] + aMatrix0.myMatrix[7] * aMatrix1.myMatrix[5] + aMatrix0.myMatrix[8] * aMatrix1.myMatrix[8];

		aMatrix0.myMatrix[0] = neo.myMatrix[0];
		aMatrix0.myMatrix[1] = neo.myMatrix[1];
		aMatrix0.myMatrix[2] = neo.myMatrix[2];
		aMatrix0.myMatrix[3] = neo.myMatrix[3];
		aMatrix0.myMatrix[4] = neo.myMatrix[4];
		aMatrix0.myMatrix[5] = neo.myMatrix[5];
		aMatrix0.myMatrix[6] = neo.myMatrix[6];
		aMatrix0.myMatrix[7] = neo.myMatrix[7];
		aMatrix0.myMatrix[8] = neo.myMatrix[8];

	}

	template <class T>
	Vector3<T> operator*(const Vector3<T>& aVector, const Matrix33<T>& aMatrix)
	{
		Vector3<T> returnThis;

		returnThis.x = aVector.x * aMatrix.myMatrix[0] + aVector.y * aMatrix.myMatrix[3] + aVector.z * aMatrix.myMatrix[6];
		returnThis.y = aVector.x * aMatrix.myMatrix[1] + aVector.y * aMatrix.myMatrix[4] + aVector.z * aMatrix.myMatrix[7];
		returnThis.z = aVector.x * aMatrix.myMatrix[2] + aVector.y * aMatrix.myMatrix[5] + aVector.z * aMatrix.myMatrix[8];

		return returnThis;
	}

	template <class T>
	bool operator==(const Matrix33<T>& aMatrix0, const Matrix33<T>& aMatrix1)
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
		
			
		return true;
	}

	template<class T>
	inline Matrix33<T>& Matrix33<T>::operator=(const Matrix33<T>& aMatrix)
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

		return *this;
	}

	template <class T>
	Matrix33<T>::Matrix33()
	{
		myMatrix[0] = 1;
		myMatrix[1] = 0;
		myMatrix[2] = 0;
		myMatrix[3] = 0;
		myMatrix[4] = 1;
		myMatrix[5] = 0;
		myMatrix[6] = 0;
		myMatrix[7] = 0;
		myMatrix[8] = 1;

	}

	template <class T>
	Matrix33<T>::Matrix33(const Matrix33<T>& aMatrix)
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

	}

	template <class T>
	Matrix33<T>::Matrix33(const Matrix44<T>& aMatrix)
	{
		myMatrix[0] = aMatrix.myMatrix[0];
		myMatrix[1] = aMatrix.myMatrix[1];
		myMatrix[2] = aMatrix.myMatrix[2];
		myMatrix[3] = aMatrix.myMatrix[4];
		myMatrix[4] = aMatrix.myMatrix[5];
		myMatrix[5] = aMatrix.myMatrix[6];
		myMatrix[6] = aMatrix.myMatrix[8];
		myMatrix[7] = aMatrix.myMatrix[9];
		myMatrix[8] = aMatrix.myMatrix[10];

	}


	template<class T>
	inline Matrix33<T> Matrix33<T>::CreateRotateAroundX(T aAngleInRadians)
	{
		Matrix33<T> returnThis;

		returnThis.myMatrix[0] = 1;
		returnThis.myMatrix[1] = 0;
		returnThis.myMatrix[2] = 0;

		returnThis.myMatrix[3] = 0;
		returnThis.myMatrix[4] = cos(aAngleInRadians);
		returnThis.myMatrix[5] = sin(aAngleInRadians);

		returnThis.myMatrix[6] = 0;
		returnThis.myMatrix[7] = -sin(aAngleInRadians);
		returnThis.myMatrix[8] = cos(aAngleInRadians);

		return returnThis;
	}

	template<class T>
	inline Matrix33<T> Matrix33<T>::CreateRotateAroundY(T aAngleInRadians)
	{
		Matrix33<T> returnThis;

		returnThis.myMatrix[0] = cos(aAngleInRadians);
		returnThis.myMatrix[1] = 0;
		returnThis.myMatrix[2] = -sin(aAngleInRadians);

		returnThis.myMatrix[3] = 0;
		returnThis.myMatrix[4] = 1;
		returnThis.myMatrix[5] = 0;

		returnThis.myMatrix[6] = sin(aAngleInRadians);
		returnThis.myMatrix[7] = 0;
		returnThis.myMatrix[8] = cos(aAngleInRadians);

		return returnThis;
	}

	template<class T>
	inline Matrix33<T> Matrix33<T>::CreateRotateAroundZ(T aAngleInRadians)
	{
		Matrix33<T> returnThis;

		returnThis.myMatrix[0] = cos(aAngleInRadians);
		returnThis.myMatrix[1] = sin(aAngleInRadians);
		returnThis.myMatrix[2] = 0;

		returnThis.myMatrix[3] = -sin(aAngleInRadians);
		returnThis.myMatrix[4] = cos(aAngleInRadians);
		returnThis.myMatrix[5] = 0;

		returnThis.myMatrix[6] = 0;
		returnThis.myMatrix[7] = 0;
		returnThis.myMatrix[8] = 1;

		return returnThis;
	}

	template<class T>
	inline Matrix33<T> Matrix33<T>::Transpose(const Matrix33<T>& aMatrixToTranspose)
	{
		Matrix33<T> returnThis;

		returnThis.myMatrix[0] = aMatrixToTranspose.myMatrix[0];
		returnThis.myMatrix[1] = aMatrixToTranspose.myMatrix[3];
		returnThis.myMatrix[2] = aMatrixToTranspose.myMatrix[6];
		returnThis.myMatrix[3] = aMatrixToTranspose.myMatrix[1];
		returnThis.myMatrix[4] = aMatrixToTranspose.myMatrix[4];
		returnThis.myMatrix[5] = aMatrixToTranspose.myMatrix[7];
		returnThis.myMatrix[6] = aMatrixToTranspose.myMatrix[2];
		returnThis.myMatrix[7] = aMatrixToTranspose.myMatrix[5];
		returnThis.myMatrix[8] = aMatrixToTranspose.myMatrix[8];
		
		return returnThis;
	}

	template<class T>
	inline void Matrix33<T>::TransposeThis()
	{
		

		T temporaryValue1 = myMatrix[1];
		T temporaryValue2 = myMatrix[2];
		T temporaryValue5 = myMatrix[5];


		myMatrix[1] = myMatrix[3];
		myMatrix[3] = temporaryValue1;

		myMatrix[2] = myMatrix[6];
		myMatrix[6] = temporaryValue2;

		myMatrix[5] = myMatrix[7];
		myMatrix[7] = temporaryValue5;

	}

}
