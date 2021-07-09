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
	auto src_path = arg.paths[0], dst_path = arg.paths[1];
	bool copy_from_disk = false; 
	//判断源路径是不是真实磁盘路径
	assert(!src_path.empty());
	if (src_path[0] == CharSet::char_at)
	{
		src_path = src_path.substr(1, src_path.length() - 1);
		copy_from_disk = true;
	}
	//获得目标路径的tokens
	auto dst_path_item = arg.tokens_map.find(dst_path);
	if (dst_path_item == arg.tokens_map.end())
	{
		Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : 文件、目录或卷名称语法错误
		return;
	}	
	std::vector<string_local> dst_path_tokens = dst_path_item->second;
	//判断目标路径是否存在
	if (!node_tree_manager.IsPathExist(dst_path_tokens))
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound + L" " + dst_path); //error : 系统找不到指定的虚拟磁盘目录
		return;
	}
	if (copy_from_disk)
	{
		CopyFromDiskToMemory(src_path, dst_path_tokens, node_tree_manager, option_switch);
	}
	else
	{
		CopyFromMemoryToMemory(arg, dst_path_tokens, node_tree_manager, option_switch);
	}
}


void CopyCommand::CopyFromDiskToMemory(const string_local& src_path, const std::vector<string_local>& dst_path_tokens, NodeTreeManager& node_tree_manager, OptionSwitch option_switch)
{
	//从真实磁盘路径复制文件
	ReturnType ret = node_tree_manager.CopyFromDisk(src_path, dst_path_tokens, option_switch);
	switch (ret)
	{
	case ReturnType::DiskPathIsNotFound:
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsNotFound + L" " + src_path);//系统找不到指定的真实磁盘路径
		break;
	case ReturnType::DiskPathIsNotDirectoyOrFile:
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsNotDirectoyOrFile + L" " + src_path);//该路径既不是目录也不是文件
		break;
	default:
		break;
	}
}


void CopyCommand::CopyFromMemoryToMemory(const CommandArg& arg, const std::vector<string_local>& dst_path_tokens, NodeTreeManager& node_tree_manager, const OptionSwitch& option_switch)
{
	//获得源路径的token
	auto src_path = arg.paths[0];
	auto src_path_item = arg.tokens_map.find(src_path);
	if (src_path_item == arg.tokens_map.end())
	{
		Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : 文件、目录或卷名称语法错误
		return;
	}
	std::vector<string_local> src_path_tokens = src_path_item->second;
	//判断源目录是否存在
	if (!node_tree_manager.IsPathExist(src_path_tokens))
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound + L" " + src_path);//error : 找不到源路径
		return;
	}
	//从虚拟磁盘路径复制文件
	ReturnType ret = node_tree_manager.CopyFromMemory(src_path_tokens, dst_path_tokens, option_switch);
}

void CopyCommand::CopyFromMemoryToDisk()
{

}

void CopyCommand::CopyFromDiskToDisk()
{

}

