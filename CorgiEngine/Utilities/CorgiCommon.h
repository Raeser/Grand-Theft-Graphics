#pragma once
#define cast_f(x) static_cast<float>(x)
#define cast_int(x) static_cast<int>(x)
#define cast_uint(x) static_cast<unsigned int>(x)
#include <sys/stat.h>

namespace Corgi
{
	inline bool FileExists(const char* aFilePath)
	{
		struct stat buffer;
		if ( stat(aFilePath, &buffer) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}