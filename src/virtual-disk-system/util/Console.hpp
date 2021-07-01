#pragma once

#include <string>
#include <iostream>


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
		static void readln(string_local& input)
		{
			std::getline(std::wcin, input);
		}
	};
	static class Write 
	{
	public:
		static void print(const string_local& str)
		{
			std::wcout << str << L" ";
		}

		static void println(const string_local& str)
		{
			std::wcout << str << L"\n";
		}
	};

};



