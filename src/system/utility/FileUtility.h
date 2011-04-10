#ifndef FILEUTILITY_H_
#define FILEUTILITY_H_

#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <stdlib.h>
#include <string>
// dirent v1.11 for windows: http://www.softagalleria.net/dirent.php
#include <dirent.h>

class FileUtility {
private:
	std::string m_appPath, m_resPath, m_usrPath;
public:
	enum PathType {
		common = 0, image, anima, sound, music, monsters, weapon, user
	};
	FileUtility(char *argPath);
	static void truncateFullPathToDir(char *path);
	static bool copyFile(const char srcPath[], const char destPath[]);
	void traceResPath();
	void setFullResPath(std::string path);
	std::string getFullPath(PathType type, std::string resource);
	std::vector<std::string> getFilesFromDir(std::string dir);
	int getFilesCountFromDir(std::string dir);
	std::vector<std::string> getSubDirsFromDir(std::string dir);
	int getSubDirsCountFromDir(std::string dir);
};

#endif /* FILEUTILITY_H_ */
