#pragma once
#include "LightLoader.h"
#include "ModelLoader.h"

class CModelInstance;
class CEnvironmentLightInstance;
class CDXFramework;
class CCameraInstance;

struct ID3D11Buffer;
enum D3D_PRIMITIVE_TOPOLOGY;

class CForwardRenderer
{
public:
	CForwardRenderer(CModelLoader& aModelLoader, CLightLoader& aLightLoader);
	~CForwardRenderer();

	bool Init(CDXFramework* aDXFramework);
	void Render(CCameraInstance* aCameraInstance, Corgi::GrowingArray<CModelInstance*>& aModelsToRender, Corgi::GrowingArray<CPointLightInstance*>& aPointLightInstances, Corgi::GrowingArray<CSpotLightInstance*> aSpotLightInstances, CEnvironmentLightInstance* aEnvironmentLight);
	__declspec("do not use") void AssignFramework(CDXFramework* aFramework);

private:
	void CreateViewPositionBuffer(CDXFramework* aDXFramework);
	void CreatePointLightBuffer(CDXFramework* aDXFramework);
	void CreateSpotLightBuffer(CDXFramework* aDXFramework);

#ifndef _RETAIL
	void UpdateDebugFeatures();
#endif
	void UpdateViewPositionBuffer(CCameraInstance * aCameraInstance);
	void UpdatePointLightBuffer(Corgi::GrowingArray<CPointLightInstance*>& aPointLightInstances);
	void UpdateSpotLightBuffer(Corgi::GrowingArray<CSpotLightInstance*>& aPointLightInstances);

	struct SInstanceBufferData
	{
		Matrix44 myToWorld = Matrix44();
	};

	struct SViewPositionBufferData
	{
		Vector3f myToView = Vector3f();
		float myTrash;
	};



	struct SPointLightBufferData
	{
		unsigned int myNumberOfUsedPointLights;
		unsigned int trashInts[3];

		struct SPointLightRenderData
		{
			Vector4f myPosition;
			Vector3f myColor;
			float myRange;
		} pointLights[8];
	};

	struct SSpotLightBufferData
	{
		unsigned int myNumberOfUsedSpotLights;
		unsigned int trashInts[3];

		struct SSpotLightRenderData
		{
			Vector4f myPosition;
			Vector3f myDirection;
			float myRange;
			Vector3f myColor;
			float myAngle; 
		} spotLights[8];
	};

private:

	CDXFramework* myDXFramework;
	CModelLoader& myModelLoader;	//this will be modelpool when it exists
	CLightLoader& myLightLoader;	//this will be lightpool when it exists
	ID3D11Buffer* myInstanceBuffer;
	ID3D11Buffer* myViewPositionBuffer;
	ID3D11Buffer* myPointLightBuffer;
	ID3D11Buffer* mySpotLightBuffer;

	SViewPositionBufferData myViewData;
	D3D_PRIMITIVE_TOPOLOGY myTopology;
};

