#include "DirCommand.h"

DirCommand::DirCommand()
{

}

DirCommand::~DirCommand()
{

}


//-features
//�г�Ŀ¼�е��ļ�����Ŀ¼�б�
//-format
//dir [/s] [/ad] [path1] [path2] ...
//-constraints
//1��/ad��ֻ�����Ŀ¼
//2��/s:�ݹ������ӡ��Ŀ¼���ļ�
//3�����paths�б�������Ĭ�ϱ�������·��
//4�����ĳpath�����ڣ����ӡ����·��
//5���ݲ�֧����path��ʹ��ͨ���
//6��ע���Сд��������
//-to udpate
//1����Ҫ�Դ�ӡ��Ϣ��ʽ��
//2����Ҫ�Եݹ�������ļ���ͳ������Ϣ
//3����Ҫͳ��ʱ����Ϣ
//4����Ҫͳ���ļ���С
void DirCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	OptionSwitch option_switch;
	//����ѡ���б�
	for (const auto& opt : arg.options)
	{
		if (StringTools::IsEqual(opt, Constant::gs_option_ad))// /ad
		{
			option_switch._ad = true;
		}
		else if (StringTools::IsEqual(opt,Constant::gs_option_s))// /s
		{
			option_switch._s = true;
		}
		else
		{
			Console::Write::PrintLine(ErrorTips::gsOptionsIsInvalid + TEXT(" ") + opt);//Error : ��Ч�Ŀ���
			return;
		}
	}
	std::vector<Path> paths = arg.paths;
	//����·��������Ĭ����ʾ��ǰ����Ŀ¼
	if (paths.empty())
	{
		Path path(Constant::gs_cur_dir_token);
		paths.emplace_back(path);
	}
	//����·���б�
	for (const Path& path : paths)
	{
		//���Դ·��tokens
		if (!path.IsValid())
		{
			//·���﷨����ȷ����ӡ����Ŀ¼
			Console::Write::PrintLine(node_tree_manager.GetCurrentPath());
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : �ļ���Ŀ¼������﷨����ȷ
			continue;
		}
		const auto path_str = path.ToString();
		const auto tokens = path.Tokens();
		//��ӡ��ǰ·���µ���Ŀ¼���ļ���Ϣ
		ReturnType ret = node_tree_manager.DisplayDirNodeByTokensAndOptions(tokens, option_switch);
		if (ret == ReturnType::UnExpectedException)
		{
			Log::Error(TEXT("δ֪����dirĿ¼ ") + path_str + TEXT(" ʧ��"));
		}
		//·�������ڣ���ӡ����Ŀ¼
		else if (ret == ReturnType::MemoryPathIsNotFound)
		{
			Console::Write::PrintLine(node_tree_manager.GetCurrentPath() + TEXT(" ��Ŀ¼"));
			Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound); //error : ϵͳ�Ҳ���ָ�����������Ŀ¼
		}
	}
}
