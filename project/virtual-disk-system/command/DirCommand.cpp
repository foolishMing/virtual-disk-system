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
void DirCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	//����ѡ���б�
	OptionSwitch option_switch;
	for (auto item : arg.options)
	{
		const string_local lc_option = StringTools::ToLowercase(item);
		if (lc_option == Constant::gs_option_ad)// /ad
		{
			option_switch._ad = true;
		}
		else if (lc_option == Constant::gs_option_s)// /s
		{
			option_switch._s = true;
		}
		else
		{
			Console::Write::PrintLine(ErrorTips::gsOptionsIsInvalid + L" " + item);//Error : ��Ч�Ŀ���
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
	for (auto path : arg.paths)
	{
		//���Դ·��tokens
		auto map_item = arg.tokens_map.find(path);
		if (map_item == arg.tokens_map.end())
		{
			//·���﷨����ȷ����ӡ����Ŀ¼
			Console::Write::PrintLine(node_tree_manager.GetCurrentPath());
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error : �ļ���Ŀ¼������﷨����ȷ
			continue;
		}
		std::vector<string_local> tokens = map_item->second;
		//���Դ·���Ƿ����
		bool is_find_path = node_tree_manager.IsPathExist(tokens);
		if (!is_find_path)
		{
			//·�������ڣ���ӡ����Ŀ¼
			Console::Write::PrintLine(node_tree_manager.GetCurrentPath());
			Console::Write::PrintLine(ErrorTips::gsMemoryFileIsNotFound); //error : �Ҳ����ļ�
			continue;
		}
		//��ӡ��ǰ·���µ���Ŀ¼���ļ���Ϣ
		bool ok = node_tree_manager.DisplayDirNodeByTokensAndOptions(tokens, option_switch);
		if (!ok)
		{
			Log::LogError(L"δ֪����dirĿ¼ " + path + L" ʧ��");
		}
	}
}
