#include "stdafx.h"
#include "TextToSpeechManager.h"
#include "..\Script\ScriptManager.h"

CTextToSpeechManager::CTextToSpeechManager()
{
}


CTextToSpeechManager::~CTextToSpeechManager()
{
}

bool CTextToSpeechManager::Init()
{

	if (FAILED(::CoInitialize(NULL)))
	{
		return false;
	}
	return true;
}

bool CTextToSpeechManager::Release()
{
	::CoUninitialize();
	return true;
}

void CTextToSpeechManager::Speak(const std::string & aText)
{
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&myVoice);
	if (SUCCEEDED(hr))
	{
		hr = myVoice->Speak(Corgi::ConvertCharPtrToWideString(aText.c_str()).c_str(), 0, NULL);
		myVoice->Release();
		myVoice = NULL;
	}
}
