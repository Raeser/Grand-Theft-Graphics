#include "stdafx.h"
#include "ResourceManager.h"
#include "ModelLoaderWrapper.h"

CResourceManager::CResourceManager(CShaderManager& aShaderManager, CAnimationLoader& aAnimationLoader) : 
	myShaderManager(aShaderManager), 
	myAnimationLoader(aAnimationLoader)
{
	myIsInited = false;
}


CResourceManager::~CResourceManager()
{
	delete myModelLoaderWrapper;
	myModelLoaderWrapper = nullptr;
}

bool CResourceManager::Init(ID3D11Device * aDevice, CScene* aScene)
{
	if (!myIsInited)
	{
		myModelLoaderWrapper = new CModelLoaderWrapper(myShaderManager, myAnimationLoader);
		myModelLoaderWrapper->Init(aDevice, aScene);
		myIsInited = true;
		return true;
	}
	return false;
}

