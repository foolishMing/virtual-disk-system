#pragma once 
#ifndef __LOG_H__
#define __LOG_H__
#include "Define.h"
//ȫ����־����
class Log
{
public:
	/*extern const std::shared_ptr<spdlog::logger> g_logger;*/
	static void LogInfo(const string_local& str);
	static void LogWarn(const string_local& str);
	static void LogError(const string_local& str);
};

#endif // !__LOG_H__
