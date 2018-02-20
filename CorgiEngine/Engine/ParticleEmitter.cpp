#include "stdafx.h"
#include "ParticleEmitter.h"
#include "Particle.h"
#include "..\Game\DataManager.h"

#define GRAVITY -9.81f

CParticleEmitter::CParticleEmitter()
{
	myCoolDown = 0.0f;
	myActiveParticleVertices.Init(4);
}


CParticleEmitter::~CParticleEmitter()
{
}


void CParticleEmitter::Init(const SEmitter & aEmitterData, ID3D11Device* aDevice, ID3D11DeviceContext* aContext)
{
	aContext;
	myData = aEmitterData;
	myRandomEndRotation = Corgi::RandomFloat(CDataManager::GetInstance().GetMiscData().randParticleEndDegreeFrom, CDataManager::GetInstance().GetMiscData().randParticleEndDegreeTo);//Provisoriskt för att få rotation för particlarna
	myMaxAmountOfParticles = myData.maxparticles;
	myActiveParticleVertices.ReInit(myMaxAmountOfParticles);
	if (myData.textureType == 0)
	{
		myIsAnimated = false;
	}
	else
	{
		myIsAnimated = true;
	}
	InitializeBuffer(aDevice);
	InitializeVertexBuffer(aDevice);
	InitializeParticles();
}

Vector4f CParticleEmitter::SetPosition(const Vector3f& aEmitterPosition)
{
	if (myData.emittertype == eEmitterType_Point)
	{
		Vector4f newPos = { aEmitterPosition.x, aEmitterPosition.y, aEmitterPosition.z, 1 };
		return newPos;
	}
	if (myData.emittertype == eEmitterType_Rectangle)
	{
		Vector4f newPos = { aEmitterPosition.x, aEmitterPosition.y, aEmitterPosition.z, 1 };
		newPos.x += Corgi::RandomFloat(-myData.rectSizeX * 2, myData.rectSizeX * 2);
		newPos.z += Corgi::RandomFloat(-myData.rectSizeZ * 2, myData.rectSizeZ * 2);

		return newPos;
	}
	Vector4f newPos = { 0,0,0,0 };
	return newPos;
}

void CParticleEmitter::InitializeParticles()
{
	SParticle newParticle;
	newParticle.currentLifetime = 0;
	newParticle.position = { 0,0,0,0 };
	newParticle.direction = myData.velocity;
	newParticle.currentLifetime = 0;
	newParticle.isAlive = false;
	newParticle.timesEmitted = 0;
	//for (int i = 0; i < myMaxAmountOfParticles; ++i)
	//{
	//	myParticles.Add(newParticle);
	//}
}

void CParticleEmitter::InitializeBuffer(ID3D11Device* aDevice)
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = { 0 };
	if (myIsAnimated)
	{
		bufferDescription.ByteWidth = sizeof(SBufferAnimated);
	}
	else
	{
		bufferDescription.ByteWidth = sizeof(SBufferStatic);
	}
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA bufferSubResourceData = { 0 };

	if (myIsAnimated)
	{
		SBufferAnimated* data = new SBufferAnimated;
		data->myColumnCount = myData.textureColumns;
		data->myRowCount = myData.textureRows;

		data->myStartColor = myData.color0;
		data->myMidColor = myData.color1;
		data->myEndColor = myData.color2;
		
		data->myStartSize = { myData.startSizeX, myData.startSizeY };
		data->myEndSize = { myData.endSizeX, myData.endSizeY };
		
		data->myMaxLifeTime = myData.lifetime;
		bufferSubResourceData.pSysMem = data;
		result = aDevice->CreateBuffer(&bufferDescription, &bufferSubResourceData, &myBuffer);
	}
	else
	{
		SBufferStatic* data = new SBufferStatic;

		data->myStartColor = myData.color0;
		data->myMidColor = myData.color1;
		data->myEndColor = myData.color2;

		data->myStartSize = { myData.startSizeX, myData.startSizeY };
		data->myEndSize = { myData.endSizeX, myData.endSizeY };

		data->myMaxLifeTime = myData.lifetime;
		data->myRandomEndRotation = myRandomEndRotation;
		bufferSubResourceData.pSysMem = data;

		result = aDevice->CreateBuffer(&bufferDescription, &bufferSubResourceData, &myBuffer);
	}


	if (FAILED(result) || S_FALSE == result)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "HAHAHA AXEL DU SUGER IDIOT");
	}
}


void CParticleEmitter::InitializeVertexBuffer(ID3D11Device * aDevice)
{
	HRESULT hr;

	unsigned int count = myMaxAmountOfParticles - 1;

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	//vertexDesc.ByteWidth = sizeof(POSITION) * count*2;	//NOTE!!!!!! this is just simply reserved space for further vertices.
	vertexDesc.ByteWidth = sizeof(SParticleVertexData) * count;
	vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexDesc.MiscFlags = 0;
	vertexDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = myActiveParticleVertices.GetPureData();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	hr = aDevice->CreateBuffer(&vertexDesc, &vertexData, &myVertexBuffer);
}

