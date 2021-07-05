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

#ifndef interface
#define __struct__ struct
#define interface __struct__
#endif 


//Ԫ�������ж��󶼱���̳�
interface Object
{
	virtual void Create() = 0;
	virtual void Destroy() = 0;
};


//���屾���ַ���
typedef std::wstring string_local;
typedef wchar_t char_local;
typedef std::wistringstream string_stream_local;

//ȫ�ֳ���
namespace Tips {
	extern const string_local gsUndefinedCommand;	//�����ڲ����ⲿ����,Ҳ���ǿ����еĳ���
	extern const string_local gsILLegalCommand;	//�����﷨����ȷ
	extern const string_local gsIllegalDiskPath; //ϵͳ�Ҳ�������·��
	extern const string_local gsIllegalMemoryPath; //ϵͳ�Ҳ����������·��
}



#endif // !__DEFINE_H__
