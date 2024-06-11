#ifndef _DIRECTORY_WALKER_HPP_
#define _DIRECTORY_WALKER_HPP_

#include <filesystem> 
#include <iostream> 
#include <functional> 

class DirectoryWalker
{
private:
	bool isRecursiveTraversalEnabled = false;
	std::function<void(const std::filesystem::path, int)> onFoundFile = nullptr;
	std::function<void(const std::filesystem::path, int)> onFoundFolder = nullptr;
public:
	
	void setRecursiveTraversalMode(bool flag) {
		isRecursiveTraversalEnabled = flag;
	}

	void setOnFoundFileLambda(std::function<void(const std::filesystem::path, int)> lambda) {
		onFoundFile = std::move(lambda);
	}

	void setOnFoundFolderLambda(std::function<void(const std::filesystem::path, int)> lambda) {
		onFoundFolder = std::move(lambda);
	}

	void ProcessFiles(std::filesystem::path path, int depth) {
		if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
			for (const auto& entry : std::filesystem::directory_iterator(path)) {
				if (std::filesystem::is_directory(entry.path())) {
					if (onFoundFolder) {
						onFoundFolder(entry.path(), depth);
					}
					if (isRecursiveTraversalEnabled) {
						ProcessFiles(entry.path(), depth + 1);
					}
				}
				else {
					if (onFoundFile) {
						onFoundFile(entry.path(), depth);
					}
				}
			}
		}
		else {
			std::cerr << "Directory not found.\n";
		}
	}

	void WalkCurrentPath() {
		ProcessFiles(std::filesystem::current_path(), 0);
	}

};

#endif
