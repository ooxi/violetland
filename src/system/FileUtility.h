#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
//dirent for windows can be obtained at http://www.softagalleria.net/dirent.php
#include <dirent.h>

#ifndef FILEUTILITY_H_
#define FILEUTILITY_H_

class FileUtility {
private:
	std::string m_appPath, m_resPath, m_usrPath;
public:
	FileUtility(char *argPath);
	static void truncateFullPathToDir(char *path);
	static bool copyFile(const char srcPath[], const char destPath[]);
	void traceResPath();
	void setFullResPath(std::string path);
	std::string getFullResPath(std::string resource);
	std::string getFullImagePath(std::string resource);
	std::string getFullAimPath(std::string resource);
	std::string getFullAnimaPath(std::string resource);
	std::string getFullSoundPath(std::string resource);
	std::string getFullUserPath(std::string resource);
	int getFilesCountFromDir(std::string dir);
};

#endif /* FILEUTILITY_H_ */
