#include "RdCommand.h"

RdCommand::RdCommand()
{

}

RdCommand::~RdCommand()
{

}


//-features
//ɾ��Ŀ¼
//-format
//rd [/s] path [path1] [path2] ...
//-constraints
//1������Ŀ¼�еĽ�㲻�ܱ�ɾ��
//2����֧��/y����
//3��path�б���Ϊ��
void RdCommand::Handle(const CommandArg& arg, NodeTreeManager& proxy)
{
	//����ѡ���б�
	OptionSwitch option_switch;
	for (auto item : arg.options)
	{
		const string_local lc_option = StringTools::ToLowercase(item);
		if (lc_option == Constant::gs_option_y)// /y
		{
			option_switch._y = true;
		}
		else
		{
			Console::Write::PrintLine(ErrorTips::gsOptionsIsInvalid + L" " + item);//Error : ��Ч�Ŀ���
			return;
		}
	}
	//·���б��п�
	if (arg.paths.empty())
	{

	}
}