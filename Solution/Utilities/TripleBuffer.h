#pragma once
#include "GrowingArray.h"
#include <atomic>
#include <mutex>

template <typename T>
class TripleBuffer
{

public:

	TripleBuffer<T>();

	TripleBuffer<T>(const TripleBuffer<T>&) = delete;
	TripleBuffer<T>& operator=(const TripleBuffer<T>&) = delete;

	Corgi::GrowingArray<T>& Read();
	void Write(const T newT);
	void Reset();
	bool FlipReader();
	void FlipWriter();

	Corgi::GrowingArray<T>& ReadMostRecent();
	void WriteAndFlip(T newT);

private:
	int myRead = 0;
	int myWrite = 1;
	int myAvailableArray = 2;
	int myMostRecentWrite = 0;

	std::mutex myReadMutex;
	std::mutex myWriteMutex;

	Corgi::GrowingArray<T> buffer[3];
};

template <typename T>
TripleBuffer<T>::TripleBuffer() {

	buffer[0].Init(512);
	buffer[1].Init(512);
	buffer[2].Init(512);
}

template <typename T>
Corgi::GrowingArray<T>& TripleBuffer<T>::Read() 
{
	std::lock_guard<std::mutex> lock(myReadMutex);
	return buffer[myRead];
}

template <typename T>
void TripleBuffer<T>::Write(const T newT) 
{
	std::lock_guard<std::mutex> lock(myWriteMutex);
	buffer[myWrite].Add(newT);
}

template <typename T>
bool TripleBuffer<T>::FlipReader() 
{
	std::lock_guard<std::mutex> lock(myReadMutex);
	if (myMostRecentWrite == myAvailableArray)
	{
		std::swap(myRead, myAvailableArray);
		buffer[myAvailableArray].RemoveAll();
		return true;
	}
	return false;
}

template <typename T>
void TripleBuffer<T>::Reset()
{
	std::lock_guard<std::mutex> lockRead(myReadMutex);
	std::lock_guard<std::mutex> lockWrite(myWriteMutex);
	buffer[0].RemoveAll();
	buffer[1].RemoveAll();
	buffer[2].RemoveAll();
}

template <typename T>
void TripleBuffer<T>::FlipWriter() 
{
	std::lock_guard<std::mutex> lock(myWriteMutex);
	myMostRecentWrite = myWrite;
	std::swap(myWrite, myAvailableArray);
	buffer[myWrite].RemoveAll();
}

template <typename T>
Corgi::GrowingArray<T>&  TripleBuffer<T>::ReadMostRecent() 
{
	FlipReader();
	return Read();
}

template <typename T>
void TripleBuffer<T>::WriteAndFlip(T newT) 
{
	Write(newT);
	FlipWriter();
}

