#pragma once

#include "../Utilities/GrowingArray.h"
#include "SpriteInstance.h"
#include "ShaderCompiler.h"

class string;
class CSprite;
class CSpriteInstance;
class CSpriteRenderer;
class CScene;
struct ID3D11Resource;
enum ModelLoaderSpace;
struct ID3D11Device;

enum eScene
{
	eScene_mainMenu,
	eScene_ingameMenu,
	eScene_Game,
	eScene_count
};

class CSpriteLoader
{

public:
	CSpriteLoader();
	~CSpriteLoader();
	void Init(Vector2f aScreenSize, ID3D11Device* aDevice, eScene aSceneToStartWith);

	CSpriteInstance* CreateSpriteInstance(long long aID);
	long long CreateSprite(const char* aPath);
	Vector2f GetSpriteSize(ID3D11Resource* aResource);
	bool ReLoadSprite(long long aID);
	CSprite& GetSprite(long long aID);
	void SetActiveScene(eScene aActiveScene);
	const Corgi::GrowingArray<CSpriteInstance>& GetCurrentInstances() const;
	long long GetAmountOfSprites() { return mySprites.Size(); };
	void UpdateActiveSpriteInstances();
private:
	eScene myCurrentScene;
	Corgi::GrowingArray<CSprite, long long> mySprites;
	Corgi::GrowingArray<std::string, long long> myLoadedSpritePaths;
	Vector2f myScreenSize;
	ID3D11Device* myDevice;
	CSpriteInstance myCursor;
	Corgi::GrowingArray<Corgi::GrowingArray<CSpriteInstance>> myInstanceContainers;

	CShaderCompiler myShaderCompiler;
};

