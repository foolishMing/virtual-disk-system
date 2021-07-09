#include "BaseNode.h"


BaseNode::BaseNode() {}

BaseNode::BaseNode(string_local name, BaseNode* parent)
{
	m_name = name;
	m_parent = parent;
}

BaseNode::~BaseNode()
{

}

const NodeType BaseNode::GetType()
{
	return m_type;
}

const string_local BaseNode::GetName()
{
	return m_name;
}

void BaseNode::SetName(const string_local& name)
{
	m_name = name;
}

BaseNode* BaseNode::GetParent()
{
	return m_parent;
}

void BaseNode::SetParent(BaseNode* parent)
{
	m_parent = parent;
}


const time_t BaseNode::GetLatestModifiedTimeStamp() 
{
	return m_latest_modify_time_stamp;
}

void BaseNode::SetLatestModifiedTimeStamp(time_t ts) 
{
	m_latest_modify_time_stamp = ts;
}
