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
//6、src中可以使用通配符，但路径的最后一个token中的通配符才是合法的
//7、真实磁盘路径格式，必须是@ + 绝对路径
//8、当前版本仅支持源目录为真实磁盘目录
void CopyCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
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
			Console::Write::PrintLine(ErrorTips::gsOptionsIsInvalid + TEXT(" ") + opt); //error : 无效的开关
			return;
		}
	}
	//检查path列表，必须有且仅有两个路径
	if (arg.paths.size() != 2)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal); //error : 命令语法不正确
		return;
	}
	Path src_path = arg.paths[0], dst_path = arg.paths[1];
	const auto src_path_str = src_path.ToString();
	const auto dst_path_str = dst_path.ToString();
	assert(!src_path_str.empty());
	assert(!dst_path_str.empty());
	//判断源路径和目标路径是不是真实磁盘路径
	bool is_src_disk = (src_path_str[0] == CharSet::char_at) ? true : false;
	bool is_dst_disk = (dst_path_str[0] == CharSet::char_at) ? true : false;
	if (!is_dst_disk)
	{
		//检查目标路径是否存在
		if (!dst_path.IsValid())
		{
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : 文件、目录或卷名语法不正确
			return;
		}
		const std::vector<string_local> dst_tokens = dst_path.Tokens();
		const string_local dst_path_str = dst_path.ToString();
		if (dst_tokens.empty())
		{
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : 文件、目录或卷名语法不正确
			return;
		}
		bool is_find_dst_path = node_tree_manager.IsPathExist(dst_tokens);
		if (!is_find_dst_path)
		{
			Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound + L" " + dst_path_str); //error : 系统找不到指定的虚拟磁盘目录
			return;
		}
		//从磁盘复制到内存
		if (is_src_disk)
		{
			this->CopyFileFromDiskToMemory(src_path, dst_path, node_tree_manager, option_switch);
			return;
		}
		//从内存复制到内存
		if (!is_src_disk)
		{
			this->CopyFileFromMemoryToMemory(src_path, dst_path, node_tree_manager, option_switch);
			return;
		}
	}
	else
	{
		//从磁盘复制到磁盘
		if (is_src_disk)
		{
			//update...
		}
		//从内存复制到磁盘
		if (!is_src_disk)
		{
			//update...
		}
	}	
}

void CopyCommand::CopyFileFromDiskToMemory(const Path& src_path, const Path& dst_path, NodeTreeManager& manager, OptionSwitch option_switch)
{
	string_local src_path_str = src_path.ToString();
	if (src_path_str.length() <= 1)
	{
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsIllegal);//error : 不合法的磁盘路径名称
		return;
	}
	//去掉源路径上的@
	src_path_str = src_path_str.substr(1, src_path_str.length() - 1);
	//获取源文件路径列表
	std::vector<string_local> file_path_list = {};
	PathTools::SearchDiskFilesByWildcard(src_path_str.c_str(), file_path_list);
	//检查源文件路径列表是否为空
	if (file_path_list.empty())
	{
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsNotFound + TEXT(" ") + src_path_str);//error : 系统找不到指定的磁盘路径名称
		return;
	}
	//进行复制操作
	auto dst_tokens = dst_path.Tokens();
	ReturnType ret =  manager.CopyFromDiskToMemory(file_path_list, dst_tokens,option_switch);
	//处理返回值
	//update ...
}


void CopyCommand::CopyFileFromMemoryToMemory(const Path& src_path, const Path& dst_path, NodeTreeManager& manager, OptionSwitch option_switch)
{
	const auto src_tokens = src_path.Tokens();
	const auto dst_tokens = dst_path.Tokens();
	ReturnType ret = manager.CopyFromMemoryToMemory(src_tokens, dst_tokens, option_switch);
	if (ret == ReturnType::MemoryPathIsNotFound)
	{
		Console::Write::PrintLine(TEXT("源路径 ") + src_path.ToString() + TEXT(" 不存在"));
	}
}

