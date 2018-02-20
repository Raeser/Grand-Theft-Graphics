#include "stdafx.h"
#include "AnimationManager.h"
#include "AnimationLoader.h"
#include "AnimationInstance.h"
#include "Animation.h"

CAnimationManager::CAnimationManager(CAnimationLoader & aLoader) :
	myLoader(aLoader)
{
}

CAnimationManager::~CAnimationManager()
{
}

void CAnimationManager::Init(int aMaxAmountOfAnimatedObjects)
{
	myAnimationInstances.Init(aMaxAmountOfAnimatedObjects);
}

CAnimationInstance* CAnimationManager::GetAnimationInstance()
{
	long long freeItem = myAnimationInstances.GetFreeItem();
	myAnimationInstances[freeItem].myUniqueInstanceID = freeItem;

	return &myAnimationInstances[freeItem];
}

void CAnimationManager::FreeAnimationInstanceAtID(long long aID)
{
	myAnimationInstances.ReleaseObject(aID);
}

void CAnimationManager::Update()
{
	float dt = Timer::GetInstance().GetDeltaTime();	
	for (int i = 0; i < myAnimationInstances.Size(); ++i)
	{
		long long animationIDToUse = -1;
		if (myAnimationInstances.IsCurrentlyUsed(i))
		{
			
			CAnimationInstance& instanceToUpdate = myAnimationInstances[i];
			instanceToUpdate.myFinishedThisFrame = false;
			if (instanceToUpdate.myIsPlaying)
			{
				instanceToUpdate.Update(dt);
			}
			if (instanceToUpdate.myShouldChangeAnimation)
			{
				if (myLoader.GetAnimationID(instanceToUpdate.myAnimationName, animationIDToUse) == false)
				{
					continue;
				}
				instanceToUpdate.SetAnimation(animationIDToUse);
			}

			CAnimation* instAnim = myLoader.GetAnimation(instanceToUpdate.GetcurrentAnimationID());
			if (instAnim != nullptr && instAnim->IsReady())
			{
				unsigned int frameToSet = instanceToUpdate.myCurrentTransformIndex;
				const eAnimationBehaviour& animationType = instanceToUpdate.GetBehaviourType();
				if (animationType == eAnimationBehaviour::eStepping)
				{
					if (instanceToUpdate.myAnimationDuration > instanceToUpdate.myStepDelay)
					{
						frameToSet = instAnim->GetNextTransformIndex(frameToSet);
						instanceToUpdate.myCurrentTransformIndex = frameToSet;
						instanceToUpdate.myAnimationDuration = 0;
					}
				}
				else if (animationType == eAnimationBehaviour::eForward)
				{
					frameToSet = instAnim->GetFrameIndexAt(instanceToUpdate.myAnimationDuration, true, instanceToUpdate.myShouldLoop);
				}
				else if (animationType == eAnimationBehaviour::eBackWard)
				{
					frameToSet = instAnim->GetFrameIndexAt(instanceToUpdate.myAnimationDuration, false, instanceToUpdate.myShouldLoop);
				}
				instanceToUpdate.SetTransforms(instAnim->GetTransformAtIndex(frameToSet));
				instanceToUpdate.myCurrentTransformIndex = frameToSet;
				if (instanceToUpdate.myShouldLoop == false)
				{
					if (animationType == eAnimationBehaviour::eForward && instanceToUpdate.myCurrentTransformIndex >= instAnim->AnimationTransformCount())
					{
						instanceToUpdate.myFinishedThisFrame = true;
					}

				}
			}
		}
	}
	TestInstanceFunctions(testinstanceID);
}

void CAnimationManager::Reset()
{
	myAnimationInstances.FreeAll();
}

void CAnimationManager::TestInstanceFunctions(long long aInstanceToTestOn)
{
	while (!myAnimationInstances.IsCurrentlyUsed(aInstanceToTestOn) && myAnimationInstances.Size() - 1 != 0)
	{
		aInstanceToTestOn++;
		if (aInstanceToTestOn > myAnimationInstances.Size() - 1)
		{
			aInstanceToTestOn = 0;
		}
		testinstanceID = aInstanceToTestOn;
	}
	CAnimationInstance& instanceToTestOn = myAnimationInstances[aInstanceToTestOn];
	const eAnimationBehaviour& behave = instanceToTestOn.GetBehaviourType();
	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Num8))
	{
		if (behave == eAnimationBehaviour::eBackWard)
		{
			instanceToTestOn.myBehaviour = eAnimationBehaviour::eForward;
		}
		else if (behave == eAnimationBehaviour::eForward)
		{
			instanceToTestOn.myBehaviour = eAnimationBehaviour::eStepping;
		}
		else if (behave == eAnimationBehaviour::eStepping)
		{
			instanceToTestOn.myBehaviour = eAnimationBehaviour::eBackWard;
		}
	}
	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Num7))
	{
		instanceToTestOn.SetAnimation("PlayerIdle");
	}
	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Num6))
	{
		instanceToTestOn.SetAnimation("GateOpen");
	}
	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Num5))
	{
		instanceToTestOn.SetAnimation("Default");
	}
	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Num4))
	{
		instanceToTestOn.Play();
	}
	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Num3))
	{
		instanceToTestOn.Stop();
	}
	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Num2))
	{
		instanceToTestOn.Reset();
	}
	if (InputManager::GetInstance().CheckKeyPressed(KeyCode::Up))
	{
		testinstanceID++;
	}
}
