#include "DirNode.h"


DirNode::DirNode(string_local name, BaseNode* parent)
{
	m_type = NodeType::Directory;
	m_parent = parent;
	m_name = name;
	Log::Info(TEXT("Create DirNode ") + name);
}

DirNode::~DirNode()
{
	if (!m_children.empty())
	{
		//update ...
	}
}

BaseNode* DirNode::FindChildByName(const string_local& node_name)
{
	//查找子目录
	for (auto& item : m_children)
	{
		if (item->IsNameEqualsTo(node_name))
		{
			return item;
		}
	}
	//特判.和..
	//if (StringTools::IsEqual(node_name, Constant::gs_cur_dir_token))
	//{
	//	return this;
	//}
	//if (StringTools::IsEqual(node_name, Constant::gs_parent_dir_token))
	//{
	//	return this->GetParent();
	//}
	return nullptr;
}

void DirNode::AppendChild(BaseNode* node)	
{
	assert(nullptr != node);
	if (nullptr == node)
	{
		//udpate : 打印日志（警告）
		return;
	}
	m_children.emplace_back(node);
}

const size_t DirNode::GetSize() const
{
	size_t sz = 0;
	for (const auto item : m_children)
	{
		sz += item->GetSize();
	}
	return sz;
}


bool DirNode::ContainsChild(const string_local& node_name)
{
	if (FindChildByName(node_name))
	{
		return true;
	}
	return false;
}


bool DirNode::RemoveChildByIndex(int index)
{
	assert(index >= 0 && index < m_children.size());
	m_children.erase(m_children.begin() + index);
	return true;
}

const time_t DirNode::GetLatestModifiedTimeStamp()
{
	return m_latest_modify_time_stamp;
}

void DirNode::SetLatestModifiedTimeStamp(time_t ts)
{
	m_latest_modify_time_stamp = ts;
}
