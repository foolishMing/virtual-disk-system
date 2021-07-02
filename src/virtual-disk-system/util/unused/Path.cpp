#include "Path.h"
using namespace nsUtil;

///����·����
void BasePath::serialize(string_local& target)
{
	for (auto item : m_path_name_vec) {
		target.append(L"/" + item.getName());
	}
}

void BasePath::split(const string_local& target)
{
	
}

string_local BasePath::toString()
{
	string_local ret = L"";
	serialize(ret);
	return ret;
}


/// ����·����
DiskPath::DiskPath(const string_local& str)
{
	assert(str.empty() || m_disk_identifier_const == str[0]);
	m_path_str = str.substr(1, str.length() - 1);
}

DiskPath::~DiskPath()
{
}

string_local DiskPath::toString()
{
	string_local ret = L"";
	ret.append(1,m_disk_identifier_const);
	serialize(ret);
	return ret;
}


/// �ڴ�·����
MemoryPath::MemoryPath(const string_local& str)
{
	
}

MemoryPath::~MemoryPath()
{

}

string_local MemoryPath::toString()
{
	string_local ret = L"";
	serialize(ret);
	return ret;
}






