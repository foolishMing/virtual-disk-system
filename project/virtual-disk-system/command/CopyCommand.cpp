#include "CopyCommand.h"


CopyCommand::CopyCommand()
{

}

CopyCommand::~CopyCommand()
{

}




//-feature
//�����ļ�
//-format
//copy src dst
//-constraints
//1����֧��/y������ͬ������
//2���������ҽ�������·��srcPath��dstPath
//2���ļ�A -> Ŀ¼B ���Ϸ�����Ҫ�����Ƿ�������insert(new node)
//3��Ŀ¼A -> Ŀ¼B ���Ϸ�����Ŀ¼A�µ������ļ�������Ŀ¼B�£�insert(new_node)
//6��src�п���ʹ��ͨ�������·�������һ��token�е�ͨ������ǺϷ���
//7����ʵ����·����ʽ��������@ + ����·��
//8����ǰ�汾��֧��ԴĿ¼Ϊ��ʵ����Ŀ¼
void CopyCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	OptionSwitch option_switch;
	//���option�б�֧��/yѡ��
	for (const auto& opt : arg.options)
	{
		if (StringTools::IsEqual(opt, Constant::gs_option_y)) // /y
		{
			option_switch._y = true;
		}
		else 
		{
			Console::Write::PrintLine(ErrorTips::gsOptionsIsInvalid + L" " + opt); //error : ��Ч�Ŀ���
			return;
		}
	}
	//���path�б��������ҽ�������·��
	if (arg.paths.size() != 2)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal); //error : �����﷨����ȷ
		return;
	}
	auto src_path = arg.paths[0], dst_path = arg.paths[1];
	bool copy_from_disk = false; 
	//�ж�Դ·���ǲ�����ʵ����·��
	assert(!src_path.empty());
	if (src_path[0] == CharSet::char_at)
	{
		src_path = src_path.substr(1, src_path.length() - 1);
		copy_from_disk = true;
	}
	//���Ŀ��·����tokens
	auto dst_path_item = arg.tokens_map.find(dst_path);
	if (dst_path_item == arg.tokens_map.end())
	{
		Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : �ļ���Ŀ¼��������﷨����
		return;
	}	
	std::vector<string_local> dst_path_tokens = dst_path_item->second;
	//�ж�Ŀ��·���Ƿ����
	if (!node_tree_manager.IsPathExist(dst_path_tokens))
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound + L" " + dst_path); //error : ϵͳ�Ҳ���ָ�����������Ŀ¼
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
	//����ʵ����·�������ļ�
	ReturnType ret = node_tree_manager.CopyFromDisk(src_path, dst_path_tokens, option_switch);
	switch (ret)
	{
	case ReturnType::DiskPathIsNotFound:
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsNotFound + L" " + src_path);//ϵͳ�Ҳ���ָ������ʵ����·��
		break;
	case ReturnType::DiskPathIsNotDirectoyOrFile:
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsNotDirectoyOrFile + L" " + src_path);//��·���Ȳ���Ŀ¼Ҳ�����ļ�
		break;
	default:
		break;
	}
}


void CopyCommand::CopyFromMemoryToMemory(const CommandArg& arg, const std::vector<string_local>& dst_path_tokens, NodeTreeManager& node_tree_manager, const OptionSwitch& option_switch)
{
	//���Դ·����token
	auto src_path = arg.paths[0];
	auto src_path_item = arg.tokens_map.find(src_path);
	if (src_path_item == arg.tokens_map.end())
	{
		Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : �ļ���Ŀ¼��������﷨����
		return;
	}
	std::vector<string_local> src_path_tokens = src_path_item->second;
	//�ж�ԴĿ¼�Ƿ����
	if (!node_tree_manager.IsPathExist(src_path_tokens))
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound + L" " + src_path);//error : �Ҳ���Դ·��
		return;
	}
	//���������·�������ļ�
	ReturnType ret = node_tree_manager.CopyFromMemory(src_path_tokens, dst_path_tokens, option_switch);
}

void CopyCommand::CopyFromMemoryToDisk()
{

}

void CopyCommand::CopyFromDiskToDisk()
{

}

