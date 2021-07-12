#include "MoveCommand.h"


MoveCommand::MoveCommand()
{

}

MoveCommand::~MoveCommand()
{

}

//-format
//move /y src dst
//-constraints
//1�����������ļ����Ա����ǣ�Ŀ¼�����ԡ���error �� �ܾ�����
//2������·���ϵĽڵ㲻����move
//3��Դ·��������ڣ�Ŀ��·�����Բ����ڣ���������Դ·����ͬһĿ¼�£���ʱ�൱��������
void MoveCommand::Handle(const CommandArg& arg, NodeTreeManager& manager)
{
	OptionSwitch option_switch;
	//�����﷨����ȷ
	for (const auto& opt : arg.options)
	{
		if (StringTools::IsEqual(opt, Constant::gs_option_y))
		{
			option_switch._y = true;
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
	auto src_path = arg.paths[0], dst_path = arg.paths[1];
	if (!src_path.IsValid() || !dst_path.IsValid())
	{
		Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error �� �ļ���Ŀ¼������ƴ���
		return;
	}
	const auto src_path_tokens = src_path.Tokens();
	const auto dst_path_tokens = dst_path.Tokens();
	//ԴĿ¼������
	if (!manager.IsPathExist(src_path_tokens))
	{
		Console::Write::PrintLine(L"Ŀ¼���ļ� " + src_path.ToString() + L" ������");
		return;
	}
	//move from src_path to dst_path
	ReturnType ret = manager.MoveByTokensAndOptions(src_path_tokens, dst_path_tokens, option_switch);
	if (ret == ReturnType::AccessDenied)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathAccessDenied);//error ��������ܾ̾�����
		return;
	}
	if (ret == ReturnType::MemoryPathIsNotFound)
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);//error : �Ҳ����������·��
		return;
	}
}












