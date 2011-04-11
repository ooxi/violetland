#ifndef FILEUTILITY_H_
#define FILEUTILITY_H_

#include <vector>
#include <sys/stat.h>
#include <cstdlib>
#include <string>

#include <boost/filesystem.hpp>

using namespace boost;

class FileUtility {
private:
	filesystem::path m_appPath, m_resPath, m_usrPath;
public:
	enum PathType {
		common = 0, image, anima, sound, music, monsters, weapon, user
	};
	FileUtility(char *argPath);
	static void truncateFullPathToDir(char *path);
	static void copyFile(filesystem::path srcPath, filesystem::path destPath);
	void traceResPath();
	void setFullResPath(std::string path);
	filesystem::path getFullPath(PathType type, std::string resource);
	std::vector<std::string> getFilesFromDir(filesystem::path dir);
	unsigned int getFilesCountFromDir(filesystem::path dir);
	std::vector<std::string> getSubDirsFromDir(filesystem::path dir);
	unsigned int getSubDirsCountFromDir(filesystem::path dir);
};

#endif /* FILEUTILITY_H_ */
