#pragma once
#include "GBuffer.h"
#include "ShaderCompiler.h"

class CCameraInstance;
class CShaderManager;
class CPointLightInstance;
class CSpotLightInstance;
class CModelLoader;
class CLightLoader;
class CResourceManager;
class CModelInstance;
class CEnvironmentLightInstance;
class CDXFramework;
class CGBuffer;
enum D3D_PRIMITIVE_TOPOLOGY;
struct ID3D11Buffer;

class CDeferredRenderer
{
public:
	CDeferredRenderer(CModelLoader& aModelLoader, CLightLoader& aLightLoader, CResourceManager& aResourceManager, CShaderManager& myShaderManager, CGBuffer& myGBuffer);
	~CDeferredRenderer();

	bool Init(CDXFramework* aDXFramework);
	void FillBuffer(CCameraInstance* aCameraInstance, Corgi::GrowingArray<CModelInstance*>& aModelsToRender, CEnvironmentLightInstance* aEnvLight);
	void RenderLights(CCameraInstance* aCameraInstance, Corgi::GrowingArray<CPointLightInstance*> aPointLights, Corgi::GrowingArray<CSpotLightInstance*> aSpotLights, CEnvironmentLightInstance* aEnvLight, CGBuffer& aGBuffer );
#ifndef _RETAIL
	void UpdateDebugFeatures();
#endif

private:
	void CreateViewPositionBuffer(CDXFramework* aDXFramework);


	void UpdateViewPositionBuffer(CCameraInstance * aCameraInstance);

	struct SInstanceBufferData
	{
		Matrix44 myToWorld = Matrix44();
	};

	struct SShadowBufferData
	{
		Matrix44 myToLight = Matrix44();
		Matrix44 myLightProjection = Matrix44();
	};

	struct SViewPositionBufferData
	{
		Vector3f myToView = Vector3f(13.f, 37.f, 1.f );
		float myTrash;
	};

	struct SFullscreenVertex
	{
		Vector4f myPosition;
		Vector2f myUV;
	};

	struct SPointLightRenderData
	{
		Vector4f myPosition;
		Vector3f myColor;
		float myRange;
	};

	struct SSpotLightRenderData
	{
		Vector4f myPosition;
		Vector3f myDirection;
		float myRange;
		Vector3f myColor;
		float myAngle;
	};

	float myMaxDistance;

private:

	void CreateInstanceBuffers();
	void CreateDataCollectionShaders();
	void CreateFullscreenRenderingPlane();
	void CreateLightRenderingShaders();
	void CreatePointLightBuffer();
	void CreateSpotLightBuffer();
	void CreateBoneBuffer();

	void CreateShadowResources();
	void UpdateShadowMVPMatrix(const Vector3f & aLightInvDirection, const Vector3f& cameraPosition);


	void UpdateInstanceBuffer(CModelInstance* aInstance);
	void UpdatePointLightBuffer(SPointLightRenderData& aPLightDataToSet);
	void UpdateSpotLightBuffer(SSpotLightRenderData& aSLightDataToSet);
	void UpdateBoneBuffer(Corgi::Matrix44<float>* aBoneDataToSet);





private:

	Corgi::GrowingArray<Corgi::Matrix44<float>> myAnimationBuffer;


	CDXFramework* myDXFramework;

	CResourceManager& myResourceManager;
	CShaderManager& myShaderManager;

	CModelLoader& myModelLoader;	//this will be modelpool when it exists
	CLightLoader& myLightLoader;	//this will be lightpool when it exists
	
	ID3D11Buffer* myInstanceBuffer;
	ID3D11Buffer* myCockpitInstanceBuffer;
	ID3D11Buffer* myViewPositionBuffer;
	ID3D11Buffer* myPointLightBuffer;
	ID3D11Buffer* mySpotLightBuffer;
	ID3D11Buffer* myBoneBuffer;
	
	//Data Collection
	long long myDataCollectionPixelShader;
	long long myCockpitDataCollectionVertexShader;
	long long myDataCollectionVertexShader;
	CGBuffer& myGBuffer;
	//

	//Light Rendering
	long long myFullscreenVertexShader;
	long long myEnvironmentLightShader;
	long long myPointLightShader;
	long long mySpotLightShader;
	SVertexDataWrapper myVertexData;
	//
	
	//Shadows
	SShadowBufferData myBufferData;
	CFullscreenTexture myShadowMap;
	long long myShadowMapVertexShaderID;
	long long myShadowMapPixelShaderID;
	long long myShadowMapAnimatedVertexShaderID;
	long long myShadowEnvironmentLightID;
	ID3D11Buffer* myMVPShadowBuffer;
	Corgi::GrowingArray<CModelInstance*> myModelInstanceBuffer;


	SViewPositionBufferData myViewData;
	D3D_PRIMITIVE_TOPOLOGY myTopology;

	CShaderCompiler myShaderCompiler;
};


