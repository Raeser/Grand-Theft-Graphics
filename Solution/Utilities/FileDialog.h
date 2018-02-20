#pragma once
#include <string>

namespace Corgi
{
	std::string GetOpenDialogFilePath(const char* aExtension);
	std::string GetSaveDialogFilePath(const char* aExtension);
	
	bool SaveFile(const std::string& aFilePath, const std::string& aData, bool aEncode = false);
	std::string OpenFile(const std::string& aFilePath, bool aEncoded = false);
}
