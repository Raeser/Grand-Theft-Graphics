#pragma once
#include <mutex>
#include <Vector>
#include "IBaseThread.h"
#include "Model.h"
#include <atomic>

class CModelLoader;
class CScene;
class std::thread;
struct ID3D11Device;
class JsonValue;

enum class ModelType
{
	ErrorCube,
	Model,
	Navmesh,
	Skybox,
	unknown
};

struct SModelWrapper
{
	CModel myResource;
	std::string myPath;
	ModelType myResourceType = ModelType::unknown;
	std::string myVSShader;
	std::string myPSShader;
};

class CModelLoaderWrapper : IBaseThread
{
public:
	CModelLoaderWrapper(CShaderManager& myShaderManager, CAnimationLoader& myAnimationLoader);
	~CModelLoaderWrapper();
	bool Init(ID3D11Device * aDevice, CScene* aScene);
	bool IsStringValid(JsonValue & currentJsonValue, int aIndex, const char * aStringToFind, bool shouldGiveOutError = true);
	void CheckInit();

	long long CreateErrorCube();
	CModelInstance * CreateInstance(const char * aPath, ModelType aModelTypeToCreate = ModelType::Model,const char* aVSShader = "../Assets/Shaders/VS_DEFModel.hlsl", const char* aPSShader = "../Assets/Shaders/PS_DEFModel.hlsl"); //2 dagars buggletande så använder vi oss av forwardrenderarens jävla shaders :(
	CModelInstance * CreateInstance(long long aID);

	CModelInstance * CreateInstance();

	long long CreateSkybox(const char * aTexturePath);
	long long CreateModel(const char * aPath, ModelType aModelTypeToCreate, const char * aVSShader = "../Assets/Shaders/VS_DEFModel.hlsl", const char * aPSShader = "../Assets/Shaders/PS_DEFModel.hlsl");//2 dagars buggletande så använder vi oss av forwardrenderarens jävla shaders :(
	bool SetResourceIDIfExists(long long & aIDOut, const std::string& aPath);

	bool AddPSShadersIfPredefined(SModelWrapper & aModelDataWrapper);

	bool AddVSShadersIfPredefined(SModelWrapper & aModelDataWrapper);

	int GetAmount();
	bool IsModelReadyToUse(const long long& aID);
	bool ReplaceResource(const long long & aID, const char * aPath, const ModelType& aModelType);
	CModel & GetModel(long long aID);
	bool GetModelBoundingBox(long long aID, Vector3f & aBoundingBoxMax, Vector3f & aBoundingBoxMin);
	bool UpdateSwitchLogic(SModelWrapper& aResourceOut);

protected:
	void CreateResourceThreaded(const long long aIDOfModelToCreate);
	void ReloadResourceFromPath(std::wstring aPath);
	bool CheckIfResourceIDExists(const long long & aID);
	void UpdateThread();

	Corgi::GrowingArray<bool, long long> myResourceReadyStatus;
	Corgi::GrowingArray<std::string, long long> myFailedModelPaths;
	Corgi::GrowingArray<std::string, long long> myResourcePathsAdded;
	Corgi::CMemoryPool<SModelWrapper, long long> myResources;
	Corgi::GrowingArray<long long, long long> myResourcesToCreate;
	Corgi::GrowingArray<long long, long long> 	myResourcesToCreateThreaded;
	std::unordered_map<std::string, std::string> myPredefinedShadersPS;
	std::unordered_map<std::string, std::string> myPredefinedShadersVS;

protected:
	CScene* myScene;


	ID3D11Device* myDeviceToForwardToLoader;
	bool myIsInited;

	std::mutex myResourcesMutex;
	std::mutex myResourcesReadyStatusMutex;
	std::mutex myResourcesToCreateMutex;
	std::mutex myResourcesAddedMutex;
	std::mutex myFailedModelPathsMutex;

	std::atomic<bool> myShouldEndThread;
	std::atomic<bool> myThreadIsDone;
	CModelLoader* myResourceLoader;
	CShaderManager& myShaderManager;
	CAnimationLoader& myAnimationLoader;

	long long myErrorCubeID;
};

