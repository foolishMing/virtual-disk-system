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
//-constraints:
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
	//dir当前工作目录
	if (0 == arg.paths.size())
	{
		bool ok = node_tree_manager.DisplayDirNodeByTokensAndOptions({}, option_switch);
		if (!ok)
		{
			Log::LogError(L"未知错误：dir当前工作目录失败");
		}
		return;
	}
	//dir列表中的目录
	int path_cnt = arg.paths.size();
	for (auto path : arg.paths)
	{
		std::vector<string_local> tokens = {};
		bool split_success = PathTools::SplitPathToTokens(path, tokens);
		if (!split_success)//error : 目录名语法不正确
		{
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);
			if (path_cnt > 1) Console::Write::PrintLine(L"处理 : " + path + L" 时出错");
			continue;
		}
	}
}
