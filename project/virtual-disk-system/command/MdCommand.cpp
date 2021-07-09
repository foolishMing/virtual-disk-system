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
		Console::Write::PrintLine(ErrorTips::gsCommandIsIllegal);
		return;
	}
	const size_t path_cnt = arg.paths.size();
	//����·��
	for (auto path : arg.paths)
	{
		assert(!path.empty()); //·��������Ϊ��
		//��ȡtokens
		auto map_item = arg.tokens_map.find(path);
		if(arg.tokens_map.end() == map_item)
		{
			Console::Write::PrintLine(ErrorTips::gsTokenNameIsIllegal);//�ļ���Ŀ¼������ƴ���
			if (path_cnt > 1) Console::Write::PrintLine(L"���� : " + path + L" ʱ����");
			continue;
		}
		std::vector<string_local> tokens = map_item->second;	
		//���·���Ƿ����
		{
			bool exist_path = node_tree_manager.IsPathExist(tokens);
			if (exist_path)//error : ·���Ѵ���
			{
				Console::Write::PrintLine(L"��Ŀ¼���ļ� " + path + L" �Ѵ���");
				if (path_cnt > 1) Console::Write::PrintLine(L"����: " + path + L" ʱ����");
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
				if (path_cnt > 1) Console::Write::PrintLine(L"���� : " + path + L" ʱ����");
				continue;
			}
			else
			{
				Console::Write::PrintLine(L"Ŀ¼ " + path + L" �����ɹ�");
				Log::LogInfo(L"Ŀ¼ " + path + L" �����ɹ�");
			}
		}
	}
}
