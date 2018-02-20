#include "stdafx.h"
#include "GraphicsEngine.h"

#include "ModelLoaderWrapper.h"
#include "ShaderManager.h"
#include "EnvironmentLight.h"
#include "EnvironmentLightInstance.h"

#include "CameraInstance.h"
#include "..\Collision\TriggerVolumeManager.h"

#define GRAPHICSENGINE_CULLEDUNICMODELS_ARRAYSIZE 10000
//TODO MOVE ALL STATES TO BLENDSTATEMANAGER AND RENAME BLENDSTATE MANAGER TO STATEMANAGER


#define HDR

CGraphicsEngine::CGraphicsEngine(CModelLoader& aModelLoader, CLightLoader& aLightLoader, CSpriteLoader& aSpriteLoader, CResourceManager& aResourceManager, CTextManager& aTextManager, CShaderManager& aShaderManager) :
	myForwardRenderer(aModelLoader, aLightLoader),
	myDeferredRenderer(aModelLoader, aLightLoader, aResourceManager, aShaderManager, myGBuffer),
	mySpriteRenderer(aSpriteLoader),
	myModelLoader(aModelLoader),
	myTextManager(aTextManager),
	myResourceManager(aResourceManager),
	myShaderManager(aShaderManager),
	myParticleRenderer(aShaderManager),
	myFullscreenRenderer(aShaderManager)
{
}


CGraphicsEngine::~CGraphicsEngine()
{
	myNoDepthStencilState->Release();
	myNoDepthStencilState = nullptr;
	myReadDepthStencilState->Release();
	myReadDepthStencilState = nullptr;
	myAllDepthStencilState->Release();
	myAllDepthStencilState = nullptr;
	if (mySpriteDepthStencilView != nullptr)
	{
		mySpriteDepthStencilView->Release();
		mySpriteDepthStencilView = nullptr;
	}
	myViewPositionBuffer->Release();
	myViewPositionBuffer = nullptr;
	myLastBackBuffer->Release();
	myLastBackBuffer = nullptr;
}




bool CGraphicsEngine::Init(CDXFramework* aFrameWork, const Vector2int& aWindowSize)
{
	myPointLightContainer.Init(1024);
	mySpotLightContainer.Init(1024);
	myModelInstanceContainer.Init(GRAPHICSENGINE_CULLEDUNICMODELS_ARRAYSIZE);
	myFrameWork = aFrameWork;
	myFrameWork->SetBackBuffer(myBackBuffer);
	myClearColor[0] = 0;
	myClearColor[1] = 0;
	myClearColor[2] = 0;
	myClearColor[3] = 0;


	myDebugBleedClearColor[0] = 0.6f;
	myDebugBleedClearColor[1] = 0.2f;
	myDebugBleedClearColor[2] = 0.8f;
	myDebugBleedClearColor[3] = 0.f;
	//myBackBuffer.Init({ aWindowSize.x, aWindowSize.y }, DXGI_FORMAT_R8G8B8A8_UNORM, true);
	DL_INITCHECK(myBlendStateManager.Init(aFrameWork), "BlendStateManager", "GraphicsEngine.cpp");
	DL_INITCHECK(myFullscreenRenderer.Init(myFrameWork), "FullScreenRenderer", "GraphicsEngine.cpp");
	const char* failedTexture = "";
	DL_INITCHECK(InitializeFullScreenTextures(aWindowSize, failedTexture), failedTexture, "GraphicsEngine.cpp");
	DL_INITCHECK(myForwardRenderer.Init(myFrameWork), "myForwardRenderer", "GraphicsEngine.cpp");
	DL_INITCHECK(myDeferredRenderer.Init(myFrameWork), "myDeferredRenderer", "GraphicsEngine.cpp");
	DL_INITCHECK(mySpriteRenderer.Init(myFrameWork), "mySpriteRenderer", "GraphicsEngine.cpp");
	DL_INITCHECK(myParticleRenderer.Init(myFrameWork), "myParticleRenderer", "GraphicsEngine.cpp");
	DL_INITCHECK(InitiateDepthStencil(), "DepthStencil", "GraphicsEngine.cpp");

	CreateViewPositionBuffer();
	return true;
}

void CGraphicsEngine::BeginFrame()
{
	myBackBuffer.ClearTexture(myClearColor);
	myHDRIntermediateTexture.ClearTexture(myClearColor);
	myGBuffer.ClearTexture(myClearColor);
	mySkyboxTexture.ClearTexture(myDebugBleedClearColor);
#ifndef _RETAIL
	myDeferredRenderer.UpdateDebugFeatures();
#endif
}

