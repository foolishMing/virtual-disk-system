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
//1���ж�Դ�ļ��Ƿ����
//2���ж��������Ƿ�Ϸ�������
void RenCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	//�����﷨����ȷ
	if (arg.options.size() != 0 || arg.paths.size() != 2)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);
		return;
	}
	assert(!arg.paths[0].empty() && !arg.paths[1].empty());//·������Ϊ��
}

