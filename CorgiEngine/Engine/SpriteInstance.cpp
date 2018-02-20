#include "stdafx.h"
#include "SpriteInstance.h"

CSpriteInstance::CSpriteInstance()
{
	myPosition = { 0.0f, 0.0f, 0.f };
	myColor = { 1.f, 1.f, 1.f, 1.f};
	myUVOffset = { 0.f,0.f };
	myShouldBeRemoved = false;
}


CSpriteInstance::~CSpriteInstance()
{
}

void CSpriteInstance::SetSpriteMap(long long aSpriteID, int amountOfStepsX, int amountOfStepsY)
{
	mySprite = aSpriteID;
	myStep = { 1.f / amountOfStepsX, 1.f / amountOfStepsY};
	myAmountOfSteps = {amountOfStepsX, amountOfStepsY};
	myCurrentStep = {0,1};
	if (amountOfStepsY == 1)
	{
		myStep.y = 0.5;
	}
	SetUVOffset({ -0.5f + (myStep.x * myCurrentStep.x) + 0.06f, -0.5f + (myStep.y * myCurrentStep.y) });
}

void CSpriteInstance::Step()
{
	myCurrentStep.x++;
	if (myCurrentStep.x >= myAmountOfSteps.x)
	{
		myCurrentStep.x = 0;
		myCurrentStep.y++;
		if (myCurrentStep.y > myAmountOfSteps.y)
		{
			myCurrentStep.y = 1;
		}
	}
	SetUVOffset({ -0.5f + (myStep.x * myCurrentStep.x) + 0.06f, -0.5f + (myStep.y * myCurrentStep.y) });
}

void CSpriteInstance::SetStep(int aIndex)
{
	myCurrentStep = { aIndex % myAmountOfSteps.x, (aIndex % myAmountOfSteps.y) + 1 };
	SetUVOffset({ -0.5f + (myStep.x * myCurrentStep.x) + 0.06f, -0.5f + (myStep.y * myCurrentStep.y) });
}

void CSpriteInstance::SetPositionFromWorldPosition(const Vector3f & aPos, const CCameraInstance * aCameraPtr)
{
	SetPositionNeg1toPos1(aCameraPtr->FromWorldSpaceToProjectionSpace(aPos));
}

void CSpriteInstance::Move(const Vector3f & aMovement)
{
	myPosition += aMovement;
}

void CSpriteInstance::SetShouldRender(bool aShouldRender)
{
	myShouldRender = aShouldRender;
}

bool CSpriteInstance::GetShouldRender()
{
	return myShouldRender;
}

int CSpriteInstance::GetAmountOfSteps()
{
	return myAmountOfSteps.x * myAmountOfSteps.y;
}

void CSpriteInstance::Remove()
{
	myShouldBeRemoved = true;
}

bool CSpriteInstance::GetShouldRemove()
{
	return myShouldBeRemoved;
}


