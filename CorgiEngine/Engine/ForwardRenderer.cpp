#include "stdafx.h"
#include "ForwardRenderer.h"
#include "EnvironmentLightInstance.h"
#include "EnvironmentLight.h"
#include "PointLight.h"
#include "PointLightInstance.h"
#include "SpotLight.h"
#include "SpotLightInstance.h"

#include "CameraInstance.h"

#ifndef _RETAIL
#include "../Utilities/InputManager.h"
void CForwardRenderer::UpdateDebugFeatures()
{
	if (InputManager::GetInstance().CheckKeyDown(KeyCode::Shift))
	{
		if (InputManager::GetInstance().CheckKeyDown(KeyCode::V))
		{
			if (myTopology == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
			{
				myTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
			}
			else
			{
				myTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			}
		}
	}
}
#endif // !_RETAIL



CForwardRenderer::CForwardRenderer(CModelLoader& aModelLoader, CLightLoader& aLightLoader):
	myModelLoader(aModelLoader),
	myLightLoader(aLightLoader)
{
}


CForwardRenderer::~CForwardRenderer()
{
	myInstanceBuffer->Release();
	myInstanceBuffer = nullptr;
	myViewPositionBuffer->Release();
	myViewPositionBuffer = nullptr;
	myPointLightBuffer->Release();
	myPointLightBuffer = nullptr;
	mySpotLightBuffer->Release();
	mySpotLightBuffer = nullptr;
}

bool CForwardRenderer::Init(CDXFramework* aDXFramework)
{

 	HRESULT result;
	myTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
 	ID3D11Device* device = aDXFramework->GetDevice();
 
 	D3D11_BUFFER_DESC bufferDescription = {};
 	bufferDescription.ByteWidth = sizeof(SInstanceBufferData);
 	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
 	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
 	result = device->CreateBuffer(&bufferDescription, nullptr, &myInstanceBuffer);
 
 	if (FAILED(result))
 	{
		DL_ERROR(eDebugLogType::Render, "%s", "ForwardRenderer failed to create instancebuffer");
 		return false;
 	}
	CreateViewPositionBuffer(aDXFramework);
	CreatePointLightBuffer(aDXFramework);
	CreateSpotLightBuffer(aDXFramework);
	return true;
}

void CForwardRenderer::CreateViewPositionBuffer(CDXFramework* aDXFramework)
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
		DL_ERROR(eDebugLogType::Render, "%s", "ForwardRenderer failed to create ViewPositionBuffer");
		if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Render, "%s", "invalidArgument !?");
		}
	}
	myViewPositionBuffer = viewBuffer;
}

void CForwardRenderer::CreatePointLightBuffer(CDXFramework * aDXFramework)
{
	ID3D11Device* device = aDXFramework->GetDevice();
	ID3D11Buffer* pLightBuffer;
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = sizeof(SPointLightBufferData);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = device->CreateBuffer(&bufferDescription, nullptr, &pLightBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "ForwardRenderer failed to create pointLightBuffer");
		if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Render, "%s", "invalidArgument !?");
		}
	}
	myPointLightBuffer = pLightBuffer;
}

void CForwardRenderer::CreateSpotLightBuffer(CDXFramework * aDXFramework)
{
	ID3D11Device* device = aDXFramework->GetDevice();
	ID3D11Buffer* spotLightBuffer;
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = sizeof(SSpotLightBufferData);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = device->CreateBuffer(&bufferDescription, nullptr, &spotLightBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "ForwardRenderer failed to create SpotLightBuffer");
		if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Render, "%s", "invalidArgument !?");
		}
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Render, "%s", "ForwardRenderer Succeded to create SpotLightBuffer");
	}
	mySpotLightBuffer = spotLightBuffer;
}


