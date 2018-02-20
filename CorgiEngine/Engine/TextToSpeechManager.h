#pragma once
#include <sapi.h>
#pragma comment (lib, "sapi.lib")

struct lua_State;

class CTextToSpeechManager
{
public:
	~CTextToSpeechManager();

	static CTextToSpeechManager& GetInstance()
	{
		static CTextToSpeechManager me;
		return me;
	}

	bool Init();
	bool Release();


	void Speak(const std::string& aText);

private:
	CTextToSpeechManager();
	ISpVoice* myVoice = NULL;
};

