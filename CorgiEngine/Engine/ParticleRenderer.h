#pragma once

class CParticleEmitterInstance;
class CDXFramework;
class CParticleManager;
class CShaderManager;

struct SEmitterRenderData;
struct SParticle;

enum D3D_PRIMITIVE_TOPOLOGY;

class CParticleRenderer
{
public:
	CParticleRenderer(CShaderManager& aShaderManager);
	~CParticleRenderer();

	bool Init(CDXFramework* aFramework);
	void Render(CCameraInstance* aCameraInstance);
	void SetParticleManager(CParticleManager* aManager);

private:
	void CreateViewPositionBuffer(CDXFramework* aDXFramework);
	void CreateInstanceBuffer(CDXFramework* aFramework);
	void UpdateViewPositionBuffer(CCameraInstance * aCameraInstancez);
	void UpdateVertexData(Corgi::GrowingArray<SParticle>& ParticlesToRender, SParticleVertexDataWrapper& aVertexDataToSet);

	struct SInstanceBufferData
	{
		Vector4f myWorldPosition = Vector4f();
	};

	struct SViewPositionBufferData
	{
		Vector3f myToView = Vector3f();
		float myTrash;
	};


private:

	ID3D11Buffer* myViewPositionBuffer;
	ID3D11Buffer* myInstanceBuffer;
	Corgi::GrowingArray<CParticleEmitterInstance*> myParticleBuffer;
	CDXFramework* myFramework;
	CParticleManager* myParticleManager;
	CShaderManager& myShaderManager;
	SViewPositionBufferData myViewData;
	D3D_PRIMITIVE_TOPOLOGY myTopology;

	long long myVertexShaderIndex;
	long long myStaticGeometryShaderIndex;
	long long myStaticPixelShaderIndex;
	long long myAnimatedGeometryShaderIndex;
	long long myAnimatedPixelShaderIndex;
};

