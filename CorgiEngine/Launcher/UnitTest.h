#pragma once
#include "..\Utilities\Matrix.h"
#include "..\Utilities\DebugLog.h"
#include <vector>

void IsSIMDEqualToNormalMatrix();

void TestSIMD()
{
	IsSIMDEqualToNormalMatrix();
	//TimeMatrices();
}

void IsSIMDEqualToNormalMatrix()
{
	DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::Resource);
	Matrix44 a, b, reg, simd;

	a.myMatrix[0] = 93;
	a.myMatrix[1] = 93;
	a.myMatrix[2] = 93;
	a.myMatrix[3] = 93;
	a.myMatrix[4] = 93;
	a.myMatrix[5] = 93;
	a.myMatrix[6] = 93;
	a.myMatrix[7] = 93;
	a.myMatrix[8] = 93;
	a.myMatrix[9] = 93;
	a.myMatrix[10] = 93;
	a.myMatrix[11] = 93;
	a.myMatrix[12] = 93;
	a.myMatrix[13] = 93;
	a.myMatrix[14] = 93;
	a.myMatrix[15] = 93;

	b.myMatrix[0] = 113;
	b.myMatrix[1] = 113;
	b.myMatrix[2] = 113;
	b.myMatrix[3] = 113;
	b.myMatrix[4] = 113;
	b.myMatrix[5] = 113;
	b.myMatrix[6] = 113;
	b.myMatrix[7] = 113;
	b.myMatrix[8] = 113;
	b.myMatrix[9] = 113;
	b.myMatrix[10] = 113;
	b.myMatrix[11] = 113;
	b.myMatrix[12] = 113;
	b.myMatrix[13] = 113;
	b.myMatrix[14] = 113;
	b.myMatrix[15] = 113;

	//Normal

	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Resource, "%s", "TESTING IF MATRIX MULITPLICATION IS EQUAL");



	bool GOODBOYE = true;
	for (int i = 0; i < 100000000; ++i)
	{
		a.myMatrix[i % 15] += 1;
		b.myMatrix[(i + i) % 15] += 1;

		reg = Matrix44::RegularMult(a, b);
		simd = a;
		Matrix44::SIMDMult(simd, b);

		if (reg == simd)
		{

		}
		else
		{
			GOODBOYE = false;
		}

	}
	if (GOODBOYE)
	{

		DL_SUCCESS(eDebugLogType::Resource, "%s", "DONE! NO ERRORS");
	}
	else
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "ERROR IN SIMD MULTIPLICATIONS");
	}

}

void TimeMatrices()
{
	DebugLog::GetInstance().ActivateFilterlog(eDebugLogType::Timer);

	{
		Matrix44 a, b, c;

		a.myMatrix[0] = 93;
		a.myMatrix[1] = 93;
		a.myMatrix[2] = 93;
		a.myMatrix[3] = 93;
		a.myMatrix[4] = 93;
		a.myMatrix[5] = 93;
		a.myMatrix[6] = 93;
		a.myMatrix[7] = 93;
		a.myMatrix[8] = 93;
		a.myMatrix[9] = 93;
		a.myMatrix[10] = 93;
		a.myMatrix[11] = 93;
		a.myMatrix[12] = 93;
		a.myMatrix[13] = 93;
		a.myMatrix[14] = 93;
		a.myMatrix[15] = 93;

		b.myMatrix[0] = 113;
		b.myMatrix[1] = 113;
		b.myMatrix[2] = 113;
		b.myMatrix[3] = 113;
		b.myMatrix[4] = 113;
		b.myMatrix[5] = 113;
		b.myMatrix[6] = 113;
		b.myMatrix[7] = 113;
		b.myMatrix[8] = 113;
		b.myMatrix[9] = 113;
		b.myMatrix[10] = 113;
		b.myMatrix[11] = 113;
		b.myMatrix[12] = 113;
		b.myMatrix[13] = 113;
		b.myMatrix[14] = 113;
		b.myMatrix[15] = 113;

		//Normal

		DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Timer, "%s", "TIMER STARTED FOR NORMAL MATRIX MULT");

		std::vector<Matrix44> v;

		std::chrono::duration<float> duration;
		for (int i = 0; i < 100000000; ++i)
		{
			a.myMatrix[i % 15] += 1;
			b.myMatrix[(i + i) % 15] += 1;

			std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
			c = Matrix44::RegularMult(a, b);
			std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

			duration += endTime - startTime;

			v.push_back(c);

		}

		float durationf = duration.count();
		DL_SUCCESS(eDebugLogType::Timer, "%s%f", "NORMAL MATRIX MULT DONE IN:    ", durationf);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//SIMD

	{
		Matrix44 a, b, c;

		a.myMatrix[0] = 93;
		a.myMatrix[1] = 93;
		a.myMatrix[2] = 93;
		a.myMatrix[3] = 93;
		a.myMatrix[4] = 93;
		a.myMatrix[5] = 93;
		a.myMatrix[6] = 93;
		a.myMatrix[7] = 93;
		a.myMatrix[8] = 93;
		a.myMatrix[9] = 93;
		a.myMatrix[10] = 93;
		a.myMatrix[11] = 93;
		a.myMatrix[12] = 93;
		a.myMatrix[13] = 93;
		a.myMatrix[14] = 93;
		a.myMatrix[15] = 93;

		b.myMatrix[0] = 113;
		b.myMatrix[1] = 113;
		b.myMatrix[2] = 113;
		b.myMatrix[3] = 113;
		b.myMatrix[4] = 113;
		b.myMatrix[5] = 113;
		b.myMatrix[6] = 113;
		b.myMatrix[7] = 113;
		b.myMatrix[8] = 113;
		b.myMatrix[9] = 113;
		b.myMatrix[10] = 113;
		b.myMatrix[11] = 113;
		b.myMatrix[12] = 113;
		b.myMatrix[13] = 113;
		b.myMatrix[14] = 113;
		b.myMatrix[15] = 113;

		//Normal

		DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Timer, "%s", "TIMER STARTED FOR SIMD MATRIX MULT");

		std::vector<Matrix44> v;

		std::chrono::duration<float> duration;
		for (int i = 0; i < 100000000; ++i)
		{
			a.myMatrix[i % 15] += 1;
			b.myMatrix[(i + i) % 15] += 1;

			c = a;
			std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
			Matrix44::SIMDMult(c, b);
			std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

			duration += endTime - startTime;

			v.push_back(c);

		}

		float durationf = duration.count();
		DL_SUCCESS(eDebugLogType::Timer, "%s%f", "SIMD MATRIX MULT DONE IN:    ", durationf);
	}



}