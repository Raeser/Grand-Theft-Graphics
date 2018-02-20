#include "stdafx.h"
#include "DeferredRenderer.h"
#include "EnvironmentLightInstance.h"
#include "EnvironmentLight.h"
#include "PointLight.h"
#include "PointLightInstance.h"
#include "SpotLight.h"
#include "SpotLightInstance.h"
#include "FullscreenTexture.h"
#include "ModelLoaderWrapper.h"

#include "LightLoader.h"
#include "ResourceManager.h"
#include "CameraInstance.h"

#include "CameraInstance.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "AnimationInstance.h"

#ifndef _RETAIL

void CDeferredRenderer::UpdateDebugFeatures()
{
	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::Shift))
	//{
	//	//if (InputManager::GetInstance().CheckKeyDown(KeyCode::L))
	//	//{
	//	//	myTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	//	//}
	//	//else if (InputManager::GetInstance().CheckKeyDown(KeyCode::T))
	//	//{
	//	//	myTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//	//}
	//}
}
#endif // !_RETAIL



CDeferredRenderer::CDeferredRenderer(CModelLoader& aModelLoader, CLightLoader& aLightLoader, CResourceManager& aResourceManager, CShaderManager& aShaderManager, CGBuffer& aGBuffer) :
	myModelLoader(aModelLoader),
	myLightLoader(aLightLoader),
	myResourceManager(aResourceManager),
	myShaderManager(aShaderManager),
	myGBuffer(aGBuffer)
{
	myAnimationBuffer.Init(64);
	for (int i = 0; i < 64; ++i)
	{
		myAnimationBuffer.Add(Corgi::Matrix44<float>());
	}
}


CDeferredRenderer::~CDeferredRenderer()
{
	myInstanceBuffer->Release();
	myCockpitInstanceBuffer->Release();
	myViewPositionBuffer->Release();
	myPointLightBuffer->Release();
	mySpotLightBuffer->Release();
	myBoneBuffer->Release();

	//Light Rendering
	myVertexData.myIndexBuffer->Release();
	myVertexData.myVertexBuffer->Release();

}

bool CDeferredRenderer::Init(CDXFramework* aDXFramework)
{
	myMaxDistance = 4000;
	myMaxDistance *= myMaxDistance;

	myDXFramework = aDXFramework;
	myTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	CreateShadowResources();
	CreateInstanceBuffers();
	CreateViewPositionBuffer(aDXFramework);
	CreateDataCollectionShaders();
	CreateLightRenderingShaders();
	CreateFullscreenRenderingPlane();
	CreatePointLightBuffer();
	CreateSpotLightBuffer();
	CreateBoneBuffer();
	return true;
}


void CDeferredRenderer::CreateViewPositionBuffer(CDXFramework* aDXFramework)
{
	ID3D11Device* device = aDXFramework->GetDevice();
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
		DL_ERROR(eDebugLogType::Render, "%s", "DeferredRenderer failed to create ViewPositionBuffer");
		if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Render, "%s", "invalidArgument !?");
		}
	}
	myViewPositionBuffer = viewBuffer;
}

void CDeferredRenderer::UpdateViewPositionBuffer(CCameraInstance * aCameraInstance)
{



	myViewData.myToView = aCameraInstance->GetPosition();
	myViewData.myTrash = Timer::GetInstance().GetTotalTime();

	ID3D11DeviceContext* context = myDXFramework->GetContext();

	D3D11_MAPPED_SUBRESOURCE data;
	HRESULT result;

	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = context->Map(myViewPositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "Camera failed to map buffers in UpdateViewPositionBuffers");
	}

	memcpy(data.pData, &myViewData, sizeof(myViewData));
	context->Unmap(myViewPositionBuffer, 0);
}

void CDeferredRenderer::CreateInstanceBuffers()
{
	HRESULT result;
	ID3D11Device* device = myDXFramework->GetDevice();

	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = sizeof(SInstanceBufferData);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	result = device->CreateBuffer(&bufferDescription, nullptr, &myInstanceBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "ForwardRenderer failed to create instancebuffer");
	}

	result = device->CreateBuffer(&bufferDescription, nullptr, &myCockpitInstanceBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "ForwardRenderer failed to create instancebuffer");
	}
}

