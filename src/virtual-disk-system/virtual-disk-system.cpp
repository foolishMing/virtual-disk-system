// virtual-disk-system.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <atlstr.h>
#include <string>
#include <cstring>

const int Max_Command_length = 255;//bug:输入超出缓冲区大小后crash

int main()
{
	wint_t input_c = 0;
	char cmdStr[Max_Command_length];
	while(std::cin.getline(cmdStr, Max_Command_length)) {
		std::cout << cmdStr << std::endl;
	}
	return 0;
}


