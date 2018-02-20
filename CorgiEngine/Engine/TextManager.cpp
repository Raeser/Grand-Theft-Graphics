#include "stdafx.h"
#include "TextManager.h"


#define NUMBER_OF_TEXTS 1024

CTextManager::CTextManager(CEngine& aEngine):
	myFontLoader(aEngine),
	myRenderer(aEngine),
	myEngine(aEngine)
{
}


CTextManager::~CTextManager()
{
}

bool CTextManager::Init()
{
	myFonts.Init(16);
	myTexts.Init(NUMBER_OF_TEXTS);
	myRenderer.Init();
	myMinTimeBetweenTicks = 0.05f;
	myTimeSinceLastTick = 0;
	std::string textToRef;
	for (int i = 0;  i < NUMBER_OF_TEXTS; ++i)
	{
		SSimplifiedText textToAdd;
		textToAdd.isActive = false;
		textToAdd.text = nullptr;
		myTexts[i] = textToAdd;
	}
	return true;
}

void CTextManager::Render()
{
	if (myFonts.Size() < 1)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Tried to print text without any loaded fonts, Please check to see that your font folder is there");
		return;
	}
	for (int i = 0; i < NUMBER_OF_TEXTS; ++i)
	{
		if (myTexts[i].isActive)
		{
			myTexts[i].bufferedText = *myTexts[i].text;
			myRenderer.Render(myTexts[i], myFonts[myTexts[i].fontID]);
		}
	}
	myEngine.GetDXFrameWork()->GetContext()->GSSetShader(nullptr, nullptr, 0);
}

void CTextManager::Update()
{
	myTimeSinceLastTick += Timer::GetInstance().GetDeltaTime();
	bool shouldScrollText = (myTimeSinceLastTick > myMinTimeBetweenTicks);

	if (shouldScrollText)
	{
		for (int i = 0; i < NUMBER_OF_TEXTS; ++i)
		{
			if (myTexts[i].isActive)
			{
				if (myTexts[i].textType == eText_Scrolling)
				{
					myTexts[i].scrollingIndex += shouldScrollText;
				}
			}
		}
		myTimeSinceLastTick = 0;
	}

}

FontID CTextManager::LoadFont(std::string aPath, EFontSize aSize, unsigned char aBorderSize)
{
	CFontData data;
	if (!myFontLoader.InitAndLoad(data, aPath, aBorderSize, aSize))
	{
		DL_ERROR(eDebugLogType::Resource, "%s%s", "failed to load font: ", aPath.c_str());
		return 0;
	}
	myFonts.Add(data);
	return myFonts.Size() - 1;
}

TextID CTextManager::AddText(std::string& aTextRefOut, Vector2f aPosition,FontID aFont, Vector4f aColor, float aScale, eTextType aType, int aScrollingStartIndex)
{
	Vector4f realPosition = { aPosition.x, aPosition.y,1, 1 };
	TextID aTextIDOut = myTexts.GetFreeItem();
	myTexts[aTextIDOut].myScreenPosition = realPosition;
	myTexts[aTextIDOut].fontID = aFont;
	myTexts[aTextIDOut].color = aColor;
	myTexts[aTextIDOut].textType = aType;
	myTexts[aTextIDOut].scale = aScale;
	myTexts[aTextIDOut].scrollingIndex = aScrollingStartIndex;
	myTexts[aTextIDOut].isActive = true;
	myTexts[aTextIDOut].text = &aTextRefOut;
	return aTextIDOut;
}


void CTextManager::ChangeTextColor(TextID aTextInstance, Vector4f & aColor)
{
	myTexts[aTextInstance].color = aColor;
}

void CTextManager::ChangeFont(TextID aTextInstance, FontID aNewFontToUse)
{
	myTexts[aTextInstance].fontID = aNewFontToUse;
}

void CTextManager::ChangePosition(TextID aTextInstance, Vector2f aPos)
{
	Vector4f realPosition = { aPos.x, aPos.y, 1, 1 };
	myTexts[aTextInstance].myScreenPosition = realPosition;
}

void CTextManager::ChangePosition(TextID aTextInstance, Vector3f aPos)
{
	Vector4f realPosition = { aPos.x, aPos.y * (cast_f(myEngine.GetWindowSize().y)/ cast_f(myEngine.GetWindowSize().x)), aPos.z, 1 };
	myTexts[aTextInstance].myScreenPosition = realPosition;
}

void CTextManager::SetTextType(TextID aTextInstance, eTextType aType)
{
	myTexts[aTextInstance].textType = aType;
}

void CTextManager::RemoveText(TextID aID)
{
	myTexts[aID].isActive = false;
	myTexts.ReleaseObject(aID);
}