void CDeferredRenderer::UpdateInstanceBuffer(CModelInstance * aInstance)
{
	D3D11_MAPPED_SUBRESOURCE data;
	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	HRESULT result;
	SInstanceBufferData instanceData;
	instanceData.myToWorld = aInstance->GetOrientation();
	result = myDXFramework->GetContext()->Map(myInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "Render did something wrong");
	}
	memcpy(data.pData, &instanceData, sizeof(instanceData));
	myDXFramework->GetContext()->Unmap(myInstanceBuffer, 0);

}

void CDeferredRenderer::CreateDataCollectionShaders()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	myDataCollectionVertexShader = myShaderManager.LoadShader("../Assets/Shaders/VS_DEFModel.hlsl", layout, 7);
	myDataCollectionPixelShader = myShaderManager.LoadShader("../Assets/Shaders/PS_DEFModel.hlsl",nullptr ,0);
}

void CDeferredRenderer::CreateLightRenderingShaders()
{

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	myFullscreenVertexShader = myShaderManager.LoadShader("../Assets/Shaders/VS_DEFFullscreenPass.hlsl", layout, 2);
	myEnvironmentLightShader = myShaderManager.LoadShader("../Assets/Shaders/PS_DEFEnvironmentLight.hlsl", nullptr, 0);
	myPointLightShader = myShaderManager.LoadShader("../Assets/Shaders/PS_DEFPointLight.hlsl", nullptr, 0);
	mySpotLightShader = myShaderManager.LoadShader("../Assets/Shaders/PS_DEFSpotLight.hlsl", nullptr, 0);
}

void CDeferredRenderer::CreatePointLightBuffer()
{
	ID3D11Device* device = myDXFramework->GetDevice();
	ID3D11Buffer* pLightBuffer;
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = sizeof(SPointLightRenderData);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = device->CreateBuffer(&bufferDescription, nullptr, &pLightBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "DeferredRenderer failed to create pointLightBuffer");
		if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Render, "%s", "invalidArgument !?");
		}
	}
	myPointLightBuffer = pLightBuffer;
}

void CDeferredRenderer::CreateSpotLightBuffer()
{
	ID3D11Device* device = myDXFramework->GetDevice();
	ID3D11Buffer* spotLightBuffer;
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = sizeof(SSpotLightRenderData);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = device->CreateBuffer(&bufferDescription, nullptr, &spotLightBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "DeferredRenderer failed to create SpotLightBuffer");
		if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Render, "%s", "invalidArgument !?");
		}
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Render, "%s", "DeferredRenderer Succeded to create SpotLightBuffer");
	}
	mySpotLightBuffer = spotLightBuffer;
}

void CDeferredRenderer::CreateBoneBuffer()
{
	ID3D11Device* device = myDXFramework->GetDevice();
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = sizeof(Corgi::Matrix44<float>) * 64;
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = device->CreateBuffer(&bufferDescription, nullptr, &myBoneBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "DeferredRenderer failed to create BoneBuffer");
		if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Render, "%s", "invalidArgument !?");
		}
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Render, "%s", "DeferredRenderer Succeded to create BoneBuffer");
	}
}

void CDeferredRenderer::CreateShadowResources()
{

	myShadowMap.Init({ 4000, 4000 }, DXGI_FORMAT_R32_FLOAT, true);
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	myShadowMapVertexShaderID = myShaderManager.LoadShader("../assets/shaders/VS_ShadowMap.hlsl", layout, 7);
	myShadowMapPixelShaderID = myShaderManager.LoadShader("../assets/shaders/PS_ShadowMap.hlsl", nullptr, 0);

	myShadowMapAnimatedVertexShaderID = myShaderManager.LoadShader("../assets/shaders/VS_ShadowMapAnimated.hlsl", layout, 7);

	D3D11_INPUT_ELEMENT_DESC layoutForEnvLight[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	myShadowEnvironmentLightID = myShaderManager.LoadShader("../assets/shaders/VS_DEFEnvironmentLight.hlsl", layoutForEnvLight, 2);
	
	ID3D11Device* device = myDXFramework->GetDevice();
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = sizeof(Corgi::Matrix44<float>) * 2;
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = device->CreateBuffer(&bufferDescription, nullptr, &myMVPShadowBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "DeferredRenderer failed to create ShadowMVP Buffer");
		if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Render, "%s", "invalidArgument !?");
		}
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Render, "%s", "DeferredRenderer Succeded to create ShadowMVP Buffer");
	}

}

