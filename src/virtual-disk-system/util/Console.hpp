#pragma once

#include <string>
#include <iostream>
#include "Common.h"

static class Console {
public:
	static void Init()
	{
		std::wcin.imbue(std::locale(std::locale(), "", LC_CTYPE));  // ��������
		std::wcout.imbue(std::locale(std::locale(), "", LC_CTYPE)); // �������
	}
	static class Read 
	{
	public:
		static void ReadLine(string_local& input)
		{
			std::getline(std::wcin, input);
		}
	};
	static class Write 
	{
	public:
		static void Print(const string_local& output)
		{
			std::wcout << output << L" ";
		}

		static void PrintLine(const string_local& output)
		{
			std::wcout << output << L"\n";
		}
	};

};



