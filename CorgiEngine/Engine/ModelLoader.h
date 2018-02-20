#pragma once
#include "../Utilities/GrowingArray.h"
#include "../Utilities/Vector.h"
#include <string>
#include "GeometryStruct.h"

class CModel;
class CScene;
class CModelInstance;
class CFBXLoaderCustom;
struct ID3D11ShaderResourceView;
enum ModelLoaderSpace;
class CShaderManager;
struct aiScene;
class CAnimationLoader;
enum HexColor
{
	eNormalColor = 0xff7d7d,
	eWhite = 0xffffffff,
	eBlack = 0x00000000
};


class CModelLoader
{
	friend class CModelLoaderWrapper;
public:
	CModelLoader(CShaderManager& aShaderManager, CAnimationLoader& aAnimLoader);
	~CModelLoader();
	bool Init(ID3D11Device* aDevice);
	bool GetIsInited() { return myIsInited; };
	void Release();
	CModelInstance* CreateInstance();
	CModelInstance * CreateInstance2(long long aModelID);
	CModelInstance * CreateInstance(long long aModelID);
	CModelInstance * CreateInstance(const char * aModelPath);

	bool CreateModel2(CModel & aModelOut, const char * aModelPath, const char * aVSShader, const char * aPSShader);

	void CreateBoundingBox(aiScene* aScene, Corgi::Vector3<float>& boundingBoxMax, Corgi::Vector3<float>& boundingBoxMin);

	long long CreateModel(const char* aModelPath, const char* aVSShader, const char* aPSShader);
	bool CreateSkybox2(CModel & aModelOut, const char * aTexturePath);
	long long CreateSkybox(const char * aTexturePath);
	bool CreateCube2(CModel & aModelOut);
	long long CreateCube();
	long long CreateSphere();
	__declspec(deprecated("this WILL CRASH your graphicscard, to create a triangle please re-write this whole function"))long long CreateTriangle();
	void AssignScene(CScene* aScene);

	CModel& GetModel(long long aID);
	ID3D11ShaderResourceView* LoadTexture(std::string& aFilePath, int aType);
	ID3D11ShaderResourceView* CreateDefaultTexture(bool aIsChequered, int aErrorTextureColor);


	int GetAmountOfModels();
private:
	Corgi::GrowingArray<CModel, long long> myModels;
	Corgi::GrowingArray<std::string, long long> myLoadedModelPaths;
	Corgi::GrowingArray<std::string, long long> myFailedModelPaths;

	CAnimationLoader& myAnimationLoader;
	CShaderManager& myShaderManager;
	CFBXLoaderCustom* myLoader;
	ID3D11Device* myDevice;
	ID3D11ShaderResourceView* myChequeredErrorTexture;
	ID3D11ShaderResourceView* myFullBlackErrorTexture;
	ID3D11ShaderResourceView* myFullWhiteErrorTexture;
	CScene* myScene;
	bool myIsInited;

};

