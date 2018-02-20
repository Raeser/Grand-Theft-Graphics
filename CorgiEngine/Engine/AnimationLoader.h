#pragma once
#include <map>
#include "..\Utilities\JsonDocument.h"

class CModel;
class CAnimation;
struct SBone;


class CAnimationLoader
{
public:
	CAnimationLoader();
	~CAnimationLoader();

	bool Init();
	void SafeCheckAnimationJson();
	bool LoadAnimations(const char* animPath, CModel& aModelOut);
	CAnimation* GetAnimation(long long aID);
	CAnimation& GetAnimation(std::string aName);
	bool GetAnimationID(std::string & aName, long long& animationIDToUse);

private:

	void PreCalculateAnimations(CAnimation& aAnimationToEvaluate);
	void CalculateNodeTransforms(float aTime, CAnimation& aAnimationToEvaluate);
	void Evaluate(float aTime, CAnimation& aAnimationToEvaluate, std::map<std::string, SBone*>& bones);

	bool DoesAnimationExistForModel(std::string aModelPath, Corgi::GrowingArray<std::string>& aAnimationPathList, Corgi::GrowingArray<std::string>& aAnimationNameList);
	bool LoadAnimationInternal(std::string& aPath, std::string& aName);
	Corgi::GrowingArray<CAnimation, long long> myAnimations;
	std::unordered_map<std::string, long long> myAnimationLUT;
	JsonDocument myAnimationDocument;

	bool myShouldEndThread; 

};

 