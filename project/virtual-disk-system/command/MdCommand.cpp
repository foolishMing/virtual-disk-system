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
void MdCommand::Handle(const CommandArg& arg, NodeTreeManager& tree_manager)
{
	//命令语法不正确
	if (0 != arg.options.size() || 0 == arg.paths.size())
	{
		Console::Write::PrintLine(Tips::gsCommandIsIllegal);
		return;
	}
	int path_cnt = arg.paths.size();
	//遍历路径
	for (auto path : arg.paths)
	{
		//获得tokens
		std::vector<string_local> tokens = {};
		bool split_success = PathTools::SplitPathToTokens(path, tokens);
		//检查tokens语法是否正确
		if (!split_success)//error : 目录名语法不正确
		{
			Console::Write::PrintLine(Tips::gsTokenNameIsIllegal);
			if(path_cnt > 1) Console::Write::PrintLine(L"处理 : " + path + L" 时出错");
			continue;
		}
		//检查路径是否存在
		bool exist_path = tree_manager.IsPathExist(tokens);
		if (exist_path)//error : 路径已存在
		{
			Console::Write::PrintLine(Tips::gsMemoryPathIsExist);
			if (path_cnt > 1) Console::Write::PrintLine(L"处理 : " + path + L" 时出错");
			continue;
		}
		assert(split_success && !exist_path);
		assert(0 != tokens.size());//空路径
		assert(tokens.back() != Constant::gs_cur_dir_token);//当前路径
		assert(tokens.back() != Constant::gs_parent_dir_token);//上级路径
		bool success = tree_manager.MkdirByPathByTokens(tokens);
		if (!split_success)
		{
			Log::LogError(L"未知错误：创建目录失败");
		}
		else 
		{
			Console::Write::PrintLine(L"目录 " + path + L" 创建成功");
		}
	}
}