void CForwardRenderer::UpdateViewPositionBuffer(CCameraInstance * aCameraInstance)
{
#ifndef _RETAIL
	UpdateDebugFeatures();
#endif // _RETAIL


	myViewData.myToView = aCameraInstance->GetPosition();
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

void CForwardRenderer::UpdatePointLightBuffer(Corgi::GrowingArray<CPointLightInstance*>& aPointLightInstances)
{
	ID3D11DeviceContext* context = myDXFramework->GetContext();

	D3D11_MAPPED_SUBRESOURCE data;
	HRESULT result;
	SPointLightBufferData dataToSet;

	dataToSet.myNumberOfUsedPointLights = min(static_cast<unsigned int>(aPointLightInstances.Size()), 8);
	for (unsigned int index = 0; index < dataToSet.myNumberOfUsedPointLights ; index++)
	{
		CPointLight& pointlight = myLightLoader.GetPointLight(aPointLightInstances[index]->GetID());
		dataToSet.pointLights[index].myPosition = aPointLightInstances[index]->GetPosition();
		Vector3f color = pointlight.GetPointLightData().myColor;
		dataToSet.pointLights[index].myColor = Vector3f(color.x, color.y, color.z);
		dataToSet.pointLights[index].myRange = pointlight.GetPointLightData().myRange;
	}


	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = context->Map(myPointLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "Model failed to map buffers in UpdatePointLightBuffers");
	}
	memcpy(data.pData, &dataToSet, sizeof(dataToSet));
	context->Unmap(myPointLightBuffer, 0);

}

void CForwardRenderer::UpdateSpotLightBuffer(Corgi::GrowingArray<CSpotLightInstance*>& aSpotLightInstances)
{
	ID3D11DeviceContext* context = myDXFramework->GetContext();

	D3D11_MAPPED_SUBRESOURCE data;
	HRESULT result;
	SSpotLightBufferData dataToSet;
	Vector4f directionSlave;
	Vector3f colorSlave;
	dataToSet.myNumberOfUsedSpotLights = min(static_cast<unsigned int>(aSpotLightInstances.Size()), 8);
	for (unsigned int index = 0; index < dataToSet.myNumberOfUsedSpotLights; index++)
	{
		CSpotLight& spotlight = myLightLoader.GetSpotLight(aSpotLightInstances[index]->GetID());
		dataToSet.spotLights[index].myPosition = aSpotLightInstances[index]->GetPosition();
		directionSlave = aSpotLightInstances[index]->GetDirection();
		dataToSet.spotLights[index].myDirection = {directionSlave.x,directionSlave.y,directionSlave.z };
		colorSlave = spotlight.GetSpotLightData().myColor;
		dataToSet.spotLights[index].myColor = Vector3f(colorSlave.x, colorSlave.y, colorSlave.z);
		dataToSet.spotLights[index].myRange = spotlight.GetSpotLightData().myRange;
		dataToSet.spotLights[index].myAngle = spotlight.GetSpotLightData().myAngle;
	}


	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = context->Map(mySpotLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "Model failed to map buffers in UpdateSpotLightBuffers");
	}
	memcpy(data.pData, &dataToSet, sizeof(dataToSet));
	context->Unmap(mySpotLightBuffer, 0);
}


void CForwardRenderer::Render(CCameraInstance* aCameraInstance, Corgi::GrowingArray<CModelInstance*>& aModelsToRender, Corgi::GrowingArray<CPointLightInstance*>& aPointLightInstances, Corgi::GrowingArray<CSpotLightInstance*> aSpotLightInstances, CEnvironmentLightInstance* aEnvironmentLight)
{

	ID3D11DeviceContext* context = myDXFramework->GetContext();
	
	CEnvironmentLight& environmentLight = myLightLoader.GetEnvironmentLight(aEnvironmentLight->GetLightID());
	SDirectionLightBufferData directionalLightData;
	SLightInstanceData environmentLightInstanceData = aEnvironmentLight->GetEnvironmentLightData();
	directionalLightData.myDirection = environmentLightInstanceData.myDirection;
	directionalLightData.myColor = environmentLightInstanceData.myColor;
	environmentLight.UpdateBuffer(directionalLightData, context);

	SEnvironmentLightData environmentLightData = environmentLight.GetEnvironmentLightBufferData();
	context->PSSetConstantBuffers(0, 1, &environmentLightData.myDirectionalLightBuffer);



	ID3D11Buffer* cameraBuffer = aCameraInstance->myCamera->GetBuffer();
	context->VSSetConstantBuffers(1, 1, &cameraBuffer);

	UpdateViewPositionBuffer(aCameraInstance);
	context->PSSetConstantBuffers(2, 1, &myViewPositionBuffer);
	SInstanceBufferData instanceData;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE data;
	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));

