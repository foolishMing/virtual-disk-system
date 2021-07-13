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
	/* ��Ҫһ��byte��©�ض��������ļ���ֻ�ܲ��ö����Ʒ�ʽ�� */
	//pFile = _wfopen(path.c_str(), L"rb");
	pFile = fopen(path.c_str(), "rb");
	if (pFile == nullptr)
	{
		return nullptr;
	}
	/* ��ȡ�ļ���С */
	fseek(pFile, 0, SEEK_END);
	file_size = ftell(pFile);
	rewind(pFile);
	/* �����ڴ�洢�����ļ� */
	auto buffer = (char*)malloc(sizeof(char) *file_size);
	if (buffer == nullptr)
	{
		return nullptr;
	}
	return buffer;
	/* ���ļ�������buffer�� */
	auto result = fread(buffer, 1, file_size, pFile);
	if (result != file_size)
	{
		return nullptr;
	}
	/* ������ʾ���ر��ļ����ͷ��ڴ� */
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
	const string_local gsCommandIsNotFound = TEXT("�����ڲ����ⲿ����,Ҳ���ǿ����еĳ���");
	const string_local gsCommandIsIllegal = TEXT("�����﷨����ȷ");

	const string_local gsOptionsIsIllegal = TEXT("������ʽ����ȷ");
	const string_local gsOptionsIsInvalid = TEXT("��Ч�Ŀ���");


	const string_local gsTokenNameIsIllegal = TEXT("�ļ�����Ŀ¼�������﷨����ȷ");

	const string_local gsDiskPathIsIllegal = TEXT("���Ϸ�����ʵ����·������");
	const string_local gsDiskPathIsNotFound = TEXT("ϵͳ�Ҳ���ָ������ʵ����·��");
	const string_local gsDiskPathIsExist = TEXT("��ʵ����·���Ѵ���");


	const string_local gsMemoryPathIsIllegal = TEXT("���Ϸ����������·������");
	const string_local gsMemoryPathIsNotFound = TEXT("ϵͳ�Ҳ���ָ�����������·��");

	const string_local gsMemoryFileIsNotFound = TEXT("ϵͳ�Ҳ���ָ������������ļ�");
	const string_local gsMemoryFileIsExist = TEXT("��������д���һ�������ļ������Ҳ����ļ�");

	const string_local gsMemoryPathIsExist = TEXT("�������·���Ѵ���");

	const string_local gsMemoryPathAccessDenied = TEXT("�������·���ܾ�����");

	const string_local gsDirNameInvalid = TEXT("�������Ŀ¼������Ч");

	const string_local gsMemoryDirIsNotEmpty = TEXT("�������Ŀ¼���ǿյ�");

	const string_local gsDiskPathIsNotDirectoyOrFile = TEXT("��ʵ����·���Ȳ���Ŀ¼Ҳ�����ļ�");
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
