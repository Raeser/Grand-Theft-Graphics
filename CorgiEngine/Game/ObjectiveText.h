#pragma once
#include "..\Utilities\GrowingArray.h"

class CEngine;
class CSpriteInstance;


struct CNumberSprites 
{
	long long numbers[11];
};

struct sNumberInstance 
{
	long long mySpriteIndex;
	CSpriteInstance* myInstance;
	Vector3f myPosition;
};

class CObjectiveText
{
public:
	CObjectiveText(CEngine& aEngine);
	~CObjectiveText();

	void Init(Vector2f aPosition, const char* aObjectiveTextSpritePath);
	void UpdateNumbers(int aCurrentNumber, int aTotalNumber);
	void UpdateTextSprite(const char* aPath);
	void Destroy();

	void Hide();
	void Show();

private:
	void LoadSprites();
	void AddNumber(int aNumber, int aIndex);
	
	float myDistanceBetweenNums;
	float myDistanceFromText;

	Vector2f myStartPosition;
	Vector3f myTextPosition;

	CEngine& myGameEngine;
	CSpriteInstance* myTextSprite;
	CNumberSprites mySpriteIndices;

	Corgi::GrowingArray<sNumberInstance> myNumbers;

	bool myIsShown;
};

