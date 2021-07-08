#include "SymlinkNode.h"

SymlinkNode::SymlinkNode(string_local name, BaseNode* parent)
{
	m_type = NodeType::SymlinkF;	//默认是文件链接
	m_parent = parent;	
	m_name = name;
}
SymlinkNode::~SymlinkNode()
{

}

void SymlinkNode::SetSymlinkNode(NodeType link_type, BaseNode* dst_node)
{
	m_type = link_type;
	m_dst_node = dst_node;
}

uint64_t SymlinkNode::GetSize()
{
	return 0;
}
