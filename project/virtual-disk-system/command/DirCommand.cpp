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
	for (auto opt : arg.options)
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
			Console::Write::PrintLine(ErrorTips::gsOptionsIsInvalid + L" " + opt);//Error : ��Ч�Ŀ���
			return;
		}
	}
	//dir����Ŀ¼
	if (arg.paths.empty())
	{
		bool ok = node_tree_manager.DisplayDirNodeByTokensAndOptions({Constant::gs_cur_dir_token}, option_switch);
		if (!ok)
		{
			Log::LogError(L"δ֪����dir����Ŀ¼ " + node_tree_manager.GetCurrentPath() + L" ʧ��");
		}
		return;
	}
	//����·���б�
	const size_t path_cnt = arg.paths.size();
	for (const Path& path : arg.paths)
	{
		const auto path_str = path.ToString();
		//���Դ·��tokens
		if (!path.IsValid())
		{
			//·���﷨����ȷ����ӡ����Ŀ¼
			Console::Write::PrintLine(node_tree_manager.GetCurrentPath());
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : �ļ���Ŀ¼������﷨����ȷ
			continue;
		}
		const auto tokens = path.Tokens();
		//���Դ·���Ƿ����
		bool is_find_path = node_tree_manager.IsPathExist(tokens);
		if (!is_find_path)
		{
			//·�������ڣ���ӡ����Ŀ¼
			Console::Write::PrintLine(node_tree_manager.GetCurrentPath() + L" ��Ŀ¼");
			Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound); //error : ϵͳ�Ҳ���ָ�����������Ŀ¼
			continue;
		}
		//��ӡ��ǰ·���µ���Ŀ¼���ļ���Ϣ
		bool ok = node_tree_manager.DisplayDirNodeByTokensAndOptions(tokens, option_switch);
		if (!ok)
		{
			Log::LogError(L"δ֪����dirĿ¼ " + path_str + L" ʧ��");
		}
	}
}
