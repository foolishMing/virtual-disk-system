#pragma once

#include <string>
#include <iostream>
#include "Common.h"

class Console {
public:
	static void Init()
	{
		std::wcin.imbue(std::locale(std::locale(), "", LC_CTYPE));  // ��������
		std::wcout.imbue(std::locale(std::locale(), "", LC_CTYPE)); // �������
	}

    class Read 
	{
	public:
		static void ReadLine(string_local& input)
		{
			std::getline(std::wcin, input);
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



