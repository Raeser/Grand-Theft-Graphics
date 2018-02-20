#include "stdafx.h"
#include "Timer.h"



Timer::~Timer()
{
}

void Timer::Update()
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

const float& Timer::GetDeltaTime() const
{
	return myDeltaTime;
}

const float& Timer::GetTotalTime() const
{
	return myTotalTime;
}
