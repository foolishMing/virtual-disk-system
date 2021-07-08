#include "RdCommand.h"

RdCommand::RdCommand()
{

}

RdCommand::~RdCommand()
{

}


//-features
//删除目录
//-format
//rd [/s] path [path1] [path2] ...
//-constraints
//1、工作目录中的结点不能被删除
//2、仅支持/y参数
//3、path列表不能为空
void RdCommand::Handle(const CommandArg& arg, NodeTreeManager& proxy)
{
	//解析选项列表
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
			Console::Write::PrintLine(ErrorTips::gsOptionsIsInvalid + L" " + item);//Error : 无效的开关
			return;
		}
	}
	//路径列表判空
	if (arg.paths.empty())
	{

	}
}