#include "SaveCommand.h"


SaveCommand::SaveCommand()
{

}

SaveCommand::~SaveCommand()
{

}

void SaveCommand::Handle(const CommandArg& arg, NodeTreeManager& manager)
{
	//��������﷨
	if (!arg.options.empty() || arg.paths.size() != 1)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : �����﷨����ȷ
		return;
	}
	//���·���Ƿ�Ϸ�
	Path path = arg.paths[0];
	string_local save_path_str = path.ToString();
	assert(!save_path_str.empty());
	if (save_path_str[0] != CharSet::char_at)
	{
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsIllegal);//error : ����·����ʽ����ȷ
		return;
	}
	save_path_str = save_path_str.substr(1, save_path_str.length() - 1);
	if (save_path_str.empty())
	{
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsIllegal); //error : ���Ϸ��Ĵ���·������
		return;
	}
	if (PathTools::IsDiskPathExist(save_path_str))
	{
		Console::Write::PrintLine(ErrorTips::gsMemoryFileIsExist); //error : ����·���Ѵ���
		return;
	}
	//���û�к�׺���Ͳ���
	auto xml_suffix_str = TEXT(".xml");
	if (save_path_str.find(xml_suffix_str) == save_path_str.npos)
	{
		save_path_str.append(xml_suffix_str);
	}
	//����ǰNodeTree���浽XML�ļ���
	ReturnType ret = manager.SaveToPath(save_path_str);
}