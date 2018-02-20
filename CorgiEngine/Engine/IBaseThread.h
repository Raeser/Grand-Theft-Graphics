#pragma once

#include <mutex>
#include <functional>

#define Callback_Function(x) std::bind(&x, this)

typedef std::function<void()> callback_function;

class IBaseThread {
public:
	IBaseThread()
	{
		myThread = nullptr;
	}
	void AssignFunctionToThread(callback_function  aFunctionToCall)
	{
		if (!myThread)
		{
			myThread = new std::thread(aFunctionToCall);
		}
	}
	std::thread* myThread;
};