#include "RenCommand.h"

RenCommand::RenCommand()
{

}

RenCommand::~RenCommand()
{

}


//-features
//������
//-format
//ren src dst
//-constrains
//1���ж�src�Ƿ����
//2���ж��������Ƿ�Ϸ�(�����ǷǷ��ַ���.��..)
//3��dstֻ֧��token����֧��·��
void RenCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	//��������﷨��ʽ
	if (arg.options.size() != 0 || arg.paths.size() != 2)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : �����﷨����ȷ
		return;
	}
	assert(!arg.paths[0].empty() && !arg.paths[1].empty());//·������Ϊ��
    string_local src_path = arg.paths[0];
	string_local dst_token_name = arg.paths[1];
	//����������Ƿ�Ϸ�
	bool is_valid_token = PathTools::IsTokensFormatLegal({ dst_token_name });
	if (!is_valid_token || dst_token_name == Constant::gs_cur_dir_token || dst_token_name == Constant::gs_parent_dir_token)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : �����﷨����ȷ
		return;
	}
	//���Դ·��tokens
	auto map_item = arg.tokens_map.find(src_path);
	if (map_item == arg.tokens_map.end())
	{
		Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : �ļ���Ŀ¼������﷨����ȷ
		return;
	}
	std::vector<string_local> tokens = map_item->second;
	//���Դ·���Ƿ����
	bool is_find_path = node_tree_manager.IsPathExist(tokens);
	if (!is_find_path)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryFileIsNotFound);//error : ϵͳ�Ҳ���ָ������������ļ�
		return;
	}
	//�޸�����
	bool rename_success = node_tree_manager.RenameNodeByTokens(tokens, dst_token_name);
	if (!rename_success)
	{
		//Log::LogError();//rename src_path dst_token_nameʧ��
	}
}