void CDeferredRenderer::UpdateShadowMVPMatrix(const Vector3f& aLightInvDirection, const Vector3f& cameraPosition)
{
	aLightInvDirection;
	ID3D11DeviceContext* context = myDXFramework->GetContext();
	Matrix44 LightProjection = Matrix44(DirectX::XMMatrixPerspectiveFovLH(1.0f, (16.f / 9.f), 60.f, 200));// ::CreateOrthographicMatrix(-10, 10, -10, 10, -10, 20);
	Matrix44 ToLight = Matrix44::LookAt(aLightInvDirection, cameraPosition, Vector3f(0, 1, 0));
	
	D3D11_MAPPED_SUBRESOURCE data;
	HRESULT result;

	SShadowBufferData shadowData;
	shadowData.myLightProjection = LightProjection;
	shadowData.myToLight = ToLight;

	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = context->Map(myMVPShadowBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "Failed To Map ShadowMVP Matrix in DeferredRenderer");
	}
	memcpy(data.pData, &shadowData, sizeof(shadowData));
	context->Unmap(myMVPShadowBuffer, 0);
}

void CDeferredRenderer::UpdatePointLightBuffer(SPointLightRenderData & aPLightDataToSet)
{
	ID3D11DeviceContext* context = myDXFramework->GetContext();

	D3D11_MAPPED_SUBRESOURCE data;
	HRESULT result;

	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = context->Map(myPointLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "LightRendering Deferred failed to map buffers in UpdatePointLightBuffers");
	}
	memcpy(data.pData, &aPLightDataToSet, sizeof(aPLightDataToSet));
	context->Unmap(myPointLightBuffer, 0);
}

void CDeferredRenderer::UpdateSpotLightBuffer(SSpotLightRenderData & aSLightDataToSet)
{
	ID3D11DeviceContext* context = myDXFramework->GetContext();

	D3D11_MAPPED_SUBRESOURCE data;
	HRESULT result;

	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = context->Map(mySpotLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "LightRendering Deferred failed to map buffers in UpdatePointLightBuffers");
	}
	memcpy(data.pData, &aSLightDataToSet, sizeof(aSLightDataToSet));
	context->Unmap(mySpotLightBuffer, 0);
}

void CDeferredRenderer::UpdateBoneBuffer(Corgi::Matrix44<float>* aBoneDataToSet)
{
	ID3D11DeviceContext* context = myDXFramework->GetContext();

	D3D11_MAPPED_SUBRESOURCE data;
	HRESULT result;

	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = context->Map(myBoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "failed to Update Bonebuffer in UpdateBoneBuffer");
	}
	memcpy(data.pData, aBoneDataToSet, sizeof(Corgi::Matrix44<float>) * 64);
	context->Unmap(myBoneBuffer, 0);
}

