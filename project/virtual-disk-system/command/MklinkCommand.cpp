#include "MklinkCommand.h"


MklinkCommand::MklinkCommand()
{

}

MklinkCommand::~MklinkCommand()
{

}

void MklinkCommand::Handle(const CommandArg& arg, NodeTreeManager& manager)
{
	OptionSwitch option_switch;
	//��������ʽ
	for (const auto& item : arg.options)
	{
		if (StringTools::IsEqual(item, Constant::gs_option_d))
		{
			option_switch._d = true;
		}
		else
		{
			Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : �����﷨����ȷ
			return;
		}
	}
	if (arg.paths.size() != 2)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : �����﷨����ȷ
		return;
	}
	if (!arg.paths[0].IsValid() || !arg.paths[1].IsValid())
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsIllegal);//error : �ļ���Ŀ¼�������﷨����ȷ
		return;
	}
	//��������·���Ƿ����
	//Console::Write::PrintLine(TEXT("����Ŀ¼���ļ� ") + srcPath.ToString() + TEXT(" ������"));
	//����������Ƿ��Ѵ���
	//Console::Write::PrintLine(TEXT("��ݷ�ʽ ") + linkPath.ToString() + TEXT(" �Ѵ���"));
	auto linkPath = arg.paths[0];
	auto srcPath = arg.paths[1];
	//Ϊ������·������������
	ReturnType ret = manager.MklinkByPathAndOptions(linkPath, srcPath, option_switch);
	if (ret == ReturnType::MemoryPathIsNotFound)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);//error : ·��������
	}
	else if (ret == ReturnType::MemoryPathNameIsIllegal)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsIllegal);//error : �ļ������Ϸ�
	}
	else if (ret == ReturnType::TypeOfLinkAndSourceIsNotMatch)
	{
		Console::Write::PrintLine(ErrorTips::gsTypeOfLinkAndSourceIsNotMatch);//error : ���������Ӷ������Ͳ�ƥ��
	}
	else if (ret == ReturnType::MemoryFileIsExist)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryFileIsExist);//error : �����ļ��Ѵ���
	}
}
