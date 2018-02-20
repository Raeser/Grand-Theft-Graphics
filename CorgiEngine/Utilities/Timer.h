#pragma once
#include <chrono>
#include <numeric>




class Timer
{
public:
#ifndef _RETAIL
#define TIMER_TIMEFUNCTION(aFunctionName, Function) {std::chrono::steady_clock::time_point ID;\
	ID = std::chrono::steady_clock::now();\
	Function;\
	std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();\
	typedef std::chrono::milliseconds milliseconds;\
	milliseconds ms = std::chrono::duration_cast<milliseconds>(endTime - ID);\
	DL_WRITELOG(eConsoleColor::WHITE, eDebugLogType::Timer, "%s%s%s%i%s", "[", #aFunctionName, "] - function took " , ms.count() ,  " miliseconds to run");}
#else
#define TIMER_TIMEFUNCTION(aFunctionName, Function) { Function; }
#endif

public:
	static Timer& GetInstance()
	{
		static Timer timer;

		return timer;
	}
	~Timer() {}

	Timer(const Timer& aTimer) = delete;
	Timer& operator=(const Timer& aTimer) = delete;

	void Update()
	{
		myTimeLastFrame = myCurrentTime;
		myCurrentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> dt = myCurrentTime - myTimeLastFrame;
		myDeltaTime = dt.count();
		myTotalTime += myDeltaTime;

		if (myDeltaTime > 0.1)
		{
			myDeltaTime = 0.1f;
		}
	}

	float GetDeltaTime() const { return myDeltaTime; }
	float GetTotalTime() const { return myTotalTime; }

private:

	Timer() {};

	float myDeltaTime = 0;
	float myTotalTime = 0;

	std::chrono::high_resolution_clock::time_point myTimeLastFrame = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point myCurrentTime = std::chrono::high_resolution_clock::now();

};

