#include "..\stdafx.h"
#include "FileWatcher.h"

Corgi::CFileWatcher & Corgi::CFileWatcher::GetInstance()
{
	static Corgi::CFileWatcher fileWatcher;
	return fileWatcher;
}

Corgi::CFileWatcher::CFileWatcher() 
	:myThread(nullptr)
	,myShouldEndThread (false)
	,myThreadIsDone (false)
	,myIsInited (false)
{
}


Corgi::CFileWatcher::~CFileWatcher()
{
	myShouldEndThread = true;
	if (myThread)
	{
		while (!myThreadIsDone)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		}
	}

}

bool Corgi::CFileWatcher::Init(bool aIsFileWatchDebugFeatureOn)
{
	if (myIsInited == false)
	{
		myIsInited = true;
		myFileWatchDebugFeaturesOn = aIsFileWatchDebugFeatureOn;
		return true;
	}
	return false;
}


void Corgi::CFileWatcher::FlushChanges()
{
	if (!myIsInited)
	{
		return;
	}
	if (!myFileWatchDebugFeaturesOn || !myThread)
	{
		return;
	}
	if (myFileChangedThreaded.size() <= 0)
	{
		return;
	}
	std::lock_guard<std::mutex> guard(myMutex);


	myFileChanged.swap(myFileChangedThreaded);

	for (std::wstring& theString : myFileChanged)
	{
		std::wstring directoryOfFile(theString);
		directoryOfFile = directoryOfFile.substr(0, directoryOfFile.find_last_of(L"\\/"));

		std::wstring theFile(theString);
		theFile = theFile.substr(theFile.find_first_of(L"\\/") + 1, theFile.size());

		std::vector<callback_function_file> callbacks = myCallbacks[theFile];
		for (unsigned int i = 0; i < callbacks.size(); i++)
		{
			if (callbacks[i])
			{
				callbacks[i](theString);
			}
		}
	}


	myFileChanged.clear();


}

