#include "RenCommand.h"

RenCommand::RenCommand()
{

}

RenCommand::~RenCommand()
{

}


//-features
//重命名
//-format
//ren src dst
//-constrains
//1、判断src是否存在
//2、判断新名字是否合法(不能是非法字符、.、..)
//3、dst只支持token，不支持路径
void RenCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	//检查命令语法格式
	if (arg.options.size() != 0 || arg.paths.size() != 2)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : 命令语法不正确
		return;
	}
	assert(!arg.paths[0].empty() && !arg.paths[1].empty());//路径不能为空
    string_local src_path = arg.paths[0];
	string_local dst_token_name = arg.paths[1];
	//检查新名字是否合法
	bool is_valid_token = PathTools::IsTokensFormatLegal({ dst_token_name });
	if (!is_valid_token || dst_token_name == Constant::gs_cur_dir_token || dst_token_name == Constant::gs_parent_dir_token)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : 命令语法不正确
		return;
	}
	//获得源路径tokens
	auto map_item = arg.tokens_map.find(src_path);
	if (map_item == arg.tokens_map.end())
	{
		Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : 文件、目录或卷名语法不正确
		return;
	}
	std::vector<string_local> tokens = map_item->second;
	//检查源路径是否存在
	bool is_find_path = node_tree_manager.IsPathExist(tokens);
	if (!is_find_path)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryFileIsNotFound);//error : 系统找不到指定的虚拟磁盘文件
		return;
	}
	//修改名称
	bool rename_success = node_tree_manager.RenameNodeByTokens(tokens, dst_token_name);
	if (!rename_success)
	{
		//Log::LogError();//rename src_path dst_token_name失败
	}
}
