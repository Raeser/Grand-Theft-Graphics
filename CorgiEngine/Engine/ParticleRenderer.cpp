#include "stdafx.h"
#include "ShaderManager.h"
#include "ParticleRenderer.h"
#include "ParticleEmitterInstance.h"
#include "ParticleEmitter.h"
#include "Particle.h"
#include "ParticleManager.h"
#include "CameraInstance.h"
#include "D3D11DataWrappers.h"
//TODO SORT PARTICLES DEPENDING ON RANGE TO PLAYER
CParticleRenderer::CParticleRenderer(CShaderManager& aShaderManager) : myShaderManager(aShaderManager)
{
	myParticleBuffer.Init(256);
}

CParticleRenderer::~CParticleRenderer()
{
	myViewPositionBuffer->Release();
	myViewPositionBuffer = nullptr;
	myInstanceBuffer->Release();
	myInstanceBuffer = nullptr;
}

bool CParticleRenderer::Init(CDXFramework * aFramework)
{
	if (aFramework == nullptr)
	{
		DL_ERROR(eDebugLogType::Render, "%s", "Framework in ParticleRenderer was nullptr");
		return false;
	}
	myFramework = aFramework;
	CreateViewPositionBuffer(myFramework);
	CreateInstanceBuffer(myFramework);


	D3D11_INPUT_ELEMENT_DESC VertexShaderLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "DIRECTION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "LIFETIME", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	myVertexShaderIndex = myShaderManager.LoadShader("../assets/shaders/VS_Particle.hlsl", VertexShaderLayout, 3);

	myStaticGeometryShaderIndex = myShaderManager.LoadShader("../assets/shaders/GS_Particle.hlsl", nullptr, 0);
	myStaticPixelShaderIndex = myShaderManager.LoadShader("../assets/shaders/PS_Particle.hlsl", nullptr, 0);

	myAnimatedGeometryShaderIndex = myShaderManager.LoadShader("../assets/shaders/GS_ParticleAnimated.hlsl", nullptr, 0);
	myAnimatedPixelShaderIndex = myShaderManager.LoadShader("../assets/shaders/PS_ParticleAnimated.hlsl", nullptr, 0);

	return true;
}

void CParticleRenderer::Render(CCameraInstance * aCameraInstance)
{

	HRESULT result;
	myParticleBuffer = myParticleManager->GetEmittorData();
	ID3D11DeviceContext* context = myFramework->GetContext();

	ID3D11Buffer* cameraBuffer = aCameraInstance->myCamera->GetBuffer();
	context->VSSetConstantBuffers(0, 1, &cameraBuffer);
	context->GSSetConstantBuffers(0, 1, &cameraBuffer);

	UpdateViewPositionBuffer(aCameraInstance);
	context->VSSetConstantBuffers(1, 1, &myViewPositionBuffer);
	SInstanceBufferData instanceData;
	D3D11_MAPPED_SUBRESOURCE data;
	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));

	myShaderManager.SetAsActiveShader(eShaderType_Vertex, myVertexShaderIndex);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	long long emittorID;
	for (CParticleEmitterInstance* emitInst : myParticleBuffer)
	{
		if (emitInst->GetShouldRender())
		{
			emittorID = emitInst->GetEmitterID();
			if (myParticleManager->myLoader.IsEmitterReady(emittorID) == false)
			{
				continue;
			}
			if (emitInst->GetIsCorrectlyInitialized() == false)
			{
				emitInst->Init(emitInst->GetEmitterID(), myParticleManager->myLoader.GetEmitter(emitInst->GetEmitterID()).GetMaxAmountOfParticles());
			}
			CParticleEmitter& Emitter = myParticleManager->myLoader.GetEmitter(emitInst->GetEmitterID());
			Corgi::GrowingArray<SParticle, long long>& particlesToRender = emitInst->GetParticles();
			Emitter.Update(particlesToRender, emitInst->GetPosition(), emitInst);
			Emitter.UpdateParticleBuffer(context, particlesToRender);
			Emitter.SetMyBufferAsActive(context);

			//set Shaders via shadermanager
			if (Emitter.GetIsAnimated())
			{
				myShaderManager.SetAsActiveShader(eShaderType_Pixel, myAnimatedPixelShaderIndex);
				myShaderManager.SetAsActiveShader(eShaderType_Geometry, myAnimatedGeometryShaderIndex);
			}
			else
			{
				myShaderManager.SetAsActiveShader(eShaderType_Pixel, myStaticPixelShaderIndex);
				myShaderManager.SetAsActiveShader(eShaderType_Geometry, myStaticGeometryShaderIndex);
			}
			Emitter.SetTextureAsActive(context);
			Vector3f position = emitInst->GetOrientation().GetPosition();
			instanceData.myWorldPosition = {position.x, position.y, position.z, 1};

			result = context->Map(myInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

			if (FAILED(result))
			{
				DL_ERROR(eDebugLogType::Render, "%s", "Render did something wrong");
			}
			memcpy(data.pData, &instanceData, sizeof(instanceData));
			context->Unmap(myInstanceBuffer, 0);
			context->VSSetConstantBuffers(2, 1, &myInstanceBuffer);


			UINT amountToDraw = max(static_cast<UINT>(Emitter.GetAmountOfActiveParticles()), 0);
			context->Draw(amountToDraw, 0);
		}
	}
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void CParticleRenderer::SetParticleManager(CParticleManager * aManager)
{
	myParticleManager = aManager;
}

void CParticleRenderer::CreateViewPositionBuffer(CDXFramework * aDXFramework)
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
		DL_ERROR(eDebugLogType::Render, "%s", "ParticleRenderer failed to create ViewPositionBuffer");
		if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Render, "%s", "invalidArgument !?");
		}
	}
	myViewPositionBuffer = viewBuffer;
}

void CParticleRenderer::CreateInstanceBuffer(CDXFramework * aFramework)
{
	HRESULT result;
	ID3D11Device* device = aFramework->GetDevice();

	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = sizeof(SInstanceBufferData);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = device->CreateBuffer(&bufferDescription, nullptr, &myInstanceBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "ParticleRenderer failed to create instancebuffer");
	}
}

void CParticleRenderer::UpdateViewPositionBuffer(CCameraInstance * aCameraInstance)
{
	myViewData.myToView = aCameraInstance->GetPosition();
	ID3D11DeviceContext* context = myFramework->GetContext();

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

void CParticleRenderer::UpdateVertexData(Corgi::GrowingArray<SParticle>& ParticlesToRender, SParticleVertexDataWrapper & aVertexDataToSet)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE data;
	SParticle* particlesToMemcpy = ParticlesToRender.GetPureData();
	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = myFramework->GetContext()->Map(aVertexDataToSet.myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "ParticleRenderer failed to map buffers in UpdateVertexData");
	}

	memcpy(data.pData, particlesToMemcpy, (sizeof(CParticle) * (ParticlesToRender.Size() - 1)));
	myFramework->GetContext()->Unmap(aVertexDataToSet.myVertexBuffer, 0);
}

