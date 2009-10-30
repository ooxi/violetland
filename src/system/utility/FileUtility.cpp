#include "FileUtility.h"

void FileUtility::truncateFullPathToDir(char *path) {
	char *lastSlash = 0;
	for (; *path; path++) {
		if (*path == '\\' || *path == '/')
			lastSlash = path;
	}
	if (lastSlash)
		*lastSlash = '\0';
}

int FileUtility::getFilesCountFromDir(std::string dir)
{
	return getFilesFromDir(dir).size();
}

std::vector<std::string> FileUtility::getFilesFromDir(std::string dir) {
	std::vector<std::string> files;
	DIR *dp;
	struct dirent *ep;

	dp = opendir(dir.c_str());
	if (dp != NULL) {
		while ((ep = readdir(dp)))
#ifdef _WIN32
			if (!(ep->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				files.push_back(ep->d_name);
#endif //_WIN32W
#if defined linux || defined __FreeBSD__
		//TODO: testing is needed, code may be broken!
			if (ep->d_type == DT_REG)
				files.push_back(ep->d_name);
#endif //linux || __FreeBSD__
		(void) closedir(dp);
	}

	return files;
}

void FileUtility::traceResPath() {
	printf("Path to resources is set to:\n\t");
	printf(m_resPath.c_str());
	printf("\n");
}

FileUtility::FileUtility(char *argPath) {
#ifdef _WIN32
	char *exePath = new char [_MAX_DIR];
	GetModuleFileName(NULL , exePath, _MAX_DIR);
	FileUtility::truncateFullPathToDir(exePath);
	m_appPath = exePath;
	m_resPath = m_appPath;
	m_resPath.append("/");
	m_usrPath = m_resPath;
	delete[] exePath;
#endif //_WIN32
#if defined linux || defined __FreeBSD__
	char result[PATH_MAX];
	if (readlink("/proc/self/exe", result, PATH_MAX) != -1) {
		FileUtility::truncateFullPathToDir(result);
		m_appPath = result;
		m_resPath = m_appPath;
		m_resPath.append("/../share/violetland/");
	} else {
#ifndef INSTALL_PREFIX
#define INSTALL_PREFIX "/usr/local";
#endif //INSTALL_PREFIX
		m_appPath = m_resPath = INSTALL_PREFIX;
		m_appPath.append("/bin");
		m_resPath.append("/share/violetland/");
	}
	m_usrPath = getenv("HOME");
	m_usrPath.append("/.config");
	mkdir(m_usrPath.c_str(), S_IRWXU | S_IRGRP | S_IROTH);
	m_usrPath.append("/violetland");
	mkdir(m_usrPath.c_str(), S_IRWXU | S_IRGRP | S_IROTH);
	m_usrPath.append("/");
#endif //linux || __FreeBSD__
	traceResPath();
}

bool FileUtility::copyFile(const char srcPath[], const char destPath[]) {
	std::ifstream src;
	std::ofstream dest;

	src.open(srcPath, std::ios::binary);
	dest.open(destPath, std::ios::binary);
	if (!src.is_open() || !dest.is_open())
		return false;

	dest << src.rdbuf();
	dest.close();
	src.close();

	return true;
}

void FileUtility::setFullResPath(std::string path) {
	m_resPath = path;
	m_resPath.append("/");
	traceResPath();
}

std::string FileUtility::getFullPath(PathType type, std::string resource)
{
	std::string path(m_resPath);
	std::string usrPath(m_usrPath);

	switch(type)
	{
	case FileUtility::common:
		return path.append(resource);
	case FileUtility::image:
		path.append("images/");
		return path.append(resource);
	case FileUtility::anima:
		path.append("images/anima/");
		return path.append(resource);
	case FileUtility::sound:
		path.append("sounds/");
		return path.append(resource);
	case FileUtility::music:
		path.append("music/");
		return path.append(resource);
	case FileUtility::user:		
		return usrPath.append(resource);
	}
}