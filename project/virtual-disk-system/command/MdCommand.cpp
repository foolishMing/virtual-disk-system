#include "MdCommand.h"
#include "../util/Common.h"
#include "../util/Console.hpp"

MdCommand::MdCommand()
{

}

MdCommand::~MdCommand()
{

}

//-features
//创建目录
//-format
//md path [path1] ...
//不需要支持额外的参数
//-constraints:
//1、options列表必须为空，否则需要打印提示信息
//2、path列表不能为空，否则需要打印提示信息
//3、path可以是绝对路径或相对路径，如果是相对路径，则需要先转换成绝对路径
//4、如果path已存在，则需要打印提示信息"子目录或文件{path}已存在".arg(path)
void MdCommand::Handle(const CommandArg& arg, NodeTreeManager& tree_manager)
{
	//命令语法不正确
	if (0 != arg.options.size() || 0 == arg.options.size())
	{
		Console::Write::PrintLine(Tips::gsIllegalCommand);
		return;
	}
	//遍历路径
	for (auto path : arg.paths)
	{
		//update...
		
	}
}
