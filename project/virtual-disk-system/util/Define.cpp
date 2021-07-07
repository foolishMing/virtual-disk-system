#include "Define.h"

namespace Tips 
{
	const string_local gsCommandIsNotFound = L"不是内部或外部命令,也不是可运行的程序";
	const string_local gsCommandIsIllegal = L"命令语法不正确";

	const string_local gsTokenNameIsIllegal = L"文件名、目录名或卷标语法不正确";

	const string_local gsDiskPathIsIllegal = L"不合法的磁盘路径名称";
	const string_local gsDiskPathIsNotFound = L"系统找不到指定的磁盘路径"; 
	const string_local gsDiskPathIsExist = L"磁盘路径已存在"; 


	const string_local gsMemoryPathIsIllegal = L"不合法的虚拟磁盘路径名称";
	const string_local gsMemoryPathIsNotFound = L"系统找不到指定的虚拟磁盘路径";
	const string_local gsMemoryPathIsExist = L"虚拟磁盘路径已存在";
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
}

namespace Constant
{
	const string_local gs_cur_dir_token = L".";
	const string_local gs_parent_dir_token = L"..";
}