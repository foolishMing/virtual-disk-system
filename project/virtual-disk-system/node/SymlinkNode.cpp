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
	m_target_node = dst_node;
	m_target_name = dst_node->GetName();
}

const size_t SymlinkNode::GetSize() const
{
	return 0;
}

const time_t SymlinkNode::GetLatestModifiedTimeStamp()
{
	return m_latest_modify_time_stamp;
}

void SymlinkNode::SetLatestModifiedTimeStamp(time_t ts)
{
	m_latest_modify_time_stamp = ts;
}
