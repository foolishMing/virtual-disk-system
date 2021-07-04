#include "FileNode.h"

FileNode::FileNode(string_local name, BaseNode* parent)
{
	m_type = NodeType::File;
	m_parent = parent;
	m_name = name;
}


FileNode::~FileNode()
{

}

string_local FileNode::GetDataByCopy()
{
	return m_file_data;
}

void FileNode::SetDataByCopy(const string_local str)
{
	m_file_data = str;
}

uint64_t FileNode::GetSize()
{
	//udpate:ÐèÒªÖØÐ´
	return 0;
}