int CParticleEmitter::GetAmountOfActiveParticles()
{
	return max(myActiveParticleVertices.Size() - 1, 0);
}

void CParticleEmitter::Update(Corgi::GrowingArray<SParticle, long long>& aParticlesToUpdate, const Vector3f& aEmitterPosition, CParticleEmitterInstance* aInstance)
{
	float dt = Timer::GetInstance().GetDeltaTime();


	//myCoolDown += dt;
	float& cooldown = aInstance->GetCooldown();
	cooldown += dt;

	for (SParticle& particle : aParticlesToUpdate)
	{

		// Check for dead/alive
		if (particle.isAlive == true)
		{
			if (particle.currentLifetime < myData.lifetime)
			{
				Vector4f newPosition = particle.position;
				Vector3f dir = particle.position.V3();

				float percent = particle.currentLifetime / myData.lifetime;

				// Add gravity
				particle.position.y += ((GRAVITY - (percent * 10)) * dt) * myData.gravity;

				// Add velocity
				particle.position.x += ((myData.velocity.x) * dt);
				particle.position.y += ((myData.velocity.y) * dt);
				particle.position.z += ((myData.velocity.z) * dt);

				// Add time to particle life
				particle.currentLifetime += dt;

				// Move particle
				particle.direction = particle.position.V3() - (dir + Vector3f(0, 0.001f, 0));

				//TODO: Find a good way to incorporate random directions without fucking too much shit up.
				//particles[i].direction.X *= particles[i].randX;
				//particles[i].direction.Y *= particles[i].randY;
			}
			else
			{
				// Particle died by lifetime, reset
				particle.currentLifetime = 0;
				particle.isAlive = false;

				// Move particle back to PS origin
				particle.position = SetPosition(aEmitterPosition);
			}
		}
		else
		{
			// This particle is dead already since before.
			// Find out if we should emit it again!
			if (cooldown > myData.emissiondelay &&
				aInstance->GetIsActive())
			{
				if (myData.looping == 0) // emit all particles only once
				{
					if (particle.timesEmitted == 0)
					{
						particle.position = SetPosition(aEmitterPosition);
						particle.isAlive = true;
						particle.timesEmitted++;

						cooldown = 0.0f;
					}
				}

				if (myData.looping == 1) // continuous emission
				{
					particle.position = SetPosition(aEmitterPosition);
					particle.isAlive = true;
					particle.timesEmitted++;

					cooldown = 0.0f;
				}
			}
		}
	}
}

void CParticleEmitter::Release()
{
	myBuffer->Release();
	myTexture->Release();
}

void CParticleEmitter::GetActiveParticles(Corgi::GrowingArray<SParticle, long long>& aDataToRender)
{

	SParticleVertexData data;
	for (SParticle& part : aDataToRender)
	{
		if (part.isAlive)
		{
			data.direction = part.direction;
			data.lifeTime = part.currentLifetime;
			data.position = part.position;
			myActiveParticleVertices.Add(data);
		}
	}
}

long long CParticleEmitter::GetVertexShader()
{
	return myVertexShader;
}

long long CParticleEmitter::GetGeometryShader()
{
	return myGeometryShader;
}

long long CParticleEmitter::GetPixelShader()
{
	return myPixelShader;
}

void CParticleEmitter::SetTexture(ID3D11ShaderResourceView * aTexture)
{
	myTexture = aTexture;
}

void CParticleEmitter::UpdateParticleBuffer(ID3D11DeviceContext * aContext, Corgi::GrowingArray<SParticle, long long>& aParticlesToSet)
{
	int count = -1;
	myActiveParticleVertices.RemoveAll();
	GetActiveParticles(aParticlesToSet);
	count = max(myActiveParticleVertices.Size() - 1, 0);

	if (count > 0)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		HRESULT hr = aContext->Map(myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			DL_ERROR(eDebugLogType::Render, "%s", "Failed to update particle vertex buffer");
		}
		memcpy((SParticleVertexData*)mappedResource.pData, myActiveParticleVertices.GetPureData(), sizeof(SParticleVertexData) * count);
		aContext->Unmap(myVertexBuffer, 0);
	}
}

void CParticleEmitter::SetMyBufferAsActive(ID3D11DeviceContext * aContext, int aSlot)
{
	UINT stride = sizeof(SParticleVertexData); 
	UINT offset = 0;
	aContext->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
	aContext->GSSetConstantBuffers(aSlot, 1, &myBuffer);
	aContext->PSSetConstantBuffers(aSlot, 1, &myBuffer);
}

void CParticleEmitter::SetTextureAsActive(ID3D11DeviceContext* aContext, int aSlot)
{
	aContext->PSSetShaderResources(aSlot, 1, &myTexture);
}

ID3D11ShaderResourceView * CParticleEmitter::GetTexture()
{
	return myTexture;
}

int CParticleEmitter::GetMaxAmountOfParticles()
{
	return myMaxAmountOfParticles;
}

bool CParticleEmitter::GetIsAnimated()
{
	return myIsAnimated;
}


const SEmitter & CParticleEmitter::GetParticleData()
{
	return myData;
}


