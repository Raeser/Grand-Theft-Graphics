#pragma once
#include "Skeleton.h"
struct aiAnimation;

struct SAnimationChannel
{
	SAnimationChannel()
	{
		myPositionKeys.Init(2);
		myRotationKeys.Init(2);
		myScalingKeys.Init(2);
	}
	void Init(unsigned int aNumPosKeys, unsigned int aNumRotKeys, unsigned int aNumScalingKeys)
	{
		myPositionKeys.Init(aNumPosKeys);
		myRotationKeys.Init(aNumRotKeys);
		myScalingKeys.Init(aNumScalingKeys);
	}

	std::string myName;
	Corgi::GrowingArray<aiVectorKey> myPositionKeys;
	Corgi::GrowingArray<aiQuatKey>	 myRotationKeys;
	Corgi::GrowingArray<aiVectorKey> myScalingKeys;
};

class CAnimation
{
	friend CAnimationLoader;

public:
	CAnimation();
	~CAnimation();
	bool Init(const aiAnimation* aAiAnim);
	const Corgi::GrowingArray<Matrix44>* GetTransformAtTime(float aTime, bool aPlayingForward, bool aIsLooping);
	const Corgi::GrowingArray<Matrix44>& GetTransformAtIndex(unsigned int aIndex);
	bool IsReady();

	unsigned int GetNextTransformIndex(unsigned int aCurrentIndex);
	unsigned int GetFrameIndexAt(float aTime, bool aPlayingForward, bool aIsLooping);
	unsigned int AnimationTransformCount();

	CSkeleton& GetSkeleton();
private:


	Corgi::GrowingArray<SAnimationChannel> myChannels;
	Corgi::GrowingArray<Corgi::GrowingArray<Matrix44>, unsigned int> myTransforms;
	Corgi::GrowingArray<std::tuple<unsigned int, unsigned int, unsigned int>> myLastPositions;
	std::string myName;
	float myLastTime;
	float myTicksPerSecond;
	float myDuration;
	bool myIsReady;
	CSkeleton mySkeleton;
};

