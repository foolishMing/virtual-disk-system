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
			Console::Write::PrintLine(ErrorTips::gsOptionsIsInvalid + TEXT(" ") + opt); //error : ��Ч�Ŀ���
			return;
		}
	}
	//���path�б��������ҽ�������·��
	if (arg.paths.size() != 2)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal); //error : �����﷨����ȷ
		return;
	}
	Path src_path = arg.paths[0], dst_path = arg.paths[1];
	const auto src_path_str = src_path.ToString();
	const auto dst_path_str = dst_path.ToString();
	assert(!src_path_str.empty());
	assert(!dst_path_str.empty());
	//�ж�Դ·����Ŀ��·���ǲ�����ʵ����·��
	bool is_src_disk = (src_path_str[0] == CharSet::char_at) ? true : false;
	bool is_dst_disk = (dst_path_str[0] == CharSet::char_at) ? true : false;
	if (!is_dst_disk)
	{
		//���Ŀ��·���Ƿ����
		if (!dst_path.IsValid())
		{
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : �ļ���Ŀ¼������﷨����ȷ
			return;
		}
		const std::vector<string_local> dst_tokens = dst_path.Tokens();
		const string_local dst_path_str = dst_path.ToString();
		if (dst_tokens.empty())
		{
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : �ļ���Ŀ¼������﷨����ȷ
			return;
		}
		bool is_find_dst_path = node_tree_manager.IsPathExist(dst_tokens);
		if (!is_find_dst_path)
		{
			Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound + L" " + dst_path_str); //error : ϵͳ�Ҳ���ָ�����������Ŀ¼
			return;
		}
		//�Ӵ��̸��Ƶ��ڴ�
		if (is_src_disk)
		{
			this->CopyFileFromDiskToMemory(src_path, dst_path, node_tree_manager, option_switch);
			return;
		}
		//���ڴ渴�Ƶ��ڴ�
		if (!is_src_disk)
		{
			this->CopyFileFromMemoryToMemory(src_path, dst_path, node_tree_manager, option_switch);
			return;
		}
	}
	else
	{
		//�Ӵ��̸��Ƶ�����
		if (is_src_disk)
		{
			//update...
		}
		//���ڴ渴�Ƶ�����
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
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsIllegal);//error : ���Ϸ��Ĵ���·������
		return;
	}
	//ȥ��Դ·���ϵ�@
	src_path_str = src_path_str.substr(1, src_path_str.length() - 1);
	//��ȡԴ�ļ�·���б�
	std::vector<string_local> file_path_list = {};
	PathTools::SearchDiskFilesByWildcard(src_path_str.c_str(), file_path_list);
	//���Դ�ļ�·���б��Ƿ�Ϊ��
	if (file_path_list.empty())
	{
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsNotFound + TEXT(" ") + src_path_str);//error : ϵͳ�Ҳ���ָ���Ĵ���·������
		return;
	}
	//���и��Ʋ���
	auto dst_tokens = dst_path.Tokens();
	ReturnType ret =  manager.CopyFromDiskToMemory(file_path_list, dst_tokens,option_switch);
	//������ֵ
	//update ...
}


void CopyCommand::CopyFileFromMemoryToMemory(const Path& src_path, const Path& dst_path, NodeTreeManager& manager, OptionSwitch option_switch)
{
	const auto src_tokens = src_path.Tokens();
	const auto dst_tokens = dst_path.Tokens();
	ReturnType ret = manager.CopyFromMemoryToMemory(src_tokens, dst_tokens, option_switch);
	if (ret == ReturnType::MemoryPathIsNotFound)
	{
		Console::Write::PrintLine(TEXT("Դ·�� ") + src_path.ToString() + TEXT(" ������"));
	}
}

