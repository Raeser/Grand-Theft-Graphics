#include "stdafx.h"
#include "AnimationInstance.h"


CAnimationInstance::CAnimationInstance()
{
	myAnimationDuration = 0;
	myCurrentAnimationIndex = -1;
	myShouldChangeAnimation = false;
	myFinishedThisFrame = false;
	myIsPlaying = false;
	myCurrentTransformIndex = 0;
	myBehaviour = eAnimationBehaviour::eForward;
	myShouldLoop = true;
}


CAnimationInstance::~CAnimationInstance()
{
}

bool CAnimationInstance::Init(long long aModelToAnimateID, eAnimationBehaviour aBehaviour)
{
	aModelToAnimateID;
	myBehaviour = aBehaviour;
	return true;
}

void CAnimationInstance::SetStepDelay(float aDelay)
{
	myStepDelay = aDelay;
}

long long CAnimationInstance::GetcurrentAnimationID()
{
	return myCurrentAnimationIndex;
}


std::string CAnimationInstance::GetCurrentAnimationStateName()
{
	return myAnimationName;
}

void CAnimationInstance::Play()
{
	myIsPlaying = true;
}

void CAnimationInstance::Stop()
{
	myIsPlaying = false;
}

void CAnimationInstance::Reset()
{
	myAnimationDuration = 0;
}


const Corgi::GrowingArray<Matrix44>& CAnimationInstance::GetCurrentTransforms() const
{
	return myCurrentTransforms;
}

void CAnimationInstance::SetAnimation(long long aID)
{
	myCurrentAnimationIndex = aID;
	myShouldChangeAnimation = false;
}

void CAnimationInstance::SetAnimation(std::string aStateName)
{
	myAnimationName = aStateName;
	myShouldChangeAnimation = true;
}

void CAnimationInstance::SetBehaviour(eAnimationBehaviour aBehaviour)
{
	myBehaviour = aBehaviour;
}

void CAnimationInstance::SetBehaviourAndStart(eAnimationBehaviour aBehaviour)
{
	myBehaviour = aBehaviour;
	myIsPlaying = true;
}

void CAnimationInstance::SetShouldLoop(bool aShouldLoop)
{
	myShouldLoop = aShouldLoop;
}

long long CAnimationInstance::GetUniqueInstanceID()
{
	return myUniqueInstanceID;
}

bool CAnimationInstance::AnimationFinishedThisFrame()
{
	return myFinishedThisFrame;
}

bool CAnimationInstance::GetShouldLoop() const
{
	return myShouldLoop;
}

void CAnimationInstance::SetAnimationTime(float aTime)
{
	myAnimationDuration = aTime;
}

const eAnimationBehaviour & CAnimationInstance::GetBehaviourType() const
{
	return myBehaviour;
}

void CAnimationInstance::Update(float aDT)
{
	myAnimationDuration += aDT;
}

void CAnimationInstance::SetTransforms(const Corgi::GrowingArray<Matrix44>& aTransformsToSet)
{
	myCurrentTransforms = aTransformsToSet;
}
