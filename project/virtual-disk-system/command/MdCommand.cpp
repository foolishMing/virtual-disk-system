#include "MdCommand.h"
#include "../util/Common.h"
#include "../util/Console.hpp"

MdCommand::MdCommand()
{

}

MdCommand::~MdCommand()
{

}

//-features
//����Ŀ¼
//-format
//md path [path1] ...
//����Ҫ֧�ֶ���Ĳ���
//-constraints:
//1��options�б����Ϊ�գ�������Ҫ��ӡ��ʾ��Ϣ
//2��path�б���Ϊ�գ�������Ҫ��ӡ��ʾ��Ϣ
//3��path�����Ǿ���·�������·������������·��������Ҫ��ת���ɾ���·��
//4�����path�Ѵ��ڣ�����Ҫ��ӡ��ʾ��Ϣ"��Ŀ¼���ļ�{path}�Ѵ���".arg(path)
void MdCommand::Handle(const CommandArg& arg, NodeTreeManager& tree_manager)
{
	//�����﷨����ȷ
	if (0 != arg.options.size() || 0 == arg.options.size())
	{
		Console::Write::PrintLine(Tips::gsIllegalCommand);
		return;
	}
	//����·��
	for (auto path : arg.paths)
	{
		//update...
		
	}
}
