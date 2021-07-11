#include "FileNode.h"

FileNode::FileNode(string_local name, BaseNode* parent)
{
	this->m_type = NodeType::File;
	this->m_parent = parent;
	this->m_name = name;
}


FileNode::~FileNode()
{
	//ɾ���ļ�����
	if (this->m_data)
	{
		::operator delete(this->m_data);
	}
}


char_local* FileNode::GetData()
{
	return m_data;
}


bool FileNode::SetData(const char_local* data, const size_t size)
{
	if (this->m_data)
	{
		::operator delete(this->m_data);
	}
	SetSize(size);
	m_data = new char_local[size];
	MemcpyLocal(m_data, data, size);
	return true;
}


bool FileNode::AppendData(const char_local* new_data, const size_t size)
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
