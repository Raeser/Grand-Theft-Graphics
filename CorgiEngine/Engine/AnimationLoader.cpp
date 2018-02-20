#include "stdafx.h"
#include "AnimationLoader.h"
#include "Animation.h"
#include "../Utilities/JsonDocument.h"
#include "AssImp/scene.h"
#include "Model.h"
#include "Skeleton.h"
#include <map>

CAnimationLoader::CAnimationLoader()
{
	myAnimations.Init(64);
}


CAnimationLoader::~CAnimationLoader()
{ 
	//myShouldEndThread = true;
	//if (myThread)
	//{
	//	while (!myThreadIsDone)
	//	{
	//		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	//	}
	//}
	//myResourceLoader->Release();
	//for (SModelWrapper& modelWrap : myResources)
	//{
	//	modelWrap.myResource.Release();
	//}
	//delete myResourceLoader;
	//myResourceLoader = nullptr;
}

bool CAnimationLoader::Init()
{
	myAnimations.Init(64);
	CAnimation createDefault;
	createDefault.myChannels.Init(1);
	createDefault.myLastPositions.Init(1);
	Corgi::GrowingArray<Matrix44> defaultTrans;
	defaultTrans.Init(2);
	defaultTrans.Add(Corgi::Matrix44<float>());
	createDefault.myTransforms.Add(defaultTrans);
	myAnimations.Add(createDefault);
	myAnimationLUT.insert(std::map<std::string, long long>::value_type("Default", myAnimations.Size() - 1));
	SafeCheckAnimationJson();
	return true;
}

void CAnimationLoader::SafeCheckAnimationJson()
{
	int amountOfAnimationsInFBX;
	const std::string fileToUse("../assets/models/AnimationConnections.json");
	std::string FBXstringToConvert;
	std::string AnimationPathstringToConvert;
	myAnimationDocument.LoadFile(fileToUse.c_str());
	if (myAnimationDocument.Find("Models"))
	{
		JsonValue modelArray = myAnimationDocument["Models"];
		const int amountOfModels = modelArray.GetSize();
		for (int i = 0; i < amountOfModels; ++i)
		{
			if (modelArray[i].Find("FbxPath"))
			{
				FBXstringToConvert = modelArray[i]["FbxPath"].GetString();
				Corgi::ConvertStringPathToSafeUse(FBXstringToConvert);
				modelArray[i]["FbxPath"].SetString(FBXstringToConvert.c_str());
			}
			if (modelArray[i].Find("Animations"))
			{
				amountOfAnimationsInFBX = modelArray[i]["Animations"].GetSize();
				for (int j = 0; j < amountOfAnimationsInFBX; ++j)
				{
					if (modelArray[i]["Animations"][j].Find("AnimationPath"))
					{
						AnimationPathstringToConvert = modelArray[i]["Animations"][j]["AnimationPath"].GetString();
						Corgi::ConvertStringPathToSafeUse(AnimationPathstringToConvert);
						modelArray[i]["Animations"][j]["AnimationPath"].SetString(AnimationPathstringToConvert.c_str());
					}
				}
			}
		}
		myAnimationDocument.SaveFile("../assets/models/AnimationConnections.json");
	}
}

bool CAnimationLoader::LoadAnimations(const char * modelPath, CModel & aModelOut)
{
	aModelOut;
	Corgi::GrowingArray<std::string> pathList;
	Corgi::GrowingArray<std::string> nameList;
	if (DoesAnimationExistForModel(modelPath, pathList, nameList))
	{
		for (int i = 0; i < nameList.Size(); ++i)
		{
			//if (myShouldEndThread)
			//{
			//	return false;
			//}
			if (LoadAnimationInternal(pathList[i], nameList[i]))
			{
				
				CAnimation& AnimationToPreCalc = myAnimations.GetLast();
				AnimationToPreCalc.GetSkeleton().Init(aiImportFile(pathList[i].c_str(), NULL), aiImportFile(modelPath, NULL));
 				PreCalculateAnimations(AnimationToPreCalc);
				AnimationToPreCalc.myIsReady = true;
			}
			aModelOut.SetDefaultAnimationID(nameList[0]);
		}
		pathList.RemoveAll();
		nameList.RemoveAll();
		return true;
	}

	return false;
}

CAnimation* CAnimationLoader::GetAnimation(long long aID)
{
	if (aID > myAnimations.Size() - 1 || aID < 0)
	{
		return nullptr;
	}
	return &myAnimations[aID];
}

