#include "stdafx.h"
#include <algorithm>
#include "ModelLoaderWrapper.h"
#include "../Utilities/JsonDocument.h"


CModelLoaderWrapper::CModelLoaderWrapper(CShaderManager& aShaderManager, CAnimationLoader& aAnimationLoader) : 
	myShaderManager(aShaderManager), 
	myAnimationLoader(aAnimationLoader)
{
	myShouldEndThread = false;
	myThreadIsDone = false;
	myResourceLoader = nullptr;
	myIsInited = false;
	myDeviceToForwardToLoader = nullptr;
}


CModelLoaderWrapper::~CModelLoaderWrapper()
{
	myShouldEndThread = true;
	if (myThread)
	{
		while (!myThreadIsDone)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		}
	}
	myResourceLoader->Release();
	for (SModelWrapper& modelWrap : myResources)
	{
		modelWrap.myResource.Release();
	}
	delete myResourceLoader;
	myResourceLoader = nullptr;
}

bool CModelLoaderWrapper::Init(ID3D11Device * aDevice, CScene* aScene)
{
	if (!myIsInited)
	{
		myResourceReadyStatus.Init(6400);
		for (auto i = 0; i < 6400; i++)
		{
			myResourceReadyStatus.Add(false);
		}
		myResourcePathsAdded.Init(6400);
		myResourcesToCreate.Init(6400);
		//myFailedModelPaths.Init(6400);
		myResources.Init(6400);
		myResourceLoader = new CModelLoader(myShaderManager, myAnimationLoader);
		myResourceLoader->Init(aDevice);
		myIsInited = true;
		myScene = aScene;
		myErrorCubeID = CreateErrorCube();

		std::string jsonFilePath = "../Assets/Data/Data/shaderData.json";
		JsonDocument jsonDocument(jsonFilePath.c_str());
		struct stat buffer;
		if (stat(jsonFilePath.c_str(), &buffer) == 0)
		{
			jsonDocument.LoadFile(jsonFilePath.c_str());

			if (!jsonDocument.Find("Data"))
			{
				DL_ERROR(eDebugLogType::UnityExporter, "%s%s", "[ModelLoaderWrapper] FAILED to load JsonFileData -- Data array not found in JsonFile: ", jsonFilePath.c_str());
			}
			else
			{
				JsonValue jsonValue = jsonDocument["Data"];

				int jsonDokumentSize = jsonValue.GetSize();
				for (int i = 0; i < jsonDokumentSize; i++)
				{
					if (IsStringValid(jsonValue, i, "fbx"))
					{			
						std::string fbx = jsonValue[i]["fbx"].GetString();
						Corgi::ConvertStringPathToSafeUse(fbx);
						if (IsStringValid(jsonValue, i, "psShader", false))
						{
							const std::string filePathPS = jsonValue[i]["psShader"].GetString();
							if (stat(filePathPS.c_str(), &buffer) == 0)
							{
								myPredefinedShadersPS.emplace(fbx, jsonValue[i]["psShader"].GetString());
							}
							else
							{
								DL_ERROR(eDebugLogType::UnityExporter, "%s%s", "[ModelLoaderWrapper] FAILED to load psShader -- file couldnt be found : ", filePathPS.c_str());
							}
						}
						
						if (IsStringValid(jsonValue, i, "vsShader", false))
						{
							const std::string filePathVS = jsonValue[i]["vsShader"].GetString();
							if (stat(filePathVS.c_str(), &buffer) == 0)
							{
								myPredefinedShadersVS.emplace(fbx, jsonValue[i]["vsShader"].GetString());
							}
							else
							{
								DL_ERROR(eDebugLogType::UnityExporter, "%s%s", "[ModelLoaderWrapper] FAILED to load vsShader -- file couldnt be found : ", filePathVS.c_str());
							}
						}
					}
				}
			}
		}
		else
		{
			DL_ERROR(eDebugLogType::UnityExporter, "%s%s", "[ModelLoaderWrapper] FAILED to load JsonFile -- jsonfile couldnt be found : ", jsonFilePath.c_str());
		}
		return true;
	}
	return false;
}

bool CModelLoaderWrapper::IsStringValid(JsonValue& currentJsonValue, int aIndex, const char* aStringToFind, bool shouldGiveOutError)
{
	if (!currentJsonValue[aIndex].Find(aStringToFind))
	{
		if (shouldGiveOutError)
		{
			DL_ERROR(eDebugLogType::UnityExporter, "%s%d%s%s", "[UNITYEXPORTER] FAILED to load at index: ", aIndex, " -- ", aStringToFind);
		}
		return false;
	}
	return true;
}

long long CModelLoaderWrapper::CreateErrorCube()
{
	return CreateModel("cube", ModelType::ErrorCube);
}

