#include "CopyCommand.h"


CopyCommand::CopyCommand()
{

}

CopyCommand::~CopyCommand()
{

}


//-feature
//复制文件
//-format
//copy src dst
//-constraints
//1、仅支持/y参数，同名覆盖
//2、必须有且仅有两个路径srcPath、dstPath
//2、文件A -> 目录B ：合法，需要考虑是否重名，insert(new node)
//3、目录A -> 目录B ：合法，将目录A下的所有文件拷贝到目录B下，insert(new_node)
//4、磁盘文件A -> 目录B : 合法，同2
//5、磁盘目录A -> 目录B : 合法，同3
//6、src中可以使用通配符，寻找路径时需要调用FuzzyMatch来匹配
void CopyCommand::Handle(const CommandArg& arg, NodeTreeManager& proxy)
{
	OptionSwitch option_switch;
	//检查option列表，支持/y选项
	for (const auto& opt : arg.options)
	{
		if (StringTools::IsEqual(opt, Constant::gs_option_y)) // /y
		{
			option_switch._y = true;
		}
		else 
		{
			Console::Write::PrintLine(ErrorTips::gsOptionsIsInvalid + L" " + opt); //error : 无效的开关
			return;
		}
	}
	//检查path列表，必须有且仅有两个路径
	if (arg.paths.size() != 2)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal); //error : 命令语法不正确
		return;
	}
	string_local srcPath = arg.paths[0];
	string_local dstPath = arg.paths[1];
}
