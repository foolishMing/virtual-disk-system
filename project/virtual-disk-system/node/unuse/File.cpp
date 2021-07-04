#include"File.h"
File::File(std::string pathname) {
	updata(pathname);
}
void File::updata(std::string pathname) {
	path = pathname;
	TCHAR  buffer[BUFSIZE] = TEXT("");
	TCHAR* lppPart = { NULL };
	GetFullPathName(pathname.c_str(), BUFSIZE, buffer, &lppPart);
	if (lppPart != NULL)
		name = lppPart;
	absolute_path = buffer;
	long hFile=_findfirst(absolute_path.c_str(), &fileInfo);
	_findclose(hFile);
}
std::string File::getName() {
	return name;
}
std::string File::getAbsolutePath() {
	return absolute_path;
}
std::string File::getPath() {
	return path;
}
std::string File::getParent() {
	return absolute_path.substr(0, absolute_path.rfind("\\"));
}
bool File::isAbsolute() {
	return path == absolute_path;
}
bool File::isDirectory() {
	if(exists())
		return fileInfo.attrib==_A_SUBDIR;
	return 0;
}
bool File::isHidden() {
	if (exists())
		return fileInfo.attrib == _A_HIDDEN;
	return 0;
}
bool File::exists() {
	return !_access(path.c_str(),EXIST);
}
bool File::canRead() {
	if (exists())
		return !_access(path.c_str(), READABLE);
	return 0;
}
bool File::canWrite() {
	if (exists())
		return !_access(path.c_str(), WRITEABLE);
	return 0;
}
bool File::canRun() {
	if (exists())
		return !_access(path.c_str(), EXECUTABLE);
	return 0;
}
bool File::isFile() {
	if (exists())
		return fileInfo.attrib == _A_ARCH;
	return 0;
}
bool File::mkdir() {
	if (_mkdir(absolute_path.c_str())) {
		updata(absolute_path);
		return 1;
	}
	return 0;
}
bool File::renameTo(std::string newName) {
	std::string newPath = getParent() + "\\" + newName;
	if (exists()&&rename(absolute_path.c_str(), newPath.c_str())) {
		updata(newPath);
		return 1;
	}
	return 0;
}
bool File::Delete() {
	if (exists() && remove(absolute_path.c_str())) {
		return 1;
	}
	return 0;
}
long long File::length() {
	return fileInfo.size;
}
Time File::getTime(time_t tt) {
	tm t;   //tm结构指针
	localtime_s(&t, &tt);
	return { t.tm_year + 1900,t.tm_mon + 1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec };
}
Time File::getCreateTime() {
	if (exists())
		return getTime(fileInfo.time_create);
	return {};
}
Time File::getAccessTime() {
	if (exists())
		return getTime(fileInfo.time_access);
	return {};
}
Time File::getWriteTime() {
	if (exists())
		return getTime(fileInfo.time_write);
	return {};
}
File File::getParentFile() {
	return File(getParent());
}
File File::getAbsoluteFile() {
	return File(getAbsoluteFile());
}
std::vector<std::string> File::list() {
	std::vector<std::string> list;
	if (isDirectory()) {
		_finddata_t temp;
		long find = _findfirst((absolute_path+"\\*").c_str(), &temp);
		_findnext(find, &temp); //去除2个超链接
		_findnext(find, &temp);
		do {
			list.push_back(temp.name);
		} while (_findnext(find, &temp) == 0);
		_findclose(find);
	}
	return list;
}
std::vector<File> File::listFile() {
	std::vector<File> listFile;
	if (isDirectory()) 
		for (auto &it : list())
			listFile.push_back(File(absolute_path + "\\" + it));
	return listFile;
}
File::~File() {}