//<��־��>
//-��������
//����ȫ����־���󣬶����������Ϣ����ͳһ����
//-��Ҫ����־
//1���û����������
//2��ÿ�������ִ�����̣�ִ�е���һ���������ʲô��
//3�����Ͻڵ����Ĵ���������
#pragma once 
#ifndef __LOG_H__
#define __LOG_H__
#include "Define.h"

class Log
{
public:
	/*extern const std::shared_ptr<spdlog::logger> g_logger;*/
	static void Info(const string_local& str);
	static void Warn(const string_local& str);
	static void Error(const string_local& str);
};

#endif // !__LOG_H__