void CDeferredRenderer::CreateFullscreenRenderingPlane()
{
	HRESULT result;

	//Create triangle vertices
	Vertex vertices[4] =
	{
		{ 1.0f, 1.0f, 0.0f,	1.f,		1.f,0.f },
		{ -1.0f, 1.0f, 0.0f, 1.f,		0.f,0.f },
		{ 1.0f, -1.0f, 0.0f, 1.f,		1.f,1.f },
		{ -1.0f, -1.0f, 0.0f, 1.f,		0.f,1.f }
	};
	unsigned int indices[6] =
	{
		0,2,1,
		3,1,2
	};

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = sizeof(vertices);
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA bufferSubResourceData = { 0 };
	bufferSubResourceData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer;
	result = myDXFramework->GetDevice()->CreateBuffer(&bufferDescription, &bufferSubResourceData, &vertexBuffer);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "VertexBuffer failed to load");
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Render, "%s", "VertexBuffer load succeded");
	}
	D3D11_BUFFER_DESC indexBufferDescription = {};
	indexBufferDescription.ByteWidth = sizeof(indices);
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferSubResourceData = { 0 };
	indexBufferSubResourceData.pSysMem = indices;

	ID3D11Buffer* indexBuffer;
	result = myDXFramework->GetDevice()->CreateBuffer(&indexBufferDescription, &indexBufferSubResourceData, &indexBuffer);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "IndexBuffer failed to load");
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Render, "%s", "IndexBuffer load succeded");
	}

	myVertexData.myNumberOfVertices = sizeof(vertices) / sizeof(Vertex);
	myVertexData.myNumberOfIndices = sizeof(indices) / sizeof(UINT);
	myVertexData.myStride = sizeof(Vertex);
	myVertexData.myOffset = 0;
	myVertexData.myVertexBuffer = vertexBuffer;
	myVertexData.myIndexBuffer = indexBuffer;	
}

