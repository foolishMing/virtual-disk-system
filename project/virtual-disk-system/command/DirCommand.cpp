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
//-to udpate
//1、需要对打印信息格式化
//2、需要对递归遍历的文件夹统计总信息
//3、需要统计时间信息
//4、需要统计文件大小
void DirCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	OptionSwitch option_switch;
	//解析选项列表
	for (const auto& opt : arg.options)
	{
		if (StringTools::IsEqual(opt, Constant::gs_option_ad))// /ad
		{
			option_switch._ad = true;
		}
		else if (StringTools::IsEqual(opt,Constant::gs_option_s))// /s
		{
			option_switch._s = true;
		}
		else
		{
			Console::Write::PrintLine(ErrorTips::gsOptionsIsInvalid + TEXT(" ") + opt);//Error : 无效的开关
			return;
		}
	}
	std::vector<Path> paths = arg.paths;
	//若无路径参数，默认显示当前工作目录
	if (paths.empty())
	{
		Path path(Constant::gs_cur_dir_token);
		paths.emplace_back(path);
	}
	//遍历路径列表
	for (const Path& path : paths)
	{
		//获得源路径tokens
		if (!path.IsValid())
		{
			//路径语法不正确，打印工作目录
			Console::Write::PrintLine(node_tree_manager.GetCurrentPath());
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : 文件、目录或卷名语法不正确
			continue;
		}
		const auto path_str = path.ToString();
		const auto tokens = path.Tokens();
		//打印当前路径下的子目录与文件信息
		ReturnType ret = node_tree_manager.DisplayDirNodeByTokensAndOptions(tokens, option_switch);
		if (ret == ReturnType::UnExpectedException)
		{
			Log::Error(TEXT("未知错误：dir目录 ") + path_str + TEXT(" 失败"));
		}
		//路径不存在，打印工作目录
		else if (ret == ReturnType::MemoryPathIsNotFound)
		{
			Console::Write::PrintLine(node_tree_manager.GetCurrentPath() + TEXT(" 的目录"));
			Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound); //error : 系统找不到指定的虚拟磁盘目录
		}
	}
}
