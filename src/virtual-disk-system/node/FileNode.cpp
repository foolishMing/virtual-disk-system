#include "FileNode.h"

FileNode::FileNode(string_local& name, BaseNode* parent)
{
	m_type = NodeType::File;
	m_parent = parent;
}


FileNode::~FileNode()
{

}

string_local FileNode::getDataByCopy()
{
	return m_file_data;
}

void FileNode::setDataByCopy(const string_local str)
{
	m_file_data = str;
}

uint64_t FileNode::GetSize()
{
	//udpate:ÐèÒªÖØÐ´
	return 0;
}