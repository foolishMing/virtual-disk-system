#include "Path.h"
using namespace nsUtil;

///基本路径类
void BasePath::serialize(std::wstring& target)
{
	for (auto item : m_path_name_vec) {
		target.append(L"/" + item.getName());
	}
}

void BasePath::split(const std::wstring& target)
{

}

std::wstring BasePath::toString()
{
	std::wstring ret = L"";
	serialize(ret);
	return ret;
}


/// 磁盘路径类
DiskPath::DiskPath(const std::wstring& str)
{
	assert(str.empty() || m_disk_identifier_const == str[0]);
	m_path_str = str.substr(1, str.length() - 1);
}

DiskPath::~DiskPath()
{
}

std::wstring DiskPath::toString()
{
	std::wstring ret = L"";
	ret.append(1,m_disk_identifier_const);
	serialize(ret);
	return ret;
}


/// 内存路径类
MemoryPath::MemoryPath(const std::wstring& str)
{
	
}

MemoryPath::~MemoryPath()
{

}

std::wstring MemoryPath::toString()
{
	std::wstring ret = L"";
	serialize(ret);
	return ret;
}






