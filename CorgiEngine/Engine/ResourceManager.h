#pragma once

class CAnimationLoader;
class CModelLoaderWrapper;
class CScene;
class CShaderManager;
struct ID3D11Device;
class CResourceManager
{
public:
	CResourceManager(CShaderManager& myShaderManager, CAnimationLoader& aAnimationLoader);
	~CResourceManager();
	bool Init(ID3D11Device * aDevice, CScene* aScene);
	CModelLoaderWrapper* GetModelLoaderWrapper() { return myModelLoaderWrapper; }
private:
	CModelLoaderWrapper* myModelLoaderWrapper;
	CShaderManager& myShaderManager;
	CAnimationLoader& myAnimationLoader;
	bool myIsInited;


};