void CGraphicsEngine::EndFrame()
{
	myBackBuffer.GetRenderTarget()->GetResource(&myLastBackBuffer);
	ID3D11ShaderResourceView* clear = nullptr;
	myFrameWork->GetContext()->PSSetShaderResources(0, 1, &clear);
	myFrameWork->GetContext()->PSSetShaderResources(1, 1, &clear);
	myFrameWork->GetContext()->PSSetShaderResources(2, 1, &clear);
	myFrameWork->GetContext()->PSSetShaderResources(3, 1, &clear);
	myFrameWork->GetContext()->PSSetShaderResources(4, 1, &clear);
	myFrameWork->GetContext()->PSSetShaderResources(5, 1, &clear);
	
}

void CGraphicsEngine::CombineAllTextures()
{
	myBlendStateManager.SetBlendState(EBlendState_Alphablend);

	myHDRIntermediateTexture.SetAsActiveTarget();
	myFrameWork->GetContext()->OMSetDepthStencilState(myNoDepthStencilState, 1);
	myFullscreenRenderer.Render(CFullscreenRenderer::EEffect_CombineTextures, { &mySkyboxTexture, nullptr });
	myFullscreenRenderer.Render(CFullscreenRenderer::EEffect_CombineTextures, { &myDeferredLightRenderingTexture, nullptr });
	myBlendStateManager.SetBlendState(EBlendState_Disabled);
	myFrameWork->GetContext()->OMSetDepthStencilState(myAllDepthStencilState, 1);

}


void CGraphicsEngine::SetParticleManager(CParticleManager * aParticleManager)
{
	myParticleRenderer.SetParticleManager(aParticleManager);
}

bool CGraphicsEngine::InitializeFullScreenTextures(const Vector2int& aWindowSize, const char* aFailedTexture)
{
	if (!myGBuffer.Init({ aWindowSize.x, aWindowSize.y }, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R8G8B8A8_UNORM, 6, true))
	{
		aFailedTexture = "myGBuffer";
		return false;
	}

#ifdef HDR
	enum DXGI_FORMAT HDR_FORMAT = DXGI_FORMAT_R32G32B32A32_FLOAT;
#else
	enum DXGI_FORMAT HDR_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;
#endif


	if (!myBloomTextures.half.Init({ aWindowSize.x / 2, aWindowSize.y / 2 }, HDR_FORMAT))
	{
		aFailedTexture = "myBloomTextures.half";
		return false;
	}
	if (!myBloomTextures.quarter1.Init({ aWindowSize.x / 4, aWindowSize.y / 4 }, HDR_FORMAT))
	{
		aFailedTexture = "myBloomTextures.quarter1";
		return false;
	}
	if (!myBloomTextures.quarter2.Init({ aWindowSize.x / 4, aWindowSize.y / 4 }, HDR_FORMAT))
	{
		aFailedTexture = "myBloomTextures.quarter2";
		return false;
	}
	if (!myHDRIntermediateTexture.Init(aWindowSize, HDR_FORMAT))
	{
		aFailedTexture = "myHDRIntermediateTexture";
		return false;
	}
	if (!myLDRIntermediateTexture.Init(aWindowSize, HDR_FORMAT))
	{
		aFailedTexture = "myLDRIntermediateTexture";
		return false;
	}
	if (!mySkyboxTexture.Init(aWindowSize, DXGI_FORMAT_R8G8B8A8_UNORM))
	{
		aFailedTexture = "mySkyboxTexture";
		return false;
	}
	if (!myDeferredLightRenderingTexture.Init(aWindowSize, DXGI_FORMAT_R8G8B8A8_UNORM))
	{
		aFailedTexture = "myDeferredLightRenderingTexture";
		return false;
	}
	if (!myColorGradingTexture.Init(aWindowSize, DXGI_FORMAT_R8G8B8A8_UNORM))
	{
		aFailedTexture = "myColorGradingTexture";
		return false;
	}

	return true;
}

bool CGraphicsEngine::InitiateDepthStencil()
{
	HRESULT result = S_OK;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = myFrameWork->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &myAllDepthStencilState);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "Depth stencil error");
		return false;
	}

	myFrameWork->GetContext()->OMSetDepthStencilState(myAllDepthStencilState, D3D11_DEPTH_WRITE_MASK_ALL);


	result = S_OK;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = myFrameWork->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &myReadDepthStencilState);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "Depth stencil error");
		return false;
	}

	myFrameWork->GetContext()->OMSetDepthStencilState(myReadDepthStencilState, 1);


	result = S_OK;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = myFrameWork->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &myNoDepthStencilState);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "Depth stencil error");
		return false;
	}

	myFrameWork->GetContext()->OMSetDepthStencilState(myReadDepthStencilState, 1);



	return true;
}

