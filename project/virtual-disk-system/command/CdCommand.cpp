#include "CdCommand.h"

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
	auto path = arg.paths[0];
	assert(!path.empty());
	//��ȡtokens
	auto map_item = arg.tokens_map.find(path);
	if (arg.tokens_map.end() == map_item)
	{
		Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : �ļ���Ŀ¼������ƴ���
		return;
	}
	//���·���Ƿ����
	std::vector<string_local> tokens = map_item->second;	
	bool exist_path = node_tree_manager.IsPathExist(tokens);
	if (!exist_path)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);
		return;
	}
	bool ret = node_tree_manager.ChangeDirByTokens(tokens);
	if (!ret)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);//error : ·��������
	}
}
