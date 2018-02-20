#pragma once
#include "..\Utilities\GrowingArray.h"

struct SScriptLoadData;

class CScriptLoader
{
public:
	CScriptLoader();
	~CScriptLoader();

	void LoadScripts();
	void ReadFiles();
	void SubscribeOnScripts();

private:

	void InternalScriptLoader();
	void InternalFileReader();

	Corgi::GrowingArray<SScriptLoadData> myScriptLoadData;
	Corgi::GrowingArray<std::string> myFilesToRead;

};

