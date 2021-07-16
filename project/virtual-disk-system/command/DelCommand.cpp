#include "DelCommand.h"

DelCommand::DelCommand()
{

}

DelCommand::~DelCommand()
{

}


//-feature
//ɾ���ļ�
//-format
//del [/s] path [path1] ...
//-constraints
//1����path��ʾһ��Ŀ¼����ɾ����Ŀ¼�µ������ļ�
//2����path��ʾһ���ļ�����ɾ�����ļ�
//3����ǰ�汾��ʱ��֧��ͨ���
void DelCommand::Handle(const CommandArg& arg, NodeTreeManager& manager)
{
	//��������﷨
	OptionSwitch option_switch;
	for (const auto& item : arg.options)
	{
		if (StringTools::IsEqual(item, Constant::gs_option_s))
		{
			option_switch._s = true;
		}
		else
		{
			Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : �����﷨����ȷ
			return;
		}
	}
	if (arg.paths.size() == 0)
	{
		//paths.emplace_back(Path{ Constant::gs_cur_dir_token });
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : �����﷨����ȷ
		return;
	}
	for (const auto& path : arg.paths)
	{
		if (!path.IsValid())
		{
			Console::Write::PrintLine(path.ToString() + TEXT(" ") + ErrorTips::gsTokenNameIsIllegal);//error : Ŀ¼�����ļ���������﷨����ȷ
			continue;
		}
		//ɾ���ļ�
		//������Ϊtoken�п��ܴ���ͨ�����������Ҫ��path�����²�У��
		auto ret = manager.DelByTokensAndOption(path, option_switch);
		if (ret == ReturnType::MemoryPathIsNotFound)
		{
			Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);
		}
		else if (ret == ReturnType::SymlinkPathIsInvalid)
		{
			Console::Write::PrintLine(ErrorTips::gsSymlinkPathIsInvalid);
		}
	}
}










