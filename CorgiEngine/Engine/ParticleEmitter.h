#pragma once
#include "ParticleDataWrapper.h"

class CDXFramework;
class CParticleManager;
class CParticleRenderer;

struct SParticle
{
	Vector4f position;
	Vector3f direction;
	float currentLifetime;

	int randX;
	int randY;
	int timesEmitted;

	bool isAlive;
};

#pragma region GraphicStructs

struct SBufferAnimated
{
	Vector4f myStartColor;
	Vector4f myMidColor;
	Vector4f myEndColor;
	Vector2f myStartSize;
	Vector2f myEndSize;
	
	float myMaxLifeTime;
	float trash;

	int myColumnCount;
	int myRowCount;

};

struct SBufferStatic
{
	Vector4f myStartColor;
	Vector4f myMidColor;
	Vector4f myEndColor;
	Vector2f myStartSize;
	Vector2f myEndSize;

	float myMaxLifeTime;
	float myRandomEndRotation;
	Vector2f trash;
};

struct SParticleVertexData
{
	Vector4f position;
	Vector3f direction;
	float lifeTime;
};
#pragma endregion


class CParticleEmitter
{
	friend CParticleManager;
	friend CParticleRenderer;
	friend CEngine;
public:
	CParticleEmitter();
	~CParticleEmitter();
	void Init(const SEmitter& emitterData, ID3D11Device* aDevice, ID3D11DeviceContext* aContext);
	void Update(Corgi::GrowingArray<SParticle, long long>& aParticlesToUpdate, const Vector3f& aEmitterPosition, CParticleEmitterInstance* aInstance);
	void Release();
	long long GetVertexShader();
	long long GetGeometryShader();
	long long GetPixelShader();
	void SetTexture(ID3D11ShaderResourceView* aTexture);
	void UpdateParticleBuffer(ID3D11DeviceContext* aContext, Corgi::GrowingArray<SParticle, long long>& aParticlesToSet);
	void SetMyBufferAsActive(ID3D11DeviceContext* aContext, int aSlot = 3);
	void SetTextureAsActive(ID3D11DeviceContext* aContext, int aSlot = 0);
	ID3D11ShaderResourceView* GetTexture(); //DO THE FUCKING TEXTURE MANAGER

	int GetMaxAmountOfParticles();
	bool GetIsAnimated();

private:

	void GetActiveParticles(Corgi::GrowingArray<SParticle, long long>& aParticlesToUpdate);

	Vector4f SetPosition(const Vector3f& aEmitterPosition);
	void InitializeParticles(); //should be held by the instances 
	void InitializeBuffer(ID3D11Device* aDevice);
	void InitializeVertexBuffer(ID3D11Device* aDevice);
	int GetAmountOfActiveParticles();

	const SEmitter& GetParticleData();

	SEmitter myData;
	//Corgi::GrowingArray<SParticle> myParticles;
	Corgi::GrowingArray<SParticleVertexData> myActiveParticleVertices;
	long long myVertexShader;
	long long myPixelShader;
	long long myGeometryShader;
	
	bool myIsAnimated;

	UINT myNumberOfVertices;
	UINT myStride;
	UINT myOffset;
	ID3D11ShaderResourceView* myTexture;

	ID3D11Buffer* myBuffer;
	ID3D11Buffer* myVertexBuffer;
	float myCoolDown;
	float myRandomEndRotation;
	int myMaxAmountOfParticles;
};

