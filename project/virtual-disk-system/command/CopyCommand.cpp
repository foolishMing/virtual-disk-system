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
//2���ļ�A -> Ŀ¼B ���Ϸ�����Ҫ�����Ƿ�������insert(new node)
//3��Ŀ¼A -> Ŀ¼B ���Ϸ�����Ŀ¼A�µ������ļ�������Ŀ¼B�£�insert(new_node)
//4�������ļ�A -> Ŀ¼B : �Ϸ���ͬ2
//5������Ŀ¼A -> Ŀ¼B : �Ϸ���ͬ3
//6��src�п���ʹ��ͨ�����Ѱ��·��ʱ��Ҫ����FuzzyMatch��ƥ��
void CopyCommand::Handle(const CommandArg& arg, NodeTreeManager& proxy)
{

}