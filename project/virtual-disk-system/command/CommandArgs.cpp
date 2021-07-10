#include "CommandArgs.h"

CommandArg::CommandArg()
{

}

CommandArg::~CommandArg()
{

}

//option是以/开头的不定长字符串
//形如/s/a的连续选项，其等价于 /s /a，即需要进行拆分
//对每一个path，将其拆分成tokens
void CommandArg::Analyse(const string_local& in)
{
	assert(!in.empty());
	std::vector<string_local> cmd_tokens = {};
	//基于空格拆分为token
	bool ok = StringTools::StringSplitBySpaceAndQuotes(in, cmd_tokens);
	//获取cmd_type
	auto cmd_str = cmd_tokens[0];
	cmd_tokens.erase(cmd_tokens.begin());
	//分离cmd_type的路径后缀`..`
	if (cmd_str.length() > 2 && StringTools::IsEqual(StringTools::GetStringSuffix(cmd_str,2), Constant::gs_parent_dir_token))
	{
		cmd_token = cmd_str.substr(0, cmd_str.length() - 2);
		Path path(Constant::gs_parent_dir_token);
		path.Split();
		paths.push_back(path);
	}
	//分离cmd_type的路径后缀`.`
	else if(cmd_str.size() > 1 && StringTools::IsEqual(StringTools::GetStringSuffix(cmd_str,1), Constant::gs_cur_dir_token))
	{
		cmd_token = cmd_str.substr(0, cmd_str.length() - 1);
		Path path(Constant::gs_cur_dir_token);
		path.Split();
		paths.push_back(path);
	}
	else {
		cmd_token = cmd_str;
	}
	//分离options和paths
	for (const auto& item : cmd_tokens)
	{
		//option
		if (item[0] == CharSet::char_slash) 
		{
			string_local buffer = L"/";
			for (int i = 1; i < item.length(); i++)
			{
				auto ch = item[i];
				if (ch == CharSet::char_slash)
				{
					options.push_back(buffer);
					buffer = L"/";
				}
				else
				{
					buffer += ch;
				}
			}
			options.push_back(buffer);
		}
		//path
		else 
		{
			Path path(item);
			path.Split();
			paths.push_back(path);	
		}
	}
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
		Console::Write::PrintLine(item.ToString());
	}
}







