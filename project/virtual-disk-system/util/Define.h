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

//����/��д����
//gs : global string��ȫ���ַ�������
//token : ·�����﷨��ȷ��Ŀ¼���ļ���������

//Ԫ����
interface Object
{
	virtual void Create() = 0;
	virtual void Destroy() = 0;
};


//���屾���ַ���
typedef std::wstring string_local;
typedef wchar_t char_local;
typedef std::wistringstream string_stream_local;


//������ʾ
namespace Tips {
	extern const string_local gsCommandIsNotFound;	//�����ڲ����ⲿ����,Ҳ���ǿ����еĳ���
	extern const string_local gsCommandIsIllegal;	//�����﷨����ȷ

	extern const string_local gsTokenNameIsIllegal; //�ļ�����Ŀ¼�������﷨����ȷ

	extern const string_local gsDiskPathIsIllegal; //���Ϸ��Ĵ���·������
	extern const string_local gsDiskPathIsNotFound; //ϵͳ�Ҳ���ָ���Ĵ���·��
	extern const string_local gsDiskPathIsExist; //����·���Ѵ���

	extern const string_local gsMemoryPathIsIllegal; //���Ϸ����������·������
	extern const string_local gsMemoryPathIsNotFound; //ϵͳ�Ҳ���ָ�����������·��
	extern const string_local gsMemoryPathIsExist; //�������·���Ѵ���
}

//�ַ���
namespace CharSet
{
	extern const char_local char_space;//�ո�
	extern const char_local char_doublequote;//˫����
	extern const char_local char_slash;//б��
	extern const char_local char_backslash;//��б��
	extern const char_local char_colon;//ð��
	extern const char_local char_asterisk;//�Ǻ�
	extern const char_local char_question;//�ʺ�
	extern const char_local char_or;//��
	extern const char_local char_lessthan;//С�ں�
	extern const char_local char_morethan;//���ں�
}

//ȫ�ֳ���
namespace Constant
{
	extern const string_local gs_cur_dir_token;//��ǰĿ¼
	extern const string_local gs_parent_dir_token;//�ϼ�Ŀ¼
}

#endif // !__DEFINE_H__
