#include "MdCommand.h"
#include "../util/Common.h"
#include "../util/Console.hpp"

MdCommand::MdCommand()
{

}

MdCommand::~MdCommand()
{

}

//-features
//����Ŀ¼
//-format
//md path [path1] ...
//-constraints:
//1��options�б����Ϊ�գ�������Ҫ��ӡ��ʾ��Ϣ
//2��path�б���Ϊ�գ�������Ҫ��ӡ��ʾ��Ϣ
//3��path�����Ǿ���·�������·������������·��������Ҫ��ת���ɾ���·��
//4�����path�Ѵ��ڣ�����Ҫ��ӡ��ʾ��Ϣ"��Ŀ¼���ļ�{path}�Ѵ���".arg(path)
void MdCommand::Handle(const CommandArg& arg, NodeTreeManager& node_tree_manager)
{
	//�����﷨����ȷ
	if (0 != arg.options.size() || 0 == arg.paths.size())
	{
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);//error : �����﷨����ȷ
		return;
	}
	const size_t path_cnt = arg.paths.size();
	//����·��
	for (const Path& path : arg.paths)
	{
		const auto path_str = path.ToString();
		assert(!path_str.empty()); //·��������Ϊ��
		//��ȡtokens
		if (!path.IsValid())
		{
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//error �� �ļ���Ŀ¼������ƴ���
			continue;
		}
		const auto tokens = path.Tokens();
		//���·���Ƿ����
		{
			bool exist_path = node_tree_manager.IsPathExist(tokens);
			if (exist_path)//error : ·���Ѵ���
			{
				Console::Write::PrintLine(L"��Ŀ¼���ļ� " + path.ToString() + L" �Ѵ���");
				if (path_cnt > 1) Console::Write::PrintLine(L"����: " + path.ToString() + L" ʱ����");
				continue;
			}
		}	
		assert(!tokens.empty());//���ܴ�����·��
		assert(tokens.back() != Constant::gs_cur_dir_token);//���ܴ�����ǰĿ¼
		assert(tokens.back() != Constant::gs_parent_dir_token);//���ܴ����ϼ�Ŀ¼
		//����Ŀ¼
		{
			bool md_success = node_tree_manager.MkdirByTokens(tokens);
			if (false == md_success)
			{
				Console::Write::PrintLine(ErrorTips::gsMemoryPathIsNotFound);//error : ����Ŀ¼ʧ��
				if (path_cnt > 1) Console::Write::PrintLine(L"���� : " + path_str + L" ʱ����");
				continue;
			}
			else
			{
				Console::Write::PrintLine(L"Ŀ¼ " + path_str + L" �����ɹ�");
				Log::LogInfo(L"Ŀ¼ " + path_str + L" �����ɹ�");
			}
		}
	}
}
