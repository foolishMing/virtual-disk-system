#include "DirCommand.h"

DirCommand::DirCommand()
{

}

DirCommand::~DirCommand()
{

}


//-features
//列出目录中的文件和子目录列表
//-format
//dir [/s] [/ad] [path1] [path2] ...
//-constraints
//1、/ad：只输出子目录
//2、/s:递归遍历打印子目录和文件
//3、如果paths列表不存在则默认遍历工作路径
//4、如果某path不存在，则打印工作路径
//5、暂不支持在path中使用通配符
//6、注意大小写敏感问题
void DirCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	//解析选项列表
	OptionSwitch option_switch;
	for (auto item : arg.options)
	{
		const string_local lc_option = StringTools::ToLowercase(item);
		if (lc_option == Constant::gs_option_ad)// /ad
		{
			option_switch._ad = true;
		}
		else if (lc_option == Constant::gs_option_s)// /s
		{
			option_switch._s = true;
		}
		else
		{
			Console::Write::PrintLine(ErrorTips::gsOptionsIsInvalid + L" " + item);//Error : 无效的开关
			return;
		}
	}
	//dir工作目录
	if (arg.paths.empty())
	{
		bool ok = node_tree_manager.DisplayDirNodeByTokensAndOptions({Constant::gs_cur_dir_token}, option_switch);
		if (!ok)
		{
			Log::LogError(L"未知错误：dir工作目录 " + node_tree_manager.GetCurrentPath() + L" 失败");
		}
		return;
	}
	//遍历路径列表
	const size_t path_cnt = arg.paths.size();
	for (auto path : arg.paths)
	{
		//获得源路径tokens
		auto map_item = arg.tokens_map.find(path);
		if (map_item == arg.tokens_map.end())
		{
			//路径语法不正确，打印工作目录
			Console::Write::PrintLine(node_tree_manager.GetCurrentPath());
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : 文件、目录或卷名语法不正确
			continue;
		}
		std::vector<string_local> tokens = map_item->second;
		//检查源路径是否存在
		bool is_find_path = node_tree_manager.IsPathExist(tokens);
		if (!is_find_path)
		{
			//路径不存在，打印工作目录
			Console::Write::PrintLine(node_tree_manager.GetCurrentPath());
			Console::Write::PrintLine(ErrorTips::gsMemoryFileIsNotFound); //error : 找不到文件
			continue;
		}
		//打印当前路径下的子目录与文件信息
		bool ok = node_tree_manager.DisplayDirNodeByTokensAndOptions(tokens, option_switch);
		if (!ok)
		{
			Log::LogError(L"未知错误：dir目录 " + path + L" 失败");
		}
	}
}
