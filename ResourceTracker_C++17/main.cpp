#include <filesystem> 
#include <iostream> 
#include "DirectoryWalker.hpp"
#include "QuickProfiler.hpp"

using namespace std; 
using namespace std::filesystem; 

int main()
{
	QuickProfiler qp;
	DirectoryWalker dw;
	unsigned int fileCount = 0;
	unsigned int folderCount = 0;
	dw.setOnFoundFileLambda([&fileCount](std::filesystem::path path, int depth) {
		for (int i = depth; i > 0; --i) {
			std::cout << " ";
		}
		cout << "File:" << path.filename() << "\n";
		++fileCount;
	});
	dw.setOnFoundFolderLambda([&folderCount](std::filesystem::path path, int depth) {
		for (int i = depth; i > 0; --i) {
			std::cout << " ";
		}
		cout << "Folder:" << path.filename() << "\n";
		++folderCount;
	});
	dw.setRecursiveTraversalMode(true);
	std::cout << "Files/Folders at: " << std::filesystem::current_path() << "\n";
	qp.Start();
	dw.WalkCurrentPath();
	qp.Stop();
	std::cout << "\n*********************\n";
	std::cout << "\nProcessing time: " << qp.seconds << " Seconds"
		<< "(" << qp.millis << " Milliseconds)" << "\n";
	std::cout << "Folder count "<<folderCount << "\n";
	std::cout << "File count " << fileCount << "\n";
	return 0;
}


