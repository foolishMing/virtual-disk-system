#define _CRT_SECURE_NO_DEPRECATE
#include "Define.h"
#include <sys/stat.h>
#include <fstream>
#include "windows.h"

void MemcpyLocal(char* dst, const char* src, size_t size)
{
	memcpy(dst, src, size);
}

void MemcatLocal(char_local* dst, const char_local* src, size_t size)
{

}

bool StatLocal(const string_local& path, stat_local* stat)
{
	return _wstat(path.c_str(), stat);
}

char* ReadDiskFileData(const std::string& path, size_t& file_size)
{
	FILE* pFile;
	/* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 */
	//pFile = _wfopen(path.c_str(), L"rb");
	pFile = fopen(path.c_str(), "rb");
	if (pFile == nullptr)
	{
		return nullptr;
	}
	/* 获取文件大小 */
	fseek(pFile, 0, SEEK_END);
	file_size = ftell(pFile);
	rewind(pFile);
	/* 分配内存存储整个文件 */
	auto buffer = (char*)malloc(sizeof(char) *file_size);
	if (buffer == nullptr)
	{
		return nullptr;
	}
	return buffer;
	/* 将文件拷贝到buffer中 */
	auto result = fread(buffer, 1, file_size, pFile);
	if (result != file_size)
	{
		return nullptr;
	}
	/* 结束演示，关闭文件并释放内存 */
	fclose(pFile);
	//free(buffer);
	return buffer;
}


uint64_t getTotalSystemMemory()
{
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	return status.ullTotalPhys;
}


namespace ErrorTips 
{
	const string_local gsCommandIsNotFound = TEXT("不是内部或外部命令,也不是可运行的程序");
	const string_local gsCommandIsIllegal = TEXT("命令语法不正确");

	const string_local gsOptionsIsIllegal = TEXT("参数格式不正确");
	const string_local gsOptionsIsInvalid = TEXT("无效的开关");


	const string_local gsTokenNameIsIllegal = TEXT("文件名、目录名或卷标语法不正确");

	const string_local gsDiskPathIsIllegal = TEXT("不合法的真实磁盘路径名称");
	const string_local gsDiskPathIsNotFound = TEXT("系统找不到指定的真实磁盘路径");
	const string_local gsDiskPathIsExist = TEXT("真实磁盘路径已存在");


	const string_local gsMemoryPathIsIllegal = TEXT("不合法的虚拟磁盘路径名称");
	const string_local gsMemoryPathIsNotFound = TEXT("系统找不到指定的虚拟磁盘路径");

	const string_local gsMemoryFileIsNotFound = TEXT("系统找不到指定的虚拟磁盘文件");
	const string_local gsMemoryFileIsExist = TEXT("虚拟磁盘中存在一个重名文件，或找不到文件");

	const string_local gsMemoryPathIsExist = TEXT("虚拟磁盘路径已存在");

	const string_local gsMemoryPathAccessDenied = TEXT("虚拟磁盘路径拒绝访问");

	const string_local gsDirNameInvalid = TEXT("虚拟磁盘目录名称无效");

	const string_local gsMemoryDirIsNotEmpty = TEXT("虚拟磁盘目录不是空的");

	const string_local gsDiskPathIsNotDirectoyOrFile = TEXT("真实磁盘路径既不是目录也不是文件");
};

namespace CharSet 
{
	const char_local char_space = TEXT(' ');
	const char_local char_doublequote = TEXT('"');
    const char_local char_slash = TEXT('/');
    const char_local char_backslash = TEXT('\\');
	const char_local char_colon = TEXT(':');
	const char_local char_asterisk = TEXT('*');
	const char_local char_question = TEXT('?');
	const char_local char_or = TEXT('|');
	const char_local char_lessthan = TEXT('<');
	const char_local char_morethan = TEXT('>');
	const char_local char_at = TEXT('@');
	const char_local char_dot = TEXT('.');
}

namespace Constant
{
	const string_local gs_cur_dir_token = TEXT(".");
	const string_local gs_parent_dir_token = TEXT("..");

	const string_local gs_option_ad = TEXT("/ad");	
	const string_local gs_option_d = TEXT("/d");
	const string_local gs_option_s = TEXT("/s");
	const string_local gs_option_y = TEXT("/y");

}

namespace XMLIdentifier
{
	const char* DirNode = "DirNode";
	const char* FileNode = "FileNode";
	const char* SymlinkNode = "SymlinkNode";

	const char* name = "name";
	const char* id = "id";
	const char* type = "type";
	const char* timestamp = "timestamp";

	const char* parentId = "parentId";
	const char* children = "children";
	const char* count = "count";

	const char* data = "data";
	const char* dataSize = "dataSize";

	const char* symlinkPath = "symlinkPath";
}
