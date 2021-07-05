#include "SymlinkNode.h"

SymlinkNode::SymlinkNode(string_local name, BaseNode* parent)
{
	m_type = NodeType::SymlinkF;	//Ĭ�����ļ�����
	m_parent = parent;	
	m_name = name;
}
SymlinkNode::~SymlinkNode()
{

}

void SymlinkNode::SetSymlink(NodeType type, BaseNode* dst_node)
{
	m_type = type;
	m_dst_node = dst_node;
}

uint64_t SymlinkNode::GetSize()
{
	return 0;
}