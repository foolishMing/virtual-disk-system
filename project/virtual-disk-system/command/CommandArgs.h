#pragma once
#ifndef __COMMANDARGS_H__
#define __COMMANDARGS_H__

#include "../util/Common.h"


class CommandArg
{
public :
	explicit CommandArg();
	virtual ~CommandArg();
	//一个合法的指令结构应为
	//command_type + args + paths
	//其中CommandType一定在数组的首部
	//而args和paths可以以任意顺序排列
	//paths可以接收任意合法字符
	void Analyse(const string_local& in);
	string_local GetLowercaseToken()
	{
		return StringTools::ToLowercase(cmd_token);
	}

public:
	string_local cmd_token = L"";
	std::vector<string_local> options = {};	//选项列表
	std::vector<string_local> paths = {};	//路径列表
};


#endif // !__COMMANDARGS_H__
