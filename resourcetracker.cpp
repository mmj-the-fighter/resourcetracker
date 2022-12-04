//TODO: Avoid global variables.

#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <ctime>


std::unordered_set<std::wstring> g_resourcefile_extensions;
std::unordered_map<std::wstring, int> g_fileTypeFileCountMap;
unsigned int g_folderCount = 0;
unsigned int g_fileCount = 0;
bool g_isRecursive = false;
bool g_isAllFilesToBeProcessed = false;

class QuickProfiler
{
public:
	std::chrono::high_resolution_clock::time_point t0;
	long long millis;
	double seconds;
	long long diff;

	QuickProfiler() {
		millis = diff = 0;
		seconds = 0.0;
	}

	inline void Start()
	{
		t0 = std::chrono::high_resolution_clock::now();
	}

	inline void Stop()
	{
		using namespace std::chrono;
		system_clock::time_point t1 = high_resolution_clock::now();
		millis = duration_cast<milliseconds>(t1 - t0).count();
		diff = (t1 - t0).count();
		seconds = static_cast<double>(millis) / 1000.0;
	}
};

std::wstring ReadExtensionFromPath(TCHAR* path)
{
	TCHAR c;
	static TCHAR ext[MAX_PATH];
	static std::wstring eext(L"");
	int i = 0;
	int j = 0;

	//find the length
	while (*(path + i) != '\0') {
		++i;
	}

	if (i == 0) {
		return eext;
	}


	//read from last char till a dot
	--i;
	while (i >= 0) {
		c = *(path + i);
		if (c != '.') {
			ext[j++] = c;
		}
		else{
			break;
		}
		--i;
	}
	ext[j] = '\0';

	// reverse buffer
	i = 0;
	j = j - 1;
	while (i < j)
	{
		c = ext[i];
		ext[i] = ext[j];
		ext[j] = c;
		++i;
		--j;
	}

	std::wstring extension(ext);
	return extension;
}

void ProcessFiles(std::wstring sourcepath, int depth)
{
	WIN32_FIND_DATA findResult;
	std::wstring tpath1 = sourcepath;
	tpath1.append(_T("\\*"));
	HANDLE handle = FindFirstFile(tpath1.c_str(), &findResult);
	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findResult.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (lstrcmp(_T("."), findResult.cFileName) == 0 ||
					lstrcmp(_T(".."), findResult.cFileName) == 0)
				{
					continue;
				}
				else
				{
					++g_folderCount;
					for (int i = 0; i < depth; i++){
						std::wcout << L" ";
					}
					std::wcout << findResult.cFileName << std::endl;
					std::wstring tpath = sourcepath;
					tpath.append(_T("\\")).append(findResult.cFileName);
					if (g_isRecursive){
						ProcessFiles(tpath, depth + 1);
					}
				}
			}
			else{
				++g_fileCount;
				//Read extension
				std::wstring ext = ReadExtensionFromPath(findResult.cFileName);
				//update count 
				if (g_fileTypeFileCountMap.find(ext) == g_fileTypeFileCountMap.end())
				{
					g_fileTypeFileCountMap[ext] = 1;
				}
				else{
					g_fileTypeFileCountMap[ext]++;
				}

				//if desired file type print filename
				if (g_isAllFilesToBeProcessed ||
					g_resourcefile_extensions.find(ext) != g_resourcefile_extensions.end())
				{
					for (int i = 0; i < depth; i++){
						std::wcout << L" ";
					}
					std::wcout << findResult.cFileName << std::endl;
				}

			}
		} while (FindNextFile(handle, &findResult));
	}
	if (handle)
	{
		FindClose(handle);
		handle = NULL;
	}
}

void InsertExtension(std::wstring ext)
{
	std::wstring extInCaps = ext;
	std::transform(extInCaps.begin(), extInCaps.end(), extInCaps.begin(), ::towupper);
	g_resourcefile_extensions.insert(ext);
	g_resourcefile_extensions.insert(extInCaps);
}

