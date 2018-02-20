#pragma once
#include "ParticleEmitterLoader.h"
#include "FullscreenTexture.h"
#include "GBuffer.h"
#include "ResourceManager.h"

#include "BlendStateManager.h"
#include "ForwardRenderer.h"
#include "SpriteRenderer.h"
#include "FullscreenRenderer.h"
#include "ParticleRenderer.h"
#include "DeferredRenderer.h"

class CEnvironmentLightInstance;

class CTextManager;
class CSpriteLoader;
class CLightLoader;
class CModelLoader;

class CDXFramework;
struct ID3D11Resource;
struct ID3D11Buffer;

struct ID3D11DepthStencilState;
class CGraphicsEngine
{
public:
	CGraphicsEngine(CModelLoader& aModelLoader, CLightLoader& aLightLoader, CSpriteLoader& aSpriteLoader, CResourceManager& aResourceManager, CTextManager& aTextManager, CShaderManager& aShaderManager);
	~CGraphicsEngine();

	bool Init(CDXFramework* aFrameWork, const Vector2int& aWindowSize);
	void BeginFrame();
	void EndFrame();

	ID3D11Resource * GetLastFrameTexture();
	void RenderDeferred(CCameraInstance* aCamera, CScene* aScene);
	void RenderLights(CCameraInstance* aCamera, CScene* aScene);
	void RenderSprites();
	void RenderSkybox(CCameraInstance* aCameraInstance, long long aSkyboxID);
	void SetWorldLight(CEnvironmentLightInstance* aCurrentWorldLight);

	void BloomEffect();
	void CombineAllTextures();
	void ColorGrading();
	void SetParticleManager(CParticleManager* aParticleManager);
	void RenderDebug(CCameraInstance* aCamera);
	void RenderParticles(CCameraInstance* aCamera);
	void RenderText();

	CSpriteRenderer& GetSpriteRenderer();

	CBlendStateManager& GetBlendStateManager();

private:
	bool InitializeFullScreenTextures(const Vector2int& aWindowSize, const char* aFailedTexture);
	bool InitiateDepthStencil();
	void CreateViewPositionBuffer();
	void RenderModels(CCameraInstance* aCamera, CScene* aScene);
	void SetEmissiveTexture(long long aModelID, ID3D11ShaderResourceView* aViewToSet); //for render to albedo
private:
	CBlendStateManager myBlendStateManager;
	CForwardRenderer myForwardRenderer;
	CSpriteRenderer mySpriteRenderer;
	CFullscreenRenderer myFullscreenRenderer;
	CParticleRenderer myParticleRenderer;
	CDeferredRenderer myDeferredRenderer;

	CShaderManager& myShaderManager;
	CTextManager& myTextManager;

	CEnvironmentLightInstance* myEnvironmentLight;
	ID3D11DepthStencilState* myNoDepthStencilState;
	ID3D11DepthStencilState* myReadDepthStencilState;
	ID3D11DepthStencilState* myAllDepthStencilState;

	ID3D11DepthStencilView* mySpriteDepthStencilView;

	CGBuffer myGBuffer;

	//skybox shit
	CResourceManager& myResourceManager;
	CModelLoader& myModelLoader;
	ID3D11Buffer* myViewPositionBuffer;
	struct SViewPositionBufferData
	{
		Vector3f myToView = Vector3f();
		float myTrash;
	} myViewData;
	//end
	CFullscreenTexture myBackBuffer;
	CFullscreenTexture myHDRIntermediateTexture;
	CFullscreenTexture myDeferredLightRenderingTexture;
	CFullscreenTexture mySkyboxTexture;
	CFullscreenTexture myColorGradingTexture;
	CFullscreenTexture myLDRIntermediateTexture;



	Vector2int myActiveTargetSize;

	float myClearColor[4];
	float myDebugBleedClearColor[4];
	
	CDXFramework* myFrameWork;
	ID3D11Resource* myLastBackBuffer;

	Corgi::GrowingArray<CPointLightInstance*> myPointLightContainer;
	Corgi::GrowingArray<CSpotLightInstance*> mySpotLightContainer;
	Corgi::GrowingArray<CModelInstance*> myModelInstanceContainer;


private:

	struct BloomTextures
	{
		CFullscreenTexture half;
		CFullscreenTexture quarter1;
		CFullscreenTexture quarter2;
	} myBloomTextures;
};

