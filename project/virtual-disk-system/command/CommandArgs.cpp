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
	//StringTools::StringSplitBySpace(in, vec);
	bool ok = StringTools::StringSplitBySpaceWithQuotes(in, vec);
	//test ����ӡ�ָ�õ��Ĳ����б�
	for (auto item : vec)
	{
		Console::Write::PrintLine(item);
	}
	//�����ַ���תСд
	for (auto item : vec)
	{
		item = StringTools::ToLowercase(item);
	}
	//��ȡcmd_type
	auto cmd_str = vec[0];
	vec.erase(vec.begin());
	string_local tow_dot_suffix = L"..";
	string_local one_dot_suffix = L".";
	//����cmd_type��·����׺`..`
	if (cmd_str.length() > 2 && StringTools::IsEqual(StringTools::GetStringSuffix(cmd_str,2), tow_dot_suffix))
	{
		cmd_token = cmd_str.substr(0, cmd_str.length() - 2);
		paths.push_back(tow_dot_suffix);
	}
	//����cmd_type��·����׺`.`
	else if(cmd_str.size() > 1 && StringTools::IsEqual(StringTools::GetStringSuffix(cmd_str,1), one_dot_suffix))
	{
		cmd_token = cmd_str.substr(0, cmd_str.length() - 1);
		paths.push_back(one_dot_suffix);
	}
	else {
		cmd_token = cmd_str;
	}
	//����options��paths
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