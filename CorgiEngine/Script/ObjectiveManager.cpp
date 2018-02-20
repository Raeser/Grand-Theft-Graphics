#include "stdafx.h"
#include "ObjectiveManager.h"


void CObjectiveManager::IncrementObjectiveCounter()
{
	++myObjectiveCounter;
}

void CObjectiveManager::DecrementObjectiveCounter()
{
	--myObjectiveCounter;
	if (myObjectiveCounter < 0)
	{
		myObjectiveCounter = 0;
	}
}

int CObjectiveManager::GetObjectiveCounter() const
{
	return myObjectiveCounter;
}

int CObjectiveManager::Reset()
{
	myObjectiveCounter = 0;
	return 0;
}

CObjectiveManager::CObjectiveManager()
{
}


CObjectiveManager::~CObjectiveManager()
{
}