void CGraphicsEngine::RenderSprites()
{
	myBlendStateManager.SetBlendState(EBlendState_SpriteBlend);
	myFrameWork->GetContext()->OMSetDepthStencilState(myNoDepthStencilState, 1);
	mySpriteRenderer.Render(myActiveTargetSize);
	myFrameWork->GetContext()->OMSetDepthStencilState(myAllDepthStencilState, 1);

	myBlendStateManager.SetDefaultState();
}

void CGraphicsEngine::CreateViewPositionBuffer()
{
	ID3D11Device* device = myFrameWork->GetDevice();
	ID3D11Buffer* viewBuffer;
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = sizeof(SViewPositionBufferData);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = device->CreateBuffer(&bufferDescription, nullptr, &viewBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "ForwardRenderer failed to create ViewPositionBuffer");
		if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Engine, "%s", "invalidArgument !?");
		}
	}
	myViewPositionBuffer = viewBuffer;
}

void CGraphicsEngine::RenderSkybox(CCameraInstance * aCameraInstance, long long aSkyboxID)
{
	if (myResourceManager.GetModelLoaderWrapper()->IsModelReadyToUse(aSkyboxID) == false)
	{
		return;
	}

	mySkyboxTexture.SetAsActiveTarget();
	myFrameWork->GetContext()->OMSetDepthStencilState(myNoDepthStencilState, 1);
	ID3D11DeviceContext* context = myFrameWork->GetContext();

	aCameraInstance->GetCamera()->UpdateBuffers(myFrameWork, aCameraInstance);
	ID3D11Buffer* cameraBuffer = aCameraInstance->GetCamera()->GetBuffer();
	context->VSSetConstantBuffers(0, 1, &cameraBuffer);

	myViewData.myToView = aCameraInstance->GetPosition();
	D3D11_MAPPED_SUBRESOURCE data;
	HRESULT result;
	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = context->Map(myViewPositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "Camera failed to map buffers in UpdateViewPositionBuffers");
	}

	memcpy(data.pData, &myViewData, sizeof(myViewData));
	context->Unmap(myViewPositionBuffer, 0);

	context->VSSetConstantBuffers(1, 1, &myViewPositionBuffer);

	CModel& model = myResourceManager.GetModelLoaderWrapper()->GetModel(aSkyboxID);
	//CModel& model = myModelLoader.GetModel(aSkyboxID);

	const SVertexDataWrapper& vertexData = model.GetVertexData();
	const SShaderDataWrapper& shaderData = model.GetShaderData();
	const SLayoutDataWrapper& layoutData = model.GetLayoutData();
	const STextureDataWrapper& textureData = model.GetTextureData();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(layoutData.myInputLayout);
	context->IASetVertexBuffers(0, 1, &vertexData.myVertexBuffer, &vertexData.myStride, &vertexData.myOffset);
	context->IASetIndexBuffer(vertexData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	context->VSSetShader(shaderData.myVertexShader, nullptr, 0);

	context->PSSetShaderResources(0, 1, &textureData.myAlbedo);
	context->PSSetShader(shaderData.myPixelShader, nullptr, 0);

	context->DrawIndexed(vertexData.myNumberOfIndices, 0, 0);
	myFrameWork->GetContext()->OMSetDepthStencilState(myAllDepthStencilState, 1);
}

void CGraphicsEngine::SetWorldLight(CEnvironmentLightInstance * aCurrentWorldLight)
{
	myEnvironmentLight = aCurrentWorldLight;
}

void CGraphicsEngine::RenderDeferred(CCameraInstance* aCamera, CScene* aScene)
{
	RenderModels(aCamera, aScene);
}

void CGraphicsEngine::RenderModels(CCameraInstance* aCamera, CScene* aScene)
{
	myModelInstanceContainer.RemoveAll();
	aScene->Cull(myModelInstanceContainer);
	myDeferredRenderer.FillBuffer(aCamera, myModelInstanceContainer, myEnvironmentLight);
}



void CGraphicsEngine::RenderLights(CCameraInstance * aCamera, CScene * aScene)
{
	mySpotLightContainer.RemoveAll();
	myPointLightContainer.RemoveAll();

	aScene->CullPointLightInstances(myPointLightContainer);
	aScene->CullSpotLightInstances(mySpotLightContainer);

	float black[4];
	black[0] = 0.f;
	black[1] = 0.f;
	black[2] = 0.f;
	black[3] = 0.f;

	myDeferredLightRenderingTexture.ClearTexture(black);
	myDeferredLightRenderingTexture.SetAsActiveTarget();
	myBlendStateManager.SetBlendState(EBlendState_Additativeblend);
	myDeferredRenderer.RenderLights(aCamera, myPointLightContainer, mySpotLightContainer, myEnvironmentLight, myGBuffer);
	myBlendStateManager.SetDefaultState();
	myActiveTargetSize = myDeferredLightRenderingTexture.GetSize();

}

