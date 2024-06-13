#include <filesystem> 
#include <unordered_set>
#include <unordered_map>
#include <iostream> 
#include <algorithm>
#include "DirectoryWalker.hpp"
#include "QuickProfiler.hpp"

void InsertExtension(
	std::unordered_set<std::string>& resourceFileExtensions,
	std::string fileExtension)
{
	std::string fileExtensionInCaps = fileExtension;
	std::transform(fileExtensionInCaps.begin(), fileExtensionInCaps.end(), fileExtensionInCaps.begin(), ::toupper);
	resourceFileExtensions.insert(fileExtension);
	resourceFileExtensions.insert(fileExtensionInCaps);
}

void PopulateWithDefaultExtensions(std::unordered_set<std::string>& resourceFileExtensions) {
	InsertExtension(resourceFileExtensions,".pdf");
	InsertExtension(resourceFileExtensions,".exe");
	InsertExtension(resourceFileExtensions,".tex");
	InsertExtension(resourceFileExtensions,".ps");
	InsertExtension(resourceFileExtensions, ".docx");
	InsertExtension(resourceFileExtensions, ".doc");
	InsertExtension(resourceFileExtensions, ".ppt");
	InsertExtension(resourceFileExtensions, ".pptx");
	InsertExtension(resourceFileExtensions, ".htm");
	InsertExtension(resourceFileExtensions, ".html");
	InsertExtension(resourceFileExtensions, ".chm");
	InsertExtension(resourceFileExtensions, ".epub");
	InsertExtension(resourceFileExtensions, ".djvu");
	InsertExtension(resourceFileExtensions, ".mp4");
	InsertExtension(resourceFileExtensions, ".3gp");
	InsertExtension(resourceFileExtensions, ".txt");
	InsertExtension(resourceFileExtensions, ".zip");
	InsertExtension(resourceFileExtensions, ".tar");
	InsertExtension(resourceFileExtensions, ".gz");
	InsertExtension(resourceFileExtensions, ".xz");
	InsertExtension(resourceFileExtensions, ".rar");
}

void DisplayFileExtensionFrequencies(std::unordered_map<std::string, int> &fileExtensionFileCountMap) 
{	
	//Sort map: Descending order
	std::vector<std::pair<std::string, int>> v;
	for (auto& kv : fileExtensionFileCountMap) {
		v.push_back(kv);
	}
	std::sort(v.begin(), v.end(),
		[&](std::pair<std::string, int>& a, std::pair<std::string, int>& b) {
			return a.second > b.second;
		}
	);
	
	//Display
	for (auto i = v.begin(); i != v.end(); i++)
	{
		std::string filetype = i->first;
		std::string s(filetype.begin(), filetype.end());
		int count = i->second;
		std::cout << "\n" << s << " type file count: " << count << std::endl;
	}
}

int main(int argc, char* argv[])
{
	bool isAllFilesToBeProcessed = false;
	std::unordered_set<std::string> resourceFileExtensions;
	
	QuickProfiler qp;
	DirectoryWalker dw;
	dw.setRecursiveTraversalMode(false);
	std::unordered_map<std::string, int> fileExtensionFileCountMap;
	unsigned int fileCount = 0;
	unsigned int folderCount = 0;
	char dotChar = '.';

	std::string recursiveOption = "-r";
	if (argc == 1)
	{
		//Test case: ResourceTracker
		PopulateWithDefaultExtensions(resourceFileExtensions);
	}
	else if (argc == 2)
	{
		std::string option = std::string(argv[1]);
		if (option == recursiveOption) {
			//Test case: ResourceTracker -r
			dw.setRecursiveTraversalMode(true);
			PopulateWithDefaultExtensions(resourceFileExtensions);
		}
		else {
			//Test case: ResourceTracker cpp
			option.insert(0, 1, dotChar);
			InsertExtension(resourceFileExtensions, option);
		}
	}
	else
	{
		std::string option = std::string(argv[1]);
		if (option == recursiveOption) {
			//Test case: ResourceTracker -r h cpp hpp
			dw.setRecursiveTraversalMode(true);
		}
		else {
			//Test case: ResourceTracker h cpp hpp
			option.insert(0, 1, dotChar);
			InsertExtension(resourceFileExtensions, option);
		}

		for (int index = 2; index < argc; ++index)
		{
			//Test case: ResourceTracker -r h cpp hpp
			//Test case: ResourceTracker  h cpp hpp
			std::string option = std::string(argv[index]);
			option.insert(0, 1, dotChar);
			InsertExtension(resourceFileExtensions, option);
		}
	}

	//Test case: ResourceTracker -r *
	//Test case: ResourceTracker * 
	std::string allFilesExt = ".*";
	if (resourceFileExtensions.find(allFilesExt) ==
		resourceFileExtensions.end()) 
	{
		isAllFilesToBeProcessed = false;
	}
	else 
	{
		isAllFilesToBeProcessed = true;
	}
		


	dw.setOnFoundFileLambda([&isAllFilesToBeProcessed, &resourceFileExtensions, &fileExtensionFileCountMap, &fileCount](std::filesystem::path path, int depth) {
		std::string fileExtension = path.extension().string();
		if (isAllFilesToBeProcessed ||
			resourceFileExtensions.find(fileExtension) != 
			resourceFileExtensions.end())
		{
			for (int i = depth; i > 0; --i) {
				std::cout << " ";
			}
			std::cout << path.filename() << "\n";
		}
		if (fileExtensionFileCountMap.find(fileExtension) == fileExtensionFileCountMap.end())
		{
			fileExtensionFileCountMap[fileExtension] = 1;
		}
		else {
			++fileExtensionFileCountMap[fileExtension];
		}
		++fileCount;
	});
	dw.setOnFoundFolderLambda([&folderCount](std::filesystem::path path, int depth) {
		for (int i = depth; i > 0; --i) {
			std::cout << " ";
		}
		std::cout << "/" << path.filename() << "\n";
		++folderCount;
	});
	
	std::cout << "Files and Folders at: " << std::filesystem::current_path() << "\n";
	std::cout << "*********************\n";
	qp.Start();
	dw.WalkCurrentPath();
	qp.Stop();
	std::cout << "\n*********Stats************\n";
	std::cout << "\nProcessing time: " << qp.seconds << " Seconds"
		<< "(" << qp.millis << " Milliseconds)" << "\n";
	std::cout << "Folder count "<<folderCount << "\n";
	std::cout << "File count " << fileCount << "\n";
	std::cout << "\n**********Extensions Stats***********\n";
	DisplayFileExtensionFrequencies(fileExtensionFileCountMap);
	return 0;
}