CAnimation& CAnimationLoader::GetAnimation(std::string aName)
{
	return myAnimations[myAnimationLUT.at(aName)];
}

bool CAnimationLoader::GetAnimationID(std::string & aName, long long& animationIDToUse)
{
	if (myAnimationLUT.find(aName) != myAnimationLUT.end())
	{
		animationIDToUse = myAnimationLUT.at(aName);
		return true;
	}
	return false;


}

bool CAnimationLoader::DoesAnimationExistForModel(std::string aModelPath, Corgi::GrowingArray<std::string>& aAnimationPathList, Corgi::GrowingArray<std::string>& aAnimationNameList)
{
		Corgi::ConvertStringPathToSafeUse(aModelPath);
		JsonValue modelArray = myAnimationDocument["Models"];
		const int size = modelArray.GetSize();
		for (int i = 0; i < size; ++i)
		{
			if (modelArray[i].Find("FbxPath"))
			{
				if (modelArray[i]["FbxPath"].GetString() == aModelPath)
				{
					std::string animationPath;
					std::string animationName;
					if (modelArray[i].Find("Animations"))
					{
						const int animationSizeList = modelArray[i]["Animations"].GetSize();
						aAnimationPathList.Init(animationSizeList);
						aAnimationNameList.Init(animationSizeList);

						for (int animationIndex = 0; animationIndex < animationSizeList; ++animationIndex)
						{
							animationName = modelArray[i]["Animations"][animationIndex]["AnimationName"].GetString();
							animationPath = modelArray[i]["Animations"][animationIndex]["AnimationPath"].GetString();
							aAnimationPathList.Add(animationPath);
							aAnimationNameList.Add(animationName);
						}
						return true;
					}
					else
					{
						DL_ERROR(eDebugLogType::Resource, "%s%s%s", "[AnimationLoader] - Failed to load animation for model at : ", aModelPath.c_str(), " Animation Array couldnt be found");
					}
				}
			}
		}
	return false;
}

bool CAnimationLoader::LoadAnimationInternal(std::string& aPath, std::string& aStateName)
{
	const aiScene* animationScene = aiImportFile(aPath.c_str(), NULL);//, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs | aiProcess_FlipWindingOrder);
	CAnimation CreatedAnimation;
	if (animationScene && CreatedAnimation.Init(animationScene->mAnimations[0]))
	{
		myAnimations.Add(CreatedAnimation);
		myAnimationLUT.insert(std::map<std::string, long long>::value_type(aStateName, myAnimations.Size() - 1));
		return true;
	}
	else
	{
		return false;
	}
}

void CAnimationLoader::PreCalculateAnimations(CAnimation& aAnimationToEvaluate)
{
	float animationTransformSize = aAnimationToEvaluate.myDuration / (aAnimationToEvaluate.myTicksPerSecond / 30.0f);
	aAnimationToEvaluate.myTransforms.ReInit(cast_int(animationTransformSize + 1)); //är kanske fel
	const Corgi::GrowingArray<SBone*>& modelBones = aAnimationToEvaluate.GetSkeleton().GetBones();
	float timestep = 1.0f / 30.0f;// 30 per second
	float dt = 0;
	for (float ticks = 0; ticks < aAnimationToEvaluate.myDuration; ticks += aAnimationToEvaluate.myTicksPerSecond / 30.0f)
	{
		dt += timestep;
		CalculateNodeTransforms(dt, aAnimationToEvaluate);
		Corgi::GrowingArray<Corgi::Matrix44<float>> trans;
		trans.Init(modelBones.Size());
		for (int a = 0; a < modelBones.Size(); ++a)
		{
			Corgi::Matrix44<float> rotationmat = modelBones[a]->Offset * modelBones[a]->GlobalTransform;
 			trans.Add(rotationmat);
		}
		aAnimationToEvaluate.myTransforms.Add(trans);
	}

}

void CAnimationLoader::CalculateNodeTransforms(float aTime, CAnimation& aAnimationToEvaluate)
{
	Evaluate(aTime, aAnimationToEvaluate, aAnimationToEvaluate.GetSkeleton().GetBonesByName());
	aAnimationToEvaluate.GetSkeleton().UpdateTransforms(aAnimationToEvaluate.GetSkeleton().GetRootNode());
}

