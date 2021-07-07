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
void MdCommand::Handle(const CommandArg& arg, NodeTreeManager& tree_manager)
{
	//�����﷨����ȷ
	if (0 != arg.options.size() || 0 == arg.paths.size())
	{
		Console::Write::PrintLine(Tips::gsCommandIsIllegal);
		return;
	}
	int path_cnt = arg.paths.size();
	//����·��
	for (auto path : arg.paths)
	{
		//���tokens
		std::vector<string_local> tokens = {};
		{
			bool split_success = PathTools::SplitPathToTokens(path, tokens);
			if (!split_success)//error : Ŀ¼���﷨����ȷ
			{
				Console::Write::PrintLine(Tips::gsTokenNameIsIllegal);
				if (path_cnt > 1) Console::Write::PrintLine(L"���� : " + path + L" ʱ����");
				continue;
			}
		}		
		//���·���Ƿ����
		{
			bool exist_path = tree_manager.IsPathExist(tokens);
			if (exist_path)//error : ·���Ѵ���
			{
				Console::Write::PrintLine(L"��Ŀ¼���ļ� " + path + L" �Ѵ���");
				if (path_cnt > 1) Console::Write::PrintLine(L"����: " + path + L" ʱ����");
				continue;
			}
		}	
		assert(0 != tokens.size());//���ܴ�����·��
		assert(tokens.back() != Constant::gs_cur_dir_token);//���ܴ�����ǰ·��
		assert(tokens.back() != Constant::gs_parent_dir_token);//���ܴ����ϼ�·��
		//����Ŀ¼
		{
			bool md_success = tree_manager.MkdirByTokens(tokens);
			if (false == md_success)
			{
				Log::LogError(Tips::gsMemoryPathIsNotFound);//error : ϵͳ�Ҳ���ָ�����������·��
				Console::Write::PrintLine(Tips::gsMemoryPathIsNotFound);
			}
			else
			{
				Console::Write::PrintLine(L"Ŀ¼ " + path + L" �����ɹ�");
				Log::LogInfo(L"Ŀ¼ " + path + L" �����ɹ�");
			}
		}
	}
}
