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
//option : ������ѡ��
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


//����ѡ���
struct OptionSwitch
{
	bool _s = false;
	bool _ad = false;
	bool _y = false;
	bool _d = false;
};

//��������
enum ReturnType
{

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
}

//�ַ���
namespace CharSet
{
	//�ո�
	extern const char_local char_space;

	//˫����
	extern const char_local char_doublequote;

	//б��
	extern const char_local char_slash;

	//��б��
	extern const char_local char_backslash;

	//ð��
	extern const char_local char_colon;

	//�Ǻ�
	extern const char_local char_asterisk;

	//�ʺ�
	extern const char_local char_question;

	//��
	extern const char_local char_or;

	//С�ں�
	extern const char_local char_lessthan;

	//���ں�
	extern const char_local char_morethan;
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

#endif // !__DEFINE_H__
