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
//4�������ļ�A -> Ŀ¼B : �Ϸ���ͬ2
//5������Ŀ¼A -> Ŀ¼B : �Ϸ���ͬ3
//6��src�п���ʹ��ͨ�����Ѱ��·��ʱ��Ҫ����FuzzyMatch��ƥ��
void CopyCommand::Handle(const CommandArg& arg, NodeTreeManager& proxy)
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
	string_local srcPath = arg.paths[0];
	string_local dstPath = arg.paths[1];
}
