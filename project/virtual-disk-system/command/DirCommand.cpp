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
//-constraints:
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
	//dir��ǰ����Ŀ¼
	if (0 == arg.paths.size())
	{
		bool ok = node_tree_manager.DisplayDirNodeByTokensAndOptions({}, option_switch);
		if (!ok)
		{
			Log::LogError(L"δ֪����dir��ǰ����Ŀ¼ʧ��");
		}
		return;
	}
	//dir�б��е�Ŀ¼
	int path_cnt = arg.paths.size();
	for (auto path : arg.paths)
	{
		std::vector<string_local> tokens = {};
		bool split_success = PathTools::SplitPathToTokens(path, tokens);
		if (!split_success)//error : Ŀ¼���﷨����ȷ
		{
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);
			if (path_cnt > 1) Console::Write::PrintLine(L"���� : " + path + L" ʱ����");
			continue;
		}
	}
}