void CAnimationLoader::Evaluate(float aTime, CAnimation& aAnimationToEvaluate, std::map<std::string, SBone*>& bones)
{
	aTime *= aAnimationToEvaluate.myTicksPerSecond;

	float time = 0.0f;
	if (aAnimationToEvaluate.myDuration > 0.0)
		time = fmod(aTime, aAnimationToEvaluate.myDuration);
	// calculate the transformations for each animation channel
	for (int a = 0; a < aAnimationToEvaluate.myChannels.Size(); a++) {
		SAnimationChannel& channel = aAnimationToEvaluate.myChannels[a];

		std::map<std::string, SBone*>::iterator bonenode = bones.find(std::string(channel.myName));

		if (bonenode == bones.end()) {
			//OUTPUT_DEBUG_MSG("did not find the bone node "<<channel->Name);
			continue;
		}

		// ******** Position *****
		aiVector3D presentPosition(0, 0, 0);
		if (channel.myPositionKeys.Size() > 0) {
			// Look for present frame number. Search from last position if time is after the last time, else from beginning
			// Should be much quicker than always looking from start for the average use case.
			int frame = (time >= aAnimationToEvaluate.myLastTime) ? std::get<0>(aAnimationToEvaluate.myLastPositions[a]) : 0;
			while (frame < channel.myPositionKeys.Size() - 1) {
				if (time < channel.myPositionKeys[frame + 1].mTime) {
					break;
				}
				frame++;
			}

			// interpolate between this frame's value and next frame's value
			unsigned int nextFrame = (frame + 1) % channel.myPositionKeys.Size();

			const aiVectorKey& key = channel.myPositionKeys[frame];
			const aiVectorKey& nextKey = channel.myPositionKeys[nextFrame];
			double diffTime = nextKey.mTime - key.mTime;
			if (diffTime < 0.0)
				diffTime += aAnimationToEvaluate.myDuration;
			if (diffTime > 0) {
				float factor = float((time - key.mTime) / diffTime);
				presentPosition = key.mValue + (nextKey.mValue - key.mValue) * factor;
			}
			else {
				presentPosition = key.mValue;
			}
			std::get<0>(aAnimationToEvaluate.myLastPositions[a]) = frame;
		}
		// ******** Rotation *********
		aiQuaternion presentRotation(1, 0, 0, 0);
		if (channel.myRotationKeys.Size() > 0)
		{
			int frame = (time >= aAnimationToEvaluate.myLastTime) ? std::get<1>(aAnimationToEvaluate.myLastPositions[a]) : 0;
			while (frame < channel.myRotationKeys.Size() - 1) {
				if (time < channel.myRotationKeys[frame + 1].mTime)
					break;
				frame++;
			}

			// interpolate between this frame's value and next frame's value
			unsigned int nextFrame = (frame + 1) % channel.myRotationKeys.Size();

			const aiQuatKey& key = channel.myRotationKeys[frame];
			const aiQuatKey& nextKey = channel.myRotationKeys[nextFrame];
			double diffTime = nextKey.mTime - key.mTime;
			if (diffTime < 0.0) diffTime += aAnimationToEvaluate.myDuration;
			if (diffTime > 0) {
				float factor = float((time - key.mTime) / diffTime);
				aiQuaternion::Interpolate(presentRotation, key.mValue, nextKey.mValue, factor);
			}
			else presentRotation = key.mValue;
			std::get<1>(aAnimationToEvaluate.myLastPositions[a]) = frame;
		}

		// ******** Scaling **********
		aiVector3D presentScaling(1, 1, 1);
		if (channel.myScalingKeys.Size() > 0) {
			int frame = (time >= aAnimationToEvaluate.myLastTime) ? std::get<2>(aAnimationToEvaluate.myLastPositions[a]) : 0;
			while (frame < channel.myScalingKeys.Size() - 1) {
				if (time < channel.myScalingKeys[frame + 1].mTime)
					break;
				frame++;
			}

			presentScaling = channel.myScalingKeys[frame].mValue;
			std::get<2>(aAnimationToEvaluate.myLastPositions[a]) = frame;
		}

		aiMatrix4x4 mat = aiMatrix4x4(presentRotation.GetMatrix());

		mat.a1 *= presentScaling.x; mat.b1 *= presentScaling.x; mat.c1 *= presentScaling.x;
		mat.a2 *= presentScaling.y; mat.b2 *= presentScaling.y; mat.c2 *= presentScaling.y;
		mat.a3 *= presentScaling.z; mat.b3 *= presentScaling.z; mat.c3 *= presentScaling.z;
		mat.a4 = presentPosition.x; mat.b4 = presentPosition.y; mat.c4 = presentPosition.z;
		mat.Transpose();

		CSkeleton::TransformMatrix(bonenode->second->LocalTransform, mat);
	}
	aAnimationToEvaluate.myLastTime = time;
}