void CGraphicsEngine::RenderParticles(CCameraInstance* aCamera)
{
	myDeferredLightRenderingTexture.SetAsActiveTarget(myGBuffer.GetDepthStencilView()); //without the gbuffers depthstencil the particles will render ontop of everything (not good)
	myBlendStateManager.SetBlendState(EBlendState_Alphablend);
	myFrameWork->GetContext()->OMSetDepthStencilState(myReadDepthStencilState, 1);
	myParticleRenderer.Render(aCamera);
	myFrameWork->GetContext()->OMSetDepthStencilState(myAllDepthStencilState, 1);
	myBlendStateManager.SetDefaultState();
}

void CGraphicsEngine::RenderText()
{
	myFrameWork->GetContext()->OMSetDepthStencilState(myNoDepthStencilState, 1);
	myBlendStateManager.SetBlendState(EBlendState_Alphablend);
	myTextManager.Render();
	myBlendStateManager.SetBlendState(EBlendState_Disabled);
	myFrameWork->GetContext()->OMSetDepthStencilState(myAllDepthStencilState, 1);

}

CSpriteRenderer & CGraphicsEngine::GetSpriteRenderer()
{
	return mySpriteRenderer;
}

CBlendStateManager & CGraphicsEngine::GetBlendStateManager()
{
	return myBlendStateManager;
	// TODO: insert return statement here
}

void CGraphicsEngine::SetEmissiveTexture(long long aModelID, ID3D11ShaderResourceView* aViewToSet)
{
	CModel& screen = myModelLoader.GetModel(aModelID);
	screen.SetTextureEmissive(aViewToSet);
}

void CGraphicsEngine::RenderDebug(CCameraInstance* aCamera)
{
	aCamera;

#ifndef _RETAIL
	myBackBuffer.SetAsActiveTarget(myGBuffer.GetDepthStencilView()); //without the gbuffers depthstencil the Lines will render ontop of everything (not good)
	myFrameWork->GetContext()->OMSetDepthStencilState(myReadDepthStencilState, 1);
	CDebugDrawer::GetInstance().RenderLines(aCamera);
	myFrameWork->GetContext()->OMSetDepthStencilState(myAllDepthStencilState, 1);
#endif // !_RETAIL
}

void CGraphicsEngine::ColorGrading()
{
	myBackBuffer.SetAsActiveTarget();
	myFullscreenRenderer.Render(CFullscreenRenderer::EEffect_ColorGrading, { &myColorGradingTexture, nullptr });
	myActiveTargetSize = myBackBuffer.GetSize();
}

void CGraphicsEngine::BloomEffect()
{
	CombineAllTextures();
	myBloomTextures.half.SetAsActiveTarget();
	myFullscreenRenderer.Render(CFullscreenRenderer::EEffect_BloomLuminance, { &myHDRIntermediateTexture, nullptr });

	myBloomTextures.quarter1.SetAsActiveTarget();
	myFullscreenRenderer.Render(CFullscreenRenderer::EEffect_Copy, { &myBloomTextures.half, nullptr });

	myBloomTextures.quarter2.SetAsActiveTarget();
	myFullscreenRenderer.Render(CFullscreenRenderer::EEffect_GaussianBlurVer, { &myBloomTextures.quarter1, nullptr });

	myBloomTextures.quarter1.SetAsActiveTarget();
	myFullscreenRenderer.Render(CFullscreenRenderer::EEffect_GaussianBlurHor, { &myBloomTextures.quarter2, nullptr });

	myBloomTextures.quarter2.SetAsActiveTarget();
	myFullscreenRenderer.Render(CFullscreenRenderer::EEffect_GaussianBlurVer, { &myBloomTextures.quarter1, nullptr });

	myBloomTextures.quarter1.SetAsActiveTarget();
	myFullscreenRenderer.Render(CFullscreenRenderer::EEffect_GaussianBlurHor, { &myBloomTextures.quarter2, nullptr });




#ifdef HDR

	myLDRIntermediateTexture.SetAsActiveTarget();
	myFullscreenRenderer.Render(CFullscreenRenderer::EEffect_BloomAdd, { &myHDRIntermediateTexture, &myBloomTextures.quarter1 });

	myColorGradingTexture.SetAsActiveTarget();
	myFullscreenRenderer.Render(CFullscreenRenderer::EEffect_Tonemap, { &myLDRIntermediateTexture, nullptr });
#else
	myColorGradingTexture.SetAsActiveTarget();
	myFullscreenRenderer.Render(CFullscreenRenderer::EEffect_BloomAdd, { &myHDRIntermediateTexture, &myBloomTextures.quarter1 });
#endif





}

ID3D11Resource * CGraphicsEngine::GetLastFrameTexture()
{
	return myLastBackBuffer;
}
