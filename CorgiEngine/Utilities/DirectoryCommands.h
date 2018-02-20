#pragma once
#include <vector>
namespace Corgi
{
	void GetFileTypePathsInDirectory(const std::string& root, const std::string& ext, std::vector<std::string>& ret,bool shouldOrderByDate = false);
}