void Corgi::CFileWatcher::UpdateChanges()
{
	while (!myShouldEndThread)
	{
		myMutex.lock();
		myAddNewFolderMutex.lock();
		for (std::wstring& folder : myThreadedFoldersToWatch)
		{
			OnFolderChange(folder);
		}

		myMutex.unlock();
		myAddNewFolderMutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	myThreadIsDone = true;
}

void Corgi::CFileWatcher::OnFolderChange(const std::wstring& aDir)
{
	std::vector<WIN32_FIND_DATA> currentFolderFiles = GetAllFilesInFolder(aDir);
	std::vector<WIN32_FIND_DATA>& savedFolderFiles = myFolders[aDir];

	for (WIN32_FIND_DATA& currentFile : currentFolderFiles)
	{
		for (WIN32_FIND_DATA& savedFile : savedFolderFiles)
		{
			if (std::wstring(currentFile.cFileName).compare(savedFile.cFileName) == 0)
			{
				ULARGE_INTEGER currentFileTime;
				SYSTEMTIME currentFileTimeSystem;
				FileTimeToSystemTime(&currentFile.ftLastWriteTime, &currentFileTimeSystem);
				currentFileTime.LowPart = currentFile.ftLastWriteTime.dwLowDateTime;
				currentFileTime.HighPart = currentFile.ftLastWriteTime.dwHighDateTime;
				__int64 currentFileTime64 = currentFileTime.QuadPart;

				ULARGE_INTEGER savedFileTime;
				SYSTEMTIME savedFileTimeSystem;
				FileTimeToSystemTime(&savedFile.ftLastWriteTime, &savedFileTimeSystem);
				savedFileTime.LowPart = savedFile.ftLastWriteTime.dwLowDateTime;
				savedFileTime.HighPart = savedFile.ftLastWriteTime.dwHighDateTime;
				__int64 savedFileTime64 = savedFileTime.QuadPart;

				if (currentFileTime64 != savedFileTime64)
				{
					std::wstring fileNameString(currentFile.cFileName);
					Corgi::ConvertWideStringPathToSafeUse(fileNameString);
					std::wstring fileThatChangedPath = aDir + L"/" + fileNameString;
					bool isDependency = myDependencies.find(fileThatChangedPath) != myDependencies.end();
					if (isDependency)
					{
						std::vector<std::wstring>& deps = myDependencies[fileThatChangedPath];
						for (std::wstring& file : deps)
						{
							OnFileChange(file);
						}

					}
					else
					{
						OnFileChange(fileThatChangedPath);
					}

					// We have a change

					savedFile = currentFile;
				}
			}
		}
	}
}

void Corgi::CFileWatcher::OnFileChange(std::wstring& aFile)
{
	for (unsigned int i = 0; i < myFileChangedThreaded.size(); i++)
	{
		if (myFileChangedThreaded[i].compare(aFile) == 0)
		{
			return;
		}
	}
	myFileChangedThreaded.push_back(aFile);

}

bool Corgi::CFileWatcher::WatchFileChangeWithDependencies(std::wstring aFile, callback_function_file aFunctionToCallOnChange)
{
#ifndef _RETAIL
	if (!myIsInited)
	{
		return false;
	}
	if (!myFileWatchDebugFeaturesOn)
	{
		return false;
	}
	
	Corgi::ConvertWideStringPathToSafeUse(aFile);
	std::ifstream stream(aFile);
	if (!stream.good())
	{
		stream.close();
		return false;
	}

	std::wstring directoryOfFile(aFile);
	directoryOfFile = directoryOfFile.substr(0, directoryOfFile.find_last_of(L"\\/"));

	if (directoryOfFile == aFile)	 //In case the input path is default directory which doesnt work
	{
		return false;
	}

	std::string line;
	const std::string includeString = "include";
	while (getline(stream, line))
	{
		std::size_t found = line.find(includeString);
		if (found != std::string::npos)
		{
			std::string foundFile(line);
			foundFile = foundFile.substr(foundFile.find_first_of("\"") + 1, foundFile.size());
			foundFile = foundFile.substr(0, foundFile.find_last_of("\""));
			Corgi::ConvertStringPathToSafeUse(foundFile);
			if (!foundFile.empty())
			{
				std::wstring depFile = directoryOfFile + L"/" + Corgi::ConvertStringToWide(foundFile);
				WatchFileChange(depFile, aFunctionToCallOnChange);
				myDependencies[depFile].push_back(aFile);
			}
		}
	}

	stream.close();
	return WatchFileChange(aFile, aFunctionToCallOnChange);
#else
	aFile; aFunctionToCallOnChange;
	return false;
#endif
}

bool Corgi::CFileWatcher::WatchFileChange(std::wstring aFile, callback_function_file aFunctionToCallOnChange)
{
#ifdef _RETAIL
	aFile; aFunctionToCallOnChange;
	return false;
#else
	if (!myIsInited)
	{
		return false;
	}

	if (!myFileWatchDebugFeaturesOn)
	{
		return false;
	}

	Corgi::ConvertWideStringPathToSafeUse(aFile);
	std::ifstream stream(aFile);
	if (!stream.good())
	{
		stream.close();
		return false;
	}
	stream.close();

	std::wstring directoryOfFile(aFile);
	directoryOfFile = directoryOfFile.substr(0, directoryOfFile.find_last_of(L"\\/"));

	if (directoryOfFile == aFile)	 //In case the input path is default directory which doesnt work
	{
		return false;
	}

	std::wstring theFile(aFile);
	theFile = theFile.substr(theFile.find_first_of(L"\\/") + 1, theFile.size());

	myCallbacks[theFile].clear();
	myCallbacks[theFile].push_back(aFunctionToCallOnChange);
	return WatchDirectory(directoryOfFile);
#endif
}

// bool Corgi::CFileWatcher::WatchFileChange(const std::string& aFile, callback_function_file aFunctionToCallOnChange)
// {
// 	return WatchFileChange(Corgi::ConvertStringToWide(aFile), aFunctionToCallOnChange);
// }
// 
// bool Corgi::CFileWatcher::WatchFileChange(const char* aFile, callback_function_file aFunctionToCallOnChange)
// {
// 	return WatchFileChange(Corgi::CharArrayToLPCWSTR(aFile), aFunctionToCallOnChange);
// }

bool Corgi::CFileWatcher::WatchDirectory(const std::wstring& aDir)
{
	FolderMap::iterator iter = myFolders.find(aDir);
	if (iter != myFolders.end())
	{
		//Already in our watch list
		return true;
	}

	myFolders[aDir] = GetAllFilesInFolder(aDir);

	if (myThread)
	{
		myAddNewFolderMutex.lock();
		myThreadedFoldersToWatch.push_back(aDir);
		myAddNewFolderMutex.unlock();
	}
	else
	{
		myThreadedFoldersToWatch.push_back(aDir);
		myThread = new std::thread(&CFileWatcher::UpdateChanges, this);
	}

	return true;
}

std::vector<WIN32_FIND_DATA> Corgi::CFileWatcher::GetAllFilesInFolder(std::wstring aDir)
{
	std::vector<WIN32_FIND_DATA> filesInFolder;
	aDir += L"/*.*";
	WIN32_FIND_DATA fd;

	HANDLE hFind = ::FindFirstFile(aDir.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				filesInFolder.push_back(fd);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return filesInFolder;
}
