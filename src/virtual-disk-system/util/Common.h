#pragma once
#ifndef COMMON_H
#define COMMON_H
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <cassert>

#ifndef interface
#define __struct__ struct
#define interface __struct__
#endif 

//���屾���ַ���
typedef std::wstring string_local;
typedef wchar_t char_local;


//Ԫ�������ж��󶼱���̳�
interface Object
{
	virtual void create() = 0;
	virtual void destroy() = 0;
};

#endif // !COMMON_H











