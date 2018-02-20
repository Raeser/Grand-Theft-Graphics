#pragma once
class CAnimationManager;
enum class eAnimationBehaviour
{
	eBackWard,
	eForward,
	eStepping
};
class CAnimationInstance
{
	friend CAnimationManager;
public:
	CAnimationInstance();
	~CAnimationInstance();
	bool Init(long long aModelToAnimateID, eAnimationBehaviour aBehaviour);
	
	long long GetcurrentAnimationID();
	std::string GetCurrentAnimationStateName();
	const Corgi::GrowingArray<Matrix44>& GetCurrentTransforms() const;
	const eAnimationBehaviour& GetBehaviourType() const;
	
	void Play();
	void Stop();
	void Reset();
	
	void SetStepDelay(float aDelay);
	void SetConnectedModelID(long long aID);
	void SetAnimation(long long aID);
	void SetAnimation(std::string aStateName); //Add StateNames in AnimationConnections.Json
	void SetBehaviour(eAnimationBehaviour aBehaviour);
	void SetBehaviourAndStart(eAnimationBehaviour aBehaviour);
	void SetShouldLoop(bool aShouldLoop);
	long long GetUniqueInstanceID();
	bool AnimationFinishedThisFrame();
	bool GetShouldLoop() const;

	void SetAnimationTime(float aTime);
private:
	void Update(float aDT);
	void SetTransforms(const Corgi::GrowingArray<Matrix44>& aTransformsToSet);

private:
	Corgi::GrowingArray<Matrix44> myCurrentTransforms;
	std::string myAnimationName;
	long long myCurrentAnimationIndex;
	long long myUniqueInstanceID;
	unsigned int myCurrentTransformIndex;
	float myAnimationDuration;
	float myStepDelay;
	eAnimationBehaviour myBehaviour;
	bool myShouldChangeAnimation;
	bool myIsPlaying;
	bool myShouldLoop;
	bool myFinishedThisFrame;
};

