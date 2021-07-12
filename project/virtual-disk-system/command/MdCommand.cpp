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
//-constraints:
//1、options列表必须为空，否则需要打印提示信息
//2、path列表不能为空，否则需要打印提示信息
//3、path可以是绝对路径或相对路径，如果是相对路径，则需要先转换成绝对路径
//4、如果path已存在，则需要打印提示信息"子目录或文件{path}已存在".arg(path)
void MdCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	//命令语法不正确
	if (0 != arg.options.size() || 0 == arg.paths.size())
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : 命令语法不正确
		return;
	}
	const size_t path_cnt = arg.paths.size();
	//遍历路径
	for (const Path& path : arg.paths)
	{
		const auto path_str = path.ToString();
		assert(!path_str.empty()); //路径不可能为空
		//获取tokens
		if (!path.IsValid())
		{
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error ： 文件、目录或卷名称错误
			continue;
		}
		const auto tokens = path.Tokens();
		//检查路径是否存在
		{
			bool exist_path = node_tree_manager.IsPathExist(tokens);
			if (exist_path)//error : 路径已存在
			{
				Console::Write::PrintLine(L"子目录或文件 " + path.ToString() + L" 已存在");
				if (path_cnt > 1) Console::Write::PrintLine(L"处理: " + path.ToString() + L" 时出错");
				continue;
			}
		}	
		assert(!tokens.empty());//不能创建空路径
		assert(tokens.back() != Constant::gs_cur_dir_token);//不能创建当前目录
		assert(tokens.back() != Constant::gs_parent_dir_token);//不能创建上级目录
		//创建目录
		{
			bool md_success = node_tree_manager.MkdirByTokens(tokens);
			if (false == md_success)
			{
				Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);//error : 创建目录失败
				if (path_cnt > 1) Console::Write::PrintLine(L"处理 : " + path_str + L" 时出错");
				continue;
			}
			else
			{
				Console::Write::PrintLine(L"目录 " + path_str + L" 创建成功");
				Log::LogInfo(L"目录 " + path_str + L" 创建成功");
			}
		}
	}
}
