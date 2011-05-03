// Copyright (c) 2004 Richard J. Wagner

#include <string>
#include <map>
#include <iostream>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_



class ConfigFile {
protected:
	std::string myDelimiter;
	std::string myComment;
	std::string mySentry;
	std::map<std::string, std::string> myContents;

	typedef std::map<std::string, std::string>::iterator mapi;
	typedef std::map<std::string, std::string>::const_iterator mapci;

public:
	ConfigFile(boost::filesystem::path filename, std::string delimiter = "=",
			std::string comment = "#", std::string sentry = "EndConfigFile");
	ConfigFile();

	template<class T> T read(const std::string& key) const;
	template<class T> T read(const std::string& key, const T& value) const;
	template<class T> bool readInto(T& var, const std::string& key) const;
	template<class T>
	bool readInto(T& var, const std::string& key, const T& value) const;

	template<class T> void add(std::string key, const T& value);
	void remove(const std::string& key);

	bool keyExists(const std::string& key) const;

	const std::string& getDelimiter() const {
		return myDelimiter;
	}
	const std::string& getComment() const {
		return myComment;
	}
	const std::string& getSentry() const {
		return mySentry;
	}
	std::string setDelimiter(const std::string& s) {
		std::string old = myDelimiter;
		myDelimiter = s;
		return old;
	}
	std::string setComment(const std::string& s) {
		std::string old = myComment;
		myComment = s;
		return old;
	}

	friend std::ostream& operator<<(std::ostream& os, const ConfigFile& cf);
	friend std::istream& operator>>(std::istream& is, ConfigFile& cf);

protected:
	template<class T> static std::string T_as_string(const T& t);
	template<class T> static T string_as_T(const std::string& s);
	static void trim(std::string& s);

public:
	struct file_not_found {
		boost::filesystem::path filename;
		file_not_found(boost::filesystem::path filename = boost::filesystem::path()) :
			filename(filename) {
		}
	};
	struct key_not_found { // thrown only by T read(key) variant of read()
		std::string key;
		key_not_found(const std::string& key_ = std::string()) :
			key(key_) {
		}
	};
};

/* static */
template<class T>
std::string ConfigFile::T_as_string(const T& t) {
	// Convert from a T to a std::string
	// Type T must support << operator
	std::ostringstream ost;
	ost << t;
	return ost.str();
}

/* static */
template<class T>
T ConfigFile::string_as_T(const std::string& s) {
	// Convert from a std::string to a T
	// Type T must support >> operator
	T t;
	std::istringstream ist(s);
	ist >> t;
	return t;
}

/* static */
template<>
inline std::string ConfigFile::string_as_T<std::string>(const std::string& s) {
	return s;
}

/* static */
template<>
inline bool ConfigFile::string_as_T<bool>(const std::string& s) {
	// Convert from a std::string to a bool
	// Interpret "false", "F", "no", "n", "0" as false
	// Interpret "true", "T", "yes", "y", "1", "-1", or anything else as true
	bool b = true;
	std::string sup = s;
	for (std::string::iterator p = sup.begin(); p != sup.end(); ++p)
		*p = toupper(*p); // make std::string all caps
	if (sup == std::string("FALSE") || sup == std::string("F") || sup
			== std::string("NO") || sup == std::string("N") || sup
			== std::string("0") || sup == std::string("NONE"))
		b = false;
	return b;
}

template<class T>
T ConfigFile::read(const std::string& key) const {
	// Read the value corresponding to key
	mapci p = myContents.find(key);
	if (p == myContents.end())
		throw key_not_found(key);
	return string_as_T<T> (p->second);
}

template<class T>
T ConfigFile::read(const std::string& key, const T& value) const {
	// Return the value corresponding to key or given default value
	// if key is not found
	mapci p = myContents.find(key);
	if (p == myContents.end())
		return value;
	return string_as_T<T> (p->second);
}

template<class T>
bool ConfigFile::readInto(T& var, const std::string& key) const {
	// Get the value corresponding to key and store in var
	// Return true if key is found
	// Otherwise leave var untouched
	mapci p = myContents.find(key);
	bool found = (p != myContents.end());
	if (found)
		var = string_as_T<T> (p->second);
	return found;
}

template<class T>
bool ConfigFile::readInto(T& var, const std::string& key, const T& value) const {
	// Get the value corresponding to key and store in var
	// Return true if key is found
	// Otherwise set var to given default
	mapci p = myContents.find(key);
	bool found = (p != myContents.end());
	if (found)
		var = string_as_T<T> (p->second);
	else
		var = value;
	return found;
}

template<class T>
void ConfigFile::add(std::string key, const T& value) {
	std::string v = T_as_string(value);
	trim(key);
	trim(v);
	myContents[key] = v;
	return;
}

#endif /* CONFIGFILE_H_ */
