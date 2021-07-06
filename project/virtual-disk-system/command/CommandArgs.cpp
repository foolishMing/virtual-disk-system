#include "CommandArgs.h"

CommandArg::CommandArg()
{

}

CommandArg::~CommandArg()
{

}

void CommandArg::Analyse(const string_local& in)
{
	assert(0 != in.size());
	std::vector<string_local> vec = {};
	bool ok = StringTools::StringSplitBySpaceWithQuotes(in, vec);
	//所有字符串转小写
	for (auto item : vec)
	{
		item = StringTools::ToLowercase(item);
	}
	//获取cmd_type
	auto cmd_str = vec[0];
	vec.erase(vec.begin());
	string_local tow_dot_suffix = L"..";
	string_local one_dot_suffix = L".";
	//分离cmd_type的路径后缀`..`
	if (cmd_str.length() > 2 && StringTools::IsEqual(StringTools::GetStringSuffix(cmd_str,2), tow_dot_suffix))
	{
		cmd_token = cmd_str.substr(0, cmd_str.length() - 2);
		paths.push_back(tow_dot_suffix);
	}
	//分离cmd_type的路径后缀`.`
	else if(cmd_str.size() > 1 && StringTools::IsEqual(StringTools::GetStringSuffix(cmd_str,1), one_dot_suffix))
	{
		cmd_token = cmd_str.substr(0, cmd_str.length() - 1);
		paths.push_back(one_dot_suffix);
	}
	else {
		cmd_token = cmd_str;
	}
	//分离options和paths
	for (auto item : vec)
	{
		if (item.length() == 2 && item[0] == '/')
		{
			options.push_back(item);
		}
		else
		{
			paths.push_back(item);
		}
	}
}

string_local CommandArg::GetLowercaseToken()
{
	return StringTools::ToLowercase(cmd_token);
}

void CommandArg::Print()
{
	//token
	Console::Write::PrintLine(L"-cmd : " + cmd_token);
	//options
	Console::Write::PrintLine(L"-args : ");
	for (auto item : options)
	{
		Console::Write::PrintLine(item);
	}
	//paths
	Console::Write::PrintLine(L"-paths : ");
	for (auto item : paths)
	{
		Console::Write::PrintLine(item);
	}
}