void CModelLoaderWrapper::CreateResourceThreaded(const long long aResourceIDToCreate)
{
	if (myThread)
	{
		myResourcesToCreateMutex.lock();
		myResourcesToCreate.Add(aResourceIDToCreate);
		myResourcesToCreateMutex.unlock();
	}
	else
	{
		myResourcesToCreate.Add(aResourceIDToCreate);

		AssignFunctionToThread(Callback_Function(CModelLoaderWrapper::UpdateThread));
	}
 }

void CModelLoaderWrapper::ReloadResourceFromPath(std::wstring aPath)
{
	std::string tempString = Corgi::ConvertWStringToString(aPath);
	Corgi::ConvertStringPathToSafeUse(tempString);
	long long tempID;
	if (SetResourceIDIfExists(tempID, tempString))
	{
		CreateResourceThreaded(tempID);
	}
	else
	{
		//Unknown Error
		assert(false);
	}

	return;
}

bool CModelLoaderWrapper::ReplaceResource(const long long& aID, const char * aPath, const ModelType& aModelType)
{
	std::string tempString(aPath);
	Corgi::ConvertStringPathToSafeUse(tempString);
	if (CheckIfResourceIDExists(aID))
	{
		ModelType tempModelType;
		myResourcesMutex.lock();
		tempModelType = myResources[aID].myResourceType;
		myResourcesMutex.unlock();

		if (tempModelType == aModelType)
		{
			CreateResourceThreaded(aID);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool CModelLoaderWrapper::CheckIfResourceIDExists(const long long& aID)
{
	myResourcesMutex.lock();
	long long size = myResources.Size();
	myResourcesMutex.unlock();
	if (size < aID || aID < 0)
	{
		return false;
	}
	return true;
}

 void CModelLoaderWrapper::UpdateThread()
 {
 	//CModelLoader* modelLoader = new CModelLoader();
 	//modelLoader->Init(myDeviceToForwardToLoader);
 
 	while (!myShouldEndThread)
 	{
 		myResourcesToCreateMutex.lock();
 		myResourcesToCreateThreaded = myResourcesToCreate;
 		myResourcesToCreate.RemoveAll();
 		myResourcesToCreateMutex.unlock();
 
 		SModelWrapper modelWrapperToCreate;
 		for (auto modelID : myResourcesToCreateThreaded)
 		{
 			if (myShouldEndThread)
 			{
 				break;
 			}
 
 			myResourcesMutex.lock();
 			modelWrapperToCreate = myResources[modelID];
 			myResourcesMutex.unlock();
 
 			bool hasBeenCreated = false;
 			switch (modelWrapperToCreate.myResourceType)
 			{
 			case ModelType::ErrorCube :
 				hasBeenCreated = myResourceLoader->CreateCube2(modelWrapperToCreate.myResource);
 				break;
 			case ModelType::Model:
 				hasBeenCreated = myResourceLoader->CreateModel2(modelWrapperToCreate.myResource, modelWrapperToCreate.myPath.c_str(), modelWrapperToCreate.myVSShader.c_str(), modelWrapperToCreate.myPSShader.c_str());
 				break;
 			case ModelType::Skybox:
 				hasBeenCreated = myResourceLoader->CreateSkybox2(modelWrapperToCreate.myResource, modelWrapperToCreate.myPath.c_str());
 				break;
 			case ModelType::unknown:
 				assert(false);
 				break;
 			}
 
 			if (hasBeenCreated)
 			{
 				myResourcesMutex.lock();
 				myResources[modelID].myResource = modelWrapperToCreate.myResource;
 				myResourcesMutex.unlock();
 			}
 			else
 			{
 				//// ERRORCUBE
 				//myFailedModelPathsMutex.lock();
 				//myFailedModelPaths.Add(modelWrapperToCreate.myPath);
 				//myFailedModelPathsMutex.unlock();
 
 				myResourcesMutex.lock();
 				myResources[modelID].myResource = myResources[0].myResource;
 				myResourcesMutex.unlock();
 			}
 			myResourcesReadyStatusMutex.lock();
 			myResourceReadyStatus[modelID] = true;
 			myResourcesReadyStatusMutex.unlock();
 		}
 		myResourcesToCreateThreaded.RemoveAll();
 		std::this_thread::sleep_for(std::chrono::milliseconds(10));
 	}
	
	//delete modelLoader;
	//modelLoader = nullptr;
 
 	myThreadIsDone = true;
 }

CModelInstance * CModelLoaderWrapper::CreateInstance(const char * aPath, ModelType aModelTypeToCreate, const char* aVSShader, const char* aPSShader)
{
	CheckInit();

	std::string myPath(aPath);
	long long myInstanceID;

	myInstanceID = CreateModel(aPath, aModelTypeToCreate, aVSShader, aPSShader);
	
	CModelInstance* modelInstanceToReturn = new CModelInstance();
	modelInstanceToReturn->SetModel(myInstanceID);

	myScene->AddModelInstance(modelInstanceToReturn);
	return modelInstanceToReturn;
}

CModelInstance * CModelLoaderWrapper::CreateInstance(long long aID)
{
	CheckInit();

	if (myResources.Size() <= aID || 0 > aID)
	{
		assert(false);
	}

	CModelInstance* modelInstanceToReturn = new CModelInstance();
	modelInstanceToReturn->SetModel(aID);
	myScene->AddModelInstance(modelInstanceToReturn);
	return modelInstanceToReturn;
}

CModelInstance * CModelLoaderWrapper::CreateInstance()
{
	CheckInit();
	CModelInstance* modelInstanceToReturn = new CModelInstance();
	modelInstanceToReturn->SetModel(myErrorCubeID);
	myScene->AddModelInstance(modelInstanceToReturn);
	return modelInstanceToReturn;
}

long long CModelLoaderWrapper::CreateSkybox(const char * aTexturePath)
{
	return CreateModel(aTexturePath, ModelType::Skybox);
}
bool CModelLoaderWrapper::SetResourceIDIfExists(long long& aIDOut, const std::string& aPath)
{
	long long tempID;
	//myFailedModelPathsMutex.lock();
	//tempID = myFailedModelPaths.Find(aPath);
	//myFailedModelPathsMutex.unlock();
	//
	//if (tempID != -1)
	//{
	//	aIDOut = 0;
	//	return true;
	//}

	myResourcesAddedMutex.lock();
	tempID = myResourcePathsAdded.Find(aPath);
	myResourcesAddedMutex.unlock();

	if (tempID != -1)
	{
		aIDOut = tempID;
		return true;
	}
	return false;
}

bool CModelLoaderWrapper::AddPSShadersIfPredefined(SModelWrapper& aModelDataWrapper)
{
	std::unordered_map<std::string, std::string>::const_iterator iter = myPredefinedShadersPS.find(aModelDataWrapper.myPath);
	if (iter != myPredefinedShadersPS.end())
	{
		aModelDataWrapper.myPSShader = iter->second;
		return true;
	}
	return false;
}

bool CModelLoaderWrapper::AddVSShadersIfPredefined(SModelWrapper& aModelDataWrapper)
{
	std::unordered_map<std::string, std::string>::const_iterator iter = myPredefinedShadersVS.find(aModelDataWrapper.myPath);
	if (iter != myPredefinedShadersVS.end())
	{
		aModelDataWrapper.myVSShader = iter->second;
		return true;
	}
	return false;
}

long long CModelLoaderWrapper::CreateModel(const char * aPath, ModelType aModelTypeToCreate, const char* aVSShader, const char* aPSShader)
{
	CheckInit();
	std::string modelPath(aPath);
	Corgi::ConvertStringPathToSafeUse(modelPath);

	long long idToReturn;
	if (SetResourceIDIfExists(idToReturn, modelPath))
	{
		return idToReturn;
	}

	myResourcesMutex.lock();
	idToReturn = myResources.GetFreeItem();
	myResources[idToReturn].myPath = modelPath;
	myResources[idToReturn].myResourceType = aModelTypeToCreate;
	if(!AddPSShadersIfPredefined(myResources[idToReturn]))
	{
		myResources[idToReturn].myPSShader = aPSShader;
	}

	if (!AddVSShadersIfPredefined(myResources[idToReturn]))
	{
		myResources[idToReturn].myVSShader = aVSShader;
	}

	myResourcesMutex.unlock();

	myResourcesAddedMutex.lock();
	myResourcePathsAdded.Add(modelPath);
	myResourcesAddedMutex.unlock();

	std::string pathWithoutFBX = modelPath;									// THIS IS FOR FAST SPLATMAP TEXTURING
	size_t position = pathWithoutFBX.find(".fbx");							// THIS IS FOR FAST SPLATMAP TEXTURING
	if (position != -1)														// THIS IS FOR FAST SPLATMAP TEXTURING
	{																		// THIS IS FOR FAST SPLATMAP TEXTURING
		pathWithoutFBX = pathWithoutFBX.substr(0, position);				// THIS IS FOR FAST SPLATMAP TEXTURING
	}																		// THIS IS FOR FAST SPLATMAP TEXTURING

	std::string fileToWatch;

	std::wstring modelPathwString = Corgi::ConvertCharPtrToWideString(modelPath.c_str());
	switch (aModelTypeToCreate)
	{
	case ModelType::ErrorCube:
		break;
	case ModelType::Model:
		Corgi::CFileWatcher::GetInstance().WatchFileChange(modelPathwString, std::bind(&CModelLoaderWrapper::ReloadResourceFromPath, this, std::placeholders::_1));
		fileToWatch = pathWithoutFBX + "_Metallic.dds";
		Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::ConvertCharPtrToWideString(fileToWatch.c_str()), std::bind(&CModelLoaderWrapper::ReloadResourceFromPath, this, modelPathwString));
		fileToWatch = pathWithoutFBX + "_Albedo.dds";
		Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::ConvertCharPtrToWideString(fileToWatch.c_str()), std::bind(&CModelLoaderWrapper::ReloadResourceFromPath, this, modelPathwString));
		fileToWatch = pathWithoutFBX + "_AmbientOcclusion.dds";
		Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::ConvertCharPtrToWideString(fileToWatch.c_str()), std::bind(&CModelLoaderWrapper::ReloadResourceFromPath, this, modelPathwString));
		fileToWatch = pathWithoutFBX + "_Emissive.dds";
		Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::ConvertCharPtrToWideString(fileToWatch.c_str()), std::bind(&CModelLoaderWrapper::ReloadResourceFromPath, this, modelPathwString));
		fileToWatch = pathWithoutFBX + "_Normals.dds";
		Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::ConvertCharPtrToWideString(fileToWatch.c_str()), std::bind(&CModelLoaderWrapper::ReloadResourceFromPath, this, modelPathwString));
		fileToWatch = pathWithoutFBX + "_Roughness.dds";
		Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::ConvertCharPtrToWideString(fileToWatch.c_str()), std::bind(&CModelLoaderWrapper::ReloadResourceFromPath, this, modelPathwString));

		break;
	case ModelType::Navmesh:
		Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::ConvertCharPtrToWideString(modelPath.c_str()), std::bind(&CModelLoaderWrapper::ReloadResourceFromPath, this, std::placeholders::_1));
		break;
	case ModelType::Skybox:
		Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::ConvertCharPtrToWideString(modelPath.c_str()), std::bind(&CModelLoaderWrapper::ReloadResourceFromPath, this, std::placeholders::_1));
		break;
	case ModelType::unknown:
		break;
	default:
		break;
	}
	
	CreateResourceThreaded(idToReturn);

	return idToReturn;
}

