#include "Define.h"

namespace Tips 
{
	const string_local gsUndefinedCommand = L"不是内部或外部命令,也不是可运行的程序";
	const string_local gsIllegalCommand = L"命令语法不正确";
	const string_local gsIllegalDiskPath = L"系统找不到磁盘路径";
	const string_local gsIllegalMemoryPath = L"系统找不到虚拟磁盘路径";
	const string_local gsIllegalName = L"文件名、目录名或卷标语法不正确";
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