#include "SymlinkNode.h"

SymlinkNode::SymlinkNode(string_local& name, BaseNode* parent)
{
	m_type = NodeType::SymlinkF;	//默认是文件链接
	m_name = name;
	m_parent = parent;
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
