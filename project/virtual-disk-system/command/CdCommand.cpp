#include "CdCommand.h"
#include "../util/Path.h"

CdCommand::CdCommand()
{

}

CdCommand::~CdCommand()
{

}

//-features
//�л�����Ŀ¼
//-format
//cd path 
//-constraints:
//1��������ֻ����0��1���Ϸ���path����������ж�����������ӡ������Ϣ
//2������Ŀ��ڵ㣬�����ڣ���ΪĿ¼�ڵ㣬���л�����Ŀ¼
void CdCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	//�����﷨����ȷ
	if (arg.options.size() != 0 || arg.paths.size() > 1)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);
		return;
	}
	//��ʾ����Ŀ¼
	if (arg.paths.empty())
	{
		node_tree_manager.ChangeDirByTokens({});
		return;
	}
	//��ȡtokens
	Path path = arg.paths[0];
	if (!path.IsValid())
	{
		Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : �ļ���Ŀ¼������ƴ���
		return;
	}
	auto tokens = path.Tokens();
	//�л�·��
	const ReturnType ret = node_tree_manager.ChangeDirByTokens(tokens);
	if (ret == ReturnType::MemoryPathIsNotFound)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);//error : �Ҳ���·��
		return;
	}
}
