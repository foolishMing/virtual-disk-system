#include "Define.h"
#include <sys/stat.h>


void MemcpyLocal(char_local* dst, const char_local* src, size_t size)
{
	wmemcpy(dst, src, size);
}

bool StatLocal(const string_local& path, stat_local* stat)
{
	return _wstat(path.c_str(), stat);
}


namespace ErrorTips 
{
	const string_local gsCommandIsNotFound = L"�����ڲ����ⲿ����,Ҳ���ǿ����еĳ���";
	const string_local gsCommandIsIllegal = L"�����﷨����ȷ";

	const string_local gsOptionsIsIllegal = L"������ʽ����ȷ";
	const string_local gsOptionsIsInvalid = L"��Ч�Ŀ���";


	const string_local gsTokenNameIsIllegal = L"�ļ�����Ŀ¼�������﷨����ȷ";

	const string_local gsDiskPathIsIllegal = L"���Ϸ�����ʵ����·������";
	const string_local gsDiskPathIsNotFound = L"ϵͳ�Ҳ���ָ������ʵ����·��"; 
	const string_local gsDiskPathIsExist = L"��ʵ����·���Ѵ���"; 


	const string_local gsMemoryPathIsIllegal = L"���Ϸ����������·������";
	const string_local gsMemoryPathIsNotFound = L"ϵͳ�Ҳ���ָ�����������·��";

	const string_local gsMemoryFileIsNotFound = L"ϵͳ�Ҳ���ָ������������ļ�";
	const string_local gsMemoryFileIsExist = L"��������д���һ�������ļ������Ҳ����ļ�";

	const string_local gsMemoryPathIsExist = L"�������·���Ѵ���";

	const string_local gsMemoryPathAccessDenied = L"�������·���ܾ�����";

	const string_local gsDirNameInvalid = L"�������Ŀ¼������Ч"; 

	const string_local gsMemoryDirIsNotEmpty = L"�������Ŀ¼���ǿյ�";

	
	const string_local gsDiskPathIsNotDirectoyOrFile = L"��ʵ����·���Ȳ���Ŀ¼Ҳ�����ļ�";
};

namespace CharSet 
{
	const char_local char_space = L' ';
	const char_local char_doublequote = '"';
    const char_local char_slash = L'/';
    const char_local char_backslash = L'\\';
	const char_local char_colon = L':';
	const char_local char_asterisk = L'*';
	const char_local char_question = L'?';
	const char_local char_or = L'|';
	const char_local char_lessthan = L'<';
	const char_local char_morethan = L'>';
	const char_local char_at = L'@';
}

namespace Constant
{
	const string_local gs_cur_dir_token = L".";
	const string_local gs_parent_dir_token = L"..";

	const string_local gs_option_ad = L"/ad";	
	const string_local gs_option_d = L"/d";
	const string_local gs_option_s = L"/s";
	const string_local gs_option_y = L"/y";
}