void PopulateWithDefaultExtensions(){
	InsertExtension(std::wstring(L"pdf"));
	InsertExtension(std::wstring(L"ps"));
	InsertExtension(std::wstring(L"docx"));
	InsertExtension(std::wstring(L"doc"));
	InsertExtension(std::wstring(L"ppt"));
	InsertExtension(std::wstring(L"pptx"));
	InsertExtension(std::wstring(L"htm"));
	InsertExtension(std::wstring(L"html"));
	InsertExtension(std::wstring(L"chm"));
	InsertExtension(std::wstring(L"epub"));
	InsertExtension(std::wstring(L"djvu"));
	InsertExtension(std::wstring(L"mp4"));
	InsertExtension(std::wstring(L"3gp"));
	InsertExtension(std::wstring(L"txt"));
	InsertExtension(std::wstring(L"zip"));
	InsertExtension(std::wstring(L"tar"));
	InsertExtension(std::wstring(L"gz"));
	InsertExtension(std::wstring(L"xz"));
	InsertExtension(std::wstring(L"rar"));
	InsertExtension(std::wstring(L"7z"));

}

void PrintDateAndTime()
{
	char datetime_text[256];
	time_t tm = time(NULL);
	ctime_s(datetime_text, sizeof(datetime_text), &tm);
	std::cout << datetime_text << std::endl;
}


int _tmain(int argc, _TCHAR* argv[])
{
	QuickProfiler profiler;

	TCHAR currentDirectory[MAX_PATH];
	DWORD result;
	profiler.Start();

	g_isRecursive = false;
	std::wstring recursiveOption = L"-r";
	if (argc == 1)
	{
		PopulateWithDefaultExtensions();
	}
	else if (argc == 2)
	{
		std::wstring option = std::wstring(argv[1]);
		if (option == recursiveOption){
			g_isRecursive = true;
			PopulateWithDefaultExtensions();
		}
		else{
			InsertExtension(option);
		}
	}
	else
	{
		std::wstring option = std::wstring(argv[1]);
		if (option == recursiveOption){
			g_isRecursive = true;
		}
		else{
			InsertExtension(option);
		}

		for (int i = 2; i < argc; i++)
		{
			std::wstring option = std::wstring(argv[i]);
			InsertExtension(option);
		}
	}


	std::wstring allfilesext = L"*";
	if (g_resourcefile_extensions.find(allfilesext) == g_resourcefile_extensions.end())
		g_isAllFilesToBeProcessed = false;
	else
		g_isAllFilesToBeProcessed = true;
	
	PrintDateAndTime();
	result = GetCurrentDirectory(MAX_PATH, currentDirectory);
	if (result == 0)
	{
		profiler.Stop();
		return 1;
	}

	//	ProcessFiles(_T("D:\\Resources"), 0);
	std::wcout << currentDirectory << std::endl;
	ProcessFiles(currentDirectory, 0);


	std::cout << "\nFiles " << g_fileCount;
	std::cout << "\nFolders " << g_folderCount;

	//sort unordered_map
	std::vector<std::pair<std::wstring, int>> v;
	for (auto& kv : g_fileTypeFileCountMap) {
		v.push_back(kv);
	}
	std::sort(v.begin(), v.end(),
		[&](std::pair<std::wstring, int>& a, std::pair<std::wstring, int>& b) {
		return a.second > b.second;
	});


	//display sorted map
	std::cout << "\n*********************";
	for (auto i = v.begin(); i != v.end(); i++)
	{
		std::wstring filetype = i->first;
		std::string s(filetype.begin(), filetype.end());
		int count = i->second;
		std::cout << "\n" << s << " type file count: " << count << std::endl;
	}
	profiler.Stop();
	std::cout << "\n*********************\n" << std::endl;
	std::cout << "\nProcessing time: " << profiler.seconds << " Seconds"
		<< "(" << profiler.millis << " Milliseconds)" << std::endl;
	return 0;
}