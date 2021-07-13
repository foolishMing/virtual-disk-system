#include "FileNode.h"

FileNode::FileNode(string_local name, BaseNode* parent)
{
	this->m_type = NodeType::File;
	this->m_parent = parent;
	this->m_name = name;
	Log::Info(TEXT("Create FileNode ") + name);
}


FileNode::~FileNode()
{
	//删除文件数据
	if (this->m_data)
	{
		::operator delete(this->m_data);
	}
}


char* FileNode::GetData()
{
	return m_data;
}


bool FileNode::SetData(const char* data, const size_t size)
{
	if (this->m_data)
	{
		::operator delete(this->m_data);
	}
	SetSize(size);
	m_data = new char[size];
	MemcpyLocal(m_data, data, size);
	return true;
}


bool FileNode::AppendData(const char* new_data, const size_t size)
{
	//update ...
	/*auto old_size = GetSize();
	auto new_size = GetSize() + size;
	SetSize(new_size);
	auto old_data = m_data;
	m_data = new char_local[new_size];*/
	return true;
}


const size_t FileNode::GetSize()const
{
	return m_size;
}


void FileNode::SetSize(const size_t size)
{
	m_size = size;
}


const time_t FileNode::GetLatestModifiedTimeStamp()
{
	return m_latest_modify_time_stamp;
}


void FileNode::SetLatestModifiedTimeStamp(time_t ts)
{
	m_latest_modify_time_stamp = ts;
}


