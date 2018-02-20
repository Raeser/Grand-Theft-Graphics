#include "stdafx.h"
#include "DirectoryCommands.h"

#include <chrono>
#include <iomanip>
#include <fstream>
#include <experimental/filesystem>

using namespace std::chrono_literals;
namespace fs = std::experimental::filesystem;



void Corgi::GetFileTypePathsInDirectory(const std::string& root, const std::string& ext, std::vector<std::string>& ret, bool shouldOrderByDate)
{
	std::vector<fs::path> myPaths;
	for (auto path : fs::recursive_directory_iterator(root.c_str()))
		if (path.path().extension() == ext)
		{
			myPaths.push_back(path);
		}

	if (shouldOrderByDate == true)
	{
		struct {
			bool operator()(fs::path a, fs::path  b) const
			{
				const std::chrono::system_clock::time_point& now = std::chrono::system_clock::now();
				const auto& ftimeA = fs::last_write_time(a);
				const auto& ftimeB = fs::last_write_time(b);
				long long timeOfFileCreationA = std::chrono::duration_cast<std::chrono::microseconds>(now - ftimeA).count();
				long long timeOfFileCreationB = std::chrono::duration_cast<std::chrono::microseconds>(now - ftimeB).count();
				return timeOfFileCreationA < timeOfFileCreationB;
			}
		} customLess;

		std::sort(myPaths.begin(), myPaths.end(), customLess);
	}

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	for (auto a : myPaths) {
		auto ftimeB = fs::last_write_time(a);
		ret.push_back(a.u8string());
	}

}