#include "SymlinkNode.h"

SymlinkNode::SymlinkNode(string_local name, BaseNode* parent)
{
	m_type = NodeType::SymlinkF;	//默认是文件链接
	m_parent = parent;	
	m_name = name;
	Log::Info(TEXT("Create SymlinkNode ") + name);
}

SymlinkNode::~SymlinkNode()
{

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

void SymlinkNode::SetTarget(NodeType type, string_local path)
{
	m_type = type;
	m_target_path_str = path;
}
