#include <iostream>
#include "FileUtility.h"


unsigned int FileUtility::getFilesCountFromDir(filesystem::path dir) {
	unsigned int count = 0;
	filesystem::directory_iterator dir_it(dir);

	while (dir_it != filesystem::directory_iterator()) {
		if (filesystem::is_regular_file(*dir_it))
			++count;
		++dir_it;
	}
	return count;
}

std::vector<std::string> FileUtility::getFilesFromDir(filesystem::path dir) {
	std::vector<std::string> files;
	filesystem::directory_iterator dir_it(dir);

	while (dir_it != filesystem::directory_iterator()) {
		if (filesystem::is_regular_file(*dir_it))
			files.push_back(dir_it->filename());
		++dir_it;
	}
	return files;
}

std::vector<std::string> FileUtility::getSubDirsFromDir(filesystem::path dir) {
	std::vector<std::string> subDirs;
	filesystem::directory_iterator dir_it(dir);

	while (dir_it != filesystem::directory_iterator()) {
		if (filesystem::is_directory(*dir_it))
			if (dir_it->path().filename()[0] != '.')
				subDirs.push_back(dir_it->path().filename());
		++dir_it;
	}
	return subDirs;
}

unsigned int FileUtility::getSubDirsCountFromDir(filesystem::path dir) {
	filesystem::directory_iterator dir_it(dir);
	unsigned int count = 0;

	while (dir_it != filesystem::directory_iterator()) {
		if (filesystem::is_directory(*dir_it))
			if (dir_it->path().filename()[0] != '.')
				++count;
		++dir_it;
	}
	return count;
}

void FileUtility::traceResPath() {
	std::cout << "Path to resources is set to:" << std::endl;
	std::cout << '\t' << m_resPath << std::endl;
	std::cout << "To change the path use -r <path> key" << std::endl;
}

FileUtility::FileUtility(char *argPath) {
#ifdef _WIN32
	m_appPath = argPath;
	m_appPath = m_appPath.parent_path();
	m_resPath = m_appPath;
	m_usrPath = m_resPath;
#endif //_WIN32
#if defined linux || defined __FreeBSD__
#ifndef INSTALL_PREFIX
#define INSTALL_PREFIX "/usr/local";
#endif //INSTALL_PREFIX
	m_appPath = INSTALL_PREFIX;
	m_appPath /= "bin";
#ifndef DATA_INSTALL_DIR
	m_appPath = argPath;
	m_appPath = m_appPath.parent_path();
	m_resPath /= "../share/violetland";
#else //DATA_INSTALL_DIR
	m_resPath = DATA_INSTALL_DIR;
#endif //DATA_INSTALL_DIR
	m_usrPath = getenv("HOME");
	m_usrPath /= ".config";
	mkdir(m_usrPath.string().c_str(), S_IRWXU | S_IRGRP | S_IROTH);
	m_usrPath /= "violetland";
	mkdir(m_usrPath.string().c_str(), S_IRWXU | S_IRGRP | S_IROTH);
#endif //linux || __FreeBSD__
	traceResPath();
}

void FileUtility::setFullResPath(std::string path) {
	m_resPath = path;
	traceResPath();
}

filesystem::path FileUtility::getFullPath(PathType type, std::string resource) const {
	filesystem::path path(m_resPath);
	filesystem::path usrPath(m_usrPath);

	switch (type) {
	case FileUtility::image:
		path /= "images";
		return path /= resource;
	case FileUtility::anima:
		path /= "images";
		path /= "anima";
		return path /= resource;
	case FileUtility::sound:
		path /= "sounds";
		return path /= resource;
	case FileUtility::music:
		path /= "music";
		return path /= resource;
	case FileUtility::monsters:
		path /= "monsters";
		return path /= resource;
	case FileUtility::weapon:
		path /= "weapon";
		return path /= resource;
	case FileUtility::user:
		return usrPath /= resource;
	default:
		return path /= resource;
	}
}
