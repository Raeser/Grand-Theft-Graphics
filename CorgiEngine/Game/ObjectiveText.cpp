#include "stdafx.h"
#include "ObjectiveText.h"
#include "..\Engine\Engine.h"
#include "..\Engine\SpriteLoader.h"
#include "..\Engine\SpriteInstance.h"
#include "GameSettings.h"

#define SLASHINDEX 10

CObjectiveText::CObjectiveText(CEngine & aEngine) 
	: myGameEngine(aEngine)
{
}

CObjectiveText::~CObjectiveText()
{
}
void CObjectiveText::Init(Vector2f aPosition, const char* aObjectiveTextSpritePath)
{
	myNumbers.Init(32);
	for (int i = 0; i < 32; i++)
	{
		sNumberInstance ins;
		ins.myPosition = { 0,0,0 };
		ins.mySpriteIndex = 0;
		ins.myInstance = myGameEngine.GetSpriteLoader().CreateSpriteInstance(0);
		ins.myInstance->SetShouldRender(false);
		myNumbers.Add(ins);
	}

	myDistanceBetweenNums = 0.02f;

	//TODO: this will cause memory leaks if we restart the game, dont care atm
	long long spriteIndex = myGameEngine.GetSpriteLoader().CreateSprite(aObjectiveTextSpritePath);
	myTextSprite = myGameEngine.GetSpriteLoader().CreateSpriteInstance(spriteIndex);

	myTextPosition = { aPosition.x, aPosition.y, 0.f };
	myTextSprite->SetPosition(CGameSettings::GetInstance().GetZeroToOneSpace(myTextPosition));
	myTextSprite->SetShouldRender(true);
	
	myDistanceFromText = 0.075f;
	myStartPosition = aPosition;
	myStartPosition.x += myDistanceFromText;
	LoadSprites();

	myIsShown = true;
}

void CObjectiveText::UpdateNumbers(int aCurrentNumber, int aTotalNumber)
{
	for (int i = 0; i < myNumbers.Size(); i++)
	{
		myNumbers[i].myInstance->SetShouldRender(false);
	}

	int totalNum = aTotalNumber;
	int currentNum = aCurrentNumber;
	int index = 0;

	if (currentNum >= 10)
	{
		int firstNum = currentNum / 10;
		int secondNum = currentNum % 10;

		AddNumber(firstNum, index);
		index++;
		AddNumber(secondNum, index);
		index++;
	}
	else
	{
		AddNumber(currentNum, index);
		index++;
	}

	AddNumber(SLASHINDEX, index);
	index++;

	if (totalNum >= 10)
	{
		int firstNum = totalNum / 10;
		int secondNum = totalNum % 10;

		AddNumber(firstNum, index);
		index++;
		AddNumber(secondNum, index);
		index++;
	}
	else 
	{
		AddNumber(totalNum, index);
		index++;
	}

}

void CObjectiveText::UpdateTextSprite(const char * aPath)
{
	myTextSprite->Remove();

	long long spriteIndex = myGameEngine.GetSpriteLoader().CreateSprite(aPath);
	myTextSprite = myGameEngine.GetSpriteLoader().CreateSpriteInstance(spriteIndex);
	myTextSprite->SetPosition(CGameSettings::GetInstance().GetZeroToOneSpace(myTextPosition));
	myTextSprite->SetShouldRender(myIsShown);
}

void CObjectiveText::Destroy()
{
	myTextSprite->Remove();
	myTextSprite = nullptr;

	for (int i = 0; i < myNumbers.Size(); ++i)
	{
		myNumbers[i].myInstance->Remove();
		myNumbers[i].myInstance = nullptr;
	}

	myNumbers.RemoveAll();
}

void CObjectiveText::Hide()
{
	myTextSprite->SetShouldRender(false);

	for (int i = 0; i < myNumbers.Size(); ++i)
	{
		myNumbers[i].myInstance->SetShouldRender(false);
	}

	myIsShown = false;
}

void CObjectiveText::Show()
{
	myTextSprite->SetShouldRender(true);

	for (int i = 0; i < myNumbers.Size(); ++i)
	{
		myNumbers[i].myInstance->SetShouldRender(true);
	}

	myIsShown = true;
}

void CObjectiveText::LoadSprites()
{
	for (int i = 0; i < 10; ++i)
	{
		std::stringstream ss;
		ss << "Assets/Sprites/objectiveIndicators/" << i << ".dds";
		mySpriteIndices.numbers[i] = myGameEngine.GetSpriteLoader().CreateSprite(ss.str().c_str());
	}

	mySpriteIndices.numbers[10] = myGameEngine.GetSpriteLoader().CreateSprite("Assets/Sprites/objectiveIndicators/forwardSlash.dds");
}

void CObjectiveText::AddNumber(int aNumber, int aIndex)
{
	sNumberInstance& number = myNumbers[aIndex];
	number.myPosition = { myStartPosition.x + (myDistanceBetweenNums * myNumbers.Size()), myStartPosition.y, 0.f };
	number.mySpriteIndex = mySpriteIndices.numbers[aNumber];
	number.myInstance->SetSprite(number.mySpriteIndex);
	number.myInstance->SetPosition(CGameSettings::GetInstance().GetZeroToOneSpace(number.myPosition));
	number.myInstance->SetShouldRender(myIsShown);
}
