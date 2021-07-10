#pragma once
#ifndef __COMMANDARGS_H__
#define __COMMANDARGS_H__

#include "../util/Common.h"



class CommandArg
{
public :
	explicit CommandArg();
	virtual ~CommandArg();
	//一个合法的指令结构应为{cmd_type + options + paths}
	//其中cmd_type一定在数组的首部
	//而options和paths可以以任意顺序排列
	//option的第一个字符一定是/
	//path中可以有双引号、空格、通配符（*和?）
	void Analyse(const string_local& in);
	//测试用，格式化打印参数列表
	void Print();

public:
	string_local cmd_token = L"";
	std::vector<string_local> options = {};	//选项列表
	std::vector<Path> paths = {};	//路径列表
};

#endif // !__COMMANDARGS_H__
