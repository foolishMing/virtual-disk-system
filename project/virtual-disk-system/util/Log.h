//<日志类>
//-生命周期
//创建全局日志对象，对所有埋点信息进行统一管理
//-重要的日志
//1、用户输入的命令
//2、每个命令的执行流程（执行到哪一步，结果是什么）
//3、树上节点对象的创建与销毁
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
