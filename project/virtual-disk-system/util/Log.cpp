#include "Log.h"
//usage
//SPDLOG_LOGGER_INFO(g_logger, "数组越界:{arg}",arg);

//#define LOGGER_FILE_MAX_SIZE 1048576 * 5
//#define LOGGER_FILE_MAX_FILES 3
//const std::shared_ptr<spdlog::logger> g_logger = spdlog::rotating_logger_mt("some_logger_name", "logs/rotating.txt", LOGGER_FILE_MAX_SIZE, LOGGER_FILE_MAX_FILES);
void Log::Info(const string_local& str)
{
	std::wcout << str << std::endl;
	//SPDLOG_LOGGER_INFO(g_logger, str);
}
void Log::Warn(const string_local& str)
{
	//SPDLOG_LOGGER_WARN(g_logger, str);
}
void Log::Error(const string_local& str)
{
	//SPDLOG_LOGGER_ERROR(g_logger, str);
}