void CDeferredRenderer::FillBuffer(CCameraInstance* aCameraInstance, Corgi::GrowingArray<CModelInstance*>& aModelsToRender, CEnvironmentLightInstance* aEnvLight)
{
	myGBuffer.SetAsActiveTarget();
	ID3D11DeviceContext* context = myDXFramework->GetContext();
	float clear[4] = { 0,0,0,0 };
	myShadowMap.ClearTexture(clear);

	ID3D11Buffer* cameraBuffer = aCameraInstance->myCamera->GetBuffer();
	context->VSSetConstantBuffers(0, 1, &cameraBuffer);
	UpdateViewPositionBuffer(aCameraInstance);
	context->PSSetConstantBuffers(1, 1, &myViewPositionBuffer);

	CEnvironmentLight& cubemap = myLightLoader.GetEnvironmentLight(aEnvLight->GetLightID());
	
	context->PSSetShaderResources(0, 1, &cubemap.GetEnvironmentLightBufferData().myCubeMap);

	CEnvironmentLight& environmentLight = myLightLoader.GetEnvironmentLight(aEnvLight->GetLightID());
	SDirectionLightBufferData directionalLightData;
	SLightInstanceData environmentLightInstanceData = aEnvLight->GetEnvironmentLightData();
	directionalLightData.myDirection = environmentLightInstanceData.myDirection.GetNormalized();
	directionalLightData.myColor = environmentLightInstanceData.myColor;
	environmentLight.UpdateBuffer(directionalLightData, context);

	SEnvironmentLightData environmentLightData = environmentLight.GetEnvironmentLightBufferData();
	context->VSSetConstantBuffers(5, 1, &environmentLightData.myDirectionalLightBuffer);

	UpdateShadowMVPMatrix(aEnvLight->GetEnvironmentLightData().myDirection.V3() + aCameraInstance->GetPosition(), aCameraInstance->GetPosition());

	SInstanceBufferData instanceData;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE data;
	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));

	for (CModelInstance* modelToRender : aModelsToRender)
	{
		if (modelToRender == nullptr)
		{
			continue;
		}
		long long modelID = modelToRender->GetModelId();
		CModel& model = myResourceManager.GetModelLoaderWrapper()->GetModel(modelID);


		//TODO: PROVISORICT !!!!!!!!
		if (model.ShouldCull())
		{
			if (!aCameraInstance->isPointInViewSpace(modelToRender->GetPosition(), 2.f))
			{
				continue;
			}
		}
		//TODO: PROVISORICT !!!!!!!!

		if (model.IsAnimated())
		{
			int boneCount = 0;
			if (modelToRender->IsAnimationReady())
			{
				const Corgi::GrowingArray<Corgi::Matrix44<float>>& AnimationTransforms = modelToRender->GetAnimationInstance()->GetCurrentTransforms();
				if (AnimationTransforms.Size() > 0)
				{
					for (int i = 0; i < AnimationTransforms.Size(); ++i)
					{
						myAnimationBuffer[i] = AnimationTransforms[i];
						++boneCount;
					}
				}
			}
			for (int i = boneCount; i < 64; ++i)
			{
				myAnimationBuffer[i] = Corgi::Matrix44<float>();
			}
			UpdateBoneBuffer(myAnimationBuffer.GetPureData());

		}

		const SVertexDataWrapper& vertexData = model.GetVertexData();
		const STextureDataWrapper& textureData = model.GetTextureData();
		const CVertexShader& VertexShaderToSet = myShaderManager.GetVertexShader(model.GetVertexShaderID());
		const CPixelShader& PixelShaderToSet = myShaderManager.GetPixelShader(model.GetPixelShaderID());
		const CGeometryShader& GeometryShaderToSet = myShaderManager.GetGeometryShader(modelToRender->GetGeometryShaderID());



		instanceData.myToWorld = modelToRender->GetOrientation();
		result = context->Map(myInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
		if (FAILED(result))
		{
			DL_ERROR(eDebugLogType::Render, "%s", "Render did something wrong");
		}
		memcpy(data.pData, &instanceData, sizeof(instanceData));
		context->Unmap(myInstanceBuffer, 0);


		context->IASetPrimitiveTopology(myTopology);
		context->IASetInputLayout(VertexShaderToSet.GetInputLayout());
		context->IASetVertexBuffers(0, 1, &vertexData.myVertexBuffer, &vertexData.myStride, &vertexData.myOffset);
		context->IASetIndexBuffer(vertexData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->VSSetConstantBuffers(2, 1, &myInstanceBuffer);
		context->VSSetConstantBuffers(3, 1, &myBoneBuffer);
		context->VSSetShader(VertexShaderToSet.GetShader(), nullptr, 0);

		context->GSSetShader(GeometryShaderToSet.GetShader(), nullptr, 0);

		context->PSSetShaderResources(1, 1, &textureData.myAlbedo);
		context->PSSetShaderResources(2, 1, &textureData.myNormalMap);
		context->PSSetShaderResources(3, 1, &textureData.myAmbientMap);
		context->PSSetShaderResources(4, 1, &textureData.myEmissiveMap);
		context->PSSetShaderResources(5, 1, &textureData.myMetalnessMap);
		context->PSSetShaderResources(6, 1, &textureData.myRoughnessMap);
		
		context->PSSetShader(PixelShaderToSet.GetShader(), nullptr, 0);

		context->DrawIndexed(vertexData.myNumberOfIndices, 0, 0);

		//Render This Model To ShadowMap
		myShadowMap.SetAsActiveTarget();
		
		context->VSSetConstantBuffers(4, 1, &myMVPShadowBuffer);
		context->PSSetConstantBuffers(4, 1, &myMVPShadowBuffer);
		
		long long VertexShader = myShadowMapVertexShaderID;
		if (model.IsAnimated())
		{
			VertexShader = myShadowMapAnimatedVertexShaderID;
		}
		const CVertexShader& ShadowVertexShaderToSet = myShaderManager.GetVertexShader(VertexShader);
		const CPixelShader& ShadowPixelShaderToSet = myShaderManager.GetPixelShader(myShadowMapPixelShaderID);
		context->VSSetShader(ShadowVertexShaderToSet.GetShader(), nullptr, 0);
		context->PSSetShader(ShadowPixelShaderToSet.GetShader(), nullptr, 0);
		
		context->DrawIndexed(vertexData.myNumberOfIndices, 0, 0);
		
		//set back GBuffer for next model
		myGBuffer.SetAsActiveTarget();
	}
}

void CDeferredRenderer::RenderLights(CCameraInstance * aCameraInstance, Corgi::GrowingArray<CPointLightInstance*> aPointLights, Corgi::GrowingArray<CSpotLightInstance*> aSpotLights, CEnvironmentLightInstance* aEnvLight, CGBuffer & aGBuffer)
{
	aGBuffer;
	aEnvLight;
	ID3D11DeviceContext* context = myDXFramework->GetContext();

	UpdateViewPositionBuffer(aCameraInstance);
	context->PSSetConstantBuffers(0, 1, &myViewPositionBuffer);
	context->PSSetConstantBuffers(2, 1, &myMVPShadowBuffer);
	context->PSSetConstantBuffers(3, 1, &myInstanceBuffer);
	ID3D11Buffer* cameraBuffer = aCameraInstance->GetCamera()->GetBuffer();
	context->VSSetConstantBuffers(4, 1, &cameraBuffer);
	myShadowMap.SetAsResourceOnSlot(6);
	//Render EnvironmentLight
	CEnvironmentLight& environmentLight = myLightLoader.GetEnvironmentLight(aEnvLight->GetLightID());
	SDirectionLightBufferData directionalLightData;
	SLightInstanceData environmentLightInstanceData = aEnvLight->GetEnvironmentLightData();
	directionalLightData.myDirection = environmentLightInstanceData.myDirection;
	directionalLightData.myColor = environmentLightInstanceData.myColor;
	environmentLight.UpdateBuffer(directionalLightData, context);
	SEnvironmentLightData environmentLightData = environmentLight.GetEnvironmentLightBufferData();
	context->PSSetConstantBuffers(1, 1, &environmentLightData.myDirectionalLightBuffer);


	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	const CVertexShader& EnvironmentShader = myShaderManager.GetVertexShader(myShadowEnvironmentLightID);
	const CVertexShader& fullscreenShader = myShaderManager.GetVertexShader(myFullscreenVertexShader);
	const CPixelShader& EnvironmentPixelShader = myShaderManager.GetPixelShader(myEnvironmentLightShader);

	context->IASetInputLayout(fullscreenShader.GetInputLayout());

	context->IASetVertexBuffers(0, 1, &myVertexData.myVertexBuffer, &myVertexData.myStride, &myVertexData.myOffset);
	context->IASetIndexBuffer(myVertexData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->VSSetShader(EnvironmentShader.GetShader(), nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	aGBuffer.SetAsResourceOnSlot(0);
	context->PSSetShader(EnvironmentPixelShader.GetShader(), nullptr, 0);

	context->DrawIndexed(myVertexData.myNumberOfIndices, 0, 0);
	//EnvironmentLightRendering finished

	const CPixelShader& PointLightPixelShader = myShaderManager.GetPixelShader(myPointLightShader);

	//Render PointLights
	for (CPointLightInstance* instance : aPointLights)
	{
		const CPointLight& pointLight = myLightLoader.GetPointLight(instance->GetID());
		SPointLightData pointLightData = pointLight.GetPointLightData();
		SPointLightRenderData pointLightRenderData;
		pointLightRenderData.myColor = pointLightData.myColor;
		pointLightRenderData.myRange = pointLightData.myRange;
		pointLightRenderData.myPosition = instance->GetPosition();

		UpdatePointLightBuffer(pointLightRenderData);

		context->PSSetConstantBuffers(1, 1, &myPointLightBuffer);
		context->PSSetShader(PointLightPixelShader.GetShader(), nullptr, 0);
		context->DrawIndexed(myVertexData.myNumberOfIndices, 0, 0);
	}

	//PointLight Rendering Finished

	const CPixelShader& spotLightShader = myShaderManager.GetPixelShader(mySpotLightShader);
	//Render SpotLights
	for (CSpotLightInstance* instance : aSpotLights)
	{
		const CSpotLight& spotLight = myLightLoader.GetSpotLight(instance->GetID());
		SSpotLightData spotLightData = spotLight.GetSpotLightData();
		SSpotLightRenderData spotLightRenderData;
		spotLightRenderData.myColor = spotLightData.myColor;
		spotLightRenderData.myRange = spotLightData.myRange;
		spotLightRenderData.myAngle = spotLightData.myAngle;
		Vector4f direction = instance->GetDirection();
		spotLightRenderData.myDirection = {direction.x, direction.y, direction.z};
		spotLightRenderData.myPosition = instance->GetPosition();
	
		UpdateSpotLightBuffer(spotLightRenderData);
	
		context->PSSetConstantBuffers(1, 1, &mySpotLightBuffer);
		context->PSSetShader(spotLightShader.GetShader(), nullptr, 0);
		context->DrawIndexed(myVertexData.myNumberOfIndices, 0, 0);
	}
	//SpotLight Rendering Finished
}