#ifdef _DEBUGGRAPHICS
	int amount = 0;
#endif

	for (int i = 0; i < aModelsToRender.Size(); ++i)
	{
#ifdef _DEBUGGRAPHICS
		++amount;
#endif
		UpdateSpotLightBuffer(aSpotLightInstances);
		UpdatePointLightBuffer(aPointLightInstances);
		if (aModelsToRender[i] == nullptr)
		{
			continue;
		}

		CModel& model = myModelLoader.GetModel(aModelsToRender[i]->GetModelId());
	
		//instance->myModel;
		const SVertexDataWrapper& vertexData = model.GetVertexData();
		const SShaderDataWrapper& shaderData = model.GetShaderData();
		const SLayoutDataWrapper& layoutData = model.GetLayoutData();
		const STextureDataWrapper& textureData = model.GetTextureData();

		instanceData.myToWorld = aModelsToRender[i]->GetOrientation();

		result = context->Map(myInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

		if (FAILED(result))
		{
			DL_ERROR(eDebugLogType::Render, "%s", "Render did something wrong");
		}
		memcpy(data.pData, &instanceData, sizeof(instanceData));
		context->Unmap(myInstanceBuffer, 0);

		context->IASetPrimitiveTopology(myTopology);
		context->IASetInputLayout(layoutData.myInputLayout);
		context->IASetVertexBuffers(0, 1, &vertexData.myVertexBuffer, &vertexData.myStride, &vertexData.myOffset);
		context->IASetIndexBuffer(vertexData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->VSSetConstantBuffers(3, 1, &myInstanceBuffer);

		context->PSSetConstantBuffers(4, 1, &myPointLightBuffer);
		context->PSSetConstantBuffers(5, 1, &mySpotLightBuffer);

		context->GSSetShader(nullptr, nullptr, 0);
		context->VSSetShader(shaderData.myVertexShader, nullptr, 0);

		if (textureData.myAlbedo)
		{
			context->PSSetShaderResources(0, 1, &textureData.myAlbedo);

		}
		if (textureData.myNormalMap)
		{
			context->PSSetShaderResources(1, 1, &textureData.myNormalMap);

		}
		if (textureData.myAmbientMap)
		{

			context->PSSetShaderResources(2, 1, &textureData.myAmbientMap);
		}
		if (textureData.myEmissiveMap)
		{

			context->PSSetShaderResources(3, 1, &textureData.myEmissiveMap);
		}
		if (textureData.myMetalnessMap)
		{

			context->PSSetShaderResources(4, 1, &textureData.myMetalnessMap);
		}
		if (textureData.myRoughnessMap)
		{
			context->PSSetShaderResources(5, 1, &textureData.myRoughnessMap);
		}
		if (environmentLightData.myCubeMap)
		{
			context->PSSetShaderResources(6, 1, &environmentLightData.myCubeMap);
		}

		context->PSSetShader(shaderData.myPixelShader, nullptr, 0);

		context->DrawIndexed(vertexData.myNumberOfIndices, 0, 0);
	}





#ifdef _DEBUGGRAPHICS
	DL_WRITELOG(eConsoleColor::GRAY,eDebugLogType::Render, "%s%i", "RENDERED:", amount);
#endif
}

void CForwardRenderer::AssignFramework(CDXFramework * aFramework)
{
	myDXFramework = aFramework;
}

