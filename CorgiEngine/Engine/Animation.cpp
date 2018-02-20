#include "stdafx.h"
#include "Animation.h"
#include "assimp/scene.h"


CAnimation::CAnimation()
{
	myIsReady = false;
	myLastTime = 0.f;
	myTicksPerSecond = 0.f;
	myDuration = 0.f;
	myTransforms.Init(2);
}

CAnimation::~CAnimation()
{
}

bool CAnimation::Init(const aiAnimation * aAiAnim)
{
	if (aAiAnim == nullptr)
	{
		return false;
	}
	myLastTime = 0.0;
	myTicksPerSecond = static_cast<float>(aAiAnim->mTicksPerSecond != 0.0f ? aAiAnim->mTicksPerSecond : 100.0f);
	myDuration = static_cast<float>(aAiAnim->mDuration);
	myName = aAiAnim->mName.data;
	//OUTPUT_DEBUG_MSG("Creating Animation named: "<<Name);
	myChannels.Init(aAiAnim->mNumChannels);
	for (unsigned int a = 0; a < aAiAnim->mNumChannels; a++) {
		SAnimationChannel newChannel;
		newChannel.Init(aAiAnim->mChannels[a]->mNumPositionKeys, aAiAnim->mChannels[a]->mNumRotationKeys, aAiAnim->mChannels[a]->mNumScalingKeys);
		newChannel.myName = aAiAnim->mChannels[a]->mNodeName.data;
		myChannels.Add(newChannel);
		for (unsigned int i(0); i < aAiAnim->mChannels[a]->mNumPositionKeys; i++)
		{
			myChannels[a].myPositionKeys.Add(aAiAnim->mChannels[a]->mPositionKeys[i]);
		}

		for (unsigned int i(0); i < aAiAnim->mChannels[a]->mNumRotationKeys; i++)
		{
			myChannels[a].myRotationKeys.Add(aAiAnim->mChannels[a]->mRotationKeys[i]);
		}

		for (unsigned int i(0); i < aAiAnim->mChannels[a]->mNumScalingKeys; i++)
		{
			myChannels[a].myScalingKeys.Add(aAiAnim->mChannels[a]->mScalingKeys[i]);
		}
	}
	myLastPositions.Init(aAiAnim->mNumChannels);
	for (unsigned int i = 0; i < aAiAnim->mNumChannels; ++i)
	{
		myLastPositions.Add(std::make_tuple(0, 0, 0));
	}

	
	return true;
}

const Corgi::GrowingArray<Matrix44>* CAnimation::GetTransformAtTime(float aTime, bool aPlayingForward, bool aIsLooping)
{
	return &myTransforms[GetFrameIndexAt(aTime, aPlayingForward, aIsLooping)];
}

const Corgi::GrowingArray<Matrix44>& CAnimation::GetTransformAtIndex(unsigned int aIndex)
{
	if (aIndex > myTransforms.Size() - 2)
	{
		return myTransforms[myTransforms.Size() - 2];
	}
	return myTransforms[aIndex];
}

bool CAnimation::IsReady()
{
	return myIsReady;
}


unsigned int CAnimation::GetNextTransformIndex(unsigned int aCurrentIndex)
{
	if (aCurrentIndex >= myTransforms.Size() - 1)
	{
		return myTransforms.Size() - 1;
	}
	else
	{
		return aCurrentIndex + 1;
	}
}

unsigned int CAnimation::GetFrameIndexAt(float aTime, bool aPlayingForward, bool aIsLooping)
{
	aTime *= myTicksPerSecond;

	float time = 0.0f;
	if (myDuration > 0.0 && aIsLooping)
		time = fmod(aTime, myDuration);
	else if (myDuration > 0.0)
	{
		if (aTime > myDuration)
		{
			time = myDuration;
		}
		else
		{
			time = aTime;
		}
	}


	float percent = time / myDuration;
	if (!aPlayingForward) percent = (percent - 1.0f)*-1.0f;// this will invert the percent so the animation plays backwards
	return static_cast<unsigned int>((static_cast<float>(myTransforms.Size()) * percent));
}

unsigned int CAnimation::AnimationTransformCount()
{
	return myTransforms.Size() - 1;
}

CSkeleton& CAnimation::GetSkeleton()
{
	return mySkeleton;
}

