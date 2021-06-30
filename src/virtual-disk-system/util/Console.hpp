#pragma once

#include <string>
#include <iostream>


static class Console {
public:
	static void Init()
	{
		std::wcin.imbue(std::locale(std::locale(), "", LC_CTYPE));  // 中文输入
		std::wcout.imbue(std::locale(std::locale(), "", LC_CTYPE)); // 中文输出
	}
	static class Read 
	{
	public:
		static void readln(std::wstring& input)
		{
			std::getline(std::wcin, input);
		}
	};
	static class Write 
	{
	public:
		static void print(const std::wstring& str)
		{
			std::wcout << str << L" ";
		}

		static void println(const std::wstring& str)
		{
			std::wcout << str << L"\n";
		}
	};
	
};



