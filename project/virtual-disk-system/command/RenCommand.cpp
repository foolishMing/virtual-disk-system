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
//4��������option����
void RenCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	//��������﷨��ʽ
	if (!arg.options.empty() || arg.paths.size() != 2)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : �����﷨����ȷ
		return;
	}
	const auto& src_path = arg.paths[0];
	const auto& dst_path = arg.paths[1];
	const auto& src_path_str = src_path.ToString();
	const auto& dst_path_str = dst_path.ToString();
	assert(!src_path_str.empty() && !dst_path_str.empty());//·������Ϊ��
	//����������Ƿ�Ϸ�
	bool is_valid_token = PathTools::IsTokensFormatLegal({ dst_path_str });
	if (!is_valid_token || dst_path_str == Constant::gs_cur_dir_token || dst_path_str == Constant::gs_parent_dir_token)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : �����﷨����ȷ
		return;
	}
	//���Դ·���Ƿ�Ϸ�
	if (!src_path.IsValid())
	{
		Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : �ļ���Ŀ¼������﷨����ȷ
		return;
	}
	//���Դ·��tokens
	auto tokens = src_path.Tokens();
	//�޸�����
	ReturnType ret = node_tree_manager.RenameNodeByTokens(tokens, dst_path_str);
	switch (ret)
	{
	case ReturnType::AccessDenied:
		Console::Write::PrintLine(ErrorTips::gsMemoryPathAccessDenied);//error : �Ƿ�����
		break;
	case ReturnType::MemoryPathIsNotFound:
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);//error : �Ҳ���·��
		break;
	case ReturnType::MemoryFileIsExist:
		Console::Write::PrintLine(ErrorTips::gsMemoryFileIsExist);//error : �Ѵ���ͬ���ļ�
		break;
	default:
		break;
	}
}
