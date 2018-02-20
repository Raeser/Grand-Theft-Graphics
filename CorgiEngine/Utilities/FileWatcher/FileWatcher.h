#pragma once
#include "../../Utilities/common_converters.h"
#include <mutex>
#include <vector>
#include <map>
#include <atomic>


// Example Code:
// Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::CharArrayToLPCWSTR("..\\Assets\\test\\test.txt"), std::bind(&CEngine::TEMP_PrintRandom, this, std::placeholders::_1));
// 
// static void TEMP_PrintRandom(std::wstring aFile)
// {
// 	std::cout << "FileWatchTest\n";
// }
class CEngine;
namespace Corgi
{
	typedef std::function<void(const std::wstring&)> callback_function_file;
	class CFileWatcher
	{
	friend class CEngine;
	public:
		static CFileWatcher& GetInstance();
		~CFileWatcher();

		/* Will check the file for includes and add them as well*/
		bool WatchFileChangeWithDependencies(std::wstring aFile, callback_function_file aFunctionToCallOnChange);
		bool WatchFileChange(std::wstring aFile, callback_function_file aFunctionToCallOnChange);
// 		bool WatchFileChange(const std::string & aFile, callback_function_file aFunctionToCallOnChange);
// 		bool WatchFileChange(const char* aFile, callback_function_file aFunctionToCallOnChange);


		void FlushChanges();
	private:
		CFileWatcher();
		bool Init(bool aIsFileWatchDebugFeatureOn);

		void UpdateChanges();
		void OnFolderChange(const std::wstring& aDir);
		bool WatchDirectory(const std::wstring& aDir);
		void OnFileChange(std::wstring& aFile);
		std::vector<WIN32_FIND_DATA> GetAllFilesInFolder(std::wstring aDir);

		std::thread* myThread;

		typedef std::map<std::wstring, std::vector<WIN32_FIND_DATA>> FolderMap;
		FolderMap myFolders;

		std::vector<std::wstring> myFileChangedThreaded;
		std::vector<std::wstring> myFileChanged;
		std::map<std::wstring, std::vector<callback_function_file> > myCallbacks;

		typedef std::map<std::wstring, std::vector<std::wstring>> Dependencies;
		Dependencies myDependencies;

		std::vector<std::wstring> myThreadedFoldersToWatch;

		std::mutex myMutex;
		std::mutex myAddNewFolderMutex;
		bool myShouldEndThread;
		bool myThreadIsDone;
		bool myIsInited;
		bool myFileWatchDebugFeaturesOn;
	};

}