#pragma once

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <string>
#include <iostream>
#include "Define.h"

class Console {
public:
	static void Init()
	{
		std::wcin.imbue(std::locale(std::locale(), "", LC_CTYPE));  // 中文输入
		std::wcout.imbue(std::locale(std::locale(), "", LC_CTYPE)); // 中文输出
	}

	class Read
	{
	public:
		static void ReadLine(string_local& input)
		{
			std::getline(std::wcin, input);
			//Log::LogInfo(fmt::format(L"getline : {}", input).c_str());
		}
	};

	class Write
	{
	public:
		static void Print(const string_local& output)
		{
			std::wcout << output;
		}

		static void PrintLine(const string_local& output)
		{
			std::wcout << output << L"\n";
		}
	};
};

#endif // !__CONSOLE_H__





