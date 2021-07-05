#pragma once 
#ifndef __LOG_H__
#define __LOG_H__
#include "Define.h"
//全局日志对象
class Log
{
public:
	/*extern const std::shared_ptr<spdlog::logger> g_logger;*/
	static void LogInfo(const string_local& str);
	static void LogWarn(const string_local& str);
	static void LogError(const string_local& str);
};

#endif // !__LOG_H__
