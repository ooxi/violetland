#include <iostream>

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

int FileUtility::getFilesCountFromDir(std::string dir) {
	return getFilesFromDir(dir).size();
}

std::vector<std::string> FileUtility::getFilesFromDir(std::string dir) {
	std::vector<std::string> files;
	DIR *dp;
	struct dirent *ep;
	struct stat st;

	dp = opendir(dir.c_str());
	if (dp != NULL) {
		while ((ep = readdir(dp))) {
			std::string path = dir + "/" + ep->d_name;
			if (stat(path.c_str(), &st) != 0) {
				std::cerr << "*** error: stat failed on: " << path << std::endl;
				continue;
			}
			if (S_ISREG(st.st_mode)) {
				files.push_back(ep->d_name);
				//std::cout << "REG\t" << ep->d_name << std::endl;
			}
		}
		closedir(dp);
	}

	return files;
}

std::vector<std::string> FileUtility::getSubDirsFromDir(std::string dir) {
	std::vector<std::string> subDirs;
	DIR *dp;
	struct dirent *ep;
	struct stat st;

	dp = opendir(dir.c_str());
	if (dp != NULL) {
		while ((ep = readdir(dp))) {
			if (ep->d_name[0] == '.')
				continue;
			std::string path = dir + "/" + ep->d_name;
			if (stat(path.c_str(), &st) != 0) {
				std::cerr << "*** error: stat failed on: " << path << std::endl;
				continue;
			}
			if (S_ISDIR(st.st_mode)) {
				subDirs.push_back(ep->d_name);
				//std::cout << "REG\t" << ep->d_name << std::endl;
			}
		}
		closedir(dp);
	}

	return subDirs;
}

int FileUtility::getSubDirsCountFromDir(std::string dir) {
	return getSubDirsFromDir(dir).size();
}

void FileUtility::traceResPath() {
	std::cout << "Path to resources is set to:" << std::endl;
	std::cout << '\t' << m_resPath << std::endl;
	std::cout << "To change the path use -r <path> key" << std::endl;
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
#ifndef INSTALL_PREFIX
#define INSTALL_PREFIX "/usr/local";
#endif //INSTALL_PREFIX
	m_appPath = INSTALL_PREFIX;
	m_appPath.append("/bin");
#ifndef DATA_INSTALL_DIR
	char result[PATH_MAX];
	int link_len = readlink("/proc/self/exe", result, PATH_MAX);
	if (link_len != -1) {
		result[link_len] = '\0';
		FileUtility::truncateFullPathToDir(result);
		m_appPath = result;
		m_resPath = m_appPath;
		m_resPath.append("/../share/violetland/");
	} else {
		m_resPath = "/usr/local/share/violetland/";
	}
#else //DATA_INSTALL_DIR
	m_resPath = DATA_INSTALL_DIR;
#endif //DATA_INSTALL_DIR
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

std::string FileUtility::getFullPath(PathType type, std::string resource) {
	std::string path(m_resPath);
	std::string usrPath(m_usrPath);

	switch (type) {
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
	case FileUtility::monsters:
		path.append("monsters/");
		return path.append(resource);
	case FileUtility::weapon:
		path.append("weapon/");
		return path.append(resource);
	case FileUtility::user:
		return usrPath.append(resource);
	default:
		return path.append(resource);
	}
}
