#pragma once
#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#ifndef _interface_
#define __struct__ struct
#define _interface_ __struct__
#endif 

//����/��д����
//gs : global string��ȫ���ַ�������
//option : ������ѡ��
//token : ·�����﷨��ȷ��Ŀ¼���ļ���������

//Ԫ����
_interface_ Object
{
	virtual void Create() = 0;
	virtual void Destroy() = 0;
};


//���屾���ַ���
typedef std::wstring string_local;
typedef wchar_t char_local;
typedef std::wistringstream string_stream_local;
typedef struct _stat64i32 stat_local;
void MemcpyLocal(char* dst, const char* src, size_t size);
void MemcatLocal(char_local* dst, const char_local* src, size_t size);
bool StatLocal(const string_local& path, stat_local* stat);
char* ReadDiskFileData(const std::string& path, size_t& file_size);


//��ȡϵͳ�����ڴ�
uint64_t getTotalSystemMemory();

//wstringstream������
//to_wstring������
//std::wcout������

//����ѡ���
struct OptionSwitch
{
	bool _s = false;
	bool _ad = false;
	bool _y = false;
	bool _d = false;
};


//��������
enum class ReturnType
{
	Success = 1000, 
	UnExpectedException,//δԤ�ڵĴ���
	DirNameIsInvalid,//�������Ŀ¼������Ч
	AccessDenied,//�������·���ܾ�����
	MemoryDirIsNotEmpty,//�������Ŀ¼���ǿյ�
	DiskPathIsNotFound,//��ʵ����·��������
	DiskPathIsNotDirectoyOrFile,//��ʵ����·���Ȳ���Ŀ¼Ҳ�����ļ�

	MemoryPathIsNotFound,//�������·��������
	MemoryPathIsNotDirectoryOrFile,//�������·���Ȳ���Ŀ¼Ҳ�����ļ�

	MemoryPathNameIsIllegal,//���Ϸ���·������
	MemoryFileIsExist,//��������д���һ�������ļ������Ҳ����ļ�
	MemoryPathIsExist,//�������·���Ѵ���

	LoadXmlFileFailed,//�޷�����ָ����XML�ļ�
	TypeOfLinkAndSourceIsNotMatch,//���Ӻ����Ӷ������Ͳ�ƥ��

	SymlinkPathIsInvalid,//��Ч��Ŀ¼����
};

//Ŀ¼�����Ϣ
struct DirInfo
{
	int32_t file_cnt; //�ļ���
	int32_t dir_cnt;  //Ŀ¼��
	size_t  tot_size; //�ܴ�С
};



//������ʾ
namespace ErrorTips {
	extern const string_local gsUnhandledException;

	//�����ڲ����ⲿ����,Ҳ���ǿ����еĳ���
	extern const string_local gsCommandIsNotFound;	

	//�����﷨����ȷ
	extern const string_local gsCommandIsIllegal;	

	//������ʽ����ȷ
	extern const string_local gsOptionsIsIllegal;

	//��Ч�Ŀ���
	extern const string_local gsOptionsIsInvalid;

	//�ļ�����Ŀ¼�������﷨����ȷ
	extern const string_local gsTokenNameIsIllegal; 

	//���Ϸ��Ĵ���·������
	extern const string_local gsDiskPathIsIllegal; 
	
	//ϵͳ�Ҳ���ָ���Ĵ���·��
	extern const string_local gsDiskPathIsNotFound; 

	//����·���Ѵ���
	extern const string_local gsDiskPathIsExist; 

	//���Ϸ����������·������
	extern const string_local gsMemoryPathIsIllegal; 

	//ϵͳ�Ҳ���ָ�����������·��
	extern const string_local gsMemoryPathIsNotFound; 

	//ϵͳ�Ҳ���ָ������������ļ�
	extern const string_local gsMemoryFileIsNotFound;

	//��������д���һ�������ļ������Ҳ����ļ�
	extern const string_local gsMemoryFileIsExist;

	//�������·���Ѵ���
	extern const string_local gsMemoryPathIsExist;

	//�������·���ܾ�����
	extern const string_local gsMemoryPathAccessDenied;

	//�������Ŀ¼������Ч
	extern const string_local gsDirNameInvalid;

	//�������Ŀ¼���ǿյ�
	extern const string_local gsMemoryDirIsNotEmpty;

	//��ʵ����·���Ȳ���Ŀ¼Ҳ�����ļ�
	extern const string_local gsDiskPathIsNotDirectoyOrFile;

	//���������Ӷ������Ͳ�ƥ��
	extern const string_local gsTypeOfLinkAndSourceIsNotMatch;

	//��Ч��Ŀ¼����
	extern const string_local gsSymlinkPathIsInvalid;
}

//�ַ���
namespace CharSet
{
	// ' '
	extern const char_local char_space;

	// "
	extern const char_local char_doublequote;

	// /
	extern const char_local char_slash;

	// '\'
	extern const char_local char_backslash;

	// :
	extern const char_local char_colon;

	// *
	extern const char_local char_asterisk;

	// ?
	extern const char_local char_question;

	// |
	extern const char_local char_or;

	// <
	extern const char_local char_lessthan;

	// >
	extern const char_local char_morethan;

	// @
	extern const char_local char_at;

	// .
	extern const char_local char_dot;

}

//ȫ�ֳ���
namespace Constant
{
	//��ǰĿ¼
	extern const string_local gs_cur_dir_token;
	
	//�ϼ�Ŀ¼
	extern const string_local gs_parent_dir_token;

	// /ad
	extern const string_local gs_option_ad;

	// /d
	extern const string_local gs_option_d;

	// /s
	extern const string_local gs_option_s;
	
	// /y
	extern const string_local gs_option_y;
}

namespace XMLIdentifier
{
	//�������� XMLElement(DirNode)
	extern const char* DirNode;
	
	//�������� XMLElement(FileNode)
	extern const char* FileNode;
	
	//�������� XMLElement(SymlinkNode)
	extern const char* SymlinkNode;

	//�������� char*(wchar_t*)
	extern const char* name;
	
	//�������� int64(uint64_t)
	extern const char* id;
	
	//�������� int(NodeType)
	extern const char* type;
	
	//�������� int64(uint64_t)
	extern const char* timestamp;

	//�������� int64(uint64_t)
	extern const char* parentId;

	//�������� XMLElement(BaseNode*)
	extern const char* children;

	//�������� int(int)
	extern const char* count;

	//�������� char*(wchar_t*)
	extern const char* data;
	
	//�������� int(uint64_t)
	extern const char* dataSize;
	
	//�������� char*(wchar_t*)
	extern const char* symlinkPath;
}

#endif // !__DEFINE_H__
