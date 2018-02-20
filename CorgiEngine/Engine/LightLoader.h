#pragma once

class CEnvironmentLight;
class CEnvironmentLightInstance;
class CPointLight;
class CPointLightInstance;
class CSpotLight;
class CSpotLightInstance;
class CScene;
class CDXFramework;

struct SPointLightData;
struct SSpotLightData;

enum ModelLoaderSpace;
enum PointLightType
{
	ePLight_Error,
	ePLight_Red,
	ePLight_Green,
	ePLight_Blue,
	ePLight_Cockpit,
	ePlight_count
};
class CLightLoader
{
public:
	CLightLoader();
	~CLightLoader();

	bool Init(CDXFramework* aFrameWork, CScene* aScene);


	CEnvironmentLightInstance* CreateEnvironmentLightInstance();
	CEnvironmentLightInstance* CreateEnvironmentLightInstance(long long aLightID, Vector4f aDirection, Vector4f aColor);
	CEnvironmentLightInstance* CreateEnvironmentLightInstance(const char* aCubeMapPath, Vector4f aDirection, Vector4f aColor);

	CPointLightInstance * CreatePointLightInstance(long long aID, Vector3f & aPos);
	CPointLightInstance* CreatePointLightInstance(long long aID, Vector4f& aPos);
	CPointLightInstance* CreatePointLightInstance(long long aID);
	CSpotLightInstance * CreateSpotLightInstance(long long aID, Vector3f & aPos, Vector3f& aDir);
	CSpotLightInstance * CreateSpotLightInstance(long long aID);
	CSpotLightInstance* CreateSpotLightInstance(long long aID, Vector4f& aPos, Vector4f& aDir);

	long long CreateEnvironmentLight(const char* aCubeMapPath);
	long long CreatePointLight(const SPointLightData& aData);
	long long CreateSpotLight(const SSpotLightData& aData);

	void AssignFrameWork(CDXFramework* aFrameWork);
	//void AssignScene(CScene* aScene, ModelLoaderSpace aSpace);
	void LoadLights();
	CEnvironmentLight& GetEnvironmentLight(long long aID);
	CPointLight& GetPointLight(long long aID);
	CSpotLight& GetSpotLight(long long aID);

	void Reset();
private:
	void LoadPointLights();
	Corgi::GrowingArray<CEnvironmentLight, long long> myEnvironmentLights;
	Corgi::GrowingArray<std::string, long long> myCubemapPaths;

	Corgi::GrowingArray<CPointLight, long long> myPointLights;

	Corgi::GrowingArray<CSpotLight, long long> mySpotLights;

	CDXFramework* myFramework;

	CScene* myScene;
};

