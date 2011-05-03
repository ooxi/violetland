#ifndef FILEUTILITY_H_
#define FILEUTILITY_H_

#include <vector>
#include <sys/stat.h>
#include <cstdlib>
#include <string>

#include <boost/filesystem.hpp>



class FileUtility {
private:
	boost::filesystem::path m_appPath, m_resPath, m_usrPath;
public:
	enum PathType {
		common = 0, image, anima, sound, music, monsters, weapon, user
	};
	FileUtility(char *argPath);
	static void truncateFullPathToDir(char *path);
	void traceResPath();
	void setFullResPath(std::string path);
	boost::filesystem::path getFullPath(PathType type, std::string resource) const;
	std::vector<std::string> getFilesFromDir(boost::filesystem::path dir);
	unsigned int getFilesCountFromDir(boost::filesystem::path dir);
	std::vector<std::string> getSubDirsFromDir(boost::filesystem::path dir);
	unsigned int getSubDirsCountFromDir(boost::filesystem::path dir);
};

#endif /* FILEUTILITY_H_ */
