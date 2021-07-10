#include "CommandArgs.h"

CommandArg::CommandArg()
{

}

CommandArg::~CommandArg()
{

}

//option����/��ͷ�Ĳ������ַ���
//����/s/a������ѡ���ȼ��� /s /a������Ҫ���в��
//��ÿһ��path�������ֳ�tokens
void CommandArg::Analyse(const string_local& in)
{
	assert(!in.empty());
	std::vector<string_local> cmd_tokens = {};
	//���ڿո���Ϊtoken
	bool ok = StringTools::StringSplitBySpaceAndQuotes(in, cmd_tokens);
	//��ȡcmd_type
	auto cmd_str = cmd_tokens[0];
	cmd_tokens.erase(cmd_tokens.begin());
	//����cmd_type��·����׺`..`
	if (cmd_str.length() > 2 && StringTools::IsEqual(StringTools::GetStringSuffix(cmd_str,2), Constant::gs_parent_dir_token))
	{
		cmd_token = cmd_str.substr(0, cmd_str.length() - 2);
		Path path(Constant::gs_parent_dir_token);
		path.Split();
		paths.push_back(path);
	}
	//����cmd_type��·����׺`.`
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
	//����options��paths
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