CModel & CModelLoaderWrapper::GetModel(long long aID)
{
	CheckInit();
	myResourcesReadyStatusMutex.lock();
	if (myResourceReadyStatus[aID] == false)
	{
		assert(false);
	}
	myResourcesReadyStatusMutex.unlock();

	myResourcesMutex.lock();
	CModel& returnMe = myResources[aID].myResource;
	myResourcesMutex.unlock();
	return returnMe;
}

bool CModelLoaderWrapper::GetModelBoundingBox(long long aID, Vector3f& aBoundingBoxMax, Vector3f& aBoundingBoxMin)
{
	CheckInit();
	if (!IsModelReadyToUse(aID))
	{
		return false;
	}

	myResourcesReadyStatusMutex.lock();
	if (myResourceReadyStatus[aID] == false)
	{
		assert(false);
	}
	myResourcesReadyStatusMutex.unlock();

	myResourcesMutex.lock();
	CModel::SBoundingBox& boundingBox = myResources[aID].myResource.GetBoundingBox();
	aBoundingBoxMax = boundingBox.myMax;
	aBoundingBoxMin = boundingBox.myMin;
	myResourcesMutex.unlock();
	return true;
}

bool CModelLoaderWrapper::UpdateSwitchLogic(SModelWrapper & aResourceOut)
{
	switch (aResourceOut.myResourceType)
	{
	case ModelType::ErrorCube:
		return myResourceLoader->CreateCube2(aResourceOut.myResource);
		break;
	case ModelType::Model:
		return myResourceLoader->CreateModel2(aResourceOut.myResource, aResourceOut.myPath.c_str(), aResourceOut.myVSShader.c_str(), aResourceOut.myPSShader.c_str());
		break;
	case ModelType::Skybox:
		return myResourceLoader->CreateSkybox2(aResourceOut.myResource, aResourceOut.myPath.c_str());
		break;
	case ModelType::unknown:
		assert(false);
		break;
	}
	return false;
}


int CModelLoaderWrapper::GetAmount()
{
	CheckInit();
	myResourcesMutex.lock();
	int returnMe = static_cast<int>(myResources.Size());
	myResourcesMutex.unlock();

	return returnMe;
}

void CModelLoaderWrapper::CheckInit()
{
	if (!myIsInited)
	{
		assert(false);
	}
}

bool CModelLoaderWrapper::IsModelReadyToUse(const long long & aID)
{
	return myResourceReadyStatus[aID];
}