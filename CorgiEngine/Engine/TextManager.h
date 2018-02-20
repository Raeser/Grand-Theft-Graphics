#pragma once
#include "FontLoader.h"
#include "..\Engine\TextRenderer.h"
#include "..\Utilities\MemoryPool.h"

#include <string>
#include "..\Utilities\Vector.h"

class CTextManager
{
public:
	CTextManager(CEngine& aEngine);
	~CTextManager();

	bool Init();
	void Render();
	void Update();
	FontID LoadFont(std::string aPath, EFontSize aSize = EFontSize_30, unsigned char aBorderSize = 1);
	TextID AddText(std::string& aTextRefOut,Vector2f aPosition, FontID aFont = 0, Corgi::Vector4<float> aColor = { 1,1,1,1 }, float aScale = 1.f, eTextType aType = eTextType::eText_Normal, int aScrollingStartIndex = 0);
	void ChangeTextColor(TextID aTextInstance, Vector4f& aColor);
	void ChangeFont(TextID aTextInstance, FontID aNewFontToUse);
	void ChangePosition(TextID aTextInstance, Vector2f aPos);

	void ChangePosition(TextID aTextInstance, Vector3f aPos);

	void SetTextType(TextID aID, eTextType aType);
	void RemoveText(TextID aID);

private:


	CTextRenderer myRenderer;
	CFontLoader myFontLoader;
	Corgi::CMemoryPool<SSimplifiedText, TextID> myTexts;
	Corgi::GrowingArray<CFontData, FontID> myFonts;
	CEngine& myEngine;
	float myTimeSinceLastTick;
	float myMinTimeBetweenTicks;
};

