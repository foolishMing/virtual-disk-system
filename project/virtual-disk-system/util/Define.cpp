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
	const string_local gsCommandIsNotFound = L"不是内部或外部命令,也不是可运行的程序";
	const string_local gsCommandIsIllegal = L"命令语法不正确";

	const string_local gsOptionsIsIllegal = L"参数格式不正确";
	const string_local gsOptionsIsInvalid = L"无效的开关";


	const string_local gsTokenNameIsIllegal = L"文件名、目录名或卷标语法不正确";

	const string_local gsDiskPathIsIllegal = L"不合法的真实磁盘路径名称";
	const string_local gsDiskPathIsNotFound = L"系统找不到指定的真实磁盘路径"; 
	const string_local gsDiskPathIsExist = L"真实磁盘路径已存在"; 


	const string_local gsMemoryPathIsIllegal = L"不合法的虚拟磁盘路径名称";
	const string_local gsMemoryPathIsNotFound = L"系统找不到指定的虚拟磁盘路径";

	const string_local gsMemoryFileIsNotFound = L"系统找不到指定的虚拟磁盘文件";
	const string_local gsMemoryFileIsExist = L"虚拟磁盘中存在一个重名文件，或找不到文件";

	const string_local gsMemoryPathIsExist = L"虚拟磁盘路径已存在";

	const string_local gsMemoryPathAccessDenied = L"虚拟磁盘路径拒绝访问";

	const string_local gsDirNameInvalid = L"虚拟磁盘目录名称无效"; 

	const string_local gsMemoryDirIsNotEmpty = L"虚拟磁盘目录不是空的";

	
	const string_local gsDiskPathIsNotDirectoyOrFile = L"真实磁盘路径既不是目录也不是文件";
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