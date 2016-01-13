#ifndef VIOLET_FILEUTILITY_H_
#define VIOLET_FILEUTILITY_H_

#include <boost/filesystem.hpp>
#include <string>
#include <vector>



namespace violet {

class FileUtility {
private:
	boost::filesystem::path m_resPath;
	boost::filesystem::path m_usrPath;
	
	static FileUtility* ofWindows(char const* argvZero);
	static FileUtility* ofUnix();
	
	void traceResPath();
	
	
public:
	enum PathType {
		common = 0,
		image,
		anima,
		sound,
		music,
		monsters,
		weapon,
		user
	};
	
	
	static FileUtility* ofOs(char const* argvZero);
	
	
	static unsigned int getFilesCountFromDir(boost::filesystem::path dir);
	static unsigned int getSubDirsCountFromDir(boost::filesystem::path dir);
	static std::vector<std::string> getFilesFromDir(boost::filesystem::path dir);
	static std::vector<std::string> getSubDirsFromDir(boost::filesystem::path dir);
	
	
	FileUtility(	boost::filesystem::path const& resources,
			boost::filesystem::path const& user);

	void setResourcePath(boost::filesystem::path const& path);
	boost::filesystem::path getFullPath(PathType type, std::string resource) const;
};
}

#endif /* VIOLET_FILEUTILITY_H_ */
