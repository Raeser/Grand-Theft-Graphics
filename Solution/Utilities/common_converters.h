#pragma once
#include <stringapiset.h>
#include <sstream>
//#include <locale>
//#include <codecvt>
//#include <string>
#include <algorithm>

#pragma warning(push)
#pragma warning(disable: 4100) //Unreferenced formal parameter

namespace Corgi
{
	static void ConvertStringToLowerCase(std::string& aStringOut)
	{
		std::transform(aStringOut.begin(), aStringOut.end(), aStringOut.begin(), ::tolower);
	}

	static void ConvertWideStringToLowerCase(std::wstring& aWideStringOut)
	{
		std::transform(aWideStringOut.begin(), aWideStringOut.end(), aWideStringOut.begin(), ::tolower);
	}

	static void StringReplaceChars(std::string& aStringOut, const char* replaceThis, const char* withThis)
	{
		std::string tempReplaceThis(replaceThis);
		std::string tempReplaceWithThis(withThis);
		size_t startPositionToReplace = aStringOut.find(tempReplaceThis);	//Increases the input parameter possibility

		while ((startPositionToReplace = aStringOut.find(tempReplaceThis, startPositionToReplace)) != std::string::npos)
		{
			aStringOut.replace(startPositionToReplace, tempReplaceThis.length(), tempReplaceWithThis);
			startPositionToReplace += tempReplaceWithThis.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}
	static void WideStringReplaceChars(std::wstring& aStringOut, const std::wstring& replaceThis, const std::wstring& withThis)
	{
		std::wstring tempReplaceThis(replaceThis);
		std::wstring tempReplaceWithThis(withThis);
		size_t startPositionToReplace = aStringOut.find(tempReplaceThis);	//Increases the input parameter possibility

		while ((startPositionToReplace = aStringOut.find(tempReplaceThis, startPositionToReplace)) != std::string::npos)
		{
			aStringOut.replace(startPositionToReplace, tempReplaceThis.length(), tempReplaceWithThis);
			startPositionToReplace += tempReplaceWithThis.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}

	static void ConvertStringPathToSafeUse(std::string& aStringOut)
	{
		StringReplaceChars(aStringOut, "\\", "/");
		ConvertStringToLowerCase(aStringOut);
	}

	static void ConvertWideStringPathToSafeUse(std::wstring& aWideStringOut)
	{
		WideStringReplaceChars(aWideStringOut, L"\\", L"/");
		ConvertWideStringToLowerCase(aWideStringOut);
	}

	static std::string FloatToString(float aFloat)
	{
		std::ostringstream ss;
		ss << aFloat;
		std::string s(ss.str());
		return s;
	}

	static int StringToInt(const std::string& aString)
	{
		return atoi(aString.c_str());
	}

	static float StringToFloat(std::string aString)
	{
		std::string::size_type sz;
		float mars = std::stof(aString, &sz);
		return mars;
	}

	static std::wstring ConvertCharPtrToWideString(const char* charArray)
	{
		wchar_t* wString = new wchar_t[4096];
		MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);

		std::wstring str(wString);
		delete[] wString;
		return str;
	}

	static std::wstring ConvertStringToWide(const std::string& aString)
	{
		std::wstring ws;
		ws.assign(aString.begin(), aString.end());
		return ws;

		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		//std::wstring wide = converter.from_bytes(aString);
		//return wide;
	}

	static std::string ConvertWStringToString(const std::wstring& aWide)
	{
		std::string ws;
		ws.assign(aWide.begin(), aWide.end());
		return ws;

		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		//std::string narrow = converter.to_bytes(aWide);
		//return narrow;
	}



	template <typename T>
	static T clamp(const T& n, const T& lower, const T& upper)
	{
		return std::fmax(lower, std::fmin(n, upper));
	}

}

#pragma warning(pop)
#pragma warning(disable: 4505) //Unreferenced local function has been removed