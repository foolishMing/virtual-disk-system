#include "LoadCommand.h"


LoadCommand::LoadCommand()
{

}

LoadCommand::~LoadCommand()
{

}

void LoadCommand::Handle(const CommandArg& arg, NodeTreeManager& manager)
{
	//��������﷨
	if (!arg.options.empty() || arg.paths.size() != 1)
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : �����﷨����ȷ
		return;
	}
	//���·���Ƿ�Ϸ�
	Path path = arg.paths[0];
	string_local load_path_str = path.ToString();
	assert(!load_path_str.empty());
	if (load_path_str[0] != CharSet::char_at)
	{
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsIllegal);//error : ����·����ʽ����ȷ
		return;
	}
	load_path_str = load_path_str.substr(1, load_path_str.length() - 1);
	if (load_path_str.empty())
	{
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsIllegal); //error : ���Ϸ��Ĵ���·������
		return;
	}
	if (!PathTools::IsDiskPathExist(load_path_str))
	{
		Console::Write::PrintLine(ErrorTips::gsDiskPathIsNotFound); //error : ����·��������
		return;
	}
	//�ж��ļ���׺���Ƿ�Ϊ.xml
	auto xml_suffix_str = TEXT(".xml");
	if (load_path_str.find(xml_suffix_str) == load_path_str.npos)
	{
		Console::Write::PrintLine(TEXT("���ǺϷ���XML��ʽ�ļ����޷���"));//error : �ļ���ʽ����ȷ
		return;
	}
	//��ȡXML������ԭNodeTree,������NodeTree
	ReturnType ret = manager.LoadFromPath(load_path_str);
	if (ReturnType::LoadXmlFileFailed == ret)
	{
		Console::Write::PrintLine(TEXT("�޷�����ָ����XML�ļ�"));
		return;
	}
}