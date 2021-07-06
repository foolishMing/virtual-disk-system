#pragma once
#ifndef __COMMON_H__
#define __COMMON_H__

#include "StringTool.h"
#include "Log.h"
#include "Console.hpp"
//#define SPDLOG_WCHAR_TO_UTF8_SUPPORT
//#include "../include/spdlog/spdlog.h"
//#include "../include/spdlog/sinks/rotating_file_sink.h"

namespace PathTools
{
	bool IsDiskPathExist(const string_local& str);
}


namespace DateTimeTools
{
	int StampToStandardDateTime(time_t stc, string_local& dst);
}

#endif // !COMMON_H
