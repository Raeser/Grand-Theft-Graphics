#include "stdafx.h"

#include "text.h"
#include "../engine.h"
#include "text_service.h"


CText::CText(const char* aPathAndName, EFontSize aFontSize, unsigned char aBorderSize)
: myTextService(&CEngine::GetInstance()->GetTextService())
{
	myFontDataPointer = nullptr;
	myColor = { 1, 0, 1, 1 };
	myScale = 1.0f;
	myPathAndName = aPathAndName;
	myFontDataPointer = CEngine::GetInstance()->GetTextService().InitAndLoad(myPathAndName, aBorderSize, aFontSize);
	//myRenderName = Tga2D::CEngine::GetInstance()->GetTextService().GetRenderName(myPathAndName, aFontSize);
}


CText::~CText()
{
}

void CText::Render()
{
	if (!myTextService)
	{
		return;
	}
	if (!myTextService->AddTextToRender(myText, myPosition, myColor, myScale, myFontDataPointer))
	{
		DL_ERROR(eDebugLogType::Resource ,"%s", "Text rendering error! Trying to render a text where the resource has been deleted! Did you clear the memory for this font? OR: Did you set the correct working directory?");
	}
}

float CText::GetWidth()
{
	if (!myTextService)
	{
		return 0.0f;
	}

	return myTextService->GetSentenceWidth(this) / CEngine::GetInstance()->GetWindowSize().x;
}
