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
	assert(0 != in.size());
	std::vector<string_local> vec = {};
	bool ok = StringTools::StringSplitBySpaceWithQuotes(in, vec);
	//获取cmd_type
	auto cmd_str = vec[0];
	vec.erase(vec.begin());
	//分离cmd_type的路径后缀`..`
	if (cmd_str.length() > 2 && StringTools::IsEqual(StringTools::GetStringSuffix(cmd_str,2), Constant::gs_parent_dir_token))
	{
		cmd_token = cmd_str.substr(0, cmd_str.length() - 2);
		paths.push_back(Constant::gs_parent_dir_token);
	}
	//分离cmd_type的路径后缀`.`
	else if(cmd_str.size() > 1 && StringTools::IsEqual(StringTools::GetStringSuffix(cmd_str,1), Constant::gs_cur_dir_token))
	{
		cmd_token = cmd_str.substr(0, cmd_str.length() - 1);
		paths.push_back(Constant::gs_cur_dir_token);
	}
	else {
		cmd_token = cmd_str;
	}
	//分离options和paths
	for (auto item : vec)
	{
		if (item[0] == CharSet::char_slash) //option
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
		else //path
		{
			paths.push_back(item);
			//将path拆分成tokens
			std::vector<string_local> tokens = {};
			bool split_success = PathTools::SplitPathToTokens(item, tokens);
			if (split_success)
			{
				tokens_map.insert(std::pair<string_local, std::vector<string_local>>(item, tokens));
			}			
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
		Console::Write::PrintLine(item);
	